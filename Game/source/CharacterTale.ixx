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
	lockedSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookLockedTaleImage"));

	m_currPage = 0;

	pageLocked[0] = level < 1;
	pageLocked[1] = level < 3;
	pageLocked[2] = level < 5;

	switch(characterName) {
		case Mayor:
			pageSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale01"));
			break;
		case Elder:
			pageSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale02"));
			break;
		case Fireman:
			pageSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale03"));
			break;
		case Dog:
			pageSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale04"));
			break;
		case Baker:
			pageSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale05"));
			break;
		case Teacher:
			pageSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale06"));
			break;
		case Librarian:
			pageSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale07"));
			break;
		case Romeo:
			pageSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale08"));
			break;
		case Felicia:
			pageSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale09"));
			break;
		case Bruiser:
			pageSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale10"));
			break;
		case Marla:
			pageSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale11"));
			break;
		case ChopChop:
			pageSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale12"));
			break;
		case WonGameChapter:
			pageSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookCharTale13"));
			break;
		default:
			CR_ASSERT(false, "Unknown character name in CharacterTale constructor");
			break;
	}

	SetPage(0);

	cegraph::Sprites::SetPosition(pageSprite, glm::vec2(18, 166));
	cegraph::Sprites::SetPosition(lockedSprite, glm::vec2(78, 396));
	cegraph::Sprites::SetZOrder(pageSprite, 40);
	cegraph::Sprites::SetZOrder(lockedSprite, 40);
}

cg::CharacterTale::~CharacterTale() {
	cegraph::Sprites::Delete(pageSprite);
	cegraph::Sprites::Delete(lockedSprite);
}

void cg::CharacterTale::SetPage(uint16_t page) {
	m_currPage = page;
	cegraph::Sprites::SetFrame(pageSprite, page);
}

void cg::CharacterTale::Update() {
	if(m_disabled) {
		cegraph::Sprites::SetVisibility(lockedSprite, false);
		cegraph::Sprites::SetVisibility(pageSprite, false);
		return;
	}

	if(pageLocked[m_currPage]) {
		cegraph::Sprites::SetVisibility(lockedSprite, true);
		cegraph::Sprites::SetVisibility(pageSprite, false);
	} else {
		cegraph::Sprites::SetVisibility(lockedSprite, false);
		cegraph::Sprites::SetVisibility(pageSprite, true);
	}
}

bool cg::CharacterTale::IsPageUnlocked(int page) {
	return !pageLocked[page];
}