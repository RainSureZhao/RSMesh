cmake_minimum_required(VERSION 3.24)
project(RSMesh)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON)
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(openmesh REQUIRED)
find_package(CGAL CONFIG REQUIRED)
find_package(flann CONFIG REQUIRED)
find_package(double-conversion CONFIG REQUIRED)

#common
file(GLOB COMMON "${PROJECT_SOURCE_DIR}/include/common/*.h")
# Geometry
file(GLOB GEOMETRY "${PROJECT_SOURCE_DIR}/include/geometry/*.h" "${PROJECT_SOURCE_DIR}/src/geometry/*.cpp")
#Point Cloud
file(GLOB POINTCLOUD "${PROJECT_SOURCE_DIR}/include/point_cloud/*.h" "${PROJECT_SOURCE_DIR}/src/point_cloud/*.cpp")
#Numeric
file(GLOB NUMERIC "${PROJECT_SOURCE_DIR}/include/numeric/*.h")
# Normal
file(GLOB NORMAL "${PROJECT_SOURCE_DIR}/include/*.h")
#polynomial
file(GLOB POLYNOMIAL "${PROJECT_SOURCE_DIR}/include/polynomial/*.h" "${PROJECT_SOURCE_DIR}/src/polynomial/*.cpp")


add_executable(RSMesh 
        main.cpp
        ${GEOMETRY}
        ${COMMON}
        ${NORMAL}
        ${POINTCLOUD}
        ${NUMERIC}
        ${POLYNOMIAL}
)
target_include_directories(RSMesh PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_include_directories(RSMesh PRIVATE ${OPENMESH_INCLUDE_DIRS})
target_link_libraries(RSMesh
        Qt::Core
        Qt::Gui
        Qt::Widgets
        ${OPENMESH_LIBRARIES}
        CGAL::CGAL
        flann::flann flann::flann_cpp
        double-conversion::double-conversion
)
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
