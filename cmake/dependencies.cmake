include(FetchContent)

# Helper function to fetch content without polluting the IDE
# ------------------------------
function(fetch_hide_from_ide name)
    set(options )
    set(oneValueArgs GIT_REPOSITORY GIT_TAG CMAKE_ARGS)
    cmake_parse_arguments(FH "${options}" "${oneValueArgs}" "" ${ARGN})

    # Download and make available
    FetchContent_Declare(${name}
            GIT_REPOSITORY ${FH_GIT_REPOSITORY}
            GIT_TAG ${FH_GIT_TAG}
            CMAKE_ARGS ${FH_CMAKE_ARGS}
    )

    # Make available while hiding from IDE
    FetchContent_GetProperties(${name})
    if(NOT ${name}_POPULATED)
        # This replaces Populate + add_subdirectory
        FetchContent_MakeAvailable(${name})

        # Optional: still hide in IDE (EXCLUDE_FROM_ALL equivalent)
        # Some projects don't need this if they provide proper targets
        if(TARGET ${name})
            set_target_properties(${name} PROPERTIES EXCLUDE_FROM_ALL TRUE)
        endif()
    endif()
endfunction()
# ------------------------------

# ------------------------------
# OpenGL
# ------------------------------
find_package(OpenGL REQUIRED)
set(OpenGL_GL_LIBRARY OpenGL::GL)

# ------------------------------
# FreeType
# ------------------------------
find_package(Freetype QUIET)
if(NOT Freetype_FOUND)
    message(STATUS "FreeType not found, fetching with FetchContent...")

    fetch_hide_from_ide(
        freetype
        GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
        GIT_TAG VER-2-13-2
    )

    set(FREETYPE_TARGET freetype)
else()
    message(STATUS "Using system-installed FreeType")
    set(FREETYPE_TARGET Freetype::Freetype)
endif()

# ------------------------------
# OGG (libogg)
# ------------------------------
find_package(Ogg QUIET)
if(NOT Ogg_FOUND)
    message(STATUS "Ogg not found, fetching with FetchContent...")

    fetch_hide_from_ide(
            ogg
            GIT_REPOSITORY https://github.com/xiph/ogg.git
            GIT_TAG v1.3.6  # latest stable tag at the moment
            CMAKE_ARGS
            -DCMAKE_POLICY_VERSION=3.5
    )

    set(OGG_TARGET ogg)
    set(OGG_INCLUDE_DIR ${ogg_SOURCE_DIR}/include)
    set(OGG_LIBRARY ${ogg_BINARY_DIR}/src/.libs/libogg.a)
else()
    message(STATUS "Using system-installed Ogg")
    set(OGG_TARGET Ogg::ogg)

    set(OGG_INCLUDE_DIR ${Ogg_INCLUDE_DIR})
    set(OGG_LIBRARY ${Ogg_LIBRARY})
endif()

# ------------------------------
# Vorbis (libvorbis)
# ------------------------------
find_package(Vorbis QUIET)
if(NOT Vorbis_FOUND)
    message(STATUS "Vorbis not found, fetching with FetchContent...")

    fetch_hide_from_ide(
            vorbis
            GIT_REPOSITORY https://github.com/xiph/vorbis.git
            GIT_TAG v1.3.7  # latest stable tag at the moment
            CMAKE_ARGS
            -DOGG_INCLUDE_DIR=${OGG_INCLUDE_DIR}
            -DOGG_LIBRARY=${OGG_LIBRARY}
    )

    set(VORBIS_TARGET vorbis)
    set(VORBISFILE_TARGET vorbisfile)
else()
    message(STATUS "Using system-installed Vorbis")
    set(VORBIS_TARGET Vorbis::vorbis)
    set(VORBISFILE_TARGET Vorbis::vorbisfile)
endif()

# ------------------------------
# OpenAL-Soft
# ------------------------------
find_package(OpenAL QUIET)
if(NOT OpenAL_FOUND)
    message(STATUS "OpenAL not found, fetching with FetchContent...")

    fetch_hide_from_ide(
            openal-soft
            GIT_REPOSITORY https://github.com/kcat/openal-soft.git
            GIT_TAG 1.22.1  # use latest stable release or tag you want
    )

    set(OPENAL_TARGET OpenAL::OpenAL)
else()
    message(STATUS "Using system-installed OpenAL")
    set(OPENAL_TARGET OpenAL::OpenAL)
endif()

# ------------------------------
# SDL2 (update to SDL3 when everything works)
# ------------------------------
find_package(SDL3 QUIET)
if(NOT SDL3_FOUND)
    message(STATUS "SDL3 not found, fetching with FetchContent...")
    
    fetch_hide_from_ide(
            sdl3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-3.2.0  # latest 3.2 patch version
    )

    set(SDL3_TARGET SDL3-static)  # or SDL3-static if appropriate
else()
    message(STATUS "Using system-installed SDL2")
    set(SDL3_TARGET SDL3::SDL3)
endif()