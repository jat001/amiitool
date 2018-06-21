cmake_minimum_required(VERSION 2.8.4)
project(nfc3d C)

include_directories(include)

if(CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fsigned-char")
endif()

set(HEADER_FILES
	include/nfc3d/drbg.h
	include/nfc3d/keygen.h
	include/nfc3d/amiibo.h
)

set(LIB_FILES
	${HEADER_FILES}
	src/drbg.c
	src/keygen.c
	src/amiibo.c
)

set(TEST_FILES
	${LIB_FILES}
	src/util.h
	src/util.c
)

set(ZEROTEST_FILES
	${TEST_FILES}
	src/zerotest.c
)
add_executable(zerotest ${ZEROTEST_FILES})
target_link_libraries(zerotest crypto)

#[[ TODO: fix this
#set(NINTEST_FILES
#	${TEST_FILES}
#	src/nintest.c
#)
#add_executable(nintest ${NINTEST_FILES})
#target_link_libraries(nintest crypto)
#]]

set(AMIITOOL_FILES
	${TEST_FILES}
	src/amiitool.c
)
add_executable(amiitool ${AMIITOOL_FILES})
target_link_libraries(amiitool crypto)

# Lua library

set (LUAMIIBO_FILES
  ${TEST_FILES}
  include/amiibo_lua.h
  src/amiibo_lua.c
)

add_library(luamiibo SHARED ${LUAMIIBO_FILES})

# Location of Proxmark's lua library
set(PROXMARK_LIBLUA "~/Documents/proxmark3/liblua")

set_property(TARGET luamiibo PROPERTY POSITION_INDEPENDENT_CODE ON)
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -undefined dynamic_lookup")
target_include_directories(luamiibo PRIVATE ${PROXMARK_LIBLUA})
target_link_libraries(luamiibo crypto)
