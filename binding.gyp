{
  "targets": [
    {
      "target_name": "run",
      "sources": [ "run.cpp" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
       "cflags" : [ "-std=c++11" ],
              "conditions": [
                [ "OS!='win'", {
                  "cflags+": [ "-std=c++11" ],
                  "cflags_c+": [ "-std=c++11" ],
                  "cflags_cc+": [ "-std=c++11" ],
                }],
                [ "OS=='mac'", {
                  "xcode_settings": {
                   'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',        # -fno-exceptions
                    "OTHER_CPLUSPLUSFLAGS" : [ "-std=c++11", "-stdlib=libc++" ],
                    "OTHER_LDFLAGS": [ "-stdlib=libc++" ],
                    "MACOSX_DEPLOYMENT_TARGET": "10.7"
                  },
                }],
              ],
    }
  ]
}