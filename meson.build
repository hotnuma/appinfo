project(
    'appinfo',
    ['c'],
    version : '1.0',
    license : 'GPL-2.0',
    default_options : [
        'c_std=c99',
    ],
)

c_args = [
    '-Wall',
    '-Wextra',
    '-O2',
]

app_deps = [
    dependency('tinyc'),
    dependency('glib-2.0'),
    dependency('gio-2.0'),
]

app_src = [
    'application.c',
    'main.c',
]

executable(
    meson.project_name(),
    c_args : c_args,
    dependencies : app_deps,
    sources : app_src,
    install : true,
)


