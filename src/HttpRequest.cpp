// HttpRequest.cpp
#include "HttpRequest.h"
#include <sstream>
#include "../nlohmann/json.hpp"
// Default constructor
HttpRequest::HttpRequest()
        : domain(""), path(""), method("GET"), port(80), body(""), headers() {}

// Constructor with domain, path, and method, uses default port
HttpRequest::HttpRequest(std::string domain, std::string path, std::string method)
        : HttpRequest(std::move(domain), std::move(path), std::move(method), 80) {}

// Constructor with domain, path, method, and port
HttpRequest::HttpRequest(std::string domain, std::string path, std::string method, int port)
        : domain(std::move(domain)), path(std::move(path)), method(std::move(method)), port(port), body(""), headers() {}

// Constructor with domain, path, method, port, and body
HttpRequest::HttpRequest(std::string domain, std::string path, std::string method, int port, std::string body)
        : domain(std::move(domain)), path(std::move(path)), method(std::move(method)), port(port), body(std::move(body)), headers() {}


std::string HttpRequest::getDomain() {
    return this->domain;
}

std::string HttpRequest::getPath() {
    return this->path;
}

std::string HttpRequest::getMethod() {
    return this->method;
}

std::string HttpRequest::getBody() {
    return this->body;
}

int HttpRequest::getPort() {
    return this->port;
}

std::vector<HttpHeader> HttpRequest::getHeaders() {
    return this->headers;
}

HttpHeader HttpRequest::getHeader(std::string key) {
    for (HttpHeader header: this->headers) {
        if (header.getKey() == key) {
            return header;
        }
    }
    return HttpHeader();
}

void HttpRequest::setDomain(std::string domain) {
    this->domain = domain;
}

void HttpRequest::setPath(std::string path) {
    this->path = path;
}

void HttpRequest::setMethod(std::string method) {
    this->method = method;
}

void HttpRequest::setBody(std::string body) {
    this->body = body;
}

void HttpRequest::setPort(int port) {
    this->port = port;
}

HttpRequest HttpRequest::addHeader(HttpHeader header) {
    this->headers.push_back(header);
    return *this;
}

std::string HttpRequest::toString() {
    std::ostringstream requestStream;

    // Start with the request line
    requestStream << method << " " << path << " HTTP/1.1\r\n";

    // Add the Host header, omitting the port if it's the default port 80
    requestStream << "Host: " << domain;
    if (port != 80) {
        requestStream << ":" << port;
    }
    requestStream << "\r\n";

    // Add other headers
    for (HttpHeader header : headers) {
        requestStream << header.getKey() << ": " << header.getValue() << "\r\n";
    }

    // End the header section with an extra CRLF
    requestStream << "\r\n";

    // Add the body, if present
    if (!body.empty()) {
        requestStream << body;
    }

    return requestStream.str();
}
