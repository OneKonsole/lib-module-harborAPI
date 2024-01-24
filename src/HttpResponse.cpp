// HttpResponse.cpp

#include "HttpResponse.h"
#include "../nlohmann/json.hpp"

// Constructor implementation
HttpResponse::HttpResponse() : statusCode(200), statusMessage("OK") {}

// Setter implementations
void HttpResponse::setStatusCode(int code) {
    statusCode = code;
}

void HttpResponse::setStatusMessage(std::string& message) {
    statusMessage = message;
}

void HttpResponse::setBody(std::string& bodyContent) {
    body = bodyContent;
}

// Getter implementations
int HttpResponse::getStatusCode() {
    return statusCode;
}

std::string HttpResponse::getStatusMessage() {
    return statusMessage;
}

std::vector<HttpHeader> HttpResponse::getHeaders() {
    return this->headers;
}

HttpResponse HttpResponse::addHeader(HttpHeader header) {
    this->headers.push_back(header);
    return *this;
}

std::string HttpResponse::getBody() {
    return body;
}

// Compiles and returns the entire HTTP response as a string
std::string HttpResponse::toString() {
    using json = nlohmann::json;

    std::string responseString;

    // Start with the HTTP version and status code
    responseString += "HTTP/1.1 " + std::to_string(statusCode) + " " + statusMessage + "\r\n";

    // Append the headers, including Content-Type as application/json if we're outputting a JSON body
    for (HttpHeader header : headers) {
        responseString += header.getKey() + ": " + header.getValue() + "\r\n";
    }

    // The Content-Type header should be added only if the body is not empty and is indeed JSON.
    if (!body.empty() && json::accept(body)) {
        responseString += "Content-Type: application/json\r\n";
    }

    // End the header section
    responseString += "\r\n";

    // Append the body. If the body is JSON, it will be serialized compactly without extra whitespaces.
    if (!body.empty()) {
        // Check if the body is a valid JSON and serialize it compactly.
        if (json::accept(body)) {
            // Use the non-throwing parse function to avoid exceptions
            json jsonBody = json::parse(body, nullptr, false);
            // Serialize the JSON object compactly if parsing was successful.
            if (!jsonBody.is_discarded()) {
                responseString += jsonBody.dump();
            } else {
                // If parsing fails, the body is not JSON, append it as a plain string.
                responseString += body;
            }
        } else {
            // If the body is not JSON, append it as-is.
            responseString += body;
        }
    }

    return responseString;
}
