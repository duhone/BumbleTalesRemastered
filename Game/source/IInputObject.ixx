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

	class IInputObject {
	  public:
		IInputObject();
		virtual ~IInputObject();

		bool Disabled() const { return m_disabled; }
		void Disabled(bool _disabled) {
			if(!_disabled && m_disabled) Reset();
			m_disabled = _disabled;
		}

		virtual void Reset() = 0;

		virtual void Update([[maybe_unused]] float time) {}
		virtual void Render()        = 0;
		virtual void FreeResources() = 0;

	  protected:
		bool m_disabled;
		CR::Engine::Audio::Handles::SoundFX m_soundFX;
		CR::Engine::Input::Handles::Region m_region;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceinput = CR::Engine::Input;
namespace cg      = CR::Game;

cg::IInputObject::IInputObject() : m_disabled(false) {
	CR::Engine::Input::Regions::create(cecore::Rect2D<int32_t>{{0, 0}, {0, 0}});
}

cg::IInputObject::~IInputObject() {
	CR::Engine::Input::Regions::release(m_region);
}