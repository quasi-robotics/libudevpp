
find_library( UDEV_LIB udev )

add_executable(enum_devs enum_devs.cpp)
target_link_libraries(enum_devs PRIVATE udev++ ${UDEV_LIB})

add_executable(mon_udev mon_udev.cpp)
target_link_libraries(mon_udev PRIVATE udev++ ${UDEV_LIB})