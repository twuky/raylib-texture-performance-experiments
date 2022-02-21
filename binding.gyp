{
  'targets': [
    {
      'target_name': 'ralts',
      'sources': [ 'src/addon.cc'],
      'include_dirs': [
				"<!@(node -p \"require('node-addon-api').include\")",
				"raylib/win/include"
			],
      'dependencies': [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7'
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      },
      'conditions': [
        ['OS=="linux"', {
          'libraries': [
              "<(module_root_dir)/raylib/linux/lib/libraylib.a"
          ],
        }],
        ['OS=="win"', {
          'libraries': [
              "<(module_root_dir)/raylib/win/lib/libraylib.a"
          ],
        }],
        ['OS=="mac"', {
          'libraries': [
              "<(module_root_dir)/raylib/mac/lib/libraylib.a"
          ],
        }]
      ]
  }],
}