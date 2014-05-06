add_executable(epw2wea
  epw2wea.c
)

if(MSVC)
  set(EPW2WEA_EXE "${CMAKE_BINARY_DIR}/Products/$(ConfigurationName)/epw2wea" PARENT_SCOPE)
else()
  set(EPW2WEA_EXE "${CMAKE_BINARY_DIR}/Products/epw2wea" PARENT_SCOPE)
endif()

install(TARGETS epw2wea
  BUNDLE DESTINATION .
  RUNTIME DESTINATION bin
)
