
find_package(SDL2 CONFIG REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
if (NOT WIN32)
    find_package(SDL2_image REQUIRED)
    include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
    find_package(SDL2_net REQUIRED)
    include_directories(${SDL2_NET_INCLUDE_DIRS})
    find_package(SDL2_mixer REQUIRED)
    include_directories(${SDL2_MIXER_INCLUDE_DIRS})
    find_package(SDL2_gfx REQUIRED)
    include_directories(${SDL2_GFX_INCLUDE_DIRS})
endif ()
if (MINGW)
    find_package(SDL2_image REQUIRED)
    include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
    find_package(SDL2_net REQUIRED)
    include_directories(${SDL2_NET_INCLUDE_DIRS})
    find_package(SDL2_mixer REQUIRED)
    include_directories(${SDL2_MIXER_INCLUDE_DIRS})
    find_package(SDL2_gfx REQUIRED)
    include_directories(${SDL2_GFX_INCLUDE_DIRS})
endif ()
if(MSVC)
    find_package(SDL2-image CONFIG REQUIRED)
    include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
    find_package(SDL2-net CONFIG REQUIRED)
    include_directories(${SDL2_NET_INCLUDE_DIRS})
    find_package(SDL2-mixer CONFIG REQUIRED)
    include_directories(${SDL2_MIXER_INCLUDE_DIRS})
    find_package(SDL2-gfx CONFIG REQUIRED)
    include_directories(${SDL2_GFX_INCLUDE_DIRS})
endif()
SET(brux_gtk_sources
        audio.cpp
        binds.cpp
        cJSON.c
        core.cpp
        fileio.cpp
        global.cpp
        graphics.cpp
        input.cpp
        main.cpp
        maths.cpp
        shapes.cpp
        sprite.cpp
        text.cpp
        tile.cpp
        tinyxml2.cpp
        tmap.cpp
         Phyisics.cpp)

add_executable(brux-gdk ${brux_gtk_sources})
if (NOT WIN32)
    target_link_libraries(brux-gdk squirrelStatic stdsquirrelStatic ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_NET_LIBRARIES} ${SDL2_GFX_LIBRARIES} ${SDL2_MIXER_LIBRARIES} chipmonkStatic)
endif ()

if (WIN32 OR MINGW)
    if (MINGW)
        target_link_libraries(brux-gdk squirrelStatic stdsquirrelStatic  SDL2::SDL2 SDL2::SDL2main ${SDL2_IMAGE_LIBRARIES} ${SDL2_NET_LIBRARIES} ${SDL2_GFX_LIBRARIES} ${SDL2_MIXER_LIBRARIES} chipmonkStatic -lmingw32 -mwindows)
    else ()
        target_link_libraries(brux-gdk SDL2::SDL2 SDL2::SDL2main SDL2::SDL2_mixer SDL2::SDL2_image SDL2::SDL2_gfx squirrelStatic stdsquirrelStatic chipmonkStatic)
    endif ()
    if (NOT DEFINED ENV{TRAVIS})
        if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
            add_custom_command(TARGET brux-gdk POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/winLibs/lib/x64" $<TARGET_FILE_DIR:brux-gdk>
                    #COMMAND ${CMAKE_COMMAND} -E copy_directory "${squirrel_BINARY_DIR}/bin" $<TARGET_FILE_DIR:brux-gdk>
                    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/bin" $<TARGET_FILE_DIR:brux-gdk>
                    )
        else ()
            add_custom_command(TARGET brux-gdk POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/winLibs/lib/x86" $<TARGET_FILE_DIR:brux-gdk>
                    #COMMAND ${CMAKE_COMMAND} -E copy_directory "${squirrel_BINARY_DIR}/bin" $<TARGET_FILE_DIR:brux-gdk>
                    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/bin" $<TARGET_FILE_DIR:brux-gdk>
                    )
        endif ()
    endif ()
endif ()
add_definitions(-DSDL_MAIN_HANDLED)

include(ExternalProject)
ExternalProject_Add(squirrelstf
        GIT_REPOSITORY https://github.com/albertodemichelis/squirrel.git
        GIT_TAG master
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/BruxLibs -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        )
ExternalProject_Get_Property(squirrelstf install_dir)
add_library(squirrelStatic STATIC IMPORTED)
add_library(stdsquirrelStatic STATIC IMPORTED)
add_dependencies(squirrelStatic squirrelstf)
add_dependencies(stdsquirrelStatic squirrelstf)

ExternalProject_Add(chipmonk2d
        GIT_REPOSITORY https://github.com/slembcke/Chipmunk2D.git
        GIT_TAG 7a8ee9a80a1f6f9659828541aaaa2f0702917708
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/BruxLibs -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        )
add_library(chipmonkStatic STATIC IMPORTED)
add_dependencies(chipmonkStatic chipmonk2d)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/BruxLibs/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/BruxLibs/include/chipmunk)

if(WIN32)
    set_property(TARGET squirrelStatic PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/BruxLibs/lib/squirrel_static.lib)
    set_property(TARGET stdsquirrelStatic PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/BruxLibs/lib/sqstdlib_static.lib)
    set_property(TARGET chipmonkStatic PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/BruxLibs/lib/chipmunk.lib)
else()
    set_property(TARGET squirrelStatic PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/BruxLibs/lib/libsquirrel_static.a)
    set_property(TARGET stdsquirrelStatic PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/BruxLibs/lib/libsqstdlib_static.a)
    set_property(TARGET chipmonkStatic PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/BruxLibs/lib/libchipmunk.a)
endif()