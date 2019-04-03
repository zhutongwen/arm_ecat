aux_source_directory(. SRC_LIST) #find all source files and save in list of a variate.

INCLUDE_DIRECTORIES("./")

add_executable(test_vector  main.cpp
                            vector.cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


