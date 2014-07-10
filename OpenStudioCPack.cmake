file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Modules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/Modules")

set(CPACK_PACKAGE_VENDOR "National Renewable Energy Laboratory")
set(CPACK_PACKAGE_CONTACT "OpenStudio@nrel.gov")

set(CPACK_PACKAGE_VERSION_MAJOR "${CMAKE_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${CMAKE_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${CMAKE_VERSION_PATCH}")
set(FULL_VERSION_STRING ${OPENSTUDIO_VERSION}.${CMAKE_VERSION_BUILD})

if(CMAKE_CL_64)
  set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${FULL_VERSION_STRING}-Win64")
  set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
elseif(UNIX AND NOT APPLE AND NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
  set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${FULL_VERSION_STRING}-${CMAKE_SYSTEM_NAME}-x86")
else()
  set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${FULL_VERSION_STRING}-${CMAKE_SYSTEM_NAME}")
endif()

if(PACKAGE_RADIANCE_AND_ENERGYPLUS)
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-InternalUseOnly")
endif()

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/copyright.txt")

set(CPACK_PACKAGE_EXECUTABLES "OpenStudio" "OpenStudio"
                               "Pat" "ParametricAnalysisTool"
                               "ResultsViewer" "ResultsViewer"
                               "RunManager" "RunManager"
)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenStudio is a cross-platform collection of software tools to support whole building energy modeling using EnergyPlus and advanced daylight analysis using Radiance. OpenStudio is an open source project to facilitate community development, extension, and private sector adoption. It includes graphical interfaces along with a Software Development Kit (SDK).")

set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CPack.Description.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/CPack.Description.txt")
set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_SOURCE_DIR}/CPack.Welcome.txt")

set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "OpenStudio\n ${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-thread1.46.1, libboost-system1.46.1, libboost-regex1.46.1, libboost-filesystem1.46.1, libboost-date-time1.46.1, libboost-program-options1.46.1, libboost-serialization1.46.1, libqt4-sql, libqt4-network, libruby1.8, ruby1.8, ruby, libqt4-sql-sqlite, libqtgui4, libqt4-xml, libqt4-webkit")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://openstudio.nrel.gov")
set(CPACK_DEBIAN_PACKAGE_SECTION "science")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/CMake/debian/postinst")

if(APPLE)
  set(CPACK_OSX_PACKAGE_VERSION 10.8)
  set(CPACK_PACKAGING_INSTALL_PREFIX "/Applications" CACHE PATH "Package install location")
  set(CPACK_PACKAGING_INSTALL_PREFIX "${CPACK_PACKAGING_INSTALL_PREFIX}/OpenStudio ${OPENSTUDIO_VERSION}")

  if(CMAKE_OSX_DEPLOYMENT_TARGET MATCHES ".*10\\.9.*" OR CMAKE_OSX_SYSROOT MATCHES ".*10\\.9.*")
    set(MAC_RUBY_2 "true")
  else()
    set(MAC_RUBY_2 "false")
  endif()
  list(FIND CMAKE_OSX_ARCHITECTURES "i386" I386_FOUND)
  if(NOT I386_FOUND EQUAL -1)
    set(I386_FOUND "true")
  else()
    set(I386_FOUND "false")
  endif()

  configure_file("${CMAKE_SOURCE_DIR}/DarwinPostFlight.sh.in" "${CMAKE_BINARY_DIR}/DarwinPostFlight.sh")
  set(CPACK_POSTFLIGHT_SCRIPT "${CMAKE_BINARY_DIR}/DarwinPostFlight.sh")
  set(CPACK_PACKAGE_RELOCATABLE false)
elseif(UNIX)
  set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
