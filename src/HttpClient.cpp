#include "HttpClient.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iterator>

HttpClient::HttpClient() {
    // Constructor logic if needed
}

HttpClient::~HttpClient() {
    // Destructor logic if needed
}

int HttpClient::openConnection(const std::string& host, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Cannot create socket");
        return -1;
    }

    struct hostent* server = gethostbyname(host.c_str());
    if (server == NULL) {
        perror("No such host");
        return -1;
    }

    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    std::memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Cannot connect");
        return -1;
    }

    // Set timeout
    struct timeval timeout;
    timeout.tv_sec = 5; // 5 seconds timeout
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    return sockfd;
}

void HttpClient::closeConnection(int sockfd) {
    close(sockfd);
}

HttpResponse HttpClient::receiveResponse(int sockfd) {
    const unsigned int bufferSize = 4096;
    char buffer[bufferSize];
    std::string responseStr, headersStr, bodyStr;
    ssize_t bytesRead;

    // First, read the headers
    bool headersComplete = false;
    while (!headersComplete && (bytesRead = recv(sockfd, buffer, bufferSize - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';
        responseStr.append(buffer);

        // Check if the headers part of the response is complete
        size_t headerEnd = responseStr.find("\r\n\r\n");
        if (headerEnd != std::string::npos) {
            headersComplete = true;
            headersStr = responseStr.substr(0, headerEnd);
            bodyStr = responseStr.substr(headerEnd + 4); // Skip the "\r\n\r\n"
        }
    }

    // Parse headers to find Content-Length
    size_t contentLength = getContentLength(headersStr);

    // Read the rest of the body based on Content-Length
    while (bodyStr.length() < contentLength && (bytesRead = recv(sockfd, buffer, bufferSize - 1, 0)) > 0) {
        buffer[bytesRead] = '\0';
        bodyStr.append(buffer);
    }

    HttpResponse response;
    response = hydrateFromResponse(headersStr + "\r\n\r\n" + bodyStr);

    return response;
}

size_t HttpClient::getContentLength(const std::string& headersStr) {
    std::istringstream headersStream(headersStr);
    std::string line;
    while (std::getline(headersStream, line) && line != "\r") {
        std::string headerKey;
        std::string headerValue;
        std::istringstream headerLineStream(line);
        if (std::getline(std::getline(headerLineStream, headerKey, ':') >> std::ws, headerValue)) {
            if (headerKey == "Content-Length" || headerKey == "content-length") {
                return std::stoul(headerValue);
            }
        }
    }
    return 0; // If there's no Content-Length header, return 0
}


HttpResponse HttpClient::sendData(int sockfd, HttpRequest& request) {
    std::string requestStr = request.toString();
    ssize_t bytesSent = ::send(sockfd, requestStr.c_str(), requestStr.length(), 0); // Use global namespace for send
    if (bytesSent < 0) {
        perror("Send failed");
        // Close the socket and return an error response or throw an exception
        closeConnection(sockfd);
        return HttpResponse(); // Return an empty response for now
    }
    return receiveResponse(sockfd); // After sending, we wait to receive the response
}

HttpResponse HttpClient::executeRequest(HttpRequest& request) {
    int sockfd = openConnection(request.getDomain(), request.getPort());
    if (sockfd < 0) {
        std::cerr << "Failed to open socket connection." << std::endl;
        return HttpResponse(); // Return an empty response or throw an exception
    }

    // Send the request and wait for the response
    HttpResponse response = sendData(sockfd, request);

    // Close the connection after receiving the response
    closeConnection(sockfd);
    // log the path and response code
    std::cout << request.getPath() << " " << response.getStatusCode() << std::endl;
    return response;
}


HttpResponse HttpClient::hydrateFromResponse(const std::string& responseStr) {
    std::istringstream responseStream(responseStr);
    std::string line;
    std::string headerKey;
    std::string headerValue;

    HttpResponse response;

    int statusCode;
    std::string statusMessage;

    // Parse status line
    if (std::getline(responseStream, line) && !line.empty()) {
        std::istringstream statusLineStream(line);
        std::string httpVersion;
        statusLineStream >> httpVersion; // We're not using it here, but we need to extract it
        statusLineStream >> statusCode;
        std::getline(statusLineStream >> std::ws, statusMessage);
    }

    response.setStatusCode(statusCode);
    response.setStatusMessage(statusMessage);

    // Parse headers
    while (std::getline(responseStream, line) && line != "\r") {
        std::istringstream headerLineStream(line);
        if (std::getline(std::getline(headerLineStream, headerKey, ':') >> std::ws, headerValue)) {
            response.addHeader(HttpHeader(headerKey, headerValue));
        }
    }

    // The rest is the body
    std::string bodyContent;

    responseStream >> std::noskipws;
    std::istream_iterator<char> it(responseStream);
    std::istream_iterator<char> end;
    std::copy(it, end, std::back_inserter(bodyContent));
    response.setBody(bodyContent);

    return response;
}