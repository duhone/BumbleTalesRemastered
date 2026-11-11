/*
 *  HomeMenuView.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 5/22/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <glm/glm.hpp>

export module CR.Game.HomeMenuView;

import CR.Game.IView;
import CR.Game.Event;
import CR.Game.InputButton;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class HomeMenuView : public IView {
	  public:
		HomeMenuView();
		virtual ~HomeMenuView();

		void Update();
		void Render();

		CR::Utility::Event OnStartStoryMode;
		CR::Utility::Event OnStartArcadeMode;
		CR::Utility::Event OnShowOptionsMenu;
		CR::Utility::Event OnShowHelpMenu;
		CR::Utility::Event OnShowMoreGamesMenu;
		CR::Utility::Event OnShowScrapbookMenu;

		// Delegate Methods
		void OnStoryModeClicked();
		void OnArcadeModeClicked();
		void OnScrapbookClicked();
		void OnHelpClicked();
		void OnOptionsClicked();

	  private:
		CR::Engine::Graphics::Handles::Sprite menuBackground;
		InputButton* storyModeButton;
		InputButton* arcadeModeButton;
		InputButton* scrapbookButton;
		InputButton* helpButton;
		InputButton* optionsButton;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

using namespace cecore::Literals;

cg::HomeMenuView::HomeMenuView() {
	storyModeButton = new InputButton();
	storyModeButton->SetSpriteAndBounds(26, 464, "MenuButton_StoryMode"_hash64, 80);
	storyModeButton->OnClicked += CR::Utility::Delegate(this, &HomeMenuView::OnStoryModeClicked);
	storyModeButton->SetSound("shopopen"_hash64);

	arcadeModeButton = new InputButton();
	arcadeModeButton->SetSpriteAndBounds(26, 584, "MenuButton_ArcadeMode"_hash64, 80);
	arcadeModeButton->OnClicked += CR::Utility::Delegate(this, &HomeMenuView::OnArcadeModeClicked);
	arcadeModeButton->SetSound("shopopen"_hash64);

	scrapbookButton = new InputButton();
	scrapbookButton->SetSpriteAndBounds(26, 704, "MenuButton_Scrapbook"_hash64, 80);
	scrapbookButton->OnClicked += CR::Utility::Delegate(this, &HomeMenuView::OnScrapbookClicked);
	scrapbookButton->SetSound("shopopen"_hash64);

	helpButton = new InputButton();
	helpButton->SetSpriteAndBounds(26, 824, "MenuButton_HelpInfo"_hash64, 80);
	helpButton->OnClicked += CR::Utility::Delegate(this, &HomeMenuView::OnHelpClicked);
	helpButton->SetSound("shopopen"_hash64);

	optionsButton = new InputButton();
	optionsButton->SetSpriteAndBounds(315, 824, "MenuButton_Options"_hash64, 80);
	optionsButton->OnClicked += CR::Utility::Delegate(this, &HomeMenuView::OnOptionsClicked);
	optionsButton->SetSound("shopopen"_hash64);

	cegraph::Sprites::Create("MenuBG"_hash64, menuBackground);
	cegraph::Sprites::SetPositions(menuBackground, glm::vec2(0, 0));
	cegraph::Sprites::SetZOrders(menuBackground, 0);
}

cg::HomeMenuView::~HomeMenuView() {
	delete storyModeButton;
	delete arcadeModeButton;
	delete scrapbookButton;
	delete helpButton;
	delete optionsButton;
	cegraph::Sprites::Delete(menuBackground);
}

void cg::HomeMenuView::Update() {
	storyModeButton->Update();
	arcadeModeButton->Update();
	scrapbookButton->Update();
	helpButton->Update();
	optionsButton->Update();
}

void cg::HomeMenuView::Render() {
	storyModeButton->Render();
	arcadeModeButton->Render();
	scrapbookButton->Render();
	helpButton->Render();
	optionsButton->Render();
}

void cg::HomeMenuView::OnStoryModeClicked() {
	OnStartStoryMode();
}

void cg::HomeMenuView::OnArcadeModeClicked() {
	OnStartArcadeMode();
}

void cg::HomeMenuView::OnScrapbookClicked() {
	OnShowScrapbookMenu();
}

void cg::HomeMenuView::OnHelpClicked() {
	OnShowHelpMenu();
}

void cg::HomeMenuView::OnOptionsClicked() {
	OnShowOptionsMenu();
}
