cmake_minimum_required(VERSION 3.16)                # Require CMake 3.16+

set(LIB_NAME Apollo)
set(EXE_NAME DemoApp)

project(${LIB_NAME} VERSION 0.1 LANGUAGES C CXX)    # Project name, version, and language

set(CMAKE_CXX_STANDARD 17)                          # Use C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)                 # Enforce that standard

# Add your library target
add_library(${LIB_NAME} STATIC
    src/audio/priv/ogg_stream.cpp
    src/audio/audio_device.cpp
    src/audio/audio_format.cpp
    src/audio/listener.cpp
    src/audio/music.cpp
    src/audio/sound.cpp
    src/audio/sound_buffer.cpp
    src/audio/sound_file_wave.cpp
    src/audio/sound_interface.cpp
    src/audio/sound_queue_buffer.cpp
    src/audio/sound_source.cpp
    src/audio/sound_stream.cpp
    src/audio/sound_stream_factory.cpp
    src/graphics/blend_mode.cpp
    src/graphics/circle_shape.cpp
    src/graphics/color.cpp
    src/graphics/font.cpp
    src/graphics/image.cpp
    src/graphics/rectangle_shape.cpp
    src/graphics/render_states.cpp
    src/graphics/render_target.cpp
    src/graphics/render_window.cpp
    src/graphics/shader.cpp
    src/graphics/shader_program.cpp
    src/graphics/shape_2d.cpp
    src/graphics/sprite.cpp
    src/graphics/text.cpp
    src/graphics/texture.cpp
    src/graphics/transformable_2d.cpp
    src/graphics/uniform_buffer_object.cpp
    src/graphics/vertex_array_object.cpp
    src/graphics/vertex_buffer_object.cpp
    src/graphics/view_2d.cpp
    src/graphics/view_3d.cpp
    src/system/assetstream.cpp
    src/system/background_worker.cpp
    src/system/clock.cpp
    src/system/memstream.cpp
    src/system/transient_context_lock.cpp
    src/utility/utility.cpp
    src/engine.cpp
)

# Also set explictily for project
set_target_properties(${LIB_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

# Headers go here
target_include_directories(${LIB_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/headers/stb_image
        ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/headers/glm
        $<INSTALL_INTERFACE:include>
)

# Link other libraries (e.g. freetype, SDL2).
# Dependencies are handled in file cmake/dependencies.cmake
include(cmake/dependencies.cmake)
target_link_libraries(${LIB_NAME} PRIVATE
        ${OPENGL_gl_LIBRARY}
        ${FREETYPE_TARGET}
        ${GLEW_LIB_TARGET}
        ${OGG_TARGET}
        ${VORBIS_TARGET}
        ${VORBISFILE_TARGET}
        ${OPENAL_TARGET}
        ${SDL2_TARGET}
)

# Also create the DemoApp
add_executable(${EXE_NAME}
        examples/demo_app/main.cpp
        examples/demo_app/test_app.cpp
)

set_target_properties(${EXE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_options(${EXE_NAME} PRIVATE -static -static-libgcc -static-libstdc++)

target_link_libraries(${EXE_NAME}
        PRIVATE
        ${LIB_NAME}
        ${SDL2_TARGET}
        Threads::Threads
)
target_include_directories(${EXE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)