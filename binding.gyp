{
  "targets": [
    {
      "target_name": "fasttext",
      "sources": [
        "fastText/src/args.h",
        "fastText/src/args.cc",
        "fastText/src/dictionary.h",
        "fastText/src/dictionary.cc",
        "fastText/src/fasttext.h",
        "fastText/src/fasttext.cc",
        "fastText/src/matrix.h",
        "fastText/src/matrix.cc",
        "fastText/src/model.h",
        "fastText/src/model.cc",
        "fastText/src/productquantizer.h",
        "fastText/src/productquantizer.cc",
        "fastText/src/qmatrix.h",
        "fastText/src/qmatrix.cc",
        "fastText/src/real.h",
        "fastText/src/utils.h",
        "fastText/src/utils.cc",
        "fastText/src/vector.h",
        "fastText/src/vector.cc",
        "src/node-util.cc",
        "src/node-argument.cc",
        "src/loadModel.cc",
        "src/train.cc",
        "src/quantize.cc",
        "src/predictWorker.cc",
        "src/nnWorker.cc",
        "src/wrapper.cc",
        "src/classifier.cc",
        "src/query.cc",
        "src/addon.cc"
      ],
      "defines": [
        "NAPI_VERSION=<(napi_build_version)",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "cflags": [
          "-std=c++11",
          "-pthread",
          "-fexceptions",
          "-O3",
          "-Wall",
          "-Wno-sign-compare",
          "-pedantic",
          "-DUSE_SSE",
          "-DUSE_SSE2"
      ],
      "conditions": [
          [ "OS=='linux'", {
              "cflags+": [ "-std=c++11", "-fexceptions" ],
              "cflags_c+": [ "-std=c++11", "-fexceptions" ],
              "cflags_cc+": [ "-std=c++11", "-fexceptions" ],
          }],
          [ "OS=='freebsd'", {
              "cflags+": [ "-std=c++11", "-fexceptions" ],
              "cflags_c+": [ "-std=c++11", "-fexceptions" ],
              "cflags_cc+": [ "-std=c++11", "-fexceptions" ],
          }],
          [ "OS=='mac'", {
              "cflags+": [ "-stdlib=libc++" ],
              "xcode_settings": {
                  "OTHER_CPLUSPLUSFLAGS" : [ "-std=c++11", "-stdlib=libc++", "-pthread" ],
                  "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
                  "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                  "MACOSX_DEPLOYMENT_TARGET": "10.7",
                  "CLANG_CXX_LANGUAGE_STANDARD":"c++11",
                  "CLANG_CXX_LIBRARY": "libc++"
              },
          }],
          [
          "OS=='win'", {
            "cflags": [
              "-Wall"
            ],
            "defines": [
              "WIN"
            ],
            "msvs_settings": {
              "VCCLCompilerTool": {
                "ExceptionHandling": "2",
                "DisableSpecificWarnings": [
                  "4244"
                ],
              },
              "VCLinkerTool": {
                "LinkTimeCodeGeneration": 1,
                "OptimizeReferences": 2,
                "EnableCOMDATFolding": 2,
                "LinkIncremental": 1,
              }
            }
          }]
      ]
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": ["<(module_name)"],
      "copies": [{
          "files": ["<(PRODUCT_DIR)/<(module_name).node"],
          "destination": "<(module_path)"
      }]
    }
  ]
}
