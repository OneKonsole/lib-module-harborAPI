#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include "HttpHeader.h"
#include <string>
#include <map>
#include <string>
#include <vector>

// Class HttpResponse declaration
class HttpResponse {
        public:
        // Constructor
        HttpResponse();

        // Setters
        void setStatusCode(int code);
        void setStatusMessage(std::string& message);
        HttpResponse addHeader(HttpHeader header);
        void setBody(std::string& bodyContent);

        // Getters
        int getStatusCode();
        std::string getStatusMessage();
        std::vector<HttpHeader> getHeaders();
        std::string getBody();

        std::string toString();

        private:
        int statusCode;
        std::string statusMessage;
        std::vector<HttpHeader> headers;
        std::string body;
};

#endif // HTTPRESPONSE_H