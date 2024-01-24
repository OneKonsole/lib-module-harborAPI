// addon.cpp
#include <napi.h>
#include <iostream>

#include "HttpHeader.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpClient.h"

// ----- HTTP HEADER WRAPPER -----
class HttpHeaderWrapper : public Napi::ObjectWrap<HttpHeaderWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports); // Init class
    HttpHeaderWrapper(const Napi::CallbackInfo &info); // Constructor
    ~HttpHeaderWrapper(); // Destructor
    static Napi::FunctionReference constructor; // ref to the class constructor

    HttpHeader getInternalInstance() { return *httpHeaderInstance; }

private:
    HttpHeader *httpHeaderInstance; // Internal isntance


    // Binding methods
    Napi::Value getKey(const Napi::CallbackInfo &info);

    Napi::Value getValue(const Napi::CallbackInfo &info);

    Napi::Value setKey(const Napi::CallbackInfo &info);

    Napi::Value setValue(const Napi::CallbackInfo &info);

    Napi::Value toString(const Napi::CallbackInfo &info);
};

Napi::FunctionReference HttpHeaderWrapper::constructor;

Napi::Object HttpHeaderWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    // Define class
    Napi::Function func = DefineClass(env, "HttpHeader", {
            InstanceMethod("getKey", &HttpHeaderWrapper::getKey),
            InstanceMethod("setKey", &HttpHeaderWrapper::setKey),
            InstanceMethod("getValue", &HttpHeaderWrapper::getValue),
            InstanceMethod("setValue", &HttpHeaderWrapper::setValue),
            InstanceMethod("toString", &HttpHeaderWrapper::toString),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("HttpHeader", func);

    return exports;
}

HttpHeaderWrapper::HttpHeaderWrapper(const Napi::CallbackInfo &info) : Napi::ObjectWrap<HttpHeaderWrapper>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int length = info.Length();

    if (length < 2 || length > 2) {
        Napi::TypeError::New(env, "Constructor must have 2 arguments").ThrowAsJavaScriptException();
        return;
    }

    // Mandatory arguments
    std::string key = info[0].As<Napi::String>().Utf8Value();
    std::string value = info[1].As<Napi::String>().Utf8Value();

    // Creating a new HttpHeader instance with the given arguments.
    this->httpHeaderInstance = new HttpHeader(key, value);
}

HttpHeaderWrapper::~HttpHeaderWrapper() {
    delete httpHeaderInstance;
}

Napi::Value HttpHeaderWrapper::getKey(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->httpHeaderInstance->getKey());
}

Napi::Value HttpHeaderWrapper::getValue(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->httpHeaderInstance->getValue());
}

Napi::Value HttpHeaderWrapper::setKey(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "One argument expected").ThrowAsJavaScriptException();
    }
    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Key should be a string").ThrowAsJavaScriptException();
    }
    std::string key = info[0].As<Napi::String>().Utf8Value();
    this->httpHeaderInstance->setKey(key);
    return env.Null();
}

Napi::Value HttpHeaderWrapper::setValue(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "One argument expected").ThrowAsJavaScriptException();
    }
    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Value should be a string").ThrowAsJavaScriptException();
    }
    std::string value = info[0].As<Napi::String>().Utf8Value();
    this->httpHeaderInstance->setValue(value);
    return env.Null();
}

Napi::Value HttpHeaderWrapper::toString(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    HttpHeader header = *this->httpHeaderInstance;

    return Napi::String::New(env, header.toString());
}

