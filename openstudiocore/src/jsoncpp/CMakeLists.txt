set(target_name jsoncpp)

add_library(${target_name} STATIC
  json.h
  json-forwards.h
  jsoncpp.cpp
)

