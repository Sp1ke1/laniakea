file(GLOB LANIAKEA_3RDPARTY_LIB_DIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*")
list(REMOVE_ITEM LANIAKEA_3RDPARTY_LIB_DIRS "CMakeLists.txt")
set ( LANIAKEA_3RDPARTY_INCLUDE )
set ( LANIAKEA_3RDPARTY_LIBS )

foreach(lib ${LANIAKEA_3RDPARTY_LIB_DIRS})

    add_subdirectory ( ${CMAKE_CURRENT_SOURCE_DIR}/${lib}/ )
endforeach()

set ( LANIAKEA_3RDPARTY_INCLUDE ${LANIAKEA_3RDPARTY_INCLUDE} PARENT_SCOPE )
set ( LANIAKEA_3RDPARTY_LIBS ${LANIAKEA_3RDPARTY_LIBS} PARENT_SCOPE )
