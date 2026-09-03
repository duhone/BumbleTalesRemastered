/*
 *  SplashScreenMenuView.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/13/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game.SplashScreenMenuView;

import CR.Game.Constants;
import CR.Game.IView;
import CR.Game.Event;
import CR.Game.SplashScreen;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class SplashScreenMenuView : public IView {
	  public:
		SplashScreenMenuView();
		virtual ~SplashScreenMenuView();

		void Update();
		void Render();

		void OnSplashScreen1Done();
		void OnSplashScreen2Done();

		Utility::Event ExitSplashScreen;

	  private:
		SplashScreen* m_splashScreen;
		CR::Engine::Audio::Handles::SoundFX tandemSound;
	};
}    // namespace CR::Game

module :private;

namespace cecore = CR::Engine::Core;
namespace ceaud  = CR::Engine::Audio;
namespace cg     = CR::Game;

cg::SplashScreenMenuView::SplashScreenMenuView() {
	tandemSound = ceaud::SoundFX::GetHandle(cecore::C_Hash64("tanlogo"));
	ceaud::SoundFX::Play(tandemSound);

	m_splashScreen = new SplashScreen(cecore::C_Hash64("Splash"), 1.5f, 1000);
	m_splashScreen->DesignSize(320, 480);
	m_splashScreen->SplashFinished += CR::Utility::Delegate(this, &SplashScreenMenuView::OnSplashScreen1Done);
	m_splashScreen->StartSplash();
}

cg::SplashScreenMenuView::~SplashScreenMenuView() {
	delete m_splashScreen;
}

void cg::SplashScreenMenuView::Update() {
	m_splashScreen->Update();
}

void cg::SplashScreenMenuView::Render() {
	m_splashScreen->ForceRender();
}

void cg::SplashScreenMenuView::OnSplashScreen1Done() {
	m_splashScreen->SplashFinished.Clear();
	m_splashScreen->SplashFinished += CR::Utility::Delegate(this, &SplashScreenMenuView::OnSplashScreen2Done);
	m_splashScreen->SetSplashFrame(1, 1.5f);
	m_splashScreen->DesignSize(320, 480);
	m_splashScreen->StartSplash();
}

void cg::SplashScreenMenuView::OnSplashScreen2Done() {
	ExitSplashScreen();
}