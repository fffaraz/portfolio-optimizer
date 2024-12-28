find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
  set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
endif()
