cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

option(ENABLE_ASAN "Enable Address Sanitizer" FALSE)
option(ENABLE_UBSAN "Enable Undefined Behaviour Sanitizer" FALSE)
option(ENABLE_LSAN "Enable Leak Sanitizer" FALSE)

if(ENABLE_ASAN)
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

if(ENABLE_UBSAN)
  add_compile_options(-fsanitize=undefined)
  add_link_options(-fsanitize=undefined)
endif()

if(ENABLE_LSAN)
  add_compile_options(-fsanitize=leak)
  add_link_options(-fsanitize=leak)
endif()
