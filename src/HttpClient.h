#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "HttpRequest.h"
#include "HttpResponse.h"
#include <string>
#include <vector>

class HttpClient {
public:
    HttpClient();

    ~HttpClient();

    // Main method to send an HTTP request and get the response
    HttpResponse executeRequest(HttpRequest &request);

private:
    // Utility methods for socket communication
    int openConnection(const std::string &host, int port);

    void closeConnection(int sockfd);

    HttpResponse receiveResponse(int sockfd);

    HttpResponse sendData(int sockfd, HttpRequest &request);

    HttpResponse hydrateFromResponse(const std::string &responseStr);

    size_t getContentLength(const std::string &headersStr);
};
#endif // HTTPCLIENT_H
