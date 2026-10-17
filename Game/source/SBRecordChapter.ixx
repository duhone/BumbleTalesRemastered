/*
 *  SBRecordChapter.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/5/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.SBRecordChapter;

import CR.Game.IRenderable;
import CR.Game.CharacterLevels;
import CR.Game.CharacterTale;
import CR.Game.IScrapbookChapter;
import CR.Game.StoryRecords;
import CR.Game.ArcadeRecords;
import CR.Game.StoryModeSaveInfo;
import CR.Game.ArcadeModeSaveInfo;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class SBRecordChapter : public IScrapbookChapter {
	  public:
		SBRecordChapter();
		virtual ~SBRecordChapter();

		// IScrapbookChapter
		void Update();
		void Render();
		void NextPage();
		void PrevPage();
		ScrapbookChapter GetChapterType();

		void LoadFromArcadeSaveInfo(ArcadeModeSaveInfo* saveInfo);
		void LoadFromStorySaveInfo(StoryModeSaveInfo* saveInfo);

	  private:
		StoryModeSaveInfo* storySaveInfo;
		ArcadeModeSaveInfo* arcadeSaveInfo;

		int currPageNum;
		IRenderable* currPage;
		void SetCurrentPage();

		CR::Engine::Graphics::Handles::Sprite scrapBookTags;
		CR::Engine::Graphics::Handles::Sprite pageTags;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::SBRecordChapter::SBRecordChapter() {
	currPageNum = 0;
	currPage    = NULL;

	pageTags = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookTags2"));
	cegraph::Sprites::SetZOrder(pageTags, 19);
	cegraph::Sprites::SetPosition(pageTags, glm::vec2(482, 104));

	scrapBookTags = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookTags1"));
	cegraph::Sprites::SetZOrder(scrapBookTags, 19);
	cegraph::Sprites::SetPosition(scrapBookTags, glm::vec2(16, 98));
	cegraph::Sprites::SetFrame(scrapBookTags, 16);

	// SetCurrentPage();
}

cg::SBRecordChapter::~SBRecordChapter() {
	if(currPage != NULL) delete currPage;

	cegraph::Sprites::Delete(scrapBookTags);
	cegraph::Sprites::Delete(pageTags);
}

void cg::SBRecordChapter::Update() {
	currPage->Update();
	cegraph::Sprites::SetFrame(pageTags, uint16_t(3 + currPageNum));
	cegraph::Sprites::SetFrame(scrapBookTags, uint16_t(13 + currPageNum));
}

void cg::SBRecordChapter::Render() {
	currPage->Render();
}

void cg::SBRecordChapter::NextPage() {
	currPageNum++;
	if(currPageNum > 1) currPageNum = 0;

	SetCurrentPage();
}

void cg::SBRecordChapter::PrevPage() {
	currPageNum--;
	if(currPageNum < 0) currPageNum = 1;

	SetCurrentPage();
}

cg::ScrapbookChapter cg::SBRecordChapter::GetChapterType() {
	return RecordsChapter;
}

void cg::SBRecordChapter::SetCurrentPage() {
	if(currPage != NULL) delete currPage;

	switch(currPageNum) {
		case 0:    // Story Records
			currPage = new StoryRecords();
			currPage->SetPosition(320, 480);
			((StoryRecords*)currPage)->LoadFromStorySaveInfo(storySaveInfo);
			break;
		case 1:    // Arcade Records
			currPage = new ArcadeRecords();
			currPage->SetPosition(320, 480);
			((ArcadeRecords*)currPage)->LoadFromArcadeSaveInfo(arcadeSaveInfo);
			break;
		default:
			break;
	}
}

void cg::SBRecordChapter::LoadFromArcadeSaveInfo(ArcadeModeSaveInfo* saveInfo) {
	arcadeSaveInfo = saveInfo;
}

void cg::SBRecordChapter::LoadFromStorySaveInfo(StoryModeSaveInfo* saveInfo) {
	storySaveInfo = saveInfo;
	SetCurrentPage();
}