cmake_minimum_required(VERSION 3.20)

project(log)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")

aux_source_directory("." M_SRC)
file(GLOB HDR_LIST  "./*.h*")


add_executable(${PROJECT_NAME} ${M_SRC} ${HDR_LIST})
