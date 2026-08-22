/*
 *  ITouchable.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/9/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game.ITouchable;

import std;
import std.compat;

export namespace CR::Game {
	class ITouchable {
	  public:
		virtual ~ITouchable() {}

		/*void TouchesBegan(UIView *view, NSSet *touches);
		void TouchesMoved(UIView *view, NSSet *touches);
		void TouchesEnded(UIView *view, NSSet *touches);
		void TouchesCancelled(UIView *view, NSSet *touches);
		*/
	  protected:
		ITouchable() {}
	};
}    // namespace CR::Game

module :private;
