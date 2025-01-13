cmake_minimum_required(VERSION 3.28)
project(sticksandstones LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)

FetchContent_Declare( fmt
        GIT_REPOSITORY
        https://github.com/fmtlib/fmt
        GIT_TAG 11.0.2 )

FetchContent_MakeAvailable(SFML fmt)

add_executable(main src/main.cpp
        src/headers.h
        src/game/entity/Entity.cpp
        src/game/entity/Entity.h
        src/game/entity/Entity2D.cpp
        src/game/entity/Entity2D.h
        src/game/Game.cpp
        src/game/Game.h
        src/helpers.cpp
        src/helpers.h
        src/game/entity/SpriteEntity.cpp
        src/game/entity/SpriteEntity.h
        src/game/entity/CollidableEntity.cpp
        src/game/entity/CollidableEntity.h
        src/game/entity/PhysicsEntity.cpp
        src/game/entity/PhysicsEntity.h
        src/game/player/PlayerController.cpp
        src/game/player/PlayerController.h
        src/game/entity/ControlledPhysicsEntity.cpp
        src/game/entity/ControlledPhysicsEntity.h
        src/game/entity/TickingEntity.cpp
        src/game/entity/TickingEntity.h
        src/game/Builders.cpp
        src/game/Builders.h
        src/game/entity/HealthController.cpp
        src/game/entity/HealthController.h
        src/game/level/Room.cpp
        src/game/level/Room.h
        src/game/level/LevelGenerator.cpp
        src/game/level/LevelGenerator.h
        src/game/level/Interactible.cpp
        src/game/level/Interactible.h
        src/game/level/Interactor.cpp
        src/game/level/Interactor.h
        src/game/projectile/Projectile.cpp
        src/game/projectile/Projectile.h
        src/game/item/ItemData.cpp
        src/game/item/ItemData.h
        src/game/player/PlayerUI.cpp
        src/game/player/PlayerUI.h
)
target_link_libraries(main PRIVATE sfml-graphics fmt)
target_compile_features(main PRIVATE cxx_std_20)

if(WIN32)
    add_custom_command(
            TARGET main
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
            VERBATIM)
endif()