cmake_minimum_required(VERSION 3.22.1)
project(brickGame2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)
find_package(GTest REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(brickGame2
        brick_game/snake/controller/game_controller.cpp
        brick_game/snake/model/apple.cpp
        brick_game/snake/model/game_model.cpp
        brick_game/snake/model/snake.cpp

        brick_game/tetris/backend.cpp
        brick_game/tetris/fsm_t.cpp

        gui/desktop/game_view.cpp
        gui/desktop/gtk_snake_view.cpp
        gui/desktop/gtk_tetris_view.cpp
        main.cpp
)

target_include_directories(brickGame2 PRIVATE
        brick_game/snake/controller/inc
        brick_game/snake/model/inc
        brick_game/tetris/inc
        brick_game/inc
        gui/desktop/inc
)

add_executable(brick_game_tests
        brick_game/snake/controller/game_controller.cpp
        brick_game/snake/model/apple.cpp
        brick_game/snake/model/game_model.cpp
        brick_game/snake/model/snake.cpp

        brick_game/tetris/backend.cpp
        brick_game/tetris/fsm_t.cpp

        tests/tests.cpp
)
target_link_libraries(brick_game_tests ${GTEST_LIBRARIES} pthread)

target_link_libraries(brickGame2 PRIVATE PkgConfig::GTKMM)
