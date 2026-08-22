/*
 *  IView.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/25/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */

export module CR.Game.IView;

import std;
import std.compat;

export namespace CR::Game {
	// originally inherited from Input_Controller from HPT engine, that doesn't exist anymore.
	class IView {
	  public:
		virtual ~IView() {};

		virtual void Update() {};
		virtual void Render() {};
		virtual void PauseAnimation([[maybe_unused]] bool pause) {};
		virtual bool IsPaused() const { return false; };
		virtual void InputChanged() {};
		// virtual void SaveOnTerminate() {}

	  protected:
		IView() {};
	};
}    // namespace CR::Game

module :private;
