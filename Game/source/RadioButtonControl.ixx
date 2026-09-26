/*
 *  RadioButtonControl.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/30/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game.RadioButtonControl;

import CR.Game.IInputObject;
import CR.Game.Event;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class RadioButtonControl : public IInputObject {
	  public:
		RadioButtonControl(uint64_t imgHash);
		virtual ~RadioButtonControl();

		// Input_Object
		void Update() override;
		void Render();

		void Reset() {};
		void FreeResources() {};

		void SetPositionAndBounds(int x, int y, int width, int height);

		void SetValue(int _value) { m_value = _value; }
		int GetValue() const { return m_value; }

		void Select();
		void Deselect() { m_isSelected = false; }

		CR::Utility::Event1<int> OnRadioButtonSelected;

	  private:
		Rect bounds;
		CR::Engine::Graphics::Handles::Sprite radioSprite;
		bool m_isSelected;
		std::vector<RadioButtonControl*> friendRadioButtons;
		int m_value;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceinput = CR::Engine::Input;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::RadioButtonControl::RadioButtonControl(uint64_t imgHash) {
	radioSprite = CR::Engine::Graphics::Sprites::Create(imgHash);
	cegraph::Sprites::SetZOrder(radioSprite, 40);

	m_isSelected = false;
}

cg::RadioButtonControl::~RadioButtonControl() {
	CR::Engine::Graphics::Sprites::Delete(radioSprite);
}

void cg::RadioButtonControl::Update() {
	if(m_disabled) {
		ceinput::Regions::update(m_region, cecore::Rect2D<int32_t>{{0, 0}, {0, 0}});
		return;
	}
	ceinput::Regions::update(
	    m_region, cecore::Rect2D<int32_t>{{bounds.left, bounds.top}, {bounds.right, bounds.bottom}});

	uint32_t state = ceinput::Regions::getState(m_region);

	if((state & ceinput::Regions::RegionStates::Pressed) != 0) {
		m_isSelected = true;
		OnRadioButtonSelected(m_value);
	}
}

void cg::RadioButtonControl::Render() {
	if(radioSprite.isValid()) {
		cegraph::Sprites::SetVisibility(radioSprite, !m_disabled);
		CR::Engine::Graphics::Sprites::SetPosition(radioSprite, {bounds.left, bounds.top});

		if(!m_isSelected) {
			CR::Engine::Graphics::Sprites::SetFrame(radioSprite, 0);
		} else {
			CR::Engine::Graphics::Sprites::SetFrame(radioSprite, 1);
		}
	}
}

void cg::RadioButtonControl::SetPositionAndBounds(int x, int y, int width, int height) {
	bounds.left   = x;
	bounds.top    = y;
	bounds.right  = width;
	bounds.bottom = height;
	CR::Engine::Graphics::Sprites::SetPosition(radioSprite, {bounds.left, bounds.top});
}

void cg::RadioButtonControl::Select() {
	if(!m_disabled) {
		m_isSelected = true;
		OnRadioButtonSelected(m_value);
	}
}
