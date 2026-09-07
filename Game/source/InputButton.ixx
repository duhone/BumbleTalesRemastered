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
		void SetSpriteAndBounds(float left, float top, uint64_t spriteHash, int zPos = 0);
		void SetSound(uint64_t hash);

		void TouchesBeganImpl(Touch& _touches);
		void TouchesMovedImpl(Touch& _touches);
		void TouchesEndedImpl(Touch& _touches);

		bool IsDown() const { return isDown; }
		// void Temp(int _value) {temp = _value;}
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

	  private:
		Rect bounds;
		Rect position;
		bool isDown;
		bool isActing;

		bool wasPressed;
		int touchID;
		CR::Engine::Graphics::Handles::Sprite objectSprite;

		bool soundOn;

		void SetSprite(uint64_t spriteHash, int zPos);

		// CR::Utility::Guid soundId;
		CR::Engine::Audio::Handles::SoundFX sound;
		CR::Engine::Audio::Handles::SoundFX disabledSound;
	};
}    // namespace CR::Game

module :private;

namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::InputButton::InputButton() {
	isDown        = false;
	wasPressed    = false;
	touchID       = -1;
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

void cg::InputButton::SetSpriteAndBounds(float left, float top, uint64_t spriteHash, int zPos) {
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

void cg::InputButton::SetSprite(uint64_t spriteHash, [[maybe_unused]] int zPos) {
	if(objectSprite.isValid()) { CR::Engine::Graphics::Sprites::Delete(objectSprite); }
	objectSprite = CR::Engine::Graphics::Sprites::Create(spriteHash);
	// set zpos
}

void cg::InputButton::TouchesBeganImpl(Touch& _touches) {
	// CGPoint glLocation;
	// for (int i = 0;i < _touches.size();++i)
	{
		if(touchID != -1 && touchID != _touches.ID) return;

		// glLocation = GetGLLocation(view, touch);

		if(_touches.X > bounds.left && _touches.X < bounds.right + bounds.left && _touches.Y > bounds.top &&
		   _touches.Y < bounds.bottom + bounds.top) {
			isDown        = true;
			isActing      = true;
			this->touchID = _touches.ID;
		} else {
			isDown   = false;
			isActing = false;
		}
	}
}

void cg::InputButton::TouchesMovedImpl(Touch& _touches) {
	// CGPoint glLocation;
	// for (int i = 0;i < _touches.size();++i)
	{
		if(touchID != -1 && touchID != _touches.ID) return;

		// glLocation = GetGLLocation(view, touch);

		if(_touches.X > bounds.left && _touches.X < bounds.right + bounds.left && _touches.Y > bounds.top &&
		   _touches.Y < bounds.bottom + bounds.top) {
			isDown   = true;
			isActing = true;
			// this->touch = touch;
		} else {
			isDown   = false;
			isActing = false;
		}
	}
}

void cg::InputButton::TouchesEndedImpl(Touch& _touches) {
	// for (int i = 0;i < _touches.size();++i)
	{
		if(touchID == _touches.ID) {
			if(isDown) {
				if(soundOn) { CR::Engine::Audio::SoundFX::Play(m_soundFX); }

				if(OnClicked.Size() > 0)
					OnClicked();
				else
					wasPressed = true;
			}

			isDown        = false;
			isActing      = false;
			this->touchID = -1;
			// break;
		}
	}
}

void cg::InputButton::Reset() {
	touchID    = -1;
	isDown     = false;
	isActing   = false;
	wasPressed = false;
}

void cg::InputButton::Render() {
	if(objectSprite != 0) {
		auto size = CR::Engine::Graphics::Sprites::GetSize(objectSprite);
		CR::Engine::Graphics::Sprites::SetPosition(
		    objectSprite, {position.left + (size.x / 2), position.top + (size.y / 2)});

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
}