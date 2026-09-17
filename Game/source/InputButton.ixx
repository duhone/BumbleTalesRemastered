/*
 *  Input_Button.h
 *  BoB
 *
 *  Created by Robert Shoemate on 1/19/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 *  Sprite Requirements:
 *	Total Frames: 2
 *	Frame 1: Up Image
 *	Frame 2: Down Image
 */
export module CR.Game.InputButton;

import CR.Game.IInputObject;
import CR.Game.Event;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class InputButton : public IInputObject {
	  public:
		InputButton();
		virtual ~InputButton();

		void SetButtonBounds(float left, float top, float width, float height);
		void SetSpriteAndBounds(float left, float top, uint64_t spriteHash, uint8_t zPos = 50);
		void SetSound(uint64_t hash);

		bool IsDown() const { return isDown; }
		bool WasPressed() {
			// reset wasPressed whenever it is checked for
			if(wasPressed) {
				wasPressed = false;
				return true;
			}

			return false;
		}

		bool IsActing() {
			// reset isActing whenever it is checked for
			if(isActing) {
				isActing = false;
				return true;
			}

			return false;
		}

		void Reset();
		virtual void Render();
		virtual void FreeResources();
		virtual void SetPosition(int x, int y);
		void SetSoundOn(bool isOn) { soundOn = isOn; }
		CR::Utility::Event OnClicked;

		void Update() override;

	  private:
		Rect bounds;
		Rect position;
		bool isActing;
		bool isDown;
		bool wasPressed;
		CR::Engine::Graphics::Handles::Sprite objectSprite;

		bool soundOn;

		void SetSprite(uint64_t spriteHash, uint8_t zPos);

		CR::Engine::Audio::Handles::SoundFX sound;
		CR::Engine::Audio::Handles::SoundFX disabledSound;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceinput = CR::Engine::Input;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::InputButton::InputButton() {
	isDown        = false;
	wasPressed    = false;
	bounds.top    = 0;
	bounds.left   = 0;
	bounds.bottom = 0;
	bounds.right  = 0;
	isActing      = false;
	soundOn       = false;
	position      = bounds;

	// m_soundFX = ISound::Instance().CreateSoundFX(CR::AssetList::sounds::click::ID);
}

cg::InputButton::~InputButton() {
	FreeResources();
}

void cg::InputButton::SetSound(uint64_t hash) {
	m_soundFX = CR::Engine::Audio::SoundFX::GetHandle(hash);
}

void cg::InputButton::FreeResources() {
	if(objectSprite.isValid()) {
		CR::Engine::Graphics::Sprites::Delete(objectSprite);
		objectSprite.invalidate();
	}
}

void cg::InputButton::SetButtonBounds(float left, float top, float width, float height) {
	Rect r;
	r.top    = (int)round(top);
	r.left   = (int)round(left);
	r.bottom = (int)round(height);
	r.right  = (int)round(width);
	bounds   = r;
}

void cg::InputButton::SetSpriteAndBounds(float left, float top, uint64_t spriteHash, uint8_t zPos) {
	SetSprite(spriteHash, zPos);

	auto size = cegraph::Sprites::GetSize(objectSprite);

	Rect r;
	r.top    = (int)round(top);
	r.left   = (int)round(left);
	r.bottom = (int)round(size.y);
	r.right  = (int)round(size.x);
	bounds   = r;
	position = r;
}

void cg::InputButton::SetSprite(uint64_t spriteHash, uint8_t zPos) {
	if(objectSprite.isValid()) { CR::Engine::Graphics::Sprites::Delete(objectSprite); }
	objectSprite = CR::Engine::Graphics::Sprites::Create(spriteHash);
	cegraph::Sprites::SetZOrder(objectSprite, zPos);
}

void cg::InputButton::Update() {
	if(m_disabled) {
		ceinput::Regions::update(m_region, cecore::Rect2D<int32_t>{{0, 0}, {0, 0}});
		return;
	}
	ceinput::Regions::update(
	    m_region, cecore::Rect2D<int32_t>{{bounds.left, bounds.top}, {bounds.right, bounds.bottom}});

	uint32_t state = ceinput::Regions::getState(m_region);

	if((state & ceinput::Regions::RegionStates::Pressed) != 0) {
		if(soundOn) { CR::Engine::Audio::SoundFX::Play(m_soundFX); }
		if(OnClicked.Size() > 0)
			OnClicked();
		else
			wasPressed = true;
	}
	isDown = (state & ceinput::Regions::RegionStates::Down) != 0;
	// Not sure what the purpose of isActing was?
	isActing = isDown;
}

void cg::InputButton::Reset() {
	isActing   = false;
	wasPressed = false;
}

void cg::InputButton::Render() {
	if(objectSprite.isValid()) {
		cegraph::Sprites::SetVisibility(objectSprite, !m_disabled);
		auto size = CR::Engine::Graphics::Sprites::GetSize(objectSprite);
		CR::Engine::Graphics::Sprites::SetPosition(objectSprite, {position.left, position.top});

		if(!isDown) {
			CR::Engine::Graphics::Sprites::SetFrame(objectSprite, 0);
		} else {
			CR::Engine::Graphics::Sprites::SetFrame(objectSprite, 1);
		}
	}
}

void cg::InputButton::SetPosition(int x, int y) {
	position.left = x;
	position.top  = y;
	bounds.left   = x;
	bounds.top    = y;

	ceinput::Regions::update(
	    m_region, cecore::Rect2D<int32_t>{{bounds.left, bounds.top}, {bounds.right, bounds.bottom}});
}