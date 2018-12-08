#based on https://github.com/SOCI/soci/blob/master/cmake/modules/FindMySQL.cmake

if (WIN32)
        # normal way:
        #find_path( MYSQL_INCLUDE_DIR
        #        NAMES "mysql.h"
        #        PATHS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libmysql/windows/include/*/" )

        # stupid way that works
        set (MYSQL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libmysql/windows/include/")
	
	find_library( MYSQL_LIBRARY
                NAMES "libmysql.lib"
                PATHS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libmysql/windows" )
		
endif()

if (MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
   set(MYSQL_FOUND TRUE)
   message(STATUS "Found MySQL: ${MYSQL_INCLUDE_DIR}, ${MYSQL_LIBRARY}")
else()
   set(MYSQL_FOUND FALSE)
   message(STATUS "MySQL not found. ${MYSQL_INCLUDE_DIR}, ${MYSQL_LIBRARY}")
endif()	

mark_as_advanced(MYSQL_INCLUDE_DIR MYSQL_LIBRARY)
