/*
 *  SplashScreen.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/13/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <glm/glm.hpp>

export module CR.Game.SplashScreen;

import CR.Game.IRenderable;
import CR.Game.Event;
import CR.Game.Timer;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class SplashScreen : public IRenderable {
	  public:
		SplashScreen(uint64_t splashAsset, float delayTime, int zPos);
		virtual ~SplashScreen();

		void SetSplashFrame(uint32_t frame, float delayTime);
		void SetFadeEffect(bool _value);
		void StartSplash();

		// IRenderable
		void Update();
		void Render();
		void ForceRender();
		void SetPosition(float xLoc, float yLoc);
		void PauseAnimation(bool pause);

		CR::Utility::Event SplashFinished;

	  private:
		CR::Engine::Graphics::Handles::Sprite splashSprite;
		float m_baseDelayTime;
		float m_delayTime;
		bool m_running;
		bool isFade;
		int fadeAlpha;
	};
}    // namespace CR::Game

module :private;

namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::SplashScreen::SplashScreen(uint64_t splashAsset, float delayTime, [[maybe_unused]] int zPos) {
	splashSprite = cegraph::Sprites::Create(splashAsset);
	SetSplashFrame(0, delayTime);
	m_running = false;
	isFade    = false;
	fadeAlpha = 255;
}

cg::SplashScreen::~SplashScreen() {
	cegraph::Sprites::Delete(splashSprite);
}

void cg::SplashScreen::SetSplashFrame(uint32_t frame, float delayTime) {
	cegraph::Sprites::SetFrame(splashSprite, (uint16_t)frame);
	SetPosition(0, 0);
	m_baseDelayTime = delayTime;
	m_delayTime     = delayTime;
}

void cg::SplashScreen::SetFadeEffect(bool _value) {
	isFade = _value;
}

void cg::SplashScreen::StartSplash() {
	m_running   = true;
	m_delayTime = m_baseDelayTime;
}

void cg::SplashScreen::Update() {
	if(!m_running) return;

	m_delayTime -= Timer::GetTimePassed();
	if(m_delayTime <= 0) {
		m_running = false;
		SplashFinished();
	}

	if(isFade) { cegraph::Sprites::SetColor(splashSprite, glm::u8vec4(255, 255, 255, fadeAlpha)); }
}

void cg::SplashScreen::Render() {
	if(!m_running) return;

	// if(m_delayTime > 0) splashSprite->Render();
}

void cg::SplashScreen::ForceRender() {
	// splashSprite->Render();
}

void cg::SplashScreen::SetPosition(float xLoc, float yLoc) {
	cegraph::Sprites::SetPosition(splashSprite, glm::vec2(xLoc, yLoc));
}

void cg::SplashScreen::PauseAnimation([[maybe_unused]] bool pause) {}
