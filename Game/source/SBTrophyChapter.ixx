/*
 *  SBTrophyChapter.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/5/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.SBTrophyChapter;

import CR.Game.CharacterLevels;
import CR.Game.CharacterTale;
import CR.Game.Event;
import CR.Game.IScrapbookChapter;
import CR.Game.TrophySaveInfo;
import CR.Game.Trophies;
import CR.Game.TrophyButton;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class SBTrophyChapter : public IScrapbookChapter {
	  public:
		SBTrophyChapter();
		virtual ~SBTrophyChapter();

		// IScrapbookChapter
		void Update();
		void Render();
		void NextPage();
		void PrevPage();
		ScrapbookChapter GetChapterType();

		void LoadFromTrophySaveInfo(TrophySaveInfo* saveInfo);

		void Reset() {};
		void FreeResources() {};

		void OnTrophyPressed(TrophyName tName);

	  private:
		int currPageNum;
		TrophySaveInfo* trophySaveInfo;
		CR::Engine::Graphics::Handles::Sprite bgSprite;
		std::vector<TrophyButton*> trophyButtons;
		bool isTrophyNameVisible;
		CR::Engine::Graphics::Handles::Sprite trophyName;
		CR::Engine::Graphics::Handles::Sprite pageTags;
		CR::Engine::Graphics::Handles::Sprite trophyInfo;
		CR::Engine::Graphics::Handles::Sprite scrapBookTags;
		Trophies* trophies;
		int selectedTrophy;

		void SetupTrophies();
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::SBTrophyChapter::SBTrophyChapter() {
	currPageNum = 0;

	bgSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookTrophyBase1"));
	cegraph::Sprites::SetZOrder(bgSprite, 0);
	cegraph::Sprites::SetPosition(bgSprite, glm::vec2(0, 144));

	trophyName = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookTrophyNames"));
	cegraph::Sprites::SetZOrder(trophyName, 10);
	cegraph::Sprites::SetPosition(trophyName, glm::vec2(42, 230));

	scrapBookTags = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookTags1"));
	cegraph::Sprites::SetZOrder(scrapBookTags, 19);
	cegraph::Sprites::SetPosition(scrapBookTags, glm::vec2(14, 98));
	cegraph::Sprites::SetFrame(scrapBookTags, 16);

	pageTags = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookTags2"));
	cegraph::Sprites::SetZOrder(pageTags, 19);
	cegraph::Sprites::SetPosition(pageTags, glm::vec2(482, 104));

	trophyInfo = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookTrophyInfo"));
	cegraph::Sprites::SetZOrder(trophyInfo, 10);
	cegraph::Sprites::SetPosition(trophyInfo, glm::vec2(68, 684));

	selectedTrophy      = -1;
	isTrophyNameVisible = false;
}

cg::SBTrophyChapter::~SBTrophyChapter() {
	cegraph::Sprites::Delete(bgSprite);
	cegraph::Sprites::Delete(trophyName);
	cegraph::Sprites::Delete(pageTags);
	cegraph::Sprites::Delete(trophyInfo);
	cegraph::Sprites::Delete(scrapBookTags);

	for(int i = 0; i < trophyButtons.size(); i++) delete trophyButtons[i];
}

void cg::SBTrophyChapter::Update() {
	cegraph::Sprites::SetFrame(trophyName, uint16_t(currPageNum));
	cegraph::Sprites::SetFrame(pageTags, uint16_t(3 + currPageNum));
}

void cg::SBTrophyChapter::Render() {
	cegraph::Sprites::SetVisibility(trophyInfo, isTrophyNameVisible);

	int tOffset = currPageNum * 12;
	for(int i = 0; i < trophyButtons.size(); i++) {
		if(i >= tOffset && i < tOffset + 12) {
			trophyButtons[i]->Disabled(false);
		} else {
			trophyButtons[i]->Disabled(true);
		}
		trophyButtons[i]->Update();
		trophyButtons[i]->Render();
	}
}

void cg::SBTrophyChapter::NextPage() {
	currPageNum++;
	if(currPageNum > 2) currPageNum = 0;

	isTrophyNameVisible = false;
}

void cg::SBTrophyChapter::PrevPage() {
	currPageNum--;
	if(currPageNum < 0) currPageNum = 2;

	isTrophyNameVisible = false;
}

cg::ScrapbookChapter cg::SBTrophyChapter::GetChapterType() {
	return TrophiesChapter;
}

void cg::SBTrophyChapter::LoadFromTrophySaveInfo(TrophySaveInfo* saveInfo) {
	trophySaveInfo = saveInfo;
	trophies       = trophySaveInfo->GetTrophies();
	SetupTrophies();
}

void cg::SBTrophyChapter::OnTrophyPressed(TrophyName tName) {
	isTrophyNameVisible = true;
	cegraph::Sprites::SetFrame(trophyInfo, (uint16_t)tName);
}

void cg::SBTrophyChapter::SetupTrophies() {
	// Create the trophy buttons
	for(int i = 0; i < 36; i++) {
		TrophyButton* tButton = new TrophyButton((TrophyName)i, 10);
		tButton->SetHasTrophy(trophies->GetTrophy((TrophyName)i));
		tButton->OnClicked +=
		    Utility::Delegate1<SBTrophyChapter, TrophyName>(this, &SBTrophyChapter::OnTrophyPressed);
		trophyButtons.push_back(tButton);
	}

	int tOffset = 0;
	int xOffset = 12;
	int yOffset = 168;
	int iX      = 0;
	int iY      = 0;

	for(int i = 0; i < 3; i++) {
		tOffset = i * 12;
		iX      = 0;
		iY      = 0;
		for(int j = tOffset; j < tOffset + 12; j++) {
			trophyButtons[j]->SetPosition(iX * 154 + xOffset, iY * 162 + yOffset);

			iX++;
			if(iX > 3) {
				iX = 0;
				iY++;
			}
		}
	}
}
