#include <engine/Engine.h>

#if CR_FINAL
#include <windows.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

import CR.Engine;

import CR.Game.Timer;
import CR.Game;

import std;
import std.compat;

// namespace ceassets = CR::Engine::Assets;
namespace ceaud   = CR::Engine::Audio;
namespace cecore  = CR::Engine::Core;
namespace ceinput = CR::Engine::Input;
namespace cegraph = CR::Engine::Graphics;
namespace ceplat  = CR::Engine::Platform;

namespace cg = CR::Game;

namespace fs = std::filesystem;

using namespace std::literals;
using namespace cecore::Literals;

namespace {
	// For this simple game we don't really need texture sets. So there will only be 1, and it will always be
	// resident. loaded on startup.
	constexpr uint64_t c_textureSetHashes[] = {"ArcadeModeResourceBar3"_hash64,
											   "ArcadeModeResourceBar4"_hash64,
											   "ArcadeModeTimeBar"_hash64,
											   "ArcadeModeTimeTag"_hash64,
											   "HelpButton_HowToPlay2"_hash64,
											   "HelpButton_MainMenu2"_hash64,
											   "HelpCredits2"_hash64,
											   "HelpInfoBG"_hash64,
											   "HelpInfoHelp"_hash64,
											   "MenuBG"_hash64,
											   "MenuButton_ArcadeMode"_hash64,
											   "MenuButton_HelpInfo"_hash64,
											   "MenuButton_Options"_hash64,
											   "MenuButton_ResumeGame"_hash64,
											   "MenuButton_Scrapbook"_hash64,
											   "MenuButton_StoryMode"_hash64,
											   "MenuSplash"_hash64,
											   "OptionsBase"_hash64,
											   "OptionsButton_EraseData"_hash64,
											   "OptionsButton_Exit2"_hash64,
											   "OptionsButton_No2"_hash64,
											   "OptionsButton_Yes2"_hash64,
											   "OptionsCheck"_hash64,
											   "OptionsConfirmErase"_hash64,
											   "OptionsHeader"_hash64,
											   "PiecesSelector1"_hash64,
											   "PiecesSelector2"_hash64,
											   "PiecesSelector3"_hash64,
											   "ScoreYouGotTags"_hash64,
											   "ScrapbookBG"_hash64,
											   "ScrapbookButton_Characters"_hash64,
											   "ScrapbookButton_Exit"_hash64,
											   "ScrapbookButton_LArrow"_hash64,
											   "ScrapbookButton_RArrow"_hash64,
											   "ScrapbookButton_Records"_hash64,
											   "ScrapbookButton_Trophies"_hash64,
											   "ScrapbookCategoryRim"_hash64,
											   "ScrapbookCharTale01"_hash64,
											   "ScrapbookCharTale02"_hash64,
											   "ScrapbookCharTale03"_hash64,
											   "ScrapbookCharTale04"_hash64,
											   "ScrapbookCharTale05"_hash64,
											   "ScrapbookCharTale06"_hash64,
											   "ScrapbookCharTale07"_hash64,
											   "ScrapbookCharTale08"_hash64,
											   "ScrapbookCharTale09"_hash64,
											   "ScrapbookCharTale10"_hash64,
											   "ScrapbookCharTale11"_hash64,
											   "ScrapbookCharTale12"_hash64,
											   "ScrapbookCharTale13"_hash64,
											   "ScrapbookFont_Records1"_hash64,
											   "ScrapbookFont_Records2"_hash64,
											   "ScrapbookLockedTaleImage"_hash64,
											   "ScrapbookRecordsBase1"_hash64,
											   "ScrapbookRecordsBase2"_hash64,
											   "ScrapbookTags1"_hash64,
											   "ScrapbookTags2"_hash64,
											   "ScrapbookTrophyBase1"_hash64,
											   "ScrapbookTrophyIcon"_hash64,
											   "ScrapbookTrophyInfo"_hash64,
											   "ScrapbookTrophyNames"_hash64,
											   "ShopButton_Plus"_hash64,
											   "ShopPlusDimmer"_hash64,
											   "ShopStarMeter"_hash64,
											   "ShopStarMeterIndicator"_hash64,
											   "StoryModeFont_StarHUD"_hash64,
											   "StoryModeMaxTag"_hash64,
											   "StoryModeResourceBar1"_hash64,
											   "StoryModeResourceBar2"_hash64,
											   "StoryModeStarHUD"_hash64,
											   "StoryModeStarHUDAnim"_hash64};
	std::unique_ptr<CR::Game::Game> m_game;

	void glfwErrorCallback(int error, const char* description) {
		CR_WARN("GLFW Error {}: {}", error, description);
	}
}    // namespace

int main(int, char*) {
	fs::current_path(ceplat::GetCurrentProcessPath());

	cecore::LogSystem logSystem;

	fs::path assetsPath = fs::canonical(ASSETS_FOLDER);

	glfwSetErrorCallback(glfwErrorCallback);

	if(!glfwInit()) {
		CR_ASSERT_ALWAYS(false, "failed to initialize glfw");
		return 0;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow* window = glfwCreateWindow(640, 960, "Bumble Tales Remastered", NULL, NULL);
	if(!window) {
		CR_ASSERT_ALWAYS(false, "failed to create glfw window");
		return 0;
	}

#if CR_FINAL
	FreeConsole();
#endif

	CR::Engine::Initialize(window, glm::uvec2(640, 960), assetsPath);

	auto textureSet = cegraph::Textures::LoadTextureSet(c_textureSetHashes);

	m_game = std::make_unique<cg::Game>();
	m_game->Initialize();

	cg::Timer::Initialize();

	uint32_t frameCount = 0;
	auto startFPSTime   = std::chrono::high_resolution_clock::now();
	while(!glfwWindowShouldClose(window)) {
		cg::Timer::Update();

		glfwPollEvents();

		CR::Engine::Update();

		m_game->Execute();

		bool gsAvailable = CR::Engine::Render();

		if(!gsAvailable) {
			gsAvailable = cegraph::ReInitialize();
			if(!gsAvailable && !glfwWindowShouldClose(window)) { std::this_thread::sleep_for(100ms); }
		}

		++frameCount;
		if(frameCount == 1024) {
			frameCount                       = 0;
			[[maybe_unused]] auto endFPSTime = std::chrono::high_resolution_clock::now();
			[[maybe_unused]] double times =
			    std::chrono::duration_cast<std::chrono::milliseconds>(endFPSTime - startFPSTime).count() /
			    1000.0f;
			startFPSTime = endFPSTime;
			CR_LOG("FPS {:.2f}", (1024.0f / times));
		}
	}

	m_game->ApplicationTerminated();
	m_game.reset();

	cegraph::Textures::ReleaseTextureSet(textureSet);

	CR::Engine::Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 1;
}