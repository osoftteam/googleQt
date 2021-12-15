target_sources(googleQt PRIVATE
    GdriveRequestArg.cpp
    GdriveRequestArg.h
    GdriveRoutes.cpp
    GdriveRoutes.h
)

add_subdirectory(about)
add_subdirectory(comments)
add_subdirectory(files)
add_subdirectory(permissions)
add_subdirectory(revisions)

if(${ARD_AUTOTEST})
    add_subdirectory(AUTOTEST)
endif()
