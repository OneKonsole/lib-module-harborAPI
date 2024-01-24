// httpRequest.h
#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <map>
#include <string>
#include "HttpHeader.h"
// Class httprequest declaration
// Required parameters : domain, port, path, method, headers, body
// By default : port = 80, method = GET, headers = {}, body = ""

/**
 * @brief HttpRequest class is a representation of an HTTP request
 * @details HttpRequest class is a representation of an HTTP request that can be sent to a server
 *
 * @param domain The domain of the server
 * @param port The port of the server
 * @param path The path of the request
 * @param method The method of the request
 * @param headers The headers of the request
 * @param body The body of the request
 *
 * @return HttpRequest object
 */
class HttpRequest {
        public:
        HttpRequest();
        HttpRequest(std::string domain, std::string path, std::string method);
        HttpRequest(std::string domain, std::string path, std::string method, int port);
        HttpRequest(std::string domain, std::string path, std::string method, int port, std::string body);
        std::string getDomain();
        std::string getPath();
        std::string getMethod();
        std::string getBody();
        int getPort();
        std::vector<HttpHeader> getHeaders();
        HttpHeader getHeader(std::string key);
        void setDomain(std::string domain);
        void setPath(std::string path);
        void setMethod(std::string method);
        void setBody(std::string body);
        void setPort(int port);
        HttpRequest addHeader(HttpHeader header);
        std::string toString();

        private:
        std::string domain;
        std::string path;
        std::string method;
        std::string body;
        int port;
        std::vector<HttpHeader> headers;
};

#endif // HTTPREQUEST_H