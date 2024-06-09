project('main', 'c', version: '0.1', default_options: ['warning_level=3'])

dep_gtk = dependency('gtk+-3.0')

subdir('src')

configure_file(input: 'utils/input_image_example_RGB.txt', output: 'input_rgb.txt', copy: true)
configure_file(input: 'utils/input_image_example_Gray.txt', output: 'input_gray.txt', copy: true)

executable(
  'main',
  'main.c',
  include_directories: includes,
  link_with: libs,
  dependencies: dep_gtk,
  install: true,
)