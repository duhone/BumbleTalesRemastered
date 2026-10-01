/*
 *  OptionsMenuView.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/22/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <glm/glm.hpp>

export module CR.Game.OptionsMenuView;

import CR.Game.IView;
import CR.Game.Event;
import CR.Game.CheckboxControl;
import CR.Game.InputButton;
import CR.Game.ConfirmEraseDialog;
import CR.Game.SettingsSaveInfo;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class OptionsMenuView : public IView {
	  public:
		OptionsMenuView(bool allowEraseData);
		~OptionsMenuView();

		void Update();
		void Render();

		void LoadFromSaveInfo(SettingsSaveInfo* saveInfo);
		void WriteToSaveInfo(SettingsSaveInfo* saveInfo);

		CR::Utility::Event OnExitClicked;
		CR::Utility::Event OnEraseData;

		// Delegate Methods
		void OnExitButtonClicked();
		void OnEraseDataButtonClicked();

		void OnConfirmEraseYes();
		void OnConfirmEraseNo();

	  private:
		CheckboxControl* hintsCheckbox;
		CheckboxControl* musicCheckbox;
		CheckboxControl* soundCheckbox;

		void OnMusicToggled();
		void OnSoundToggled();

		bool eraseDataVisible;
		CR::Engine::Graphics::Handles::Sprite menuBackground;
		CR::Engine::Graphics::Handles::Sprite headerSprite;
		InputButton* exitButton;
		InputButton* eraseDataButton;
		ConfirmEraseDialog* confirmEraseDialog;

		bool m_RenderConfirmEraseDialog;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceaudio = CR::Engine::Audio;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::OptionsMenuView::OptionsMenuView(bool allowEraseData) {
	eraseDataVisible = allowEraseData;

	exitButton = new InputButton();
	exitButton->SetSpriteAndBounds(102, 840, cecore::C_Hash64("OptionsButton_Exit2"), 40);
	exitButton->OnClicked += CR::Utility::Delegate(this, &OptionsMenuView::OnExitButtonClicked);
	exitButton->SetSound(cecore::C_Hash64("shopopen"));

	headerSprite = cegraph::Sprites::Create(cecore::C_Hash64("OptionsHeader"));
	cegraph::Sprites::SetPosition(headerSprite, glm::vec2(130, 46));
	cegraph::Sprites::SetZOrder(headerSprite, 40);

	eraseDataButton = nullptr;
	if(eraseDataVisible) {
		eraseDataButton = new InputButton();
		eraseDataButton->SetSpriteAndBounds(404, 16, cecore::C_Hash64("OptionsButton_EraseData"), 40);
		eraseDataButton->OnClicked += CR::Utility::Delegate(this, &OptionsMenuView::OnEraseDataButtonClicked);
		eraseDataButton->SetSound(cecore::C_Hash64("shopopen"));
		cegraph::Sprites::SetPosition(headerSprite, glm::vec2(20, 46));
	}

	menuBackground = cegraph::Sprites::Create(cecore::C_Hash64("OptionsBase"));
	cegraph::Sprites::SetPosition(menuBackground, glm::vec2(0, 0));

	confirmEraseDialog = new ConfirmEraseDialog();
	confirmEraseDialog->OnYes += CR::Utility::Delegate(this, &OptionsMenuView::OnConfirmEraseYes);
	confirmEraseDialog->OnNo += CR::Utility::Delegate(this, &OptionsMenuView::OnConfirmEraseNo);
	m_RenderConfirmEraseDialog = false;

	// Checkboxes
	hintsCheckbox = new CheckboxControl();
	hintsCheckbox->SetSpriteAndPosition(cecore::C_Hash64("OptionsCheck"), 94, 180);
	hintsCheckbox->SetButtonBounds(60, 204, 140, 140);

	musicCheckbox = new CheckboxControl();
	musicCheckbox->SetSpriteAndPosition(cecore::C_Hash64("OptionsCheck"), 94, 388);
	musicCheckbox->SetButtonBounds(60, 414, 140, 240);
	musicCheckbox->OnCheckChanged += CR::Utility::Delegate(this, &OptionsMenuView::OnMusicToggled);

	soundCheckbox = new CheckboxControl();
	soundCheckbox->SetSpriteAndPosition(cecore::C_Hash64("OptionsCheck"), 94, 600);
	soundCheckbox->SetButtonBounds(60, 626, 140, 140);
	soundCheckbox->OnCheckChanged += CR::Utility::Delegate(this, &OptionsMenuView::OnSoundToggled);
}

cg::OptionsMenuView::~OptionsMenuView() {
	if(eraseDataButton != nullptr) delete eraseDataButton;

	if(confirmEraseDialog != nullptr) delete confirmEraseDialog;

	delete hintsCheckbox;
	delete musicCheckbox;
	delete soundCheckbox;
	delete exitButton;
	cegraph::Sprites::Delete(menuBackground);
	cegraph::Sprites::Delete(headerSprite);
}

void cg::OptionsMenuView::Update() {
	if(m_RenderConfirmEraseDialog) {
		hintsCheckbox->Disabled(true);
		musicCheckbox->Disabled(true);
		soundCheckbox->Disabled(true);
	} else {
		hintsCheckbox->Disabled(false);
		musicCheckbox->Disabled(false);
		soundCheckbox->Disabled(false);
	}
	hintsCheckbox->Update();
	musicCheckbox->Update();
	soundCheckbox->Update();
	exitButton->Update();
	if(eraseDataButton != nullptr) eraseDataButton->Update();
	confirmEraseDialog->Update();
	confirmEraseDialog->SetDisabled(!m_RenderConfirmEraseDialog);
}

void cg::OptionsMenuView::Render() {
	hintsCheckbox->Render();
	musicCheckbox->Render();
	soundCheckbox->Render();
	exitButton->Render();

	if(eraseDataButton != nullptr) eraseDataButton->Render();

	confirmEraseDialog->Render();
}

void cg::OptionsMenuView::LoadFromSaveInfo(SettingsSaveInfo* saveInfo) {
	hintsCheckbox->SetChecked(saveInfo->GetOptionsHintsOn());
	musicCheckbox->SetChecked(saveInfo->GetOptionsMusicOn());
	soundCheckbox->SetChecked(saveInfo->GetOptionsSoundOn());
}

void cg::OptionsMenuView::WriteToSaveInfo(SettingsSaveInfo* saveInfo) {
	saveInfo->SetOptionsHintsOn(hintsCheckbox->IsChecked());
	saveInfo->SetOptionsMusicOn(musicCheckbox->IsChecked());
	saveInfo->SetOptionsSoundOn(soundCheckbox->IsChecked());
}

void cg::OptionsMenuView::OnExitButtonClicked() {
	OnExitClicked();
}

void cg::OptionsMenuView::OnEraseDataButtonClicked() {
	m_RenderConfirmEraseDialog = true;
}

void cg::OptionsMenuView::OnConfirmEraseYes() {
	m_RenderConfirmEraseDialog = false;
	OnEraseData();

	OnMusicToggled();
	OnSoundToggled();
}

void cg::OptionsMenuView::OnConfirmEraseNo() {
	m_RenderConfirmEraseDialog = false;
}

void cg::OptionsMenuView::OnMusicToggled() {
	ceaudio::setMusicVolume(musicCheckbox->IsChecked() ? 1.0f : 0.0f);
}

void cg::OptionsMenuView::OnSoundToggled() {
	ceaudio::setFXVolume(soundCheckbox->IsChecked() ? 1.0f : 0.0f);
}
