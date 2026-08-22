/*
 *  ISaveOnTerminate.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/26/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game.ISaveOnTerminate;

import std;
import std.compat;

export namespace CR::Game {
	// This interface is to be used on GameStates. The current "SaveOnTerminate" GameState must be registered
	// with the SaveGameManager. If the app terminates, this gamestate will be notified and can then notify
	// its child views.

	class ISaveOnTerminate {
	  public:
		virtual ~ISaveOnTerminate() {};

		virtual void OnSaveOnTerminate() = 0;

	  protected:
		ISaveOnTerminate() {};
	};
}    // namespace CR::Game

module :private;
