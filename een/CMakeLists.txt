cmake_minimum_required(VERSION 3.28.3)
project(een)

file(GLOB_RECURSE SOURCE
  src/*.cpp
)
add_library(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME}  PRIVATE
  "."
  "include"
  "../attributes/include"
  "/usr/local/include/mosquitto_client"
  "../embedded_common/connection/include"
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME}
  libtahu.a
  libmosquitto_client.a
  libmosquitto.so
  libconnections.a
  libiothub_client.a
  libiothub_client_mqtt_transport.a
  libumqtt.a
  libaziotsharedutil.a
  libparson.a
  libprov_auth_client.a
  libcurl.so
  libssl.so
  libcrypto.so
  libhsm_security_client.a
  libutpm.a
  libuuid.so
  libuhttp.a
  attributes
)