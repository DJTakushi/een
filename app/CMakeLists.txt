set(TARGET app)

add_executable(${TARGET} main.cpp)

target_include_directories(${TARGET} PRIVATE
  "."
  "../een/include"
  "../attributes/include"
  "../embedded_common/connection/include"
)
target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_link_libraries(${TARGET}
  attributes
  een
  libcurl.so
  libssl.so
  libcrypto.so
  libuuid.so
)