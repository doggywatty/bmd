cmake_minimum_required(VERSION 3.20)
project(BigMoneyRebuild CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SEXY_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/fw12")
set(IMAGELIB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/ImageLib")
set(PAKLIB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/PakLib")
set(CRYPT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/Crypt")
include("${CMAKE_CURRENT_SOURCE_DIR}/fw12_sources.cmake")

list(TRANSFORM FW12_SOURCES PREPEND "${SEXY_ROOT}/")
list(TRANSFORM OGG_SOURCES PREPEND "${SEXY_ROOT}/")

# ImageLib source list (reuse 1.34 project entries, excluding jpeg2000)
file(GLOB_RECURSE IMAGELIB_SOURCES
    "${IMAGELIB_ROOT}/ImageLib.cpp"
    "${IMAGELIB_ROOT}/png/*.c"
    "${IMAGELIB_ROOT}/jpeg/*.c"
    "${IMAGELIB_ROOT}/zlib/*.c"
)
list(FILTER IMAGELIB_SOURCES EXCLUDE REGEX "j2k")
list(FILTER IMAGELIB_SOURCES EXCLUDE REGEX "jmemdos.c")
list(FILTER IMAGELIB_SOURCES EXCLUDE REGEX "jmemmac.c")
list(FILTER IMAGELIB_SOURCES EXCLUDE REGEX "jmemname.c")

set(PAKLIB_SOURCES
    "${PAKLIB_ROOT}/PakInterface.cpp"
)

set(CRYPT_SOURCES
    "${CRYPT_ROOT}/BigInt.cpp"
    "${CRYPT_ROOT}/RegKey.cpp"
)

add_library(SexyAppFramework STATIC
    ${FW12_SOURCES}
    ${OGG_SOURCES}
    ${IMAGELIB_SOURCES}
    ${PAKLIB_SOURCES}
    ${CRYPT_SOURCES}
)

target_include_directories(SexyAppFramework PUBLIC
    "${SEXY_ROOT}"
    "${IMAGELIB_ROOT}"
    "${IMAGELIB_ROOT}/png"
    "${IMAGELIB_ROOT}/jpeg"
    "${IMAGELIB_ROOT}/zlib"
    "${IMAGELIB_ROOT}/jpeg2000"
    "${PAKLIB_ROOT}"
    "${CRYPT_ROOT}"
    "${CMAKE_CURRENT_SOURCE_DIR}/sdk-include"
)

target_compile_definitions(SexyAppFramework PUBLIC
    WIN32
    _LIB
    _WINDOWS
    _VISUALC_
    _JPEGLIB_
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
)
target_compile_definitions(SexyAppFramework PRIVATE NDEBUG)

target_compile_options(SexyAppFramework PRIVATE
    /W3
    /wd4996
    /wd4244
    /wd4267
    /wd4018
    /wd4101
    /wd4189
    /wd4700
    /wd4701
    /wd4702
    /wd4703
    /wd4706
    /wd4311
    /wd4312
    /wd4302
    /wd4838
    /MP
)

target_link_libraries(SexyAppFramework PUBLIC
    winmm wsock32 ddraw dsound dxguid ole32 oleaut32 advapi32
    user32 gdi32 shell32 version comdlg32
)

add_executable(BigMoney WIN32
    game/main.cpp
    game/WinBMApp.cpp
    game/GameClasses.cpp
    game/GameClasses_Add.cpp
    game/GameClasses_BoardDraw.cpp
    game/BoardVtable_3.cpp
    game/BoardVtable_4.cpp
    game/BoardVtable_1.cpp
    game/BoardVtable_2.cpp
    game/BoardStubs_5_53.cpp
)
target_include_directories(BigMoney PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/fw12"
    "${CMAKE_CURRENT_SOURCE_DIR}/Crypt"
    "${CMAKE_CURRENT_SOURCE_DIR}/game"
)
target_link_libraries(BigMoney PRIVATE SexyAppFramework)
set_target_properties(BigMoney PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")

