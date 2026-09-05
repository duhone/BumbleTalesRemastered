#include <engine/Engine.h>

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

namespace {
	// For this simple game we don't really need texture sets. So there will only be 1, and it will always be
	// resident. loaded on startup.
	constexpr uint64_t c_textureSetHashes[] = {cecore::C_Hash64("Splash")};
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
			frameCount      = 0;
			auto endFPSTime = std::chrono::high_resolution_clock::now();
			double times =
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