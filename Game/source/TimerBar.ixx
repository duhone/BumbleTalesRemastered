/*
 *  TimerBar.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 8/2/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.TimerBar;

import CR.Game.IRenderable;
import CR.Game.Event;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class TimerBar {
	  public:
		TimerBar(int xLoc, int yLoc);
		virtual ~TimerBar();

		// IRenderable
		void Update();
		void Render();
		void SetPosition(float xLoc, float yLoc);
		void PauseAnimation(bool pause);

		void SetMaxTime(float _value);
		void ResetTimer();
		void IncreaseTimer(float time);
		void DecreaseTimer(float time);

		float GetCurrTime() { return currTime; }

		Utility::Event TimerExpired;

	  private:
		glm::vec2 offset;
		CR::Engine::Graphics::Handles::Sprite timeSprite;
		CR::Engine::Graphics::Handles::Sprite barSprite;

		float maxTime;
		float currTime;
	};
}    // namespace CR::Game

module :private;

namespace cegraph = CR::Engine::Graphics;
namespace cecore  = CR::Engine::Core;
namespace cg      = CR::Game;

cg::TimerBar::TimerBar(int xLoc, int yLoc) {
	timeSprite = cegraph::Sprites::Create(cecore::C_Hash64("ArcadeModeTimeTag"));
	cegraph::Sprites::SetZOrders(timeSprite, 80);
	cegraph::Sprites::SetPositions(timeSprite, offset);

	barSprite = cegraph::Sprites::Create(cecore::C_Hash64("ArcadeModeTimeBar"));
	cegraph::Sprites::SetZOrders(barSprite, 80);
	cegraph::Sprites::SetPositions(barSprite, offset);

	maxTime  = 30;
	currTime = maxTime;

	SetPosition((float)xLoc, (float)yLoc);
}

cg::TimerBar::~TimerBar() {
	cegraph::Sprites::Delete(timeSprite);
	cegraph::Sprites::Delete(barSprite);
}

void cg::TimerBar::Update() {
	float frame = (1 - ((float)currTime / (float)maxTime)) * 52.0f;
	cegraph::Sprites::SetFrames(barSprite, (uint16_t)frame);
}

void cg::TimerBar::Render() {}

void cg::TimerBar::SetPosition(float xLoc, float yLoc) {
	offset.x = xLoc;
	offset.y = yLoc;

	cegraph::Sprites::SetPositions(timeSprite, offset);
	cegraph::Sprites::SetPositions(barSprite, offset + glm::vec2(123, 0));
}

void cg::TimerBar::PauseAnimation([[maybe_unused]] bool pause) {}

void cg::TimerBar::SetMaxTime(float _value) {
	maxTime = _value;

	if(currTime > maxTime) currTime = maxTime;
}

void cg::TimerBar::ResetTimer() {
	currTime = maxTime;
}

void cg::TimerBar::IncreaseTimer(float time) {
	currTime += time;
	if(currTime >= maxTime) currTime = maxTime;
}

void cg::TimerBar::DecreaseTimer(float time) {
	currTime -= time;

	if(currTime <= 0) {
		currTime = 0;
		TimerExpired();
	}
}