/*
 *  ScrapbookMenuView.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/4/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <glm/glm.hpp>

export module CR.Game.ScrapbookMenuView;

import CR.Game.IView;
import CR.Game.Event;
import CR.Game.IScrapbookChapter;
import CR.Game.InputButton;
import CR.Game.SBCharacterChapter;
import CR.Game.SBTrophyChapter;
import CR.Game.SBRecordChapter;
import CR.Game.StoryModeSaveInfo;
import CR.Game.ArcadeModeSaveInfo;
import CR.Game.TrophySaveInfo;
import CR.Game.BuildingLevels;
import CR.Game.CharacterLevels;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class ScrapbookMenuView : public IView {
	  public:
		ScrapbookMenuView(bool wonGame = false);
		virtual ~ScrapbookMenuView();

		void Update();
		void Render();

		void OnExitButtonClicked();
		void OnPrevButtonClicked();
		void OnNextButtonClicked();
		void OnCharactersButtonClicked();
		void OnRecordsButtonClicked();
		void OnTrophiesButtonClicked();

		void LoadFromArcadeSaveInfo(ArcadeModeSaveInfo* saveInfo);
		void LoadFromTrophySaveInfo(TrophySaveInfo* saveInfo);
		void LoadFromSaveInfo(StoryModeSaveInfo* saveInfo);

		Utility::Event OnExitClicked;

	  private:
		IScrapbookChapter* m_currChapter;
		CR::Engine::Graphics::Handles::Sprite baseSprite;
		CR::Engine::Graphics::Handles::Sprite rimSprite;

		InputButton* nextButton;
		InputButton* prevButton;
		InputButton* exitButton;

		InputButton* charactersButton;
		InputButton* recordsButton;
		InputButton* trophiesButton;

		CharacterLevels* characterLevels;
		BuildingLevels* buildingLevels;

		StoryModeSaveInfo* storyModeSaveInfo;
		ArcadeModeSaveInfo* arcadeModeSaveInfo;
		TrophySaveInfo* trophySaveInfo;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceaudio = CR::Engine::Audio;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::ScrapbookMenuView::ScrapbookMenuView(bool wonGame) {
	baseSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookBG"));
	cegraph::Sprites::SetPosition(baseSprite, glm::vec2(0, 0));
	cegraph::Sprites::SetZOrder(baseSprite, 50);

	rimSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCategoryRim"));
	cegraph::Sprites::SetPosition(rimSprite, glm::vec2(10, 10));
	cegraph::Sprites::SetZOrder(rimSprite, 40);

	exitButton = new InputButton();
	exitButton->SetSpriteAndBounds(224, 864, cecore::C_Hash64("ScrapbookButton_Exit"), 30);
	exitButton->OnClicked += Utility::Delegate(this, &ScrapbookMenuView::OnExitButtonClicked);
	exitButton->SetSound(cecore::C_Hash64("shopopen"));

	prevButton = new InputButton();
	prevButton->SetSpriteAndBounds(4, 864, cecore::C_Hash64("ScrapbookButton_LArrow"), 40);
	prevButton->OnClicked += Utility::Delegate(this, &ScrapbookMenuView::OnPrevButtonClicked);
	prevButton->SetSound(cecore::C_Hash64("scrapbookpageturn1"));

	nextButton = new InputButton();
	nextButton->SetSpriteAndBounds(434, 864, cecore::C_Hash64("ScrapbookButton_RArrow"), 30);
	nextButton->OnClicked += Utility::Delegate(this, &ScrapbookMenuView::OnNextButtonClicked);
	nextButton->SetSound(cecore::C_Hash64("scrapbookpageturn1"));

	charactersButton = new InputButton();
	charactersButton->SetSpriteAndBounds(2, 10, cecore::C_Hash64("ScrapbookButton_Characters"), 30);
	charactersButton->OnClicked += Utility::Delegate(this, &ScrapbookMenuView::OnCharactersButtonClicked);
	charactersButton->SetSound(cecore::C_Hash64("shopopen"));

	recordsButton = new InputButton();
	recordsButton->SetSpriteAndBounds(212, 10, cecore::C_Hash64("ScrapbookButton_Records"), 30);
	recordsButton->OnClicked += Utility::Delegate(this, &ScrapbookMenuView::OnRecordsButtonClicked);
	recordsButton->SetSound(cecore::C_Hash64("shopopen"));

	trophiesButton = new InputButton();
	trophiesButton->SetSpriteAndBounds(422, 10, cecore::C_Hash64("ScrapbookButton_Trophies"), 30);
	trophiesButton->OnClicked += Utility::Delegate(this, &ScrapbookMenuView::OnTrophiesButtonClicked);
	trophiesButton->SetSound(cecore::C_Hash64("shopopen"));

	m_currChapter = new SBCharacterChapter(wonGame);

	auto musicHandle = ceaudio::Music::GetHandle(cecore::C_Hash64("bgmScrapbook"));
	ceaudio::Music::Play(musicHandle);
}

cg::ScrapbookMenuView::~ScrapbookMenuView() {
	cegraph::Sprites::Delete(baseSprite);
	cegraph::Sprites::Delete(rimSprite);
	delete exitButton;
	delete prevButton;
	delete nextButton;
	delete charactersButton;
	delete recordsButton;
	delete trophiesButton;
	delete m_currChapter;
}

void cg::ScrapbookMenuView::Update() {
	m_currChapter->Update();
}

void cg::ScrapbookMenuView::Render() {
	prevButton->Render();
	exitButton->Render();
	nextButton->Render();
	charactersButton->Render();
	recordsButton->Render();
	trophiesButton->Render();
	m_currChapter->Render();
}

void cg::ScrapbookMenuView::OnExitButtonClicked() {
	OnExitClicked();
}

void cg::ScrapbookMenuView::OnPrevButtonClicked() {
	m_currChapter->PrevPage();
}

void cg::ScrapbookMenuView::OnNextButtonClicked() {
	m_currChapter->NextPage();
}

void cg::ScrapbookMenuView::OnCharactersButtonClicked() {
	if(m_currChapter->GetChapterType() == CharactersChapter) return;

	delete m_currChapter;
	m_currChapter = new SBCharacterChapter();
	((SBCharacterChapter*)m_currChapter)->SetHasWonGame(storyModeSaveInfo->GetHasWonGame());
	((SBCharacterChapter*)m_currChapter)->SetCharacterLevels(characterLevels);

	cegraph::Sprites::SetPosition(rimSprite, glm::vec2(20, 20));
}

void cg::ScrapbookMenuView::OnRecordsButtonClicked() {
	if(m_currChapter->GetChapterType() == RecordsChapter) return;

	delete m_currChapter;
	m_currChapter = new SBRecordChapter();
	((SBRecordChapter*)m_currChapter)->LoadFromArcadeSaveInfo(arcadeModeSaveInfo);
	((SBRecordChapter*)m_currChapter)->LoadFromStorySaveInfo(storyModeSaveInfo);

	cegraph::Sprites::SetPosition(rimSprite, glm::vec2(220, 20));
}

void cg::ScrapbookMenuView::OnTrophiesButtonClicked() {
	if(m_currChapter->GetChapterType() == TrophiesChapter) return;

	delete m_currChapter;
	m_currChapter = new SBTrophyChapter();
	((SBTrophyChapter*)m_currChapter)->LoadFromTrophySaveInfo(trophySaveInfo);

	cegraph::Sprites::SetPosition(rimSprite, glm::vec2(430, 20));
}

void cg::ScrapbookMenuView::LoadFromSaveInfo(StoryModeSaveInfo* saveInfo) {
	storyModeSaveInfo = saveInfo;

	characterLevels = saveInfo->GetCharacterLevels();
	buildingLevels  = saveInfo->GetBuildingLevels();

	// original code was doing a dynamic cast here, but we know that m_currChapter is a SBCharacterChapter at
	// this point, so we can just static_cast it as far as I can tell. nonsense OOP code anyway, fix some day.
	if(static_cast<SBCharacterChapter*>(m_currChapter) != nullptr) {
		((SBCharacterChapter*)m_currChapter)->SetCharacterLevels(characterLevels);
		((SBCharacterChapter*)m_currChapter)->SetHasWonGame(storyModeSaveInfo->GetHasWonGame());
	}
}

void cg::ScrapbookMenuView::LoadFromArcadeSaveInfo(ArcadeModeSaveInfo* saveInfo) {
	arcadeModeSaveInfo = saveInfo;
}

void cg::ScrapbookMenuView::LoadFromTrophySaveInfo(TrophySaveInfo* saveInfo) {
	trophySaveInfo = saveInfo;
}