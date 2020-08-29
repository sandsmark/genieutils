# Available settings:
#
# GUTILS_TOOLS:BOOL     if true to enable compilation of gutils tools
# GUTILS_TEST:BOOL      if true some debug/test classes will be compiled

cmake_minimum_required(VERSION 3.13)

###### macos is absolute shit ########
if (APPLE)
    message(WARNING "\n"
        "#############################################################################################\n"
        "   MacOS does not have proper C++ support, so bug reports not accepted, you're on your own\n"
        "#############################################################################################\n"
        )
endif()

project(genieutils)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Let freeaoe set this with `set(GUTILS_STATIC True)`
cmake_policy(SET CMP0077 NEW)

option(ENABLE_SANITIZERS "Enable runtime sanitizing (for development)")
option(GUTILS_TOOLS "Build extra tools")
option(GUTILS_TEST "Build tests")
option(GUTILS_STATIC "Build static library so it can be built into the final executable")

set(Genieutils_LIBRARY genieutils)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/)

if (ENABLE_IPO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT IPO_AVAILABLE OUTPUT IPO_ERROR)
    if (IPO_AVAILABLE)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION True)
    else()
        message(STATUS "IPO / LTO not supported: <${IPO_ERROR}>")
    endif()
else()
    message(STATUS "Not enabling IPO/LTO")
endif()


# dependencies:

find_package(ZLIB QUIET)
if (ZLIB_FOUND)
    set(ZLIB_LIBRARIES ZLIB::ZLIB)
else()
    message(WARNING "zlib not found, falling back to bundled miniz. This might hurt performance, especially on 32bit")

    set(DAT_SRC ${DAT_SRC}
        extern/miniz/miniz.c
    )
    include_directories(extern/miniz)
    set(ZLIB_LIBRARIES)
endif()


if (NOT WIN32)
    find_package(Iconv REQUIRED)

    if (ICONV_SECOND_ARGUMENT_IS_CONST)
        add_definitions(-DICONV_SECOND_ARGUMENT_IS_CONST)
    endif()
else()
    message(STATUS "Using bundled win-iconv")
    add_definitions(-DICONV_SECOND_ARGUMENT_IS_CONST)
    include_directories(extern/win-iconv/)
endif()

if (GUTILS_TOOLS)
  find_package(Boost 1.55 COMPONENTS program_options REQUIRED)
endif(GUTILS_TOOLS)

if (GUTILS_TEST)
  message(STATUS "Building tests")

  find_package(Boost 1.55 COMPONENTS unit_test_framework program_options REQUIRED)

  set(Boost_USE_MULTITHREADED      ON)
  set(GU_INCLUDE_DIRS ${GU_INCLUDE_DIRS})
endif (GUTILS_TEST)

if (ENABLE_SANITIZERS)
    message("Enabling asan and ubsan")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif()

# TODO: write proper cmake config for pcrio
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/extern/pcrio/pcrio.c)
    message(STATUS "Using bundled pcrio")
    set(PCRIO_LIBRARIES)
    set(PCRIO_SRC extern/pcrio/pcrio.c)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/pcrio/)
else()
    set(PCRIO_LIBRARIES pcrio)
    set(PCRIO_SRC)
endif()

# include directories:

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/extern/zstr/src/zstr.hpp)
    message(STATUS "Using bundled zstr")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/zstr/src/)
endif()

set(GU_INCLUDE_DIRS include/)

if(WIN32)
  set(GU_INCLUDE_DIRS ${GU_INCLUDE_DIRS})
endif()

if (${ICONV_FOUND})
  set(GU_INCLUDE_DIRS ${GU_INCLUDE_DIRS} ${ICONV_INCLUDE_DIR})
endif (${ICONV_FOUND})

include_directories(${GU_INCLUDE_DIRS})

if(NOT MSVC)
    add_definitions(-Wall -Wsign-compare)
else()
    add_definitions(/wd4244 /wd4018 /wd4267 /wd4996 /wd4800)
endif()

#------------------------------------------------------------------------------#
# Source files: 
#------------------------------------------------------------------------------#
set(FILE_SRC
    src/file/ISerializable.cpp
    src/file/IFile.cpp
    src/file/Compressor.cpp
    src/file/CabFile.cpp
    src/file/LZ4.cpp
    #    src/file/lzx.c
)



set(LANG_SRC ${PCRIO_SRC}
    src/lang/LangFile.cpp
)

if (WIN32)
    set(LANG_SRC ${LANG_SRC}
        extern/win-iconv/win_iconv.c
    )
    set(ICONV_LIBRARIES)
endif()

set(DAT_SRC ${DAT_SRC}
    src/dat/Research.cpp 
   
    src/dat/Civ.cpp
    src/dat/TechageEffect.cpp
    src/dat/Techage.cpp

    src/dat/TechTree.cpp
    src/dat/TerrainCommon.cpp
    src/dat/Terrain.cpp
    src/dat/TerrainBlock.cpp
    src/dat/TerrainBorder.cpp
    src/dat/GraphicAttackSound.cpp
    src/dat/GraphicDelta.cpp
    src/dat/Graphic.cpp
    src/dat/SoundItem.cpp
    src/dat/Sound.cpp
    src/dat/PlayerColour.cpp
    src/dat/DatFile.cpp
    src/dat/TerrainPassGraphic.cpp
    src/dat/TerrainRestriction.cpp

    src/dat/Unit.cpp
    src/dat/UnitCommand.cpp
    src/dat/UnitHeader.cpp
    src/dat/UnitLine.cpp
    src/dat/RandomMap.cpp

    src/dat/unit/AttackOrArmor.cpp
    src/dat/unit/DamageGraphic.cpp
    src/dat/unit/Moving.cpp
    src/dat/unit/Action.cpp
    src/dat/unit/Combat.cpp
    src/dat/unit/Missile.cpp
    src/dat/unit/Creatable.cpp
    src/dat/unit/Building.cpp
    )

