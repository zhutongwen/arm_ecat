
aux_source_directory(. SRC_LIST) #find all source files and save in list of a variate.

INCLUDE_DIRECTORIES("./")

add_executable(clint    client.cpp tcpstream.cpp tcpconnector.cpp)
add_executable(server   server.cpp tcpstream.cpp tcpacceptor.cpp)

add_executable(UDPclint   UDPclientWithConnect.cpp)
add_executable(UDPserver   UDPserver.cpp)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
