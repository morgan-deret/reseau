PROJECT(lan CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
ADD_DEFINITIONS(-Wall -std=gnu++0x -ggdb)
ADD_EXECUTABLE(server server.cc)
ADD_EXECUTABLE(client client.cc)
TARGET_LINK_LIBRARIES(client -Wl,--copy-dt-needed-entries -lnetez)
TARGET_LINK_LIBRARIES(server -Wl,--copy-dt-needed-entries -lnetez)

