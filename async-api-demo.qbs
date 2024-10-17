CppApplication {
    consoleApplication: true
    cpp.cxxLanguageVersion: "c++23"
    cpp.enableRtti: false
    install: true
    files: [
        "async_api.cpp",
        "async_api.hpp",
        "main.cpp",
        "thread_pool.cpp",
        "thread_pool.hpp",
    ]
}
