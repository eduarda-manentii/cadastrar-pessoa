project('listar-cadastro', 'cpp', default_options : ['cpp_std=c++20'])

subdir('lib/sqlite3x')
subdir('lib/persistent')

executable('listar-cadastro', [
    'main.cpp',
    'my_window.cpp',
    'cadastro_window.cpp',
    'listagem_window.cpp',
    'entry_box.cpp',
    'db-config.cpp',
    'models/pessoa.cpp',
    persistent_files,
    sqlite3x_files,
], dependencies: [
    dependency('gtkmm-3.0'),
    dependency('boost'),
    dependency('sqlite3'),
], include_directories : [
    include_directories('lib')
]
)

