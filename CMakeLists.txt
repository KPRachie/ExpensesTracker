cmake_minimum_required(VERSION 3.28)

project(
    ExpensesTracker
    VERSION 0.1.0
    DESCRIPTION ""
    HOMEPAGE_URL ""
)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES 0)

# vcpkg
include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

# FetchContent
include(FetchContent)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(SQLiteCpp REQUIRED)

add_executable (${PROJECT_NAME}
	"main.cpp"
    "DataAccess.cpp"
    "UserLogic.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SQLiteCpp
    ${CMAKE_THREAD_LIBS_UNIT}
    ${OPENSSL_LIBRARY}
    ${Boost_LIBRARIES}
    "C:/kp_rachie/lib/vcpkg/installed/x64-windows/lib/TgBot.lib"
)

add_compile_definitions(${PROJECT_NAME} PRIVATE
    SQLITE_THREADSAFE=2
)
