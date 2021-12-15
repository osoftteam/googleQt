target_sources(googleQt PRIVATE
    GcontactCache.cpp
    GcontactCache.h
    GcontactCacheUtil.cpp
    GcontactCacheUtil.h
    GcontactParts.cpp
    GcontactParts.h
    GcontactRequestArg.cpp
    GcontactRequestArg.h
    GcontactRoutes.cpp
    GcontactRoutes.h
)

add_subdirectory(contact_group)
add_subdirectory(contacts)

if(${ARD_AUTOTEST})
    add_subdirectory(AUTOTEST)
endif()
