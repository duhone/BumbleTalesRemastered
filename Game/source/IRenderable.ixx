/*
 *  IRenderable.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/16/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

export module CR.Game.IRenderable;

import std;
import std.compat;

export namespace CR::Game {
	class IRenderable {
	  public:
		virtual ~IRenderable() = default;

		virtual void Update() = 0;
		virtual void Render() = 0;

		virtual void SetPosition(float xLoc, float yLoc) = 0;
		virtual void PauseAnimation(bool pause)          = 0;

	  protected:
		IRenderable() {};
	};
}    // namespace CR::Game

module :private;
