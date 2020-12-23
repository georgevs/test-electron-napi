{
  'targets': [
    {
      'target_name': 'addon',
      'include_dirs': [
        "<!@(node -p \"require('node-addon-api').include\")",
      ],
      "cflags_cc": [
        "-std=c++17"
      ],
      'defines': [
        'NAPI_VERSION=<(napi_build_version)',
        'NAPI_DISABLE_CPP_EXCEPTIONS',
      ],
      'sources': [
        './addon.cc',
      ],
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)",
        }
      ]
    }
  ]
}
