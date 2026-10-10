/*
 *  RepeatedSpriteHelper.h
 *  Bumble Tales
 *
 *  Created by Eric Duhon on 8/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.RepeatedSpriteHelper;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class RepeatedSpriteHelper {
	  public:
		RepeatedSpriteHelper(uint64_t _image, int _zInitial);
		~RepeatedSpriteHelper();
		void Start();
		void End();
		void NewSprite();
		void SetPosition(int _x, int _y);
		void SetFrame(int _frame);
		// void SetFrameRate(int _frameRate);
		// void SetImage(int _image);

	  private:
		std::vector<CR::Engine::Graphics::Handles::Sprite> m_sprites;
		int m_currentSprite;
		bool m_singleSetMode;
		int m_zInitial;
		uint64_t m_imageHash;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

cg::RepeatedSpriteHelper::RepeatedSpriteHelper(uint64_t _imageHash, int _zInitial) :
    m_imageHash(_imageHash), m_zInitial(_zInitial) {}

cg::RepeatedSpriteHelper::~RepeatedSpriteHelper() {
	for(const auto sprite : m_sprites) { cegraph::Sprites::Delete(sprite); }
}

void cg::RepeatedSpriteHelper::NewSprite() {
	++m_currentSprite;
	if(m_currentSprite >= m_sprites.size()) {
		// there was something called single set mode in old hoopty engine. no clue what it was for.
		m_sprites.push_back(cegraph::Sprites::Create(m_imageHash));
		cegraph::Sprites::SetZOrder(m_sprites.back(), (uint8_t)m_zInitial);
	}
}

void cg::RepeatedSpriteHelper::SetPosition(int _x, int _y) {
	if(m_currentSprite != -1) { cegraph::Sprites::SetPosition(m_sprites[m_currentSprite], {_x, _y}); }
}

void cg::RepeatedSpriteHelper::SetFrame(int _frame) {
	if(m_currentSprite != -1) { cegraph::Sprites::SetFrame(m_sprites[m_currentSprite], (uint16_t)_frame); }
}

/* TODO: ill figure out what to do with this once I see it used. we used fixed frame rates these days for
smoothness. void cg::RepeatedSpriteHelper::SetFrameRate(int _frameRate) { if(m_currentSprite != -1)
m_sprites[m_currentSprite]->SetFrameRate(_frameRate);
}
*/

void cg::RepeatedSpriteHelper::Start() {
	m_currentSprite = -1;
}

void cg::RepeatedSpriteHelper::End() {
	if(m_currentSprite != -1 && m_sprites.size() > m_currentSprite + 1) {
		for(int i = m_currentSprite + 1; i < m_sprites.size(); ++i) {
			cegraph::Sprites::Delete(m_sprites[i]);
		}
		m_sprites.resize(m_currentSprite + 1);
	}
}

/* TODO: is this needed? if so need to recreate all the sprites
void cg::RepeatedSpriteHelper::SetImage(int _image) {
    m_image = _image;
}
*/
