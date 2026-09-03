block()

set(root "${CMAKE_CURRENT_LIST_DIR}/..")

set(CR_INTERFACE_HEADERS
)

set(CR_INTERFACE_MODULES
)

set(CR_IMPLEMENTATION
    ${root}/source/AppConfig.ixx
    ${root}/source/Constants.ixx
    ${root}/source/Event.ixx
    ${root}/source/FSM.ixx
    ${root}/source/FunctionObjects.ixx
    ${root}/source/HomeMenuView.ixx
    ${root}/source/IRenderable.ixx
    ${root}/source/ISavableView.ixx
    ${root}/source/ISaveOnTerminate.ixx
    ${root}/source/IScrapbookChapter.ixx
    ${root}/source/ITouchable.ixx
    ${root}/source/IView.ixx
    ${root}/source/main.cpp
    ${root}/source/MainMenuGameState.ixx
    ${root}/source/Property.ixx
    ${root}/source/SplashScreen.ixx
    ${root}/source/SplashScreenMenu.ixx
    ${root}/source/SplashScreenMenuView.ixx
    ${root}/source/Timer.ixx
)

set(CR_BUILD_FILES
    ${root}/build/build.cmake
)

set(CR_SCHEMA_FILES
)

set(CR_GENERATED_FILES
)

add_executable(game 
)

settingsCR(game)

target_link_libraries(game PUBLIC
	engine
)

target_compile_definitions(game PRIVATE ASSETS_FOLDER="${root}/../Assets")

endblock()