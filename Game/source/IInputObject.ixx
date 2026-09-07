/*
 *  Input_Object.h
 *  Input
 *
 *  Created by Robert Shoemate on 1/19/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
export module CR.Game.IInputObject;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {

	struct Rect {
		int top, bottom, left, right;
	};

	struct Touch {
		Touch() : X(0), Y(0), Active(false), ID(-1) {}
		Touch(int _id) : X(0), Y(0), Active(false), ID(_id) {}
		float X;        // position x
		float Y;        // position y
		bool Active;    // is touch active (currently down)
		int ID;         // touch's unique identifier
	};

	class IInputObject {
	  public:
		IInputObject() : m_disabled(false) {}

		virtual ~IInputObject() {}

		void TouchesBegan(Touch& _touches) {
			if(!m_disabled) TouchesBeganImpl(_touches);
		}
		void TouchesMoved(Touch& _touches) {
			if(!m_disabled) TouchesMovedImpl(_touches);
		}
		void TouchesEnded(Touch& _touches) {
			if(!m_disabled) TouchesEndedImpl(_touches);
		}
		bool Disabled() const { return m_disabled; }
		void Disabled(bool _disabled) {
			if(!_disabled && m_disabled) Reset();
			m_disabled = _disabled;
		}

		virtual void TouchesBeganImpl(Touch& _touches) = 0;
		virtual void TouchesMovedImpl(Touch& _touches) = 0;
		virtual void TouchesEndedImpl(Touch& _touches) = 0;
		virtual void Reset()                           = 0;

		virtual void Update([[maybe_unused]] float time) {}
		virtual void Render()        = 0;
		virtual void FreeResources() = 0;

	  protected:
		bool m_disabled;
		CR::Engine::Audio::Handles::SoundFX m_soundFX;
	};
}    // namespace CR::Game