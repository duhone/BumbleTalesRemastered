/*
 *  SeparatorItem.mm
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/9/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

export module CR.Game.SeparatorItem;

import CR.Engine;
import CR.Game.ShopItem;

import std;
import std.compat;

export namespace CR::Game {
	enum class HeaderType : uint8_t { CharacterHeader, BuildingHeader };

	class SeparatorItem : public ShopItem {
	  public:
		 explicit SeparatorItem(HeaderType headerType);
		 ~SeparatorItem() override = default;

		 HeaderType GetHeaderType() const;

	  private:
		 HeaderType m_headerType;
	};
}    // namespace CR::Game

module :private;

namespace cecore = CR::Engine::Core;
namespace cg     = CR::Game;

cg::SeparatorItem::SeparatorItem(cg::HeaderType headerType) : m_headerType(headerType) {
	switch(m_headerType) {
		case HeaderType::CharacterHeader:
			SetItemSprite(cecore::C_Hash64("ShopHeader_Characters"));
			break;
		case HeaderType::BuildingHeader:
			SetItemSprite(cecore::C_Hash64("ShopHeader_Buildings"));
			break;
	}

	m_height    = 90;
	disableInput = true;
}

cg::HeaderType cg::SeparatorItem::GetHeaderType() const {
	return m_headerType;
}
