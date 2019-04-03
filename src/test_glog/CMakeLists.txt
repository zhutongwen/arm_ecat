aux_source_directory(. SRC_LIST) #find all source files and save in list of a variate.

#INCLUDE_DIRECTORIES("./log")
INCLUDE_DIRECTORIES("/usr/local/include")


add_executable(test_glog  main.cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

target_link_libraries(test_glog  ${CMAKE_SOURCE_DIR}/src/test_glog/glog/lib/libglog.a)
