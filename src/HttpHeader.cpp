// HttpRequest.cpp
#include "HttpHeader.h"

HttpHeader::HttpHeader() {
    this->key = "";
    this->value = "";
}

HttpHeader::HttpHeader(std::string key, std::string value) {
    this->key = key;
    this->value = value;
}

std::string HttpHeader::getKey() {
    return this->key;
}

std::string HttpHeader::getValue() {
    return this->value;
}

void HttpHeader::setKey(std::string key) {
    this->key = key;
}

void HttpHeader::setValue(std::string value) {
    this->value = value;
}

std::string HttpHeader::toString() {
    return this->key + ": " + this->value;
}
