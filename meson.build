project('cfacad', 'c',
  version : '1.0.0',
  default_options : ['warning_level=3'])

cc = meson.get_compiler('c')

# Проверка и добавление необходимых библиотек
math_dep = cc.find_library('m', required : false)

# Список исходных файлов
src_files = [
  'src/main.c',
  'src/file_entry.c',
  'src/emoji_utils.c',
  'src/display_utils.c',
]

# Создание исполняемого файла
executable('cfacad',
           src_files,
           dependencies : [math_dep],
           install : true)
