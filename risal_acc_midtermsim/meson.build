project('my-cpp-project', 'cpp',
    version: '1.0.0',
    default_options: [
        'cpp_std=c++20',
        'warning_level=3',
        'werror=true', # Treat all warnings as errors
        'optimization=s' # Optimize for size
    ]
)

cpp_args = [
    # Recommended warnings for g++/clang
    '-Wpedantic',
    '-Wformat=2',
    '-Wundef',
    '-Wcast-align',
    '-Wwrite-strings',
    '-Wold-style-cast',
    '-Wnull-dereference',
    '-Wctor-dtor-privacy',
    '-Wno-missing-field-initializers',
]

add_global_arguments(cpp_args, language : 'cpp')

# Add Boost.Serialization dependency
boost_serialization = dependency('boost', modules : ['serialization'], required : true)

app_sources = [
    'main.cpp',
    'user.h'
# Add other source files here, e.g., 'src/helper.cpp'
]

executable('my_app',
    app_sources,
    dependencies: [boost_serialization],
    install: true
)
