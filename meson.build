project('facad', 'c',
  version : '2.20.16',
  default_options : ['warning_level=3'])

cc = meson.get_compiler('c')

math_dep = cc.find_library('m', required : false)

src_files = [
  'src/main.c',
  'src/file_card.c',
  'src/dir_config.c',
  'src/emoji_utils.c',
  'src/args_parser.c',
  'src/longlisting.c',
  'src/dir_analytics.c',
  'src/dev_dir_utils.c',
  'src/display_utils.c',
  'src/emoji_mappings.c',
  'src/git_utils.c',
  'src/git_integration.c',
]

version = meson.project_version()

conf_data = configuration_data()
conf_data.set_quoted('VERSION', version)

configure_file(
  output : 'config.h',
  configuration : conf_data
)

inc = include_directories('.')

executable('facad',
           src_files,
           include_directories : inc,
           dependencies : [math_dep],
           install : true)

# Install man page
install_man('docs/man/facad.1')
