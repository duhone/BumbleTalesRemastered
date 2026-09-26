/*
 *  CheckboxControl.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/27/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game.CheckboxControl;

import CR.Game.IInputObject;
import CR.Game.Event;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class CheckboxControl : public IInputObject {
	  public:
		CheckboxControl();
		virtual ~CheckboxControl();

		// Input_Object
		void Update() override;
		void Render();
		void Reset();
		void FreeResources();

		void SetSpriteAndPosition(uint64_t spriteHash, int xLoc, int yLoc);
		void SetButtonBounds(float left, float top, float width, float height);
		void SetSpriteAndBounds(float left, float top, uint64_t spriteHash);

		bool IsChecked() const { return m_isChecked; }
		bool SetChecked(bool _value) {
			m_isChecked = _value;
			return m_isChecked;
		}

		CR::Utility::Event OnCheckChanged;

	  private:
		void SetSprite(uint64_t spriteHash, uint8_t zPos);

		Rect bounds;
		bool m_isChecked;
		CR::Engine::Graphics::Handles::Sprite checkSprite;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceinput = CR::Engine::Input;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::CheckboxControl::CheckboxControl() {
	m_isChecked = true;
}

cg::CheckboxControl::~CheckboxControl() {
	if(checkSprite.isValid()) {
		CR::Engine::Graphics::Sprites::Delete(checkSprite);
		checkSprite.invalidate();
	}
}

void cg::CheckboxControl::SetSpriteAndPosition(uint64_t spriteHash, int xLoc, int yLoc) {
	SetSprite(spriteHash, 40);

	CR::Engine::Graphics::Sprites::SetPosition(checkSprite, {xLoc, yLoc});
}

void cg::CheckboxControl::SetButtonBounds(float left, float top, float width, float height) {
	Rect r;
	r.top    = (int)top;
	r.left   = (int)left;
	r.bottom = (int)height;
	r.right  = (int)width;
	bounds   = r;
}

void cg::CheckboxControl::SetSpriteAndBounds(float left, float top, uint64_t spriteHash) {
	SetSprite(spriteHash, 40);

	auto size = cegraph::Sprites::GetSize(checkSprite);

	Rect r;
	r.top    = (int)top;
	r.left   = (int)left;
	r.bottom = size.y;
	r.right  = size.x;
	bounds   = r;

	CR::Engine::Graphics::Sprites::SetPosition(checkSprite, {bounds.left, bounds.top});
}

void cg::CheckboxControl::Update() {
	if(m_disabled) {
		ceinput::Regions::update(m_region, cecore::Rect2D<int32_t>{{0, 0}, {0, 0}});
		return;
	}
	ceinput::Regions::update(
	    m_region, cecore::Rect2D<int32_t>{{bounds.left, bounds.top}, {bounds.right, bounds.bottom}});

	uint32_t state = ceinput::Regions::getState(m_region);

	if((state & ceinput::Regions::RegionStates::Pressed) != 0) {
		m_isChecked = !m_isChecked;
		OnCheckChanged();
	}
}

void cg::CheckboxControl::Render() {
	if(checkSprite.isValid()) {
		cegraph::Sprites::SetVisibility(checkSprite, !m_disabled);
		auto size = CR::Engine::Graphics::Sprites::GetSize(checkSprite);
		CR::Engine::Graphics::Sprites::SetPosition(checkSprite, {bounds.left, bounds.top});

		if(!m_isChecked) {
			CR::Engine::Graphics::Sprites::SetFrame(checkSprite, 0);
		} else {
			CR::Engine::Graphics::Sprites::SetFrame(checkSprite, 1);
		}
	}
}

void cg::CheckboxControl::Reset() {}

void cg::CheckboxControl::FreeResources() {}

void cg::CheckboxControl::SetSprite(uint64_t spriteHash, uint8_t zPos) {
	if(checkSprite.isValid()) { CR::Engine::Graphics::Sprites::Delete(checkSprite); }
	checkSprite = CR::Engine::Graphics::Sprites::Create(spriteHash);
	cegraph::Sprites::SetZOrder(checkSprite, zPos);
}
