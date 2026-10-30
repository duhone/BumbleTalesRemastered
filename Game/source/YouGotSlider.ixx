/*
 *  YouGotSlider.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 8/9/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.YouGotSlider;

import CR.Game.IRenderable;
import CR.Game.Timer;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class YouGotSlider {
	  public:
		YouGotSlider();
		virtual ~YouGotSlider();

		void Update();
		void Render();

		void SetPosition(float xLoc, float yLoc);
		void PauseAnimation(bool pause);

		void SetTagDisplay(bool gotTrophy, bool gotStar);
		void SetSlideToPoint(int xLoc, int yLoc);

	  private:
		glm::vec2 offset;
		glm::vec2 slideToPoint;

		CR::Engine::Graphics::Handles::Sprite youGotTagsSprite;

		bool m_gotTrophy;
		bool m_gotStar;

		float maxDisplayDelay;
		float currDisplayDelay;
		int currTagDisplay;

		int xStep;
		bool sliding;
	};
}    // namespace CR::Game

module :private;

namespace cegraph = CR::Engine::Graphics;
namespace cecore  = CR::Engine::Core;
namespace cg      = CR::Game;

cg::YouGotSlider::YouGotSlider() {
	youGotTagsSprite = cegraph::Sprites::Create(cecore::C_Hash64("ScoreYouGotTags"));
	cegraph::Sprites::SetZOrders(youGotTagsSprite, 40);

	m_gotTrophy = false;
	m_gotStar   = false;

	SetPosition(0, 0);

	maxDisplayDelay  = 1.5;
	currDisplayDelay = maxDisplayDelay;

	currTagDisplay = 0;
	xStep          = 0;
	sliding        = false;
}

cg::YouGotSlider::~YouGotSlider() {
	cegraph::Sprites::Delete(youGotTagsSprite);
}

void cg::YouGotSlider::Update() {
	if(m_gotTrophy && m_gotStar) {
		currDisplayDelay -= Timer::GetTimePassed();

		if(currDisplayDelay <= 0) {
			currDisplayDelay = maxDisplayDelay;
			if(currTagDisplay == 1)
				currTagDisplay = 0;
			else
				currTagDisplay = 1;
		}
	}

	cegraph::Sprites::SetFrames(youGotTagsSprite, (uint16_t)currTagDisplay);

	if(sliding) {
		SetPosition(offset.x + xStep, offset.y);
		if(offset.x >= slideToPoint.x) {
			SetPosition(slideToPoint.x, offset.y);
			sliding = false;
		}
	}
}

void cg::YouGotSlider::Render() {
	cegraph::Sprites::SetVisibilities(youGotTagsSprite, m_gotTrophy || m_gotStar);
}

void cg::YouGotSlider::SetPosition(float xLoc, float yLoc) {
	offset.x = xLoc;
	offset.y = yLoc;

	cegraph::Sprites::SetPositions(youGotTagsSprite, glm::vec2(offset.x, offset.y));
}

void cg::YouGotSlider::PauseAnimation([[maybe_unused]] bool pause) {}

void cg::YouGotSlider::SetTagDisplay(bool gotTrophy, bool gotStar) {
	m_gotTrophy = gotTrophy;
	m_gotStar   = gotStar;

	if(m_gotTrophy && !m_gotStar) {
		currTagDisplay = 1;
	} else if(!m_gotTrophy && m_gotStar) {
		currTagDisplay = 0;
	}
}

void cg::YouGotSlider::SetSlideToPoint(int xLoc, int yLoc) {
	slideToPoint.x = static_cast<float>(xLoc);
	slideToPoint.y = static_cast<float>(yLoc);

	sliding = true;
	xStep   = int((slideToPoint.x - offset.x) / 40);
}