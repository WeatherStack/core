project(ws_core)

# Set sources
file(
    GLOB_RECURSE ${PROJECT_NAME}_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

# Create library
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})

# Target includes
# Target includes
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

# Link all libraries
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ZLIB::ZLIB
    OpenSSL::SSL
    Crow::Crow
)

if(WIN32)
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        # The below needs to be separated, as they are only required on Windows
        ws2_32
        mswsock
    )
endif()