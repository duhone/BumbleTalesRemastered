cmake_minimum_required (VERSION 4.5 FATAL_ERROR)

set(root "${CMAKE_CURRENT_LIST_DIR}")

set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo;Profile;Final") 

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project (BumbleTales LANGUAGES CXX C)
  
include (${root}/conjure/common.cmake)
include (${root}/conjure/3rdParty/build/build.cmake)
include (${root}/conjure/Engine/build/build.cmake)
include (${root}/conjure/Tools/build/build.cmake)
include (${root}/Game/build/build.cmake)
	