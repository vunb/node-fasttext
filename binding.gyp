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
        "src/node-argument.cc",
        "src/loadModel.cc",
        "src/train.cc",
        "src/quantize.cc",
        "src/classifier.h",
        "src/classifierWorker.cc",
        "src/query.h",
        "src/nnWorker.cc",
        "src/wrapper.cc",
        "src/addon.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"],
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
                  "OTHER_CPLUSPLUSFLAGS" : [ 
                    "-std=c++0x", 
                    "-stdlib=libc++", 
                    "-march=native",
                    "-O3",
                    "-funroll-loops",
                    "-c",
                    "-pthread" ],
                  "OTHER_LDFLAGS": [ 
                    "-stdlib=libc++"
                  ],
                  "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                  "MACOSX_DEPLOYMENT_TARGET": "10.7",
                  "CLANG_CXX_LANGUAGE_STANDARD":"c++0x",
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