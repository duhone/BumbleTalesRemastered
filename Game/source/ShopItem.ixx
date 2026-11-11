/*
 *  ShopItem.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/7/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

#include <glm/glm.hpp>

export module CR.Game.ShopItem;

import CR.Engine;

import CR.Game.Constants;
import CR.Game.IRenderable;
import CR.Game.IInputObject;
import CR.Game.InputButton;
import CR.Game.Event;
import CR.Game.StarMeter;

import std;
import std.compat;

export namespace CR::Game {
	class ShopItem : public IRenderable {
	  public:
		ShopItem();
		virtual ~ShopItem();

		// IRenderable
		virtual void Update();
		virtual void Render();
		virtual void SetPosition(float xLoc, float yLoc);
		void PauseAnimation(bool pause);

		int GetLevel() const;
		void CancelLevelUp() { m_level--; }
		int GetHeight() const;

		int GetInformationAsset();
		glm::ivec2 GetInformationDesignSize() const { return m_infoDesignSize; }

		void SetStarCount(int _value);
		void PlusClicked();

		CR::Utility::Event1<ShopItem*> OnClicked;
		CR::Utility::Event1<ShopItem*> OnPlusClicked;

	  protected:
		void SetItemSprite(uint64_t a_spriteHash);

		glm::vec2 offset;
		CR::Engine::Graphics::Handles::Sprite itemSprite;
		int m_level;
		int m_height;
		int m_infoAsset;
		glm::ivec2 m_infoDesignSize;

		Rect bounds;
		CR::Engine::Input::Handles::Region m_region;

		bool isLevelable;
		StarMeter* starMeter;
		InputButton* plusButton;
		CR::Engine::Graphics::Handles::Sprite plusDimmerSprite;
		bool m_plusDimmed{false};

		bool disableInput;
	};
}    // namespace CR::Game

module :private;

namespace cecore  = CR::Engine::Core;
namespace ceinput = CR::Engine::Input;
namespace cegraph = CR::Engine::Graphics;
namespace cg      = CR::Game;

using namespace CR::Engine::Core::Literals;

cg::ShopItem::ShopItem() {
	disableInput = false;
	offset       = {0.0f, 0.0f};
	bounds       = {};
	m_region     = ceinput::Regions::create(cecore::Rect2D<int32_t>{{0, 0}, {0, 0}});

	m_level     = -1;
	m_height    = 150;
	m_infoAsset = 0;
	starMeter   = new StarMeter();

	plusButton = new InputButton();
	plusButton->SetSpriteAndBounds(0, 0, "ShopButton_Plus"_hash64, 40);
	plusButton->OnClicked += CR::Utility::Delegate(this, &ShopItem::PlusClicked);

	cegraph::Sprites::Create("ShopPlusDimmer"_hash64, plusDimmerSprite);
	cegraph::Sprites::SetZOrders(plusDimmerSprite, 30);
	cegraph::Sprites::SetVisibilities(plusDimmerSprite, false);
}

cg::ShopItem::~ShopItem() {
	delete starMeter;
	delete plusButton;
	if(itemSprite.isValid()) { cegraph::Sprites::Delete(itemSprite); }
	cegraph::Sprites::Delete(plusDimmerSprite);
	ceinput::Regions::release(m_region);
}

void cg::ShopItem::SetItemSprite(uint64_t a_spriteHash) {
	if(itemSprite.isValid()) { cegraph::Sprites::Delete(itemSprite); }
	cegraph::Sprites::Create(a_spriteHash, itemSprite);
	cegraph::Sprites::SetZOrders(itemSprite, 60);
	cegraph::Sprites::SetVisibilities(itemSprite, true);
	ShopItem::SetPosition(offset.x, offset.y);
}

void cg::ShopItem::Update() {
	starMeter->SetFilledStars(m_level);
	starMeter->Update();

	m_plusDimmed = starMeter->GetStarIndicator() <= 0 || starMeter->GetFilledStars() >= 5;
	plusButton->SetVisible(m_level >= 0);
	plusButton->Disabled(disableInput || m_level < 0 || m_plusDimmed);
	plusButton->Update();

	if(disableInput) {
		ceinput::Regions::update(m_region, cecore::Rect2D<int32_t>{{0, 0}, {0, 0}});
		return;
	}
	ceinput::Regions::update(
		m_region, cecore::Rect2D<int32_t>{{bounds.left, bounds.top}, {bounds.right, bounds.bottom}});

	uint32_t state{};
	ceinput::Regions::getStates(m_region, state);
	if((state & ceinput::Regions::RegionStates::Down) != 0) { OnClicked(this); }
}

void cg::ShopItem::Render() {
	starMeter->SetVisible(m_level >= 0);
	starMeter->Render();

	plusButton->Render();

	cegraph::Sprites::SetVisibilities(plusDimmerSprite, m_level >= 0 && m_plusDimmed);
}

void cg::ShopItem::SetPosition(float xLoc, float yLoc) {
	offset.x = xLoc;
	offset.y = yLoc;

	starMeter->SetPosition(offset.x - 125, offset.y - 8);
	plusButton->SetPosition((int)(offset.x + 164), (int)(offset.y - 10));
	cegraph::Sprites::SetPositions(plusDimmerSprite, glm::vec2{offset.x + 164, offset.y - 10});

	glm::uvec2 itemSize{0, 0};
	if(itemSprite.isValid()) { cegraph::Sprites::GetSizes(itemSprite, itemSize); }
	glm::vec2 itemTopLeft = offset - glm::vec2(itemSize) / 2.0f;
	if(itemSprite.isValid()) { cegraph::Sprites::SetPositions(itemSprite, itemTopLeft); }

	bounds.top    = (int)itemTopLeft.y;
	bounds.bottom = (int)itemSize.y;
	bounds.left   = (int)itemTopLeft.x;
	bounds.right  = (int)itemSize.x;
}

void cg::ShopItem::PauseAnimation([[maybe_unused]] bool pause) {}

int cg::ShopItem::GetLevel() const {
	return m_level;
}

int cg::ShopItem::GetHeight() const {
	return m_height;
}

int cg::ShopItem::GetInformationAsset() {
	return m_infoAsset;
}

void cg::ShopItem::SetStarCount(int _value) {
	starMeter->SetStarIndicator(_value);
}

void cg::ShopItem::PlusClicked() {
	if(starMeter->GetStarIndicator() > 0 && m_level >= 0 && m_level < 5) {
		m_level++;
		OnPlusClicked(this);
	}
}