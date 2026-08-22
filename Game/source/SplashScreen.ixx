/*
 *  SplashScreen.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/13/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
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
		SplashScreen(int splashAsset, float delayTime, int zPos);
		virtual ~SplashScreen();

		void SetSplashAsset(int splashAsset, float delayTime);
		void SetFadeEffect(bool _value);
		void StartSplash();

		// IRenderable
		void Update();
		void Render();
		void ForceRender();
		void SetPosition(float xLoc, float yLoc);
		void PauseAnimation(bool pause);
		void SetFrameSet(int frameSet);
		CR::Utility::Event SplashFinished;
		void DesignSize([[maybe_unused]] int _width, [[maybe_unused]] int _height) {
			// TODO: no design size yet in new graphics engine
			// if(splashSprite) splashSprite->SetDesignSize(_width, _height);
		}

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

namespace cg = CR::Game;

cg::SplashScreen::SplashScreen(int splashAsset, float delayTime, [[maybe_unused]] int zPos) {
	// splashSprite = graphics_engine->CreateSprite1(false, zPos);
	SetSplashAsset(splashAsset, delayTime);
	m_running = false;
	isFade    = false;
	fadeAlpha = 255;
}

cg::SplashScreen::~SplashScreen() {
	// splashSprite->Release();
}

void cg::SplashScreen::SetSplashAsset([[maybe_unused]] int splashAsset, float delayTime) {
	// splashSprite->SetImage(splashAsset);
	SetPosition(160, 240);
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

	if(isFade) {
		// CR::Math::Color32 color = CR::Math::Color32(255, 255, 255, fadeAlpha);
		// splashSprite->Color(color);
	}
}

void cg::SplashScreen::Render() {
	if(!m_running) return;

	// if(m_delayTime > 0) splashSprite->Render();
}

void cg::SplashScreen::ForceRender() {
	// splashSprite->Render();
}

void cg::SplashScreen::SetPosition([[maybe_unused]] float xLoc, [[maybe_unused]] float yLoc) {
	// splashSprite->SetPositionAbsolute(xLoc, yLoc);
}

void cg::SplashScreen::PauseAnimation([[maybe_unused]] bool pause) {}

void cg::SplashScreen::SetFrameSet([[maybe_unused]] int frameSet) {
	// splashSprite->SetFrameSet(frameSet);
}