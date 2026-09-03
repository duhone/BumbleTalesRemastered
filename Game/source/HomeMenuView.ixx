/*
 *  HomeMenuView.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 5/22/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game.HomeMenuView;

import CR.Game.IView;
import CR.Game.Event;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class HomeMenuView : public IView {
	  public:
		HomeMenuView();
		~HomeMenuView();

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
		void OnMoreGamesClicked();

	  private:
		CR::Engine::Graphics::Handles::Sprite menuBackground;
		/*Input_Button* storyModeButton;
		Input_Button* arcadeModeButton;
		Input_Button* scrapbookButton;
		Input_Button* helpButton;
		Input_Button* optionsButton;
		Input_Button* moreGamesButton;*/
	};
}    // namespace CR::Game

module :private;

namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::HomeMenuView::HomeMenuView() {
	/*storyModeButton = new Input_Button();
	storyModeButton->SetSpriteAndBounds(13, 232, CR::AssetList::Menu_Story_Mode_Button, 800);
	storyModeButton->DesignSize(289, 57);
	storyModeButton->OnClicked += Delegate(this, &HomeMenuView::OnStoryModeClicked);
	storyModeButton->SetSound(CR::AssetList::sounds::shopopen::ID);
	input_objects.push_back(storyModeButton);

	arcadeModeButton = new Input_Button();
	arcadeModeButton->SetSpriteAndBounds(13, 292, CR::AssetList::Menu_Arcade_Mode_Button, 800);
	arcadeModeButton->DesignSize(289, 57);
	arcadeModeButton->OnClicked += Delegate(this, &HomeMenuView::OnArcadeModeClicked);
	arcadeModeButton->SetSound(CR::AssetList::sounds::shopopen::ID);
	input_objects.push_back(arcadeModeButton);

	scrapbookButton = new Input_Button();
	scrapbookButton->SetSpriteAndBounds(13, 352, CR::AssetList::Menu_Scrapbook_Button, 800);
	scrapbookButton->DesignSize(289, 57);
	scrapbookButton->OnClicked += Delegate(this, &HomeMenuView::OnScrapbookClicked);
	scrapbookButton->SetSound(CR::AssetList::sounds::shopopen::ID);
	input_objects.push_back(scrapbookButton);

	helpButton = new Input_Button();
	helpButton->SetSpriteAndBounds(13, 412, CR::AssetList::Menu_Help_Info_Button, 800);
	helpButton->DesignSize(98, 57);
	// helpButton->SetSpriteAndBounds(13, 412, CR::AssetList::Menu_More_Games_Button, 800);
	helpButton->OnClicked += Delegate(this, &HomeMenuView::OnHelpClicked);
	helpButton->SetSound(CR::AssetList::sounds::shopopen::ID);
	input_objects.push_back(helpButton);

	optionsButton = new Input_Button();
	optionsButton->SetSpriteAndBounds(109, 412, CR::AssetList::Menu_Options_Button, 800);
	optionsButton->DesignSize(97, 57);
	optionsButton->OnClicked += Delegate(this, &HomeMenuView::OnOptionsClicked);
	optionsButton->SetSound(CR::AssetList::sounds::shopopen::ID);
	input_objects.push_back(optionsButton);

	moreGamesButton = new Input_Button();
	moreGamesButton->SetSpriteAndBounds(205, 412, CR::AssetList::Menu_More_Games_Button, 800);
	moreGamesButton->DesignSize(97, 57);
	moreGamesButton->OnClicked += Delegate(this, &HomeMenuView::OnMoreGamesClicked);
	moreGamesButton->SetSound(CR::AssetList::sounds::shopopen::ID);
	input_objects.push_back(moreGamesButton);

	menuBackground = graphics_engine->CreateSprite1(false, 1000);
	menuBackground->SetImage(CR::AssetList::Menu_Background);
	menuBackground->SetDesignSize(320, 480);
	menuBackground->SetPositionAbsolute(160, 240);*/
}

cg::HomeMenuView::~HomeMenuView() {
	/*delete storyModeButton;
	delete arcadeModeButton;
	delete scrapbookButton;
	delete helpButton;
	delete optionsButton;
	delete moreGamesButton;
	menuBackground->Release();*/
}

void cg::HomeMenuView::Update() {}

void cg::HomeMenuView::Render() {
	/*graphics_engine->BeginFrame();
	menuBackground->Render();
	storyModeButton->Render();
	arcadeModeButton->Render();
	scrapbookButton->Render();
	helpButton->Render();
	optionsButton->Render();
	moreGamesButton->Render();
	graphics_engine->EndFrame();*/
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

void cg::HomeMenuView::OnMoreGamesClicked() {
	OnShowMoreGamesMenu();
}
