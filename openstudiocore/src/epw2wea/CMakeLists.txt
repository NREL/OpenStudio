ADD_EXECUTABLE( epw2wea
  epw2wea.c
)

IF (MSVC)
  SET(EPW2WEA_EXE "${CMAKE_BINARY_DIR}/Products/$(ConfigurationName)/epw2wea" PARENT_SCOPE)
ELSE()
  SET(EPW2WEA_EXE "${CMAKE_BINARY_DIR}/Products/epw2wea" PARENT_SCOPE)
ENDIF()

INSTALL(TARGETS epw2wea
    BUNDLE DESTINATION . 
    RUNTIME DESTINATION bin 
    )
