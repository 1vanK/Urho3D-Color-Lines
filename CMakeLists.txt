# Based on http://urho3d.github.io/documentation/1.4/_using_library.html

# Set project name
project (Urho3DColorLines)

# Define target name
set (TARGET_NAME Lines)

# Set paths
set (CMAKE_MODULE_PATH d:/MyGames/Urho3D/Urho3D/CMake/Modules)
set (ENV{URHO3D_HOME} d:/MyGames/Urho3D/Build)

# Set minimum version
cmake_minimum_required (VERSION 2.8.6)
if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
    if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
        # INTERFACE_LINK_LIBRARIES defines the link interface
        cmake_policy (SET CMP0022 NEW)
    endif ()
    if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
        # Disallow use of the LOCATION target property - therefore we set to OLD as we still need it
        cmake_policy (SET CMP0026 OLD)
        # MACOSX_RPATH is enabled by default
        cmake_policy (SET CMP0042 NEW)
    endif ()
endif ()

# Include Urho3D Cmake common module
include (Urho3D-CMake-common)

# Find Urho3D library
find_package (Urho3D REQUIRED)
include_directories (${URHO3D_INCLUDE_DIRS})

# Define source files
define_source_files ()

# Setup target with resource copying
setup_main_executable ()
