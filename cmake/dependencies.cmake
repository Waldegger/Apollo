include(FetchContent)

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

    FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
        GIT_TAG VER-2-13-2
    )
    FetchContent_MakeAvailable(freetype)

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

    FetchContent_Declare(
        ogg
        GIT_REPOSITORY https://github.com/xiph/ogg.git
        GIT_TAG v1.3.6  # latest stable tag at the moment
        CMAKE_ARGS
            -DCMAKE_POLICY_VERSION=3.5
    )
    FetchContent_MakeAvailable(ogg)

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

    FetchContent_Declare(
            vorbis
            GIT_REPOSITORY https://github.com/xiph/vorbis.git
            GIT_TAG v1.3.7  # latest stable tag at the moment
            CMAKE_ARGS
                -DOGG_INCLUDE_DIR=${OGG_INCLUDE_DIR}
                -DOGG_LIBRARY=${OGG_LIBRARY}
    )
    FetchContent_MakeAvailable(vorbis)

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

    FetchContent_Declare(
        openal-soft
        GIT_REPOSITORY https://github.com/kcat/openal-soft.git
        GIT_TAG 1.22.1  # use latest stable release or tag you want
    )
    FetchContent_MakeAvailable(openal-soft)

    set(OPENAL_TARGET OpenAL::OpenAL)
else()
    message(STATUS "Using system-installed OpenAL")
    set(OPENAL_TARGET OpenAL::OpenAL)
endif()

# ------------------------------
# SDL2 (update to SDL3 when everything works)
# ------------------------------
find_package(SDL2 QUIET)
if(NOT SDL2_FOUND)
    message(STATUS "SDL2 not found, fetching with FetchContent...")

    FetchContent_Declare(
        sdl2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.26.5  # latest stable release tag
    )
    FetchContent_MakeAvailable(sdl2)

    set(SDL2_TARGET SDL2-static)  # or SDL2-static if appropriate
else()
    message(STATUS "Using system-installed SDL2")
    set(SDL2_TARGET SDL2::SDL2)
endif()