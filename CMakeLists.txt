cmake_minimum_required(VERSION 3.29)
project(QtHydraMarket)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.7.2/mingw_64")

add_compile_options(-l sqlite3)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

qt6_wrap_ui(UI_HEADERS ${UI_FILES})


add_executable(QtHydraMarket
        sqlite/sqlite3.c
        sqlite/sqlite3.h
        source/menu/mainwindow.cpp
        include/ui/mainwindow.h
        source/menu/mainwindow.ui
        source/main.cpp
        source/manager/ErrorManager.cpp
        source/manager/ErrorManager.h
        source/Product.cpp
        include/Product.h
        source/auth/SellerAuth.cpp
        source/auth/SellerAuth.h
        source/database/DatabaseLoader.cpp
        source/database/DatabaseLoader.h
        source/entity/Admin.cpp
        include/entity/Admin.h
        source/entity/User.cpp
        include/entity/User.h
        source/entity/Seller.cpp
        include/entity/Seller.h
        source/entity/Buyer.cpp
        source/entity/Buyer.cpp
        include/entity/Buyer.h
        source/management/CartManagement.cpp
        source/management/CartManagement.h
        source/management/ProductManagement.cpp
        source/management/ProductManagement.h
        source/Storage.cpp
        include/Storage.h
        include/ui/registerwindow.h
        source/menu/registerwindow.cpp
        source/menu/registerwindow.ui
        source/menu/sellermenu.cpp
        include/ui/sellermenu.h
        source/menu/sellermenu.ui
        source/menu/buyermenu.cpp
        include/ui/buyermenu.h
        source/menu/addproduct.cpp
        source/menu/addproduct.ui
        include/ui/addproduct.h
        source/menu/registerseller.cpp
        source/menu/registerseller.ui
        include/ui/registerseller.h
        source/auth/BuyerAuth.cpp
        source/auth/BuyerAuth.h

)
target_link_libraries(QtHydraMarket
        Qt::Core
        Qt::Gui
        Qt::Widgets
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
