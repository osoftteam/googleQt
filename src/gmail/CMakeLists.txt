target_sources(googleQt PRIVATE
    GmailCache.cpp
    GmailCache.h
    GmailCacheRoutes.cpp
    GmailCacheRoutes.h
    GmailRequestArg.cpp
    GmailRequestArg.h
    GmailRoutes.cpp
    GmailRoutes.h
)

add_subdirectory(attachments)
add_subdirectory(drafts)
add_subdirectory(errors)
add_subdirectory(history)
add_subdirectory(labels)
add_subdirectory(messages)
add_subdirectory(threads)
add_subdirectory(users)

if(${ARD_AUTOTEST})
    add_subdirectory(AUTOTEST)
endif()
