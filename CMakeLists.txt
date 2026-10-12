cmake_minimum_required(VERSION 3.16)

project(GoGame VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick Sql REQUIRED)
find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS WebSockets)

qt_add_executable(appGoGame
    main.cpp
)

set_source_files_properties(
    qml/Core/Constants.qml
    qml/Core/Motion.qml
    qml/Core/Radius.qml
    qml/Core/Spacing.qml
    qml/Core/Theme.qml
    qml/Core/Typography.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(appGoGame
    URI GoGame
    VERSION 1.0

    RESOURCES assets/fonts/genshin.ttf
    QML_FILES
        qml/Components/battle/CountdownClock.qml
        qml/Components/battle/ElementPointBar.qml
        qml/Components/cards/Card.qml
        qml/Components/cards/CharacterCard.qml
        qml/Components/cards/SupportCard.qml
        qml/Components/common/Loading.qml
        qml/Components/common/Popup.qml
        qml/Components/controls/GameButton.qml
        qml/Components/controls/GameIcon.qml
        qml/Components/controls/GameImage.qml
        qml/Components/controls/GamePanel.qml
        qml/Components/controls/GameText.qml
        qml/Components/layout/GameBackground.qml
        qml/Components/layout/GamePanel.qml
        qml/Core/Constants.qml
        qml/Core/Motion.qml
        qml/Core/Radius.qml
        qml/Core/SceneManager.qml
        qml/Core/Spacing.qml
        qml/Core/Theme.qml
        qml/Core/Typography.qml
        qml/Screens/LoginScreen.qml
        qml/Screens/MainMenu.qml
        main.qml
SOURCES src/Core/AI.cpp src/Core/AI.h src/Core/Game.cpp src/Core/Game.h src/Managers/AuthManager.cpp src/Managers/AuthManager.h src/Models/Board.cpp src/Models/Board.h src/Models/UI.cpp src/Models/UI.h src/Network/NetworkClient.cpp src/Network/NetworkClient.h src/Network/Protocol.cpp src/Network/Protocol.h main.cpp
RESOURCES users.sqlite
SOURCES src/Core/Enums.h
SOURCES src/Core/Enumutils.h src/Core/Enumutils.cpp
SOURCES src/Managers/MatchmakingManager.h src/Managers/MatchmakingManager.cpp
QML_FILES qml/Screens/SettingsScreen.qml
QML_FILES qml/Screens/Board.qml


)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appGoGame PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appGoGame
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appGoGame
    PRIVATE
        Qt6::Core
        Qt6::Quick
        Qt6::Qml
        Qt6::Network
        Qt6::WebSockets
        Qt6::Sql
)

include(GNUInstallDirs)
install(TARGETS appGoGame
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
add_subdirectory(server)