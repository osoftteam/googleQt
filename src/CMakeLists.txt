target_sources(googleQt PRIVATE
    Endpoint.cpp
    Endpoint.h
    GoogleClient.cpp
    GoogleClient.h
    GoogleRouteBase.h
)

add_subdirectory(gcontact)
add_subdirectory(gdrive)
add_subdirectory(gmail)
add_subdirectory(google)
add_subdirectory(gtask)
