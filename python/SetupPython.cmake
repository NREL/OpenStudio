option(BUILD_PYTHON_PIP_PACKAGE "Prepare a folder to build a pip module" OFF)

# find_package(Python) has the problem that on github actions in particular it'll pick up the most recent python (eg 3.9) from the tool cache
# even if you have used the setup-python action and set it to 3.8
if (PYTHON_VERSION)
  find_package(Python ${PYTHON_VERSION} EXACT COMPONENTS Interpreter Development REQUIRED)
else()
  find_package(Python COMPONENTS Interpreter Development REQUIRED)
endif()

include_directories(SYSTEM ${Python_INCLUDE_DIRS})
set(ALL_PYTHON_BINDING_TARGETS "") # global list of python bindings
set(ALL_PYTHON_BINDING_DEPENDS "") # global list of library dependencies of the generated wrapper cxx files
set(ALL_PYTHON_WRAPPER_FILES "") # global list of generated wrapper cxx files
set(ALL_PYTHON_WRAPPER_TARGETS "") # global list targets that build generated wrapper cxx files
set(ALL_PYTHON_GENERATED_SRCS "") # global list of generated .py files

if(BUILD_PYTHON_PIP_PACKAGE)
  set(PipRepositoryValues "testpypi;pypi" CACHE INTERNAL
    "List of possible values for which repository to upload to")
  set(PYTHON_PIP_REPOSITORY "testpypi" CACHE STRING "Which pip repository to use for upload")
  set_property(CACHE PYTHON_PIP_REPOSITORY PROPERTY STRINGS ${PipRepositoryValues})

  if(NOT PYTHON_PIP_REPOSITORY IN_LIST PipRepositoryValues)
    message(FATAL_ERROR "PYTHON_PIP_REPOSITORY must be one of ${PipRepositoryValues}")
  endif()

  set(PYTHON_PACKAGE_FOLDER "${PROJECT_BINARY_DIR}/Products/python_package")
  file(MAKE_DIRECTORY "${PYTHON_PACKAGE_FOLDER}")
  set(ALL_PYTHON_PACKAGE_TARGETS "") # global list of python bindings
  set(ALL_PYTHON_PACKAGE_GENERATED_SRCS "") # global list of generated .py files for pyhton_package
endif()
