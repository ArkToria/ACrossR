qt_add_library(content STATIC)
qt6_add_qml_module(content
    URI "content"
    VERSION 1.0
    QML_FILES
        App.qml
        AboutPage.qml
        CardGrid.qml
        CustomButton.qml
        Dashboard.qml
        Fab.qml
        GroupCard.ui.qml
        GroupIcon.qml
        GroupPage.qml
        HomePage.qml
        HomePageCard.qml
        HomePageDrawer.qml
        LogsPage.qml
        MainPanelButton.qml
        NodeCard.ui.qml
        RoutePage.qml
        StartButton.ui.qml
        AcrossCpp.qml
    RESOURCES
        fonts/fonts.txt
)