// HTTP RESPONSE WRAPPER
class HttpResponseWrapper : public Napi::ObjectWrap<HttpResponseWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports); // Init class
    HttpResponseWrapper(const Napi::CallbackInfo &info); // Constructor
    ~HttpResponseWrapper(); // Destructor
    static Napi::FunctionReference constructor; // ref to the class constructor

    HttpResponse getInternalInstance() { return *httpResponseInstance; }

    void SetInternalInstance(HttpResponse response) { *httpResponseInstance = response; }

    // Binding methods
    Napi::Value getStatusCode(const Napi::CallbackInfo &info);

    Napi::Value getStatusMessage(const Napi::CallbackInfo &info);

    Napi::Value getHeaders(const Napi::CallbackInfo &info);

    Napi::Value getBody(const Napi::CallbackInfo &info);

    Napi::Value setStatusCode(const Napi::CallbackInfo &info);

    Napi::Value setStatusMessage(const Napi::CallbackInfo &info);

    Napi::Value addHeader(const Napi::CallbackInfo &info);

    Napi::Value setBody(const Napi::CallbackInfo &info);

    Napi::Value toString(const Napi::CallbackInfo &info);

private:
    HttpResponse *httpResponseInstance; // Internal isntance
};

Napi::FunctionReference HttpResponseWrapper::constructor;

Napi::Object HttpResponseWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    // Define class
    Napi::Function func = DefineClass(env, "HttpResponse", {
            InstanceMethod("getStatusCode", &HttpResponseWrapper::getStatusCode),
            InstanceMethod("getStatusMessage", &HttpResponseWrapper::getStatusMessage),
            InstanceMethod("getHeaders", &HttpResponseWrapper::getHeaders),
            InstanceMethod("getBody", &HttpResponseWrapper::getBody),
            InstanceMethod("setStatusCode", &HttpResponseWrapper::setStatusCode),
            InstanceMethod("setStatusMessage", &HttpResponseWrapper::setStatusMessage),
            InstanceMethod("addHeader", &HttpResponseWrapper::addHeader),
            InstanceMethod("setBody", &HttpResponseWrapper::setBody),
            InstanceMethod("toString", &HttpResponseWrapper::toString),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("HttpResponse", func);

    return exports;
}

HttpResponseWrapper::HttpResponseWrapper(const Napi::CallbackInfo &info) : Napi::ObjectWrap<HttpResponseWrapper>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int length = info.Length();

    if (length > 0) {
        Napi::TypeError::New(env, "Constructor must have 0 arguments").ThrowAsJavaScriptException();
        return;
    }

    // Creating a new HttpResponse instance with the given arguments.
    this->httpResponseInstance = new HttpResponse();
};

HttpResponseWrapper::~HttpResponseWrapper() {
    delete httpResponseInstance;
}

Napi::Value HttpResponseWrapper::getStatusCode(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::Number::New(env, this->httpResponseInstance->getStatusCode());
}

Napi::Value HttpResponseWrapper::getStatusMessage(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->httpResponseInstance->getStatusMessage());
}

Napi::Value HttpResponseWrapper::getHeaders(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    // Get the vector of HttpHeader objects
    std::vector <HttpHeader> headers = this->httpResponseInstance->getHeaders();

    // Create a new empty array to hold the header objects
    Napi::Array headersArray = Napi::Array::New(env);

    for (size_t i = 0; i < headers.size(); ++i) {
        // Wrap the HttpHeader in a new HttpHeaderWrapper instance
        Napi::Value arg = Napi::External<HttpHeader>::New(env, &headers[i]);
        // get the key and value of the header
        std::string key = headers[i].getKey();
        std::string value = headers[i].getValue();
        Napi::Object headerObj = HttpHeaderWrapper::constructor.New(
                {Napi::String::New(env, key), Napi::String::New(env, value)}
        );

        // Add the HttpHeaderWrapper object to the headers array
        headersArray[i] = headerObj;
    }

    // Return the JavaScript array that contains the HttpHeaderWrapper objects
    return headersArray;
}

Napi::Value HttpResponseWrapper::getBody(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->httpResponseInstance->getBody());
}

Napi::Value HttpResponseWrapper::setStatusCode(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "One argument expected").ThrowAsJavaScriptException();
    }
    if (!info[0].IsNumber()) {
        Napi::TypeError::New(env, "Status code should be a number").ThrowAsJavaScriptException();
    }
    int statusCode = info[0].As<Napi::Number>().Int32Value();
    this->httpResponseInstance->setStatusCode(statusCode);
    return env.Null();
}

