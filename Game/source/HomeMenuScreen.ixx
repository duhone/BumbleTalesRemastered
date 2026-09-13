/*
 *  HomeMenuScreen.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 5/22/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

export module CR.Game.HomeMenuScreen;

import CR.Game.HomeMenuView;
import CR.Game.Constants;
import CR.Game.FSM;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class HomeMenuScreen : public CR::Utility::IState {
	  public:
		HomeMenuScreen(std::move_only_function<void()> onStartStoryMode,
		               std::move_only_function<void()> onStartArcadeMode);
		virtual ~HomeMenuScreen();

		void OnStartStoryMode();
		void OnStartArcadeMode();
		void OnShowOptionsMenu();
		void OnShowHelpMenu();
		void OnShowMoreGamesMenu();
		void OnShowScrapbookMenu();

		// IState
		virtual bool Begin();
		virtual void End();
		virtual int Process();

	  private:
		HomeMenuView* view;

		CR::Engine::Audio::Handles::Music m_musicHandle;

		std::move_only_function<void()> m_onStartStoryMode;
		std::move_only_function<void()> m_onStartArcadeMode;

		bool m_requestShowOptionsMenu;
		bool m_requestShowHelpMenu;
		bool m_requestShowMoreGamesMenu;
		bool m_requestShowScrapbookMenu;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceaud   = CR::Engine::Audio;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::HomeMenuScreen::HomeMenuScreen(std::move_only_function<void()> onStartStoryMode,
                                   std::move_only_function<void()> onStartArcadeMode) :
    m_onStartStoryMode(std::move(onStartStoryMode)), m_onStartArcadeMode(std::move(onStartArcadeMode)) {
	m_requestShowOptionsMenu   = false;
	m_requestShowHelpMenu      = false;
	m_requestShowMoreGamesMenu = false;
	m_requestShowScrapbookMenu = false;

	m_musicHandle = ceaud::Music::GetHandle(cecore::C_Hash64("bgmMenu"));
}

cg::HomeMenuScreen::~HomeMenuScreen() {
	delete view;
}

// IState
bool cg::HomeMenuScreen::Begin() {
	view = new HomeMenuView();
	view->OnStartStoryMode += Delegate(this, &HomeMenuScreen::OnStartStoryMode);
	view->OnStartArcadeMode += Delegate(this, &HomeMenuScreen::OnStartArcadeMode);
	view->OnShowOptionsMenu += Delegate(this, &HomeMenuScreen::OnShowOptionsMenu);
	view->OnShowHelpMenu += Delegate(this, &HomeMenuScreen::OnShowHelpMenu);
	view->OnShowMoreGamesMenu += Delegate(this, &HomeMenuScreen::OnShowMoreGamesMenu);
	view->OnShowScrapbookMenu += Delegate(this, &HomeMenuScreen::OnShowScrapbookMenu);

	ceaud::Music::Play(m_musicHandle);

	return true;
}

void cg::HomeMenuScreen::End() {
	delete view;
	view = nullptr;
}

int cg::HomeMenuScreen::Process() {
	view->Update();
	view->Render();

	if(m_requestShowOptionsMenu) {
		m_requestShowOptionsMenu = false;
		return cg::Constants::OPTIONS_MENU_STATE;
	} else if(m_requestShowHelpMenu) {
		m_requestShowHelpMenu = false;
		return cg::Constants::HELP_MENU_STATE;
	} else if(m_requestShowMoreGamesMenu) {
		m_requestShowMoreGamesMenu = false;
		return cg::Constants::MORE_GAMES_MENU_STATE;
	} else if(m_requestShowScrapbookMenu) {
		m_requestShowScrapbookMenu = false;
		return cg::Constants::SCRAPBOOK_MENU_STATE;
	}

	return CR::Utility::IState::UNCHANGED;
}

void cg::HomeMenuScreen::OnStartStoryMode() {
	m_onStartStoryMode();
}

void cg::HomeMenuScreen::OnStartArcadeMode() {
	m_onStartArcadeMode();
}

void cg::HomeMenuScreen::OnShowOptionsMenu() {
	// mainMenuGameState->ShowOptionsMenu();
	m_requestShowOptionsMenu = true;
}

void cg::HomeMenuScreen::OnShowHelpMenu() {
	m_requestShowHelpMenu = true;
}

void cg::HomeMenuScreen::OnShowMoreGamesMenu() {
	m_requestShowMoreGamesMenu = true;
}

void cg::HomeMenuScreen::OnShowScrapbookMenu() {
	m_requestShowScrapbookMenu = true;
}