cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/BruxRelease)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/BruxRelease)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/BruxRelease)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/BruxInstall)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/" CACHE STRING "" FORCE)

if(MSVC)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
            CACHE STRING "Vcpkg toolchain file")
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/vcpkg/vcpkg.exe")
        execute_process(COMMAND "${CMAKE_SOURCE_DIR}/vcpkg/bootstrap-vcpkg.bat" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
    set(MAKE_CMD "vcpkg.exe")
    list(APPEND MAKE_CMD install sdl2 sdl2-image sdl2-mixer sdl2-net sdl2-ttf sdl2-gfx)
    set(ENV{VCPKG_ROOT} ${CMAKE_SOURCE_DIR}/vcpkg)
    execute_process(COMMAND ${MAKE_CMD} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vcpkg)
    execute_process(COMMAND "cmake ." WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
    #set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/vcpkg/installed/x86-windows/share/sdl2" CACHE STRING "" FORCE)

endif()
if (WIN32 AND NOT MINGW)
    #set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/winLibs" CACHE STRING "" FORCE)
    #include_directories(${PROJECT_SOURCE_DIR}/winLibs/include)
endif ()
# set the project name
project(brux-gdk)

# add the executable
# add_executable(Tutorial tutorial.cxx)

add_subdirectory(rte)

install(CODE [[

  file(GET_RUNTIME_DEPENDENCIES
    RESOLVED_DEPENDENCIES_VAR MyDeps
    UNRESOLVED_DEPENDENCIES_VAR FailFind
    EXECUTABLES  $<TARGET_FILE:brux-gdk>
    )
  file(GLOB RemoveWin ${CMAKE_BINARY_DIR}/BruxInstall/bin/*.exe)
  file(REMOVE ${RemoveWin})
  file(COPY $<TARGET_FILE:brux-gdk> DESTINATION ${CMAKE_BINARY_DIR}/BruxInstall/bin)
  file(COPY ../test/ DESTINATION ${CMAKE_BINARY_DIR}/BruxInstall/bin)
  file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/BruxInstall/include)
  file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/BruxInstall/lib)
  list(FILTER FailFind EXCLUDE REGEX ".*appmodel.*")
  list(FILTER MyDeps EXCLUDE REGEX "C:/WINDOWS/system32/.*")
  message("${MyDeps}")
  message("${FailFind}")
  # copy dependencies at first level
  FOREACH(item ${MyDeps})
    file(COPY ${item} DESTINATION ${CMAKE_BINARY_DIR}/BruxInstall/bin)
  ENDFOREACH()
  file(GLOB Libs ${CMAKE_BINARY_DIR}/BruxInstall/bin/*.dll)
    file(GET_RUNTIME_DEPENDENCIES
    RESOLVED_DEPENDENCIES_VAR MyDeps
    UNRESOLVED_DEPENDENCIES_VAR FailFind
    LIBRARIES ${Libs}
    )
  list(FILTER FailFind EXCLUDE REGEX ".*appmodel.*")
  list(FILTER MyDeps EXCLUDE REGEX "C:/WINDOWS/system32/.*")
  message("${MyDeps}")
  message("${FailFind}")
  FOREACH(item ${MyDeps})
    file(COPY ${item} DESTINATION ${CMAKE_BINARY_DIR}/BruxInstall/bin)
  ENDFOREACH()
        if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
            file(COPY ../WinLibs/lib/x64/ DESTINATION ${CMAKE_BINARY_DIR}/BruxInstall/bin)
        else ()
            file(COPY ../WinLibs/lib/x86/ DESTINATION ${CMAKE_BINARY_DIR}/BruxInstall/bin)
        endif ()
        file(GLOB RemoveWin ${CMAKE_BINARY_DIR}/BruxInstall/bin/*.lib)
        file(REMOVE ${RemoveWin})
  ]])