Napi::Value HttpResponseWrapper::setStatusMessage(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "One argument expected").ThrowAsJavaScriptException();
    }
    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Status message should be a string").ThrowAsJavaScriptException();
    }
    std::string statusMessage = info[0].As<Napi::String>().Utf8Value();
    this->httpResponseInstance->setStatusMessage(statusMessage);
    return env.Null();
}

Napi::Value HttpResponseWrapper::addHeader(const Napi::CallbackInfo &info) {

    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "One argument expected").ThrowAsJavaScriptException();
    }

    HttpHeaderWrapper *headerWrapper = Napi::ObjectWrap<HttpHeaderWrapper>::Unwrap(info[0].As<Napi::Object>());
    HttpHeader header = headerWrapper->getInternalInstance();
    this->httpResponseInstance->addHeader(header);

    return env.Null();
}

Napi::Value HttpResponseWrapper::setBody(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "One argument expected").ThrowAsJavaScriptException();
    }
    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Body should be a string").ThrowAsJavaScriptException();
    }
    std::string body = info[0].As<Napi::String>().Utf8Value();
    this->httpResponseInstance->setBody(body);
    return env.Null();
}

Napi::Value HttpResponseWrapper::toString(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->httpResponseInstance->toString());
}


// ----- HTTP REQUEST WRAPPER -----
class HttpRequestWrapper : public Napi::ObjectWrap<HttpRequestWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports); // Init class
    HttpRequestWrapper(const Napi::CallbackInfo &info); // Constructor
    ~HttpRequestWrapper(); // Destructor
    HttpRequest GetInternalInstance() { return *httpRequestInstance; }

private:
    static Napi::FunctionReference constructor; // ref to the class constructor
    HttpRequest *httpRequestInstance; // Internal isntance

    // Binding methods
    Napi::Value getDomain(const Napi::CallbackInfo &info);

    Napi::Value getPath(const Napi::CallbackInfo &info);

    Napi::Value getMethod(const Napi::CallbackInfo &info);

    Napi::Value getBody(const Napi::CallbackInfo &info);

    Napi::Value getPort(const Napi::CallbackInfo &info);

    Napi::Value getHeaders(const Napi::CallbackInfo &info);

    Napi::Value getHeader(const Napi::CallbackInfo &info);

    Napi::Value setDomain(const Napi::CallbackInfo &info);

    Napi::Value setPath(const Napi::CallbackInfo &info);

    Napi::Value setMethod(const Napi::CallbackInfo &info);

    Napi::Value setBody(const Napi::CallbackInfo &info);

    Napi::Value setPort(const Napi::CallbackInfo &info);

    Napi::Value addHeader(const Napi::CallbackInfo &info);

    Napi::Value toString(const Napi::CallbackInfo &info);
};

Napi::FunctionReference HttpRequestWrapper::constructor;

