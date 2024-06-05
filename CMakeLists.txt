add_library(elftool
    src/elf.cpp
)
target_include_directories(elftool PUBLIC include)
target_compile_features(elftool PUBLIC cxx_std_20)
