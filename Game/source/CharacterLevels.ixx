/*
 *  CharacterLevels.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/30/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

export module CR.Game.CharacterLevels;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	enum CharacterName {
		Mayor              = 0,
		Elder              = 1,
		Fireman            = 2,
		Dog                = 3,
		Baker              = 4,
		Teacher            = 5,
		Librarian          = 6,
		Romeo              = 7,
		Felicia            = 8,
		Bruiser            = 9,
		Marla              = 10,
		ChopChop           = 11,
		NoCharacter        = 12,
		WonGameChapter     = 13,
		NumCharacterLevels = 14
	};

	class CharacterLevels {
	  public:
		CharacterLevels();
		virtual ~CharacterLevels();

		int GetCharacterLevel(CharacterName charName) const {
			CR_ASSERT(charName < NumCharacterLevels, "Invalid character name");
			return m_levels[charName];
		}
		void SetCharacterLevel(CharacterName charName, int _value) {
			CR_ASSERT(charName < NumCharacterLevels, "Invalid character name");
			m_levels[charName] = _value;
		}

		void Save(FILE* _file);
		void Load(FILE* _file);
		void Reset();

	  private:
		int m_levels[NumCharacterLevels];
	};
}    // namespace CR::Game

inline std::istream& operator>>(std::istream& strm, CR::Game::CharacterName& _cname) {
	std::string temp;
	strm >> temp;
	if(temp == "Mayor") _cname = CR::Game::Mayor;
	if(temp == "Elder") _cname = CR::Game::Elder;
	if(temp == "Fireman") _cname = CR::Game::Fireman;
	if(temp == "Dog") _cname = CR::Game::Dog;
	if(temp == "Baker") _cname = CR::Game::Baker;
	if(temp == "Teacher") _cname = CR::Game::Teacher;
	if(temp == "Librarian") _cname = CR::Game::Librarian;
	if(temp == "Romeo") _cname = CR::Game::Romeo;
	if(temp == "Felicia") _cname = CR::Game::Felicia;
	if(temp == "Bruiser") _cname = CR::Game::Bruiser;
	if(temp == "Marla") _cname = CR::Game::Marla;
	if(temp == "ChopChop") _cname = CR::Game::ChopChop;
	if(temp == "NoCharacter") _cname = CR::Game::NoCharacter;
	if(temp == "WonGameChapter") _cname = CR::Game::WonGameChapter;

	return strm;
}

module :private;

namespace cg = CR::Game;

cg::CharacterLevels::CharacterLevels() {
	Reset();
}

cg::CharacterLevels::~CharacterLevels() {}

void cg::CharacterLevels::Save(FILE* _file) {
	fwrite(m_levels, 1, sizeof(m_levels), _file);
}

void cg::CharacterLevels::Load(FILE* _file) {
	fread(m_levels, 1, sizeof(m_levels), _file);
}

void cg::CharacterLevels::Reset() {
	memset(m_levels, 0, sizeof(m_levels));
}
