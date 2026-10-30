/*
 *  ResourceBar.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/16/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.ResourceBar;

import CR.Game.IRenderable;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	enum ResourceBarSize { NormalResourceBar, TinyResourceBar };

	class ResourceBar : public IRenderable {
	  public:
		ResourceBar(int xLoc, int yLoc, ResourceBarSize rSize = NormalResourceBar);
		virtual ~ResourceBar();

		// IRenderable
		void Update();
		void Render();
		void SetPosition(float xLoc, float yLoc);
		void PauseAnimation(bool pause);

		void SetMaxValue(int _value);
		void AddToCurrValue(int _value);
		void ResetValue();

		bool IsFull();

	  private:
		glm::vec2 offset;
		CR::Engine::Graphics::Handles::Sprite resourceBarSprite;

		int maxValue;
		int currValue;
		bool isFullAnimation;

		void SetResourceBarSprite();

		uint64_t assetResourceBarHash;
		uint64_t assetResourceBarFullHash;
		int barFrames;
	};
}    // namespace CR::Game

module :private;

namespace cegraph = CR::Engine::Graphics;
namespace cecore  = CR::Engine::Core;
namespace cg      = CR::Game;

cg::ResourceBar::ResourceBar(int xLoc, int yLoc, ResourceBarSize rSize) {
	offset.x = (float)xLoc;
	offset.y = (float)yLoc;

	isFullAnimation = false;
	maxValue        = 1;
	currValue       = 0;

	if(rSize == NormalResourceBar) {
		assetResourceBarHash     = cecore::C_Hash64("StoryModeResourceBar1");
		assetResourceBarFullHash = cecore::C_Hash64("StoryModeResourceBar1");
		barFrames                = 51;
	} else if(rSize == TinyResourceBar) {
		assetResourceBarHash     = cecore::C_Hash64("ArcadeModeResourceBar3");
		assetResourceBarFullHash = cecore::C_Hash64("ArcadeModeResourceBar4");
		barFrames                = 21;
	}

	resourceBarSprite = cegraph::Sprites::Create(assetResourceBarHash);
	cegraph::Sprites::SetZOrders(resourceBarSprite, 80);
	cegraph::Sprites::SetPositions(resourceBarSprite, offset);
}

cg::ResourceBar::~ResourceBar() {
	if(resourceBarSprite.isValid()) { cegraph::Sprites::Delete(resourceBarSprite); }
}

void cg::ResourceBar::Update() {
	if(!isFullAnimation) {
		int frameSet = (int)(((float)currValue / (float)maxValue) * (float)barFrames);
		cegraph::Sprites::SetFrames(resourceBarSprite, (uint16_t)frameSet);
	}
}

void cg::ResourceBar::Render() {}

void cg::ResourceBar::SetPosition(float xLoc, float yLoc) {
	offset.x = xLoc;
	offset.y = yLoc;
	cegraph::Sprites::SetPositions(resourceBarSprite, offset);
}

void cg::ResourceBar::PauseAnimation(bool pause) {
	cegraph::Sprites::SetAnimationPauses(resourceBarSprite, pause);
}

void cg::ResourceBar::SetMaxValue(int _value) {
	maxValue = _value;
	if(currValue > maxValue) currValue = maxValue;

	SetResourceBarSprite();
}

void cg::ResourceBar::AddToCurrValue(int _value) {
	currValue += _value;
	if(currValue > maxValue) currValue = maxValue;

	SetResourceBarSprite();
}

void cg::ResourceBar::ResetValue() {
	currValue       = 0;
	isFullAnimation = false;

	if(resourceBarSprite.isValid()) { cegraph::Sprites::Delete(resourceBarSprite); }

	resourceBarSprite = cegraph::Sprites::Create(assetResourceBarHash);
	cegraph::Sprites::SetZOrders(resourceBarSprite, 80);
	cegraph::Sprites::SetPositions(resourceBarSprite, offset);
}

void cg::ResourceBar::SetResourceBarSprite() {
	if(currValue >= maxValue) {
		if(!isFullAnimation) {
			if(resourceBarSprite.isValid()) { cegraph::Sprites::Delete(resourceBarSprite); }

			resourceBarSprite = cegraph::Sprites::Create(assetResourceBarFullHash);
			cegraph::Sprites::SetZOrders(resourceBarSprite, 80);
			cegraph::Sprites::SetPositions(resourceBarSprite, offset);
		}

		isFullAnimation = true;
	} else {
		if(isFullAnimation) {
			if(resourceBarSprite.isValid()) { cegraph::Sprites::Delete(resourceBarSprite); }

			resourceBarSprite = cegraph::Sprites::Create(assetResourceBarHash);
			cegraph::Sprites::SetZOrders(resourceBarSprite, 80);
			cegraph::Sprites::SetPositions(resourceBarSprite, offset);
		}

		isFullAnimation = false;
	}
}

bool cg::ResourceBar::IsFull() {
	return isFullAnimation;
}