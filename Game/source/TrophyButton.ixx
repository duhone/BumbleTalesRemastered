/*
 *  TrophyButton.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 8/22/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.TrophyButton;

import CR.Game.Event;
import CR.Game.IInputObject;
import CR.Game.TrophySaveInfo;
import CR.Game.Trophies;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class TrophyButton : public IInputObject {
	  public:
		TrophyButton(TrophyName trophyName, int zPos);
		virtual ~TrophyButton();

		void Reset() {};
		void Update() {}
		void Render();
		void FreeResources() {};

		void SetPosition(int xLoc, int yLoc);
		void SetHasTrophy(bool hasTrophy);

		CR::Utility::Event1<TrophyName> OnClicked;

	  private:
		CR::Engine::Graphics::Handles::Sprite trophySprite;
		glm::vec2 offset;
		bool hasTrophy;
		TrophyName trophyName;
		int m_touchID;
		Rect bounds;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceinput = CR::Engine::Input;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::TrophyButton::TrophyButton(TrophyName trophyName, int zPos) : m_touchID(-1) {
	trophySprite = cegraph::Sprites::Create(cecore::C_Hash64("ScrapbookTrophyIcon"));
	cegraph::Sprites::SetZOrders(trophySprite, (uint8_t)zPos);
	cegraph::Sprites::SetPositions(trophySprite, glm::vec2(0, 144));
	cegraph::Sprites::SetFrames(trophySprite, (uint16_t)trophyName);

	SetPosition(0, 0);
	hasTrophy        = false;
	this->trophyName = trophyName;
	//	touch = NULL;
}

cg::TrophyButton::~TrophyButton() {
	cegraph::Sprites::Delete(trophySprite);
}

void cg::TrophyButton::Render() {
	if(!m_disabled) {
		ceinput::Regions::update(
		    m_region, cecore::Rect2D<int32_t>{{bounds.left, bounds.top}, {bounds.right, bounds.bottom}});
	} else {
		ceinput::Regions::update(m_region, cecore::Rect2D<int32_t>{{0, 0}, {0, 0}});
	}
	cegraph::Sprites::SetVisibilities(trophySprite, hasTrophy);

	uint32_t state{};
	ceinput::Regions::getStates(m_region, state);

	if((state & ceinput::Regions::RegionStates::Down) != 0) {
		if(OnClicked.Size() > 0) OnClicked(trophyName);
	}
}

void cg::TrophyButton::SetPosition(int xLoc, int yLoc) {
	offset.x = (float)xLoc;
	offset.y = (float)yLoc;

	cegraph::Sprites::SetPositions(trophySprite, glm::vec2(xLoc, yLoc));

	auto buttonSize = cegraph::Sprites::GetSize(trophySprite);

	bounds.top    = yLoc;
	bounds.bottom = buttonSize.y;
	bounds.left   = xLoc;
	bounds.right  = buttonSize.x;
}

void cg::TrophyButton::SetHasTrophy(bool a_hasTrophy) {
	hasTrophy = a_hasTrophy;
}
