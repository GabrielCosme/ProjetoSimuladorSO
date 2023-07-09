if(ENABLE_LINTER OR ENABLE_LINTER_FIX)
  set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

  if(ENABLE_LINTER)
    set(ENABLE_LINTER_FIX OFF)
  else(ENABLE_LINTER)
    set(CMAKE_CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY};--fix")
  endif(ENABLE_LINTER)

  if(CATKIN_ENABLE_TESTING)
    set(CMAKE_CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY};--checks=\
                                -cppcoreguidelines-owning-memory,\
                                -cppcoreguidelines-special-member-functions,\
                                -cppcoreguidelines-avoid-magic-numbers,\
                                -readability-magic-numbers,\
                                -cppcoreguidelines-avoid-goto")
  endif(CATKIN_ENABLE_TESTING)
endif()
