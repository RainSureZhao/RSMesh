cmake_minimum_required(VERSION 3.24)
project(RSMesh)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CGAL_DO_NOT_WARN_ABOUT_CMAKE_BUILD_TYPE TRUE)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS ON)

set(MKL_ARCH "intel64")
if(UNIX)
    set(MKL_LINK "static")
elseif(MSVC)
    set(MKL_LINK "dynamic")
endif()
set(MKL_THREADING "sequential")
set(MKL_INTERFACE "lp64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        3DCore 3DRender 3DExtras 3DInput
        Quick Quick3D
        REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(openmesh REQUIRED)
find_package(CGAL CONFIG REQUIRED)
find_package(flann CONFIG REQUIRED)
# find_package(double-conversion CONFIG REQUIRED)
find_package(MKL CONFIG REQUIRED)
find_package(OpenMP REQUIRED)
#find_package(Ceres CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_subdirectory(third_party)
set(MKL_INCLUDE_DIRS "${MKL_ROOT}/include")
# !!! Add MKL redist directory to PATH, or you will get -1073741515 (0xC0000135)
set(MKL_LIBRARIES   "${MKL_ROOT}/redist/intel64/mkl_def.2.dll"
                    "${MKL_ROOT}/redist/intel64/mkl_mc.2.dll"
                    "${MKL_ROOT}/redist/intel64/mkl_mc3.2.dll"
                    "${MKL_ROOT}/redist/intel64/mkl_avx.2.dll"
                    "${MKL_ROOT}/redist/intel64/mkl_avx2.2.dll"
                    "${MKL_ROOT}/redist/intel64/mkl_avx512.2.dll"
                    "${MKL_ROOT}/redist/intel64/mkl_core.2.dll"
                    "${MKL_ROOT}/redist/intel64/mkl_intel_thread.2.dll"
                    "${MKL_ROOT}/redist/intel64/mkl_sequential.2.dll"
                    "${MKL_ROOT}/redist/intel64/mkl_rt.2.dll"
                    )
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
#interpolation
file(GLOB INTERPOLATION "${PROJECT_SOURCE_DIR}/include/interpolation/*.h" "${PROJECT_SOURCE_DIR}/src/interpolation/*.cpp")
#rbf
file(GLOB RBF "${PROJECT_SOURCE_DIR}/include/rbf/*.h" "${PROJECT_SOURCE_DIR}/src/rbf/*.cpp")
#fmm
file(GLOB FMM "${PROJECT_SOURCE_DIR}/include/fmm/*.h" "${PROJECT_SOURCE_DIR}/src/fmm/*.cpp")
#krylov
file(GLOB KRYLOV "${PROJECT_SOURCE_DIR}/include/krylov/*.h" "${PROJECT_SOURCE_DIR}/src/krylov/*.cpp")
#preconditioner
file(GLOB PRECONDITIONER "${PROJECT_SOURCE_DIR}/include/preconditioner/*.h" "${PROJECT_SOURCE_DIR}/src/preconditioner/*.cpp")
#isosurface
file(GLOB ISOSURFACE "${PROJECT_SOURCE_DIR}/include/isosurface/*.h" "${PROJECT_SOURCE_DIR}/src/isosurface/*.cpp")
#ui
file(GLOB UI "${PROJECT_SOURCE_DIR}/resources/ui/*.ui" "${PROJECT_SOURCE_DIR}/resources/ui/*.h" "${PROJECT_SOURCE_DIR}/resources/ui/*.cpp" "${PROJECT_SOURCE_DIR}/resources/ui/*.qml")
#qt
file(GLOB QT "${PROJECT_SOURCE_DIR}/include/qt/*.h" "${PROJECT_SOURCE_DIR}/src/qt/*.cpp")


#examples
file(GLOB EXAMPLES "${PROJECT_SOURCE_DIR}/include/examples/*.h" "${PROJECT_SOURCE_DIR}/examples/*.cpp")

qt6_add_resources(QRC_FILES
        ${PROJECT_SOURCE_DIR}/resources/qml.qrc
)

add_executable(RSMesh
        Application.h
        main.cpp
        ${GEOMETRY}
        ${COMMON}
        ${NORMAL}
        ${POINTCLOUD}
        ${NUMERIC}
        ${POLYNOMIAL}
        ${INTERPOLATION}
        ${RBF}
        ${FMM}
        ${KRYLOV}
        ${PRECONDITIONER}
        ${EXAMPLES}
        ${ISOSURFACE}
        ${UI}
        ${QRC_FILES}
        ${QT}
)

if(MSVC)
    target_link_libraries(RSMesh PUBLIC
            MKL::MKL
    )
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND WIN32)
    target_link_libraries(RSMesh PUBLIC
            ${MKL_LIBRARIES}
    )
endif()
target_compile_definitions(RSMesh PUBLIC
        EIGEN_USE_MKL_ALL
        RSMESH_USE_MKL
)

target_compile_definitions(RSMesh PUBLIC
        -DEIGEN_DONT_PARALLELIZE
        -DEIGEN_MPL2_ONLY
)

# Disable warnings in msvc
if(MSVC)
    target_compile_options(RSMesh PUBLIC
            /wd4819
            /wd5055
            /wd4702
            /wd4038
    )
endif()

if(MSVC)
    target_compile_definitions(RSMesh PUBLIC
            -DWIN32_LEAN_AND_MEAN
            -DNOMINMAX
    )
endif()

if(MSVC)
    # 为MSVC添加/Zc:__cplusplus选项
    target_compile_options(RSMesh PUBLIC
            /Zc:__cplusplus)
endif()


target_include_directories(RSMesh PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(RSMesh PRIVATE ${OPENMESH_INCLUDE_DIRS})
target_include_directories(RSMesh PRIVATE ${MKL_INCLUDE_DIRS})
target_include_directories(RSMesh PUBLIC "resources/ui")

target_link_libraries(RSMesh PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt6::3DCore Qt6::3DRender Qt6::3DExtras Qt6::3DInput
        Qt6::Quick Qt6::Quick3D
        ${OPENMESH_LIBRARIES}
        CGAL::CGAL
        flann::flann flann::flann_cpp
        ScalFmm
        OpenMP::OpenMP_CXX
        double-conversion
#        ceres
        Boost::boost
        assimp::assimp
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
