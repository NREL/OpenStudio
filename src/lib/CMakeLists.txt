# Some operating systems don't like compiling a library with
# no source files

add_library(openstudiolib_static STATIC empty.cpp)

target_link_libraries(
 openstudiolib_static
 PUBLIC
 openstudio_airflow
 openstudio_model
 openstudio_energyplus
 openstudio_measure
 openstudio_osversion
 openstudio_sdd
 openstudio_isomodel
 openstudio_gbxml
 openstudio_radiance
)

add_library(openstudiolib SHARED empty.cpp)

if (MSVC)
  set(ENABLE_LINK_ALL "/WHOLEARCHIVE")
  set(DISABLE_LINK_ALL "")
elseif (APPLE)
  set(ENABLE_LINK_ALL "-all_load")
  set(DISABLE_LINK_ALL "")
else()
  set(ENABLE_LINK_ALL "-Wl,--whole-archive")
  set(DISABLE_LINK_ALL "-Wl,--no-whole-archive")
endif()

target_link_libraries(
 openstudiolib
 PRIVATE
 ${ENABLE_LINK_ALL}
 openstudio_airflow
 openstudio_model
 openstudio_energyplus
 openstudio_measure
 openstudio_osversion
 openstudio_sdd
 openstudio_isomodel
 openstudio_gbxml
 openstudio_radiance
 ${DISABLE_LINK_ALL}
)