Napi::Object HttpRequestWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    // Define class
    Napi::Function func = DefineClass(env, "HttpRequest", {
            InstanceMethod("getDomain", &HttpRequestWrapper::getDomain),
            InstanceMethod("setDomain", &HttpRequestWrapper::setDomain),
            InstanceMethod("getPath", &HttpRequestWrapper::getPath),
            InstanceMethod("setPath", &HttpRequestWrapper::setPath),
            InstanceMethod("getMethod", &HttpRequestWrapper::getMethod),
            InstanceMethod("setMethod", &HttpRequestWrapper::setMethod),
            InstanceMethod("getBody", &HttpRequestWrapper::getBody),
            InstanceMethod("setBody", &HttpRequestWrapper::setBody),
            InstanceMethod("getPort", &HttpRequestWrapper::getPort),
            InstanceMethod("setPort", &HttpRequestWrapper::setPort),
            InstanceMethod("getHeaders", &HttpRequestWrapper::getHeaders),
            InstanceMethod("getHeader", &HttpRequestWrapper::getHeader),
            InstanceMethod("addHeader", &HttpRequestWrapper::addHeader),
            InstanceMethod("toString", &HttpRequestWrapper::toString),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("HttpRequest", func);

    return exports;
}

HttpRequestWrapper::HttpRequestWrapper(const Napi::CallbackInfo &info) : Napi::ObjectWrap<HttpRequestWrapper>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int length = info.Length();

    if (length < 2 || length > 5) {
        Napi::TypeError::New(env, "Constructor must have between 2 and 5 arguments").ThrowAsJavaScriptException();
        return;
    }

    // Mandatory arguments
    std::string domain = info[0].As<Napi::String>().Utf8Value();
    std::string path = info[1].As<Napi::String>().Utf8Value();

    // Optional arguments with default values
    std::string method = "GET"; // Default method is GET
    int port = 80; // Default port is 80
    std::string body = ""; // Default body is an empty string

    if (length > 2 && !info[2].IsUndefined()) {
        method = info[2].As<Napi::String>().Utf8Value();
    }

    if (length > 3 && !info[3].IsUndefined()) {
        if (!info[3].IsNumber()) {
            Napi::TypeError::New(env, "Port must be a number").ThrowAsJavaScriptException();
            return;
        }
        port = info[3].As<Napi::Number>().Int32Value();
    }

    if (length > 4 && !info[4].IsUndefined()) {
        body = info[4].As<Napi::String>().Utf8Value();
    }

    // Creating a new HttpRequest instance with the given arguments.
    this->httpRequestInstance = new HttpRequest(domain, path, method, port, body);
}

HttpRequestWrapper::~HttpRequestWrapper() {
    delete httpRequestInstance;
}

Napi::Value HttpRequestWrapper::getDomain(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->httpRequestInstance->getDomain());
}

Napi::Value HttpRequestWrapper::getPath(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->httpRequestInstance->getPath());
}

Napi::Value HttpRequestWrapper::getMethod(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->httpRequestInstance->getMethod());
}

Napi::Value HttpRequestWrapper::getBody(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->httpRequestInstance->getBody());
}

Napi::Value HttpRequestWrapper::getPort(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::Number::New(env, this->httpRequestInstance->getPort());
}

Napi::Value HttpRequestWrapper::getHeaders(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    // Get the vector of HttpHeader objects
    std::vector <HttpHeader> headers = this->httpRequestInstance->getHeaders();

    // Create a new empty array to hold the header objects
    Napi::Array headersArray = Napi::Array::New(env);

    for (size_t i = 0; i < headers.size(); ++i) {
        // Wrap the HttpHeader in a new HttpHeaderWrapper instance
        Napi::Value arg = Napi::External<HttpHeader>::New(env, &headers[i]);
        // get the key and value of the header
        std::string key = headers[i].getKey();
        std::string value = headers[i].getValue();
        Napi::Object headerObj = HttpHeaderWrapper::constructor.New(
                {Napi::String::New(env, key), Napi::String::New(env, value)}
        );

        // Add the HttpHeaderWrapper object to the headers array
        headersArray[i] = headerObj;
    }

    // Return the JavaScript array that contains the HttpHeaderWrapper objects
    return headersArray;
}

Napi::Value HttpRequestWrapper::getHeader(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "One argument expected").ThrowAsJavaScriptException();
    }
    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Key should be a string").ThrowAsJavaScriptException();
    }

    std::string key = info[0].As<Napi::String>().Utf8Value();
    HttpHeader header = this->httpRequestInstance->getHeader(key);
    Napi::Object headerObj = HttpHeaderWrapper::constructor.New(
            {Napi::String::New(env, header.getKey()), Napi::String::New(env, header.getValue())}
    );
    return headerObj;
}

Napi::Value HttpRequestWrapper::setDomain(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "One argument expected").ThrowAsJavaScriptException();
    }
    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Domain should be a string").ThrowAsJavaScriptException();
    }
    std::string domain = info[0].As<Napi::String>().Utf8Value();
    this->httpRequestInstance->setDomain(domain);
    return env.Null();
}

Napi::Value HttpRequestWrapper::setPath(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "One argument expected").ThrowAsJavaScriptException();
    }
    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Path should be a string").ThrowAsJavaScriptException();
    }
    std::string path = info[0].As<Napi::String>().Utf8Value();
    this->httpRequestInstance->setPath(path);
    return env.Null();
}

