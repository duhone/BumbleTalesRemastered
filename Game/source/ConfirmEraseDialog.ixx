/*
 *  ConfirmEraseDialog.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/22/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game.ConfirmEraseDialog;

import CR.Game.IInputObject;
import CR.Game.Event;
import CR.Game.InputButton;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class ConfirmEraseDialog {
	  public:
		ConfirmEraseDialog();
		virtual ~ConfirmEraseDialog();

		// IRenderable
		void Update();
		void Render();
		void SetPosition(float xLoc, float yLoc);
		void PauseAnimation(bool pause);

		// Events
		CR::Utility::Event OnYes;
		CR::Utility::Event OnNo;

		// Delegates
		void OnYesClicked();
		void OnNoClicked();
		virtual void InputChanged() {}

	  private:
		CR::Engine::Graphics::Handles::Sprite confirmEraseSprite;
		InputButton* yesButton;
		InputButton* noButton;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceinput = CR::Engine::Input;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::ConfirmEraseDialog::ConfirmEraseDialog() {
	confirmEraseSprite = cegraph::Sprites::Create(cecore::C_Hash64("OptionsConfirmErase"));
	cegraph::Sprites::SetZOrder(confirmEraseSprite, 30);

	yesButton = new InputButton();
	yesButton->SetSpriteAndBounds(0, 0, cecore::C_Hash64("OptionsButton_Yes2"), 0);
	yesButton->OnClicked += CR::Utility::Delegate(this, &ConfirmEraseDialog::OnYesClicked);
	yesButton->SetSound(cecore::C_Hash64("shopopen"));

	noButton = new InputButton();
	noButton->SetSpriteAndBounds(0, 0, cecore::C_Hash64("OptionsButton_No2"), 0);
	noButton->OnClicked += CR::Utility::Delegate(this, &ConfirmEraseDialog::OnNoClicked);
	noButton->SetSound(cecore::C_Hash64("shopopen"));

	SetPosition(320, 480);
}

cg::ConfirmEraseDialog::~ConfirmEraseDialog() {}

// IRenderable
void cg::ConfirmEraseDialog::Update() {
	yesButton->Update();
	noButton->Update();
}

void cg::ConfirmEraseDialog::Render() {
	yesButton->Render();
	noButton->Render();
}

void cg::ConfirmEraseDialog::SetPosition(float xLoc, float yLoc) {
	CR::Engine::Graphics::Sprites::SetPosition(confirmEraseSprite, {0, 0});
	yesButton->SetPosition(uint32_t(xLoc) - 270, uint32_t(yLoc) + 10);
	noButton->SetPosition(uint32_t(xLoc) + 16, uint32_t(yLoc) + 10);
}

void cg::ConfirmEraseDialog::PauseAnimation([[maybe_unused]] bool pause) {}

void cg::ConfirmEraseDialog::OnYesClicked() {
	OnYes();
}

void cg::ConfirmEraseDialog::OnNoClicked() {
	OnNo();
}