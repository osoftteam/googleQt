target_sources(googleQt PRIVATE
    GtaskCache.cpp
    GtaskCache.h
    GtaskRequestArg.cpp
    GtaskRequestArg.h
    GtaskRoutes.cpp
    GtaskRoutes.h
)

add_subdirectory(tasklists)
add_subdirectory(tasks)

if(${ARD_AUTOTEST})
    add_subdirectory(AUTOTEST)
endif()
