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

cg::HomeMenuView::HomeMenuView() {
	storyModeButton = new InputButton();
	storyModeButton->SetSpriteAndBounds(26, 464, cecore::C_Hash64("MenuButton_StoryMode"), 80);
	storyModeButton->OnClicked += CR::Utility::Delegate(this, &HomeMenuView::OnStoryModeClicked);
	storyModeButton->SetSound(cecore::C_Hash64("shopopen"));

	arcadeModeButton = new InputButton();
	arcadeModeButton->SetSpriteAndBounds(26, 584, cecore::C_Hash64("MenuButton_ArcadeMode"), 80);
	arcadeModeButton->OnClicked += CR::Utility::Delegate(this, &HomeMenuView::OnArcadeModeClicked);
	arcadeModeButton->SetSound(cecore::C_Hash64("shopopen"));

	scrapbookButton = new InputButton();
	scrapbookButton->SetSpriteAndBounds(26, 704, cecore::C_Hash64("MenuButton_Scrapbook"), 80);
	scrapbookButton->OnClicked += CR::Utility::Delegate(this, &HomeMenuView::OnScrapbookClicked);
	scrapbookButton->SetSound(cecore::C_Hash64("shopopen"));

	helpButton = new InputButton();
	helpButton->SetSpriteAndBounds(26, 824, cecore::C_Hash64("MenuButton_HelpInfo"), 80);
	helpButton->OnClicked += CR::Utility::Delegate(this, &HomeMenuView::OnHelpClicked);
	helpButton->SetSound(cecore::C_Hash64("shopopen"));

	optionsButton = new InputButton();
	optionsButton->SetSpriteAndBounds(315, 824, cecore::C_Hash64("MenuButton_Options"), 80);
	optionsButton->OnClicked += CR::Utility::Delegate(this, &HomeMenuView::OnOptionsClicked);
	optionsButton->SetSound(cecore::C_Hash64("shopopen"));

	menuBackground = cegraph::Sprites::Create(cecore::C_Hash64("MenuBG"));
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
