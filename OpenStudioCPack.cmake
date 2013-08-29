SET(CPACK_PACKAGE_VENDOR "National Renewable Energy Laboratory" )
set(CPACK_PACKAGE_CONTACT "OpenStudio@nrel.gov" )

SET(CPACK_PACKAGE_VERSION_MAJOR "${CMAKE_VERSION_MAJOR}" )
SET(CPACK_PACKAGE_VERSION_MINOR "${CMAKE_VERSION_MINOR}" )
SET(CPACK_PACKAGE_VERSION_PATCH "${CMAKE_VERSION_PATCH}" ) 
SET(FULL_VERSION_STRING ${OPENSTUDIO_VERSION}.${CMAKE_VERSION_BUILD})

IF (CMAKE_CL_64)
  SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${FULL_VERSION_STRING}-Win64") 
  SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
ElSE()
  SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${FULL_VERSION_STRING}-${CMAKE_SYSTEM_NAME}") 
ENDIF()

SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/copyright.txt") 

SET( CPACK_PACKAGE_EXECUTABLES "OpenStudio" "OpenStudio"
                               "Pat" "ParametricAnalysisTool"
                               "ResultsViewer" "ResultsViewer"
                               "RunManager" "RunManager"
   )

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenStudio is a cross-platform collection of software tools to support whole building energy modeling using EnergyPlus and advanced daylight analysis using Radiance. OpenStudio is an open source project to facilitate community development, extension, and private sector adoption. It includes graphical interfaces along with a Software Development Kit (SDK).")

SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "OpenStudio\n ${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-thread1.46.1, libboost-system1.46.1, libboost-regex1.46.1, libboost-filesystem1.46.1, libboost-date-time1.46.1, libboost-program-options1.46.1, libboost-serialization1.46.1, libqt4-sql, libqt4-network, libruby1.8, ruby1.8, ruby, libqt4-sql-sqlite, libqtgui4, libqt4-xml, libqt4-webkit")
SET(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://openstudio.nrel.gov")
SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
SET(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/CMake/debian/postinst")

IF( APPLE )
  SET( CPACK_OSX_PACKAGE_VERSION 10.7 )
  SET( CPACK_PACKAGING_INSTALL_PREFIX "/Applications" CACHE PATH "Package install location" )
  SET( CPACK_PACKAGING_INSTALL_PREFIX "${CPACK_PACKAGING_INSTALL_PREFIX}/OpenStudio ${OPENSTUDIO_VERSION}" )
  CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/DarwinPostFlight.sh.in" "${CMAKE_BINARY_DIR}/DarwinPostFlight.sh")
  SET( CPACK_POSTFLIGHT_SCRIPT "${CMAKE_BINARY_DIR}/DarwinPostFlight.sh" )
  SET( CPACK_PACKAGE_RELOCATABLE false )
ELSEIF(UNIX)
  SET( CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" )
ELSE()
  # Icon in Control Panel->Programs.
  SET( CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/openstudiocore/icons/os.ico" )
  CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/openstudiocore/icons/nsis.bmp" "${CMAKE_BINARY_DIR}/nsis.bmp" COPYONLY)
  SET( CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\OpenStudio.exe" )
  SET( CPACK_NSIS_URL_INFO_ABOUT "http://openstudio.nrel.gov/" )
  SET( CPACK_NSIS_HELP_LINK "http://openstudio.nrel.gov/forum" )
  # Registry keys
  SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
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

    WriteRegStr HKCR '.osp-journal' '' 'OpenStudio.osp-journal'
    WriteRegStr HKCR 'OpenStudio.osp-journal' '' 'OpenStudio Project Journal'

    System::Call 'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'
  ")
  SET( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
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
  ")
ENDIF()


###############################################################################
# Additional install commands

SET(CPACK_INSTALL_CMAKE_PROJECTS
   "${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/;Required;ALL;/"
    )

IF(BUILD_SIMXML)
  LIST(APPEND  CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_BINARY_DIR}/SimXml-prefix/src/SimXml-build/;Required;ALL;/")
ENDIF()

INCLUDE(CPack)
