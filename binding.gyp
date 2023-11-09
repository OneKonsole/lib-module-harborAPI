{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "src/HttpRequest.cpp",  "src/HttpHeader.cpp", "src/HttpResponse.cpp", "src/HttpClient.cpp", "src/addon.cpp" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }
  ]
}
