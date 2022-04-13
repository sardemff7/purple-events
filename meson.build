project('purple-events', 'c',
    version: '0.99.3',
    meson_version: '>=0.58.0',
    license: [ 'GPL3+' ],
    default_options: [
        'c_std=gnu11',
        'warning_level=2',
    ],
)

purple_min_version='2.6.0'
package_developer='sardemff7'

glib = dependency('glib-2.0')
purple = dependency('purple', version: '>= @0@'.format(purple_min_version))

add_project_arguments(
    '-fvisibility=hidden',
    '-Wno-unused-parameter',
    language: 'c'
)

header_conf = configuration_data()
header_conf.set_quoted('PACKAGE_NAME', meson.project_name())
header_conf.set_quoted('PACKAGE_VERSION', meson.project_version())
header_conf.set_quoted('MODULES_VERSION', meson.project_version())
header_conf.set_quoted('GETTEXT_PACKAGE', meson.project_name())
header_conf.set_quoted('PURPLE_EVENTS_PLUGIN_ID', 'core-@0@-@1@'.format(package_developer, meson.project_name()))
header_conf.set_quoted('PURPLE_DATADIR', purple.get_variable('datadir'))
header_conf.set('PURPLE_EVENTS_EXPORT', '__attribute__((visibility("default")))')

config_h = configure_file(output: 'config.h', configuration: header_conf)

plugindir = purple.get_variable('plugindir')

install_headers(files(
        'include/purple-events.h',
    ),
    subdir: 'purple-events',
)
incdirs = include_directories('include')

purple_events_lib = library('purple-events', config_h, files(
        'include/purple-events.h',
        'src/lib/utils.c',
    ),
    version: '1.0.0',
    dependencies: [ glib, purple ],
    include_directories: [ incdirs ],
    install: true,
)

purple_events = declare_dependency(link_with: purple_events_lib, include_directories: [ incdirs ])

pkgconfig = import('pkgconfig')

pkgconfig.generate(purple_events_lib,
    filebase: meson.project_name(),
    name: meson.project_name(),
    version: meson.project_version(),
    description: 'libpurple events handling plugin and library',
    subdirs: 'purple-events',
)

install_data(files(
        'vapi/purple-events.deps',
        'vapi/purple-events.vapi',
    ),
    install_dir: get_option('datadir') / 'vala' / 'vapi'
)


shared_module('events', config_h, files(
        'include/purple-events.h',
        'src/plugin/utils.h',
        'src/plugin/utils.c',
        'src/plugin/ui.h',
        'src/plugin/ui.c',
        'src/plugin/callbacks.h',
        'src/plugin/callbacks.c',
        'src/plugin/context.h',
        'src/plugin/events.h',
        'src/plugin/events.c',
    ),
    name_prefix: 'purple-',
    dependencies: [ glib, purple ],
    include_directories: [ incdirs ],
    install: true,
    install_dir: plugindir,
)

shared_module('events-exec', config_h, files(
        'include/purple-events.h',
        'src/plugin-exec/exec.c',
    ),
    name_prefix: 'purple-',
    dependencies: [ glib, purple, purple_events ],
    install: true,
    install_dir: plugindir,
)

subdir('po')
