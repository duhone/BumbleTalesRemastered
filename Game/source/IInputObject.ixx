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

	// this is being used as top, bottom, width, height in button class at least. should rename.
	struct Rect {
		int top, bottom, left, right;
	};

	enum class ClickEvent { None, Pressed, Clicked, Cancelled };

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

		virtual void Update() {}
		virtual void Render()        = 0;
		virtual void FreeResources() = 0;

	  protected:
		// Updates the input region to bounds (or clears it when disabled) and returns this frame's click event.
		// A press must start inside the region. Releasing inside the region clicks, dragging out cancels.
		ClickEvent PollClick(const Rect& bounds);
		bool IsHeld() const { return m_held; }
		void ResetClick() { m_held = false; }

		bool m_disabled;
		CR::Engine::Audio::Handles::SoundFX m_soundFX;
		CR::Engine::Input::Handles::Region m_region;

	  private:
		bool m_held{false};
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceinput = CR::Engine::Input;
namespace cg      = CR::Game;

cg::IInputObject::IInputObject() : m_disabled(false) {
	m_region = CR::Engine::Input::Regions::create(cecore::Rect2D<int32_t>{{0, 0}, {0, 0}});
}

cg::IInputObject::~IInputObject() {
	CR::Engine::Input::Regions::release(m_region);
}

cg::ClickEvent cg::IInputObject::PollClick(const Rect& bounds) {
	if(m_disabled) {
		ceinput::Regions::update(m_region, cecore::Rect2D<int32_t>{{0, 0}, {0, 0}});
		m_held = false;
		return ClickEvent::None;
	}
	ceinput::Regions::update(
		m_region, cecore::Rect2D<int32_t>{{bounds.left, bounds.top}, {bounds.right, bounds.bottom}});

	uint32_t state{};
	ceinput::Regions::getStates(m_region, state);

	if(!m_held) {
		if((state & ceinput::Regions::RegionStates::Pressed) != 0) {
			m_held = true;
			return ClickEvent::Pressed;
		}
		return ClickEvent::None;
	}

	if((state & ceinput::Regions::RegionStates::Down) != 0) { return ClickEvent::None; }

	m_held = false;
	if((state & ceinput::Regions::RegionStates::Released) != 0) { return ClickEvent::Clicked; }
	return ClickEvent::Cancelled;
}