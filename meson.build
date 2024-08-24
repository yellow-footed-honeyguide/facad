project('facad', 'c',
  version : '1.0.0',
  default_options : ['warning_level=3'])

cc = meson.get_compiler('c')

math_dep = cc.find_library('m', required : false)

src_files = [
  'src/main.c',
  'src/file_entry.c',
  'src/emoji_utils.c',
  'src/display_utils.c',
  'src/dev_utils.c',
]

executable('facad',
           src_files,
           dependencies : [math_dep],
           install : true)

