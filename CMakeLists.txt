# Available settings:
#
# GUTILS_TOOLS:BOOL     if true to enable compilation of gutils tools
# GUTILS_TEST:BOOL      if true some debug/test classes will be compiled

cmake_minimum_required(VERSION 3.7)

set(Genieutils_LIBRARY genieutils)

project(genieutils)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/)

# dependencies:

find_package(ZLIB QUIET)

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
  set(Boost_USE_MULTITHREADED      ON)
  find_package(Boost 1.55 COMPONENTS unit_test_framework program_options REQUIRED)
  find_package(SFML COMPONENTS SYSTEM WINDOW GRAPHICS REQUIRED)
endif (GUTILS_TEST)

# include directories:

set(GU_INCLUDE_DIRS include/ extern/zstr/src/ ../)

if(WIN32)
  set(GU_INCLUDE_DIRS ${GU_INCLUDE_DIRS})
endif()

if (${ICONV_FOUND})
  set(GU_INCLUDE_DIRS ${GU_INCLUDE_DIRS} ${ICONV_INCLUDE_DIR})
endif (${ICONV_FOUND})

if (${GUTILS_TEST})
  set(GU_INCLUDE_DIRS ${GU_INCLUDE_DIRS} ${SFML_INCLUDE_DIR})
endif (${GUTILS_TEST})

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
    src/file/lzx.c
)

set(PCRIO_SRC ../pcrio/pcrio.c)

set(LANG_SRC ${PCRIO_SRC}
    src/lang/LangFile.cpp
)

if (WIN32)
    set(LANG_SRC ${LANG_SRC}
        extern/win-iconv/win_iconv.c
    )
    set(ICONV_LIBRARIES)
endif()

set(DAT_SRC
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
    )

# Tool sources:

set(TEST_SRC
    src/tools/test/test.cpp
    src/tools/bincompare/bincomp.cpp
   )

set(EXTRACT_SRC src/tools/extract/datextract.cpp)

set(BINCOMP_SRC src/tools/bincompare/bincomp.cpp
                src/tools/bincompare/main.cpp)
                


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

#------------------------------------------------------------------------------#
# Executeable:
#------------------------------------------------------------------------------#

set(ALL_SRC
    ${FILE_SRC} ${LANG_SRC} ${DAT_SRC}
    ${RESOURCE_SRC} ${UTIL_SRC} ${SCRIPT_SRC}
)

if (GENIEUTILS_STATIC_BUILD)
    message(STATUS "Building genieutils as static library")
    add_library(${Genieutils_LIBRARY} STATIC ${ALL_SRC})
else()
    add_library(${Genieutils_LIBRARY} SHARED ${ALL_SRC})
endif()


target_link_libraries(${Genieutils_LIBRARY} ${ZLIB_LIBRARIES} ${ICONV_LIBRARIES})

INSTALL(TARGETS ${Genieutils_LIBRARY}
        LIBRARY DESTINATION lib
)

install (
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h*"
)


#add_executable(main main.cpp)
#target_link_libraries(main ${Genieutils_LIBRARY})

#------------------------------------------------------------------------------#
# Dev tools:
#------------------------------------------------------------------------------#

 # test doesn't build :(
 # add_executable(test ${TEST_SRC})
 # target_link_libraries(test ${Genieutils_LIBRARY} ${Boost_LIBRARIES} ) #${ZLIB_LIBRARIES} ${Boost_LIBRARIES} ${Genieutils_LIBRARY})

if (GUTILS_TOOLS)
  add_executable(datextract ${EXTRACT_SRC})
  target_link_libraries(datextract ${ZLIB_LIBRARIES} ${Boost_LIBRARIES} ${Genieutils_LIBRARY})

  add_executable(bincomp ${BINCOMP_SRC})
endif (GUTILS_TOOLS)

#------------------------------------------------------------------------------#
# Debug/Test:
#------------------------------------------------------------------------------#
if (GUTILS_TEST)
  set(GU_INCLUDE_DIRS ${GU_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
  add_executable(main main.cpp)
  target_link_libraries(main ${Genieutils_LIBRARY}
                      ${SFML_WINDOW_LIBRARY}
                      ${SFML_SYSTEM_LIBRARY}
                      ${SFML_GRAPHICS_LIBRARY}
                      ${Boost_LIBRARIES}
  )
endif(GUTILS_TEST)
