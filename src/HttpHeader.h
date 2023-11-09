// httpHeader.h
#ifndef HHEADER_H
#define HHEADER_H

#include <string>

// Class httpHeader declaration
class HttpHeader{
        public:
        HttpHeader();
        HttpHeader(std::string key, std::string value);
        std::string getKey();
        std::string getValue();
        void setKey(std::string key);
        void setValue(std::string value);
        std::string toString();

        private:
        std::string key;
        std::string value;
};

#endif // HTTPREQUEST_H