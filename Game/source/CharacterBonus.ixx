/*
 *  CharacterBonus.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/1/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

export module CR.Game.CharacterBonus;

import CR.Game.Constants;
import CR.Game.CharacterLevels;

import std;
import std.compat;

export namespace CR::Game {
	struct CharBonus {
		std::array<int, 6> resourceBonus{};

		Constants::ResourceType dropRateBonusType{Constants::NoTile};

		CharBonus() = default;

		CharBonus(int _leaf, int _brick, int _wood, int _crystal, int _diamond, int _gold, Constants::ResourceType _dropRateBonusType) :
			resourceBonus{_leaf, _brick, _wood, _crystal, _diamond, _gold},
			dropRateBonusType{_dropRateBonusType} {}

		int GetResourceBonus(Constants::ResourceType rType) const { return resourceBonus[static_cast<size_t>(rType)]; }
	};

	class CharacterBonus {
	  public:
		CharacterBonus();
		virtual ~CharacterBonus();

		CharBonus GetBonus(CharacterName charName, int level) const;
		CharBonus GetBonusDiff(CharacterName charName, int level) const;
		int GetBonusAsset(CharacterName charName, int level) const;
	};
}    // namespace CR::Game

module :private;

namespace cg = CR::Game;

cg::CharacterBonus::CharacterBonus() {}

cg::CharacterBonus::~CharacterBonus() {}

cg::CharBonus cg::CharacterBonus::GetBonus(CharacterName charName, int level) const {
	using namespace Constants;

	if(level == 0) { return CharBonus(); }

	switch(charName) {
		case Mayor:
			if(level == 1) return CharBonus(0, 0, 0, 0, 0, 1, NoTile);
			else if(level == 2) return CharBonus(0, 0, 0, 1, 0, 1, NoTile);
			else if(level == 3) return CharBonus(0, 0, 0, 1, 0, 2, NoTile);
			else if(level == 4) return CharBonus(0, 0, 0, 2, 0, 2, NoTile);
			else if(level == 5) return CharBonus(0, 0, 0, 2, 0, 2, Gold);
			break;
		case Elder:
			if(level == 1) return CharBonus(1, 0, 0, 0, 0, 0, NoTile);
			else if(level == 2) return CharBonus(1, 0, 0, 0, 0, 1, NoTile);
			else if(level == 3) return CharBonus(2, 0, 0, 0, 0, 1, NoTile);
			else if(level == 4) return CharBonus(2, 0, 1, 0, 0, 1, NoTile);
			else if(level == 5) return CharBonus(2, 0, 1, 1, 0, 1, NoTile);
			break;
		case Fireman:
			if(level == 1) return CharBonus(0, 1, 0, 0, 0, 0, NoTile);
			else if(level == 2) return CharBonus(0, 1, 1, 0, 0, 0, NoTile);
			else if(level == 3) return CharBonus(0, 2, 1, 0, 0, 0, NoTile);
			else if(level == 4) return CharBonus(0, 2, 2, 0, 0, 0, NoTile);
			else if(level == 5) return CharBonus(0, 2, 2, 0, 0, 0, Brick);
			break;
		case Dog:
			if(level == 1) return CharBonus(0, 1, 0, 0, 0, 0, NoTile);
			else if(level == 2) return CharBonus(0, 1, 1, 0, 0, 0, NoTile);
			else if(level == 3) return CharBonus(1, 1, 1, 0, 0, 0, NoTile);
			else if(level == 4) return CharBonus(2, 1, 1, 0, 0, 0, NoTile);
			else if(level == 5) return CharBonus(2, 2, 1, 0, 0, 0, NoTile);
			break;
		case Baker:
			if(level == 1) return CharBonus(0, 0, 0, 1, 0, 0, NoTile);
			else if(level == 2) return CharBonus(0, 0, 0, 1, 1, 0, NoTile);
			else if(level == 3) return CharBonus(0, 0, 0, 2, 1, 0, NoTile);
			else if(level == 4) return CharBonus(0, 0, 0, 2, 1, 1, NoTile);
			else if(level == 5) return CharBonus(0, 0, 0, 2, 1, 1, Crystal);
			break;
		case Teacher:
			if(level == 1) return CharBonus(1, 0, 0, 0, 0, 0, NoTile);
			else if(level == 2) return CharBonus(1, 1, 0, 0, 0, 0, NoTile);
			else if(level == 3) return CharBonus(2, 1, 0, 0, 0, 0, NoTile);
			else if(level == 4) return CharBonus(2, 1, 0, 0, 0, 1, NoTile);
			else if(level == 5) return CharBonus(2, 1, 0, 0, 0, 1, Leaf);
			break;
		case Librarian:
			if(level == 1) return CharBonus(0, 0, 1, 0, 0, 0, NoTile);
			else if(level == 2) return CharBonus(1, 0, 1, 0, 0, 0, NoTile);
			else if(level == 3) return CharBonus(1, 1, 1, 0, 0, 0, NoTile);
			else if(level == 4) return CharBonus(2, 1, 1, 0, 0, 0, NoTile);
			else if(level == 5) return CharBonus(2, 1, 2, 0, 0, 0, NoTile);
			break;
		case Romeo:
			if(level == 1) return CharBonus(0, 0, 0, 0, 1, 0, NoTile);
			else if(level == 2) return CharBonus(0, 0, 0, 0, 2, 0, NoTile);
			else if(level == 3) return CharBonus(0, 0, 0, 0, 2, 1, NoTile);
			else if(level == 4) return CharBonus(0, 0, 0, 1, 2, 1, NoTile);
			else if(level == 5) return CharBonus(0, 0, 0, 2, 2, 1, NoTile);
			break;
		case Felicia:
			if(level == 1) return CharBonus(0, 0, 0, 0, 1, 0, NoTile);
			else if(level == 2) return CharBonus(0, 0, 0, 1, 1, 0, NoTile);
			else if(level == 3) return CharBonus(0, 0, 0, 1, 2, 0, NoTile);
			else if(level == 4) return CharBonus(1, 0, 0, 1, 2, 0, NoTile);
			else if(level == 5) return CharBonus(1, 0, 0, 1, 2, 0, Diamond);
			break;
		case Bruiser:
			if(level == 1) return CharBonus(0, 1, 0, 0, 0, 0, NoTile);
			else if(level == 2) return CharBonus(0, 2, 0, 0, 0, 0, NoTile);
			else if(level == 3) return CharBonus(0, 2, 0, 0, 0, 1, NoTile);
			else if(level == 4) return CharBonus(1, 2, 0, 0, 0, 1, NoTile);
			else if(level == 5) return CharBonus(1, 2, 0, 1, 0, 1, NoTile);
			break;
		case Marla:
			if(level == 1) return CharBonus(1, 0, 0, 0, 0, 0, NoTile);
			else if(level == 2) return CharBonus(1, 0, 0, 1, 0, 0, NoTile);
			else if(level == 3) return CharBonus(1, 1, 0, 1, 0, 0, NoTile);
			else if(level == 4) return CharBonus(1, 1, 1, 1, 0, 0, NoTile);
			else if(level == 5) return CharBonus(1, 2, 1, 1, 0, 0, NoTile);
			break;
		case ChopChop:
			if(level == 1) return CharBonus(0, 0, 1, 0, 0, 0, NoTile);
			else if(level == 2) return CharBonus(0, 0, 1, 0, 0, 1, NoTile);
			else if(level == 3) return CharBonus(0, 0, 2, 0, 0, 1, NoTile);
			else if(level == 4) return CharBonus(0, 1, 2, 0, 0, 1, NoTile);
			else if(level == 5) return CharBonus(0, 1, 2, 0, 0, 1, Wood);
			break;
		default: return CharBonus();
	}

	return CharBonus();
}

cg::CharBonus cg::CharacterBonus::GetBonusDiff(CharacterName charName, int level) const {
	CharBonus cBonus1 = GetBonus(charName, level + 1);
	const CharBonus cBonus2 = GetBonus(charName, level);

	for(size_t i = 0; i < cBonus1.resourceBonus.size(); ++i) {
		cBonus1.resourceBonus[i] -= cBonus2.resourceBonus[i];
	}

	return cBonus1;
}

int cg::CharacterBonus::GetBonusAsset(CharacterName charName, int level) const {
	using namespace Constants;

	const CharBonus cBonus = GetBonusDiff(charName, level);

	if(level >= 5) { return 12; }

	if(cBonus.resourceBonus[0] > 0) { return 2; }
	if(cBonus.resourceBonus[1] > 0) { return 0; }
	if(cBonus.resourceBonus[2] > 0) { return 1; }
	if(cBonus.resourceBonus[3] > 0) { return 3; }
	if(cBonus.resourceBonus[4] > 0) { return 5; }
	if(cBonus.resourceBonus[5] > 0) { return 4; }

	switch(cBonus.dropRateBonusType) {
		case Leaf: return 8;
		case Brick: return 6;
		case Wood: return 7;
		case Crystal: return 9;
		case Diamond: return 11;
		case Gold: return 10;
		default: break;
	}

	return -1;
}
