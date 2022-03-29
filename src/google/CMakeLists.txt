add_subdirectory(demo)
add_subdirectory(endpoint)

if(${ARD_AUTOTEST})
    add_subdirectory(AUTOTEST)
endif()
