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

cg::HelpMenuView::HelpMenuView(bool showCredits) {
	exitButton = new InputButton();
	exitButton->OnClicked += CR::Utility::Delegate(this, &HelpMenuView::OnExitButtonClicked);
	exitButton->SetSound(cecore::C_Hash64("shopopen"));

	howToPlayButton = new InputButton();
	howToPlayButton->OnClicked += CR::Utility::Delegate(this, &HelpMenuView::OnHowToPlayClicked);
	howToPlayButton->SetSpriteAndBounds(322, 864, cecore::C_Hash64("HelpButton_HowToPlay2"), 40);
	howToPlayButton->SetSound(cecore::C_Hash64("shopopen"));

	leftButton = new InputButton();
	leftButton->OnClicked += CR::Utility::Delegate(this, &HelpMenuView::OnLeftClicked);
	leftButton->SetSpriteAndBounds(4, 864, cecore::C_Hash64("ScrapbookButton_LArrow"), 40);
	leftButton->SetSound(cecore::C_Hash64("shopopen"));

	rightButton = new InputButton();
	rightButton->OnClicked += CR::Utility::Delegate(this, &HelpMenuView::OnRightClicked);
	rightButton->SetSpriteAndBounds(442, 864, cecore::C_Hash64("ScrapbookButton_RArrow"), 40);
	rightButton->SetSound(cecore::C_Hash64("shopopen"));

	menuBackground = cegraph::Sprites::Create(cecore::C_Hash64("HelpInfoBG"));
	cegraph::Sprites::SetPosition(menuBackground, glm::vec2(0, 0));
	cegraph::Sprites::SetZOrder(menuBackground, 0);

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
	m_currScreenSprite     = cegraph::Sprites::Create(spriteHash);
}

void cg::HelpMenuView::SetScreen(HelpMenuScreenType screenType) {
	m_currScreen = screenType;

	// Screen Backdrop
	switch(screenType) {
		case InfoHelpCredits:
			SetCurrentSprite(cecore::C_Hash64("HelpCredits2"));
			break;
		case InfoHelp1:
			SetCurrentSprite(cecore::C_Hash64("HelpInfoHelp"));
			cegraph::Sprites::SetFrame(m_currScreenSprite, 0);
			break;
		case InfoHelp2:
			SetCurrentSprite(cecore::C_Hash64("HelpInfoHelp"));
			cegraph::Sprites::SetFrame(m_currScreenSprite, 1);
			break;
		case InfoHelp3:
			SetCurrentSprite(cecore::C_Hash64("HelpInfoHelp"));
			cegraph::Sprites::SetFrame(m_currScreenSprite, 2);
			break;
			break;
		case InfoHelp4:
			SetCurrentSprite(cecore::C_Hash64("HelpInfoHelp"));
			cegraph::Sprites::SetFrame(m_currScreenSprite, 3);
			break;
		default:
			break;
	}

	// Screen Buttons
	if(screenType == InfoHelpCredits) {
		howToPlayButton->Disabled(false);
		rightButton->Disabled(true);
		leftButton->Disabled(true);
		exitButton->SetSpriteAndBounds(4, 864, cecore::C_Hash64("HelpButton_MainMenu2"), 40);
		cegraph::Sprites::SetPosition(m_currScreenSprite, glm::vec2(8, 6));
	} else {
		howToPlayButton->Disabled(true);

		if(screenType == InfoHelp1)
			leftButton->Disabled(true);
		else
			leftButton->Disabled(false);

		if(screenType == InfoHelp4)
			rightButton->Disabled(true);
		else
			rightButton->Disabled(false);

		exitButton->SetSpriteAndBounds(224, 864, cecore::C_Hash64("ScrapbookButton_Exit"), 40);
		cegraph::Sprites::SetPosition(m_currScreenSprite, glm::vec2(4, 6));
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