cmake_minimum_required(VERSION 3.5)
project(codebee_bots)
add_compile_options(-std=c++1y)

add_executable(random_bot random_bot.cpp helper-package/bot.cpp helper-package/map.cpp helper-package/cell.cpp)

add_executable(potent_bot potent_bot.cpp helper-package/bot.cpp helper-package/map.cpp helper-package/cell.cpp)

add_executable(harvester_bot harvester_bot.cpp helper-package/bot.cpp helper-package/map.cpp helper-package/cell.cpp)

add_executable(harvester_bot2 harvester_bot2.cpp helper-package/bot.cpp helper-package/map.cpp helper-package/cell.cpp)

add_executable(attack_bot attack_bot.cpp helper-package/bot.cpp helper-package/map.cpp helper-package/cell.cpp)

add_executable(expanding_bot expanding_bot.cpp helper-package/bot.cpp helper-package/map.cpp helper-package/cell.cpp)

add_executable(expanding_bot_old expanding_bot_old.cpp helper-package/bot.cpp helper-package/map.cpp helper-package/cell.cpp)

add_executable(balanced_bot balanced_bot.cpp helper-package/bot.cpp helper-package/map.cpp helper-package/cell.cpp)

add_executable(smart_bot smart_bot.cpp helper-package/bot.cpp helper-package/map.cpp helper-package/cell.cpp)

add_executable(smart_bot_old smart_bot_old.cpp helper-package/bot.cpp helper-package/map.cpp helper-package/cell.cpp)