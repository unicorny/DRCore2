include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if (NOT DEFINED DRCore2_INSTALL_CMAKEDIR)
   set(DRCore2_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/DRCore2"
       CACHE STRING "Path to DRCore2 CMake files")
endif ()

install(TARGETS DRCore2 EXPORT DRCore2_Targets
    RUNTIME COMPONENT DRCore2_Runtime
    LIBRARY COMPONENT DRCore2_Runtime
    NAMELINK_COMPONENT DRCore2_Development
    ARCHIVE COMPONENT DRCore2_Development
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(DIRECTORY "${DRCore2_SOURCE_DIR}/include/" "${DRCore2_BINARY_DIR}/include/"
    TYPE INCLUDE
    COMPONENT DRCore2_Development)

if (BUILD_SHARED_LIBS)
    set(type shared)
else ()
    set(type static)
endif ()
    
install(EXPORT DRCore2_Targets
    DESTINATION "${DRCore2_INSTALL_CMAKEDIR}"
    NAMESPACE DRCore2::
    FILE DRCore2-${type}-targets.cmake
    COMPONENT DRCore2_Development)

write_basic_package_version_file(
    DRCore2ConfigVersion.cmake
    COMPATIBILITY SameMajorVersion)

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/DRCore2Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/DRCore2ConfigVersion.cmake"
    DESTINATION "${DRCore2_INSTALL_CMAKEDIR}"
    COMPONENT DRCore2_Development)