set(RESOURCE_SRC
    src/resource/PalFile.cpp
    src/resource/SlpFile.cpp
    src/resource/SlpFrame.cpp
    src/resource/SlpTemplate.cpp
    src/resource/DrsFile.cpp
    src/resource/Color.cpp
    src/resource/BinaFile.cpp
    src/resource/UIFile.cpp
    src/resource/BlendomaticFile.cpp
    src/resource/EdgeFiles.cpp
    src/resource/WavFile.cpp
    src/resource/SmpFrame.cpp
    src/resource/SmpFile.cpp
    src/resource/SmxFile.cpp
    src/resource/SmxFrame.cpp
    )

set(SCRIPT_SRC
    src/script/ScnFile.cpp
    src/script/scn/ScnResource.cpp
    src/script/scn/ScnPlayerData.cpp
    src/script/scn/MapDescription.cpp
    src/script/scn/Trigger.cpp
    )

set(UTIL_SRC
    src/util/Logger.cpp
    src/util/Utility.cpp
    )

# Tool sources:

set(TEST_SRC
    src/tools/test/test.cpp
    src/tools/bincompare/bincomp.cpp
   )

set(EXTRACT_SRC src/tools/extract/datextract.cpp)

set(DRSTOOL_SRC src/tools/drstool/drstool.cpp)
set(CPXTOOL_SRC src/tools/cpxtool/cpxtool.cpp)
set(PCRIOTOOL_SRC src/tools/pcriotool/pcriotool.cpp)

set(BINCOMP_SRC src/tools/bincompare/bincomp.cpp
                src/tools/bincompare/main.cpp)

#------------------------------------------------------------------------------#
# Executeable:
#------------------------------------------------------------------------------#

set(ALL_SRC
    ${FILE_SRC} ${LANG_SRC} ${DAT_SRC}
    ${RESOURCE_SRC} ${UTIL_SRC} ${SCRIPT_SRC}
)

if (GUTILS_STATIC)
    message(STATUS "Building genieutils as static library")
    add_library(genieutils STATIC ${ALL_SRC})
else()
    add_library(genieutils SHARED ${ALL_SRC})
    set_property(TARGET genieutils PROPERTY POSITION_INDEPENDENT_CODE True)
endif()

install (
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h*"
    )

install(TARGETS genieutils)

target_link_libraries(genieutils
    ${PCRIO_LIBRARIES}
    ${ICONV_LIBRARIES}
    ${ZLIB_LIBRARIES}
    )


#------------------------------------------------------------------------------#
# Dev tools:
#------------------------------------------------------------------------------#

if (GUTILS_TOOLS)
  message(STATUS "Building tools")

  find_package(Qt5 OPTIONAL_COMPONENTS Widgets QUIET)
  if (Qt5Widgets_FOUND)
      message(STATUS "Qt5 Widgets found, enabling viewer for SLP/SMP files and DRS explorer")
      add_subdirectory(src/tools/picviewer)
      add_subdirectory(src/tools/drsexplorer)
  endif()

  add_executable(genie-datextract ${EXTRACT_SRC})
  install(TARGETS genie-datextract)
  target_link_libraries(genie-datextract genieutils ${Boost_LIBRARIES})

  add_executable(drstool ${DRSTOOL_SRC})
  install(TARGETS drstool)
  target_link_libraries(drstool genieutils)

  add_executable(cpxtool ${CPXTOOL_SRC})
  install(TARGETS cpxtool)
  target_link_libraries(cpxtool genieutils)

  add_executable(pcriotool ${PCRIOTOOL_SRC})
  install(TARGETS pcriotool)
  target_link_libraries(pcriotool genieutils)

  add_executable(genie-bindiff ${BINCOMP_SRC})
  install(TARGETS genie-bindiff)
endif (GUTILS_TOOLS)

#------------------------------------------------------------------------------#
# Debug/Test:
#------------------------------------------------------------------------------#
if (GUTILS_TEST)
    add_library(genieutils_tests_common OBJECT
        src/tools/bincompare/bincomp.cpp
        )

    set(GU_INCLUDE_DIRS ${GU_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

    add_executable(wtftest main.cpp)
    target_link_libraries(wtftest genieutils ${Boost_LIBRARIES})

    add_executable(DatTest
        tests/DatTest.cpp
        $<TARGET_OBJECTS:genieutils_tests_common>
        )
    add_test(NAME DatTest COMMAND DatTest)
    target_link_libraries(DatTest genieutils ${Boost_LIBRARIES})

    add_executable(LangTest
        tests/LangTest.cpp
        $<TARGET_OBJECTS:genieutils_tests_common>
        )
    target_link_libraries(LangTest genieutils ${Boost_LIBRARIES})
    add_test(NAME LangTest COMMAND LangTest)

    enable_testing()
endif(GUTILS_TEST)

if (ENABLE_CLANG_TIDY)
    find_program(
        CLANG_TIDY_EXE
        NAMES "clang-tidy"
        DOC "Path to clang-tidy executable"
        )
    if(NOT CLANG_TIDY_EXE)
        message(STATUS "clang-tidy not found.")
    else()
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
        set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()
    if(CLANG_TIDY_EXE)
        set_target_properties(
            genieutils PROPERTIES
            CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
            )
    endif()
endif()
