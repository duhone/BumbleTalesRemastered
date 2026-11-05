/*
 *  CharacterTale.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/5/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.CharacterTale;

import CR.Game.CharacterLevels;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class CharacterTale {
	  public:
		CharacterTale(CharacterName characterName, int level);
		virtual ~CharacterTale();

		void SetPage(uint16_t page);
		bool IsPageUnlocked(int page);
		void SetDisabled(bool disabled) { m_disabled = disabled; }

		void Update();

	  private:
		int m_currPage;
		CR::Engine::Graphics::Handles::Sprite pageSprite;
		CR::Engine::Graphics::Handles::Sprite lockedSprite;
		bool pageLocked[3];
		bool m_disabled{true};
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::CharacterTale::CharacterTale(CharacterName characterName, int level) {
	cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookLockedTaleImage"), lockedSprite);

	m_currPage = 0;

	pageLocked[0] = level < 1;
	pageLocked[1] = level < 3;
	pageLocked[2] = level < 5;

	switch(characterName) {
		case Mayor:
			cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale01"), pageSprite);
			break;
		case Elder:
			cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale02"), pageSprite);
			break;
		case Fireman:
			cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale03"), pageSprite);
			break;
		case Dog:
			cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale04"), pageSprite);
			break;
		case Baker:
			cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale05"), pageSprite);
			break;
		case Teacher:
			cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale06"), pageSprite);
			break;
		case Librarian:
			cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale07"), pageSprite);
			break;
		case Romeo:
			cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale08"), pageSprite);
			break;
		case Felicia:
			cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale09"), pageSprite);
			break;
		case Bruiser:
			cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale10"), pageSprite);
			break;
		case Marla:
			cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale11"), pageSprite);
			break;
		case ChopChop:
			cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale12"), pageSprite);
			break;
		case WonGameChapter:
			cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale13"), pageSprite);
			break;
		default:
			CR_ASSERT(false, "Unknown character name in CharacterTale constructor");
			break;
	}

	SetPage(0);

	cegraph::Sprites::SetPositions(pageSprite, glm::vec2(18, 166));
	cegraph::Sprites::SetPositions(lockedSprite, glm::vec2(78, 396));
	cegraph::Sprites::SetZOrders(pageSprite, 40);
	cegraph::Sprites::SetZOrders(lockedSprite, 40);
}

cg::CharacterTale::~CharacterTale() {
	cegraph::Sprites::Delete(pageSprite);
	cegraph::Sprites::Delete(lockedSprite);
}

void cg::CharacterTale::SetPage(uint16_t page) {
	m_currPage = page;
	cegraph::Sprites::SetFrames(pageSprite, page);
}

void cg::CharacterTale::Update() {
	if(m_disabled) {
		cegraph::Sprites::SetVisibilities(lockedSprite, false);
		cegraph::Sprites::SetVisibilities(pageSprite, false);
		return;
	}

	if(pageLocked[m_currPage]) {
		cegraph::Sprites::SetVisibilities(lockedSprite, true);
		cegraph::Sprites::SetVisibilities(pageSprite, false);
	} else {
		cegraph::Sprites::SetVisibilities(lockedSprite, false);
		cegraph::Sprites::SetVisibilities(pageSprite, true);
	}
}

bool cg::CharacterTale::IsPageUnlocked(int page) {
	return !pageLocked[page];
}