/*
 *  SBCharacterChapter.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/4/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.SBCharacterChapter;

import CR.Game.CharacterLevels;
import CR.Game.CharacterTale;
import CR.Game.IScrapbookChapter;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class SBCharacterChapter : public IScrapbookChapter {
	  public:
		SBCharacterChapter(bool wonGame = false);
		virtual ~SBCharacterChapter();

		// IScrapbookChapter
		void Update();
		void Render();
		void NextPage();
		void PrevPage();
		ScrapbookChapter GetChapterType();
		void SetCharacterLevels(CharacterLevels* characterLevels);
		void UpdatePage();
		void SetHasWonGame(bool _value);

	  private:
		CharacterLevels* m_characterLevels;
		std::vector<CharacterTale*> characterTales;
		CR::Engine::Graphics::Handles::Sprite scrapbookTags;
		CR::Engine::Graphics::Handles::Sprite scrapbookTags2;
		int m_currPage;
		bool hasWonGame;
		bool justWonGame;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::SBCharacterChapter::SBCharacterChapter(bool wonGame) {
	m_currPage = 0;

	scrapbookTags = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookTags1"));
	cegraph::Sprites::SetZOrder(scrapbookTags, 40);
	cegraph::Sprites::SetPosition(scrapbookTags, glm::vec2(16, 98));

	scrapbookTags2 = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookTags2"));
	cegraph::Sprites::SetZOrder(scrapbookTags2, 40);
	cegraph::Sprites::SetPosition(scrapbookTags2, glm::vec2(482, 104));

	hasWonGame  = false;
	justWonGame = wonGame;
}

cg::SBCharacterChapter::~SBCharacterChapter() {
	for(int i = 0; i < characterTales.size(); i++) delete characterTales[i];

	cegraph::Sprites::Delete(scrapbookTags);
	cegraph::Sprites::Delete(scrapbookTags2);
}

void cg::SBCharacterChapter::Update() {}

void cg::SBCharacterChapter::Render() {}

void cg::SBCharacterChapter::NextPage() {
	m_currPage++;

	if(hasWonGame) {
		if(m_currPage > 38) m_currPage = 0;
	} else {
		if(m_currPage > 35) m_currPage = 0;
	}

	UpdatePage();
}

void cg::SBCharacterChapter::PrevPage() {
	m_currPage--;

	if(hasWonGame) {
		if(m_currPage < 0) m_currPage = 38;
	} else {
		if(m_currPage < 0) m_currPage = 35;
	}

	UpdatePage();
}

void cg::SBCharacterChapter::UpdatePage() {
	int currCharacter = m_currPage / 3;
	int characterPage = m_currPage % 3;

	for(int i = 0; i < characterTales.size(); i++) {
		if(currCharacter == i) {
			characterTales[currCharacter]->SetPage((uint16_t)characterPage);
			characterTales[i]->SetDisabled(false);
		} else {
			characterTales[i]->SetDisabled(true);
		}
		characterTales[i]->Update();
	}

	cegraph::Sprites::SetFrame(scrapbookTags, (uint16_t)currCharacter);
	cegraph::Sprites::SetFrame(scrapbookTags2, (uint16_t)characterPage);
}

cg::ScrapbookChapter cg::SBCharacterChapter::GetChapterType() {
	return CharactersChapter;
}

void cg::SBCharacterChapter::SetCharacterLevels(CharacterLevels* characterLevels) {
	m_characterLevels = characterLevels;

	for(int i = 0; i < characterTales.size(); i++) delete characterTales[i];

	characterTales.clear();

	for(int i = 0; i < 12; i++) {
		characterTales.push_back(
		    new CharacterTale((CharacterName)i, m_characterLevels->GetCharacterLevel((CharacterName)i)));
	}

	characterTales.push_back(new CharacterTale(WonGameChapter, 5));

	if(justWonGame)
		m_currPage = 36;
	else
		m_currPage = 0;
	UpdatePage();
}

void cg::SBCharacterChapter::SetHasWonGame(bool _value) {
	hasWonGame = _value;
}