else()
  # Icon in Control Panel->Programs.
  set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/openstudiocore/icons/os.ico")
  configure_file("${CMAKE_SOURCE_DIR}/openstudiocore/icons/nsis.bmp" "${CMAKE_BINARY_DIR}/nsis.bmp" COPYONLY)
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\OpenStudio.exe")
  set(CPACK_NSIS_URL_INFO_ABOUT "http://openstudio.nrel.gov/")
  set(CPACK_NSIS_HELP_LINK "http://openstudio.nrel.gov/forum")
  # Registry keys
  set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
    WriteRegStr HKLM 'Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\App Paths\\\\OpenStudio.exe' '' '$INSTDIR\\\\bin\\\\OpenStudio.exe'
    WriteRegStr HKLM 'Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\App Paths\\\\Pat.exe' '' '$INSTDIR\\\\bin\\\\Pat.exe'

    WriteRegStr HKCR 'Applications\\\\OpenStudio.exe\\\\shell\\\\open\\\\command' '' '$INSTDIR\\\\bin\\\\OpenStudio.exe \\\"%1\\\"'
    WriteRegStr HKCR 'Applications\\\\Pat.exe\\\\shell\\\\open\\\\command' '' '$INSTDIR\\\\bin\\\\Pat.exe \\\"%1\\\"'

    WriteRegStr HKCR 'Applications\\\\OpenStudio.exe\\\\SupportedTypes' '.osm' ''
    WriteRegStr HKCR 'Applications\\\\Pat.exe\\\\SupportedTypes' '.osp' ''

    WriteRegStr HKCR '.osm\\\\OpenWithList\\\\OpenStudio.exe' '' ''
    WriteRegStr HKCR '.osp\\\\OpenWithList\\\\Pat.exe' '' ''

    WriteRegStr HKCR '.osc' '' 'OpenStudio.osc'
    WriteRegStr HKCR 'OpenStudio.osc' '' 'OpenStudio Component'
    WriteRegStr HKCR 'OpenStudio.osc\\\\DefaultIcon' '' '$INSTDIR\\\\bin\\\\OpenStudio.exe,2'

    WriteRegStr HKCR '.osm' '' 'OpenStudio.osm'
    WriteRegStr HKCR 'OpenStudio.osm' '' 'OpenStudio Model'
    WriteRegStr HKCR 'OpenStudio.osm\\\\DefaultIcon' '' '$INSTDIR\\\\bin\\\\OpenStudio.exe,1'
    WriteRegStr HKCR 'OpenStudio.osm\\\\\\\\shell\\\\open\\\\command' '' '$INSTDIR\\\\bin\\\\OpenStudio.exe \\\"%1\\\"'

    WriteRegStr HKCR '.osp' '' 'OpenStudio.osp'
    WriteRegStr HKCR 'OpenStudio.osp' '' 'OpenStudio Project'
    WriteRegStr HKCR 'OpenStudio.osp\\\\DefaultIcon' '' '$INSTDIR\\\\bin\\\\Pat.exe,1'
    WriteRegStr HKCR 'OpenStudio.osp\\\\\\\\shell\\\\open\\\\command' '' '$INSTDIR\\\\bin\\\\Pat.exe \\\"%1\\\"'

    WriteRegStr HKCR '.ossr' '' 'OpenStudio.ossr'
    WriteRegStr HKCR 'OpenStudio.ossr' '' 'OpenStudio Standard Result'
    WriteRegStr HKCR 'OpenStudio.ossr\\\\DefaultIcon' '' '$INSTDIR\\\\bin\\\\OpenStudio.exe,3'

    WriteRegStr HKCR '.osp-journal' '' 'OpenStudio.osp-journal'
    WriteRegStr HKCR 'OpenStudio.osp-journal' '' 'OpenStudio Project Journal'

    System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'
  ")
  set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
    DeleteRegKey HKLM 'Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\App Paths\\\\OpenStudio.exe'
    DeleteRegKey HKLM 'Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\App Paths\\\\Pat.exe'
    DeleteRegKey HKCR 'Applications\\\\OpenStudio.exe'
    DeleteRegKey HKCR 'Applications\\\\Pat.exe'
    DeleteRegKey HKCR '.osm\\\\OpenWithList\\\\OpenStudio.exe'
    DeleteRegKey HKCR '.osp\\\\OpenWithList\\\\Pat.exe'
    DeleteRegKey HKCR 'OpenStudio.osc'
    DeleteRegKey HKCR 'OpenStudio.osm'
    DeleteRegKey HKCR 'OpenStudio.osp'
    DeleteRegKey HKCR 'OpenStudio.osp-journal'
    DeleteRegKey HKCR 'OpenStudio.ossr'
  ")
endif()


###############################################################################
# Additional install commands

set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/;Required;ALL;/")

if(BUILD_SIMXML)
  list(APPEND CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR}/SimXml-prefix/src/SimXml-build/;Required;ALL;/")
endif()

include(CPack)

if(APPLE)
  configure_file(CPack.Info.plist.in.in "${CMAKE_BINARY_DIR}/Modules/CPack.Info.plist.in")
endif()
