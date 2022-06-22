project(common)

add_library(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/type.cc)
add_library(sub::common ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/
)