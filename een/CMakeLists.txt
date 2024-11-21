cmake_minimum_required(VERSION 3.28.3)
project(een)

file(GLOB_RECURSE SOURCE
  src/*.cpp
)
add_library(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME}  PRIVATE
  "include"
  "../attributes/include"
  "../embedded_common/mosquitto_client/include"
  "../embedded_common/connection/include"
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME}
  libtahu.a
  mosquitto_client
  libmosquitto.so
  connections
  libcurl.so
  libssl.so
  libcrypto.so
  libuuid.so
  attributes
)