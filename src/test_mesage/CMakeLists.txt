aux_source_directory(. SRC_LIST) #find all source files and save in list of a variate.

INCLUDE_DIRECTORIES("./")

add_executable(test_message_sand  sand.cpp)
add_executable(test_message_receive  receive.cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


