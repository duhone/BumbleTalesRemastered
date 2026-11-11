/*
 *  HelpMenuView.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/24/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <glm/glm.hpp>

export module CR.Game.HelpMenuView;

import CR.Game.IView;
import CR.Game.Event;
import CR.Game.InputButton;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	enum HelpMenuScreenType { InfoHelpCredits, InfoHelp1, InfoHelp2, InfoHelp3, InfoHelp4 };

	class HelpMenuView : public IView {
	  public:
		HelpMenuView(bool showCredits);
		~HelpMenuView();

		void Update();
		void Render();

		CR::Utility::Event OnExitClicked;

		// Delegate Methods
		void OnExitButtonClicked();
		void OnLeftClicked();
		void OnRightClicked();
		void OnHowToPlayClicked();

	  private:
		void SetScreen(HelpMenuScreenType screenType);
		void SetCurrentSprite(uint64_t spriteHash);

		bool showCredits;
		CR::Engine::Graphics::Handles::Sprite menuBackground;
		InputButton* exitButton;
		InputButton* howToPlayButton;
		InputButton* leftButton;
		InputButton* rightButton;

		HelpMenuScreenType m_currScreen;
		CR::Engine::Graphics::Handles::Sprite m_currScreenSprite;
		uint64_t m_currScreenSpriteHash;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

using namespace cecore::Literals;

cg::HelpMenuView::HelpMenuView(bool showCredits) {
	exitButton = new InputButton();
	exitButton->OnClicked += CR::Utility::Delegate(this, &HelpMenuView::OnExitButtonClicked);
	exitButton->SetSound("shopopen"_hash64);

	howToPlayButton = new InputButton();
	howToPlayButton->OnClicked += CR::Utility::Delegate(this, &HelpMenuView::OnHowToPlayClicked);
	howToPlayButton->SetSpriteAndBounds(322, 864, "HelpButton_HowToPlay2"_hash64, 40);
	howToPlayButton->SetSound("shopopen"_hash64);

	leftButton = new InputButton();
	leftButton->OnClicked += CR::Utility::Delegate(this, &HelpMenuView::OnLeftClicked);
	leftButton->SetSpriteAndBounds(4, 864, "ScrapbookButton_LArrow"_hash64, 40);
	leftButton->SetSound("shopopen"_hash64);

	rightButton = new InputButton();
	rightButton->OnClicked += CR::Utility::Delegate(this, &HelpMenuView::OnRightClicked);
	rightButton->SetSpriteAndBounds(442, 864, "ScrapbookButton_RArrow"_hash64, 40);
	rightButton->SetSound("shopopen"_hash64);

	cegraph::Sprites::Create("HelpInfoBG"_hash64, menuBackground);
	cegraph::Sprites::SetPositions(menuBackground, glm::vec2(0, 0));
	cegraph::Sprites::SetZOrders(menuBackground, 0);

	if(showCredits)
		SetScreen(InfoHelpCredits);
	else
		SetScreen(InfoHelp1);
}

cg::HelpMenuView::~HelpMenuView() {
	delete exitButton;
	delete leftButton;
	delete rightButton;
	delete howToPlayButton;

	cegraph::Sprites::Delete(menuBackground);
	if(m_currScreenSprite.isValid()) { cegraph::Sprites::Delete(m_currScreenSprite); }
}

void cg::HelpMenuView::Update() {
	exitButton->Update();
	howToPlayButton->Update();
	leftButton->Update();
	rightButton->Update();
}

void cg::HelpMenuView::Render() {
	exitButton->Render();
	howToPlayButton->Render();
	leftButton->Render();
	rightButton->Render();
}

void cg::HelpMenuView::SetCurrentSprite(uint64_t spriteHash) {
	if(m_currScreenSpriteHash == spriteHash) { return; }

	if(m_currScreenSprite.isValid()) { cegraph::Sprites::Delete(m_currScreenSprite); }
	m_currScreenSpriteHash = spriteHash;
	cegraph::Sprites::Create(spriteHash, m_currScreenSprite);
}

void cg::HelpMenuView::SetScreen(HelpMenuScreenType screenType) {
	m_currScreen = screenType;

	// Screen Backdrop
	switch(screenType) {
		case InfoHelpCredits:
			SetCurrentSprite("HelpCredits2"_hash64);
			break;
		case InfoHelp1:
			SetCurrentSprite("HelpInfoHelp"_hash64);
			cegraph::Sprites::SetFrames(m_currScreenSprite, 0);
			break;
		case InfoHelp2:
			SetCurrentSprite("HelpInfoHelp"_hash64);
			cegraph::Sprites::SetFrames(m_currScreenSprite, 1);
			break;
		case InfoHelp3:
			SetCurrentSprite("HelpInfoHelp"_hash64);
			cegraph::Sprites::SetFrames(m_currScreenSprite, 2);
			break;
			break;
		case InfoHelp4:
			SetCurrentSprite("HelpInfoHelp"_hash64);
			cegraph::Sprites::SetFrames(m_currScreenSprite, 3);
			break;
		default:
			break;
	}

	// Screen Buttons
	if(screenType == InfoHelpCredits) {
		howToPlayButton->Disabled(false);
		howToPlayButton->SetVisible(true);
		rightButton->Disabled(true);
		rightButton->SetVisible(false);
		leftButton->Disabled(true);
		leftButton->SetVisible(false);
		exitButton->SetSpriteAndBounds(4, 864, "HelpButton_MainMenu2"_hash64, 40);
		cegraph::Sprites::SetPositions(m_currScreenSprite, glm::vec2(8, 6));
	} else {
		howToPlayButton->Disabled(true);
		howToPlayButton->SetVisible(false);

		if(screenType == InfoHelp1) {
			leftButton->Disabled(true);
			leftButton->SetVisible(false);
		} else {
			leftButton->Disabled(false);
			leftButton->SetVisible(true);
		}

		if(screenType == InfoHelp4) {
			rightButton->Disabled(true);
			rightButton->SetVisible(false);
		} else {
			rightButton->Disabled(false);
			rightButton->SetVisible(true);
		}

		exitButton->SetSpriteAndBounds(224, 864, "ScrapbookButton_Exit"_hash64, 40);
		cegraph::Sprites::SetPositions(m_currScreenSprite, glm::vec2(4, 6));
	}
}

void cg::HelpMenuView::OnExitButtonClicked() {
	OnExitClicked();
}

void cg::HelpMenuView::OnHowToPlayClicked() {
	SetScreen(InfoHelp1);
}

void cg::HelpMenuView::OnLeftClicked() {
	SetScreen((HelpMenuScreenType)((int)m_currScreen - 1));
}

void cg::HelpMenuView::OnRightClicked() {
	SetScreen((HelpMenuScreenType)((int)m_currScreen + 1));
}