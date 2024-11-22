set(TARGET app)

add_executable(${TARGET} main.cpp)

target_include_directories(${TARGET} PRIVATE
  "."
  "../een/include"
)
target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_link_libraries(${TARGET}
  een
)