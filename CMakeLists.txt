cmake_minimum_required (VERSION 3.11)

project(rw-tui
  LANGUAGES CXX
  VERSION 1.0.0
)

# Set compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable clang-tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy" DOC "Path to clang-tidy executable")
if(CLANG_TIDY_EXE)
  set(CMAKE_CXX_CLANG_TIDY 
    ${CLANG_TIDY_EXE};
    -checks=*,-fuchsia-*,-google-*,-zircon-*,-abseil-*,-modernize-use-trailing-return-type;
    -warnings-as-errors=*;
  )
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
else()
  message(STATUS "clang-tidy not found.")
endif()

file(GLOB_RECURSE RW_TUI_SRC
     "src/*.cpp"
)
 
add_executable(rw-tui ${RW_TUI_SRC})
target_include_directories(rw-tui PRIVATE src/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/lib/ftxui ${CMAKE_BINARY_DIR}/ftxui EXCLUDE_FROM_ALL)
 
target_link_libraries(rw-tui
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  -static
)

# Add installation rules
install(TARGETS rw-tui DESTINATION bin)