Napi::Value HttpRequestWrapper::setMethod(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "One argument expected").ThrowAsJavaScriptException();
    }
    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Method should be a string").ThrowAsJavaScriptException();
    }
    std::string method = info[0].As<Napi::String>().Utf8Value();
    this->httpRequestInstance->setMethod(method);
    return env.Null();
}

Napi::Value HttpRequestWrapper::setBody(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "One argument expected").ThrowAsJavaScriptException();
    }
    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Body should be a string").ThrowAsJavaScriptException();
    }
    std::string body = info[0].As<Napi::String>().Utf8Value();
    this->httpRequestInstance->setBody(body);
    return env.Null();
}

Napi::Value HttpRequestWrapper::setPort(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "One argument expected").ThrowAsJavaScriptException();
    }
    if (!info[0].IsNumber()) {
        Napi::TypeError::New(env, "Port should be a number").ThrowAsJavaScriptException();
    }
    int port = info[0].As<Napi::Number>().Int32Value();
    this->httpRequestInstance->setPort(port);
    return env.Null();
}

Napi::Value HttpRequestWrapper::addHeader(const Napi::CallbackInfo &info) {

    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "One argument expected").ThrowAsJavaScriptException();
    }

    HttpHeaderWrapper *headerWrapper = Napi::ObjectWrap<HttpHeaderWrapper>::Unwrap(info[0].As<Napi::Object>());
    HttpHeader header = headerWrapper->getInternalInstance();
    this->httpRequestInstance->addHeader(header);

    return env.Null();
}

Napi::Value HttpRequestWrapper::toString(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->httpRequestInstance->toString());
}

// ----- HttpClient WRAPPER -----
class HttpClientWrapper : public Napi::ObjectWrap<HttpClientWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    static Napi::FunctionReference constructor; // This needs to be public to be accessible

    HttpClientWrapper(const Napi::CallbackInfo &info);

    ~HttpClientWrapper();

    HttpRequest *GetInternalInstance();

    static Napi::Object
    NewInstance(Napi::Env env, const HttpResponse &response); // Static method to create a new instance


    Napi::Value ExecuteRequest(const Napi::CallbackInfo &info);

private:
    HttpClient *httpClientInstance;
};

Napi::FunctionReference HttpClientWrapper::constructor;

HttpClientWrapper::HttpClientWrapper(const Napi::CallbackInfo &info)
        : Napi::ObjectWrap<HttpClientWrapper>(info) {
    httpClientInstance = new HttpClient();
}

HttpClientWrapper::~HttpClientWrapper() {
    delete httpClientInstance;
}

Napi::Object HttpClientWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "HttpClient", {
            InstanceMethod("executeRequest", &HttpClientWrapper::ExecuteRequest),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("HttpClient", func);
    return exports;
}

Napi::Value HttpClientWrapper::ExecuteRequest(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (!info[0].IsObject()) {
        Napi::TypeError::New(env, "Argument must be an instance of HttpRequest").ThrowAsJavaScriptException();
        return env.Null();
    }

    HttpRequestWrapper *requestWrapper = Napi::ObjectWrap<HttpRequestWrapper>::Unwrap(info[0].As<Napi::Object>());
    HttpRequest request = requestWrapper->GetInternalInstance();

    HttpResponse response = this->httpClientInstance->executeRequest(request);

    Napi::Object responseObj = HttpResponseWrapper::constructor.New({});
    HttpResponseWrapper *responseWrapper = Napi::ObjectWrap<HttpResponseWrapper>::Unwrap(responseObj);
    // Set the internal instance of the wrapper to the response
    responseWrapper->SetInternalInstance(response);
    // Return the wrapper
    return responseObj;
}


// ----- INIT ALL -----

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    HttpHeaderWrapper::Init(env, exports);
    HttpResponseWrapper::Init(env, exports);
    HttpRequestWrapper::Init(env, exports);
    HttpClientWrapper::Init(env, exports);

    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, InitAll
);