/*
 *  SettingsSaveInfo.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 6/26/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
module;

#include <engine/Engine.h>

export module CR.Game.SettingsSaveInfo;

import CR.Game.SaveInfo;

import CR.Engine;

import std;
import std.compat;

export namespace CR::Game {
	class SettingsSaveInfo : public SaveInfo {
	  public:
		SettingsSaveInfo();
		virtual ~SettingsSaveInfo();

		bool GetOptionsHintsOn() const { return m_hintsOn; }
		void SetOptionsHintsOn(bool _value) { m_hintsOn = _value; }

		bool GetOptionsMusicOn() const { return m_musicOn; }
		void SetOptionsMusicOn(bool _value) { m_musicOn = _value; }

		bool GetOptionsSoundOn() const { return m_soundOn; }
		void SetOptionsSoundOn(bool _value) { m_soundOn = _value; }

		virtual void Save();
		virtual void Load();
		virtual void Reset();

	  protected:
	  private:
		FILE* m_file;

		bool m_hintsOn;
		bool m_musicOn;
		bool m_soundOn;
	};
}    // namespace CR::Game

module :private;

namespace cg = CR::Game;

cg::SettingsSaveInfo::SettingsSaveInfo() :
    m_file(nullptr), m_hintsOn(true), m_soundOn(true), m_musicOn(true) {}

cg::SettingsSaveInfo::~SettingsSaveInfo() {}

void cg::SettingsSaveInfo::Save() {
	FILE* file = nullptr;
	file       = fopen("settings.sav", "wb");

	if(!file) {
		Reset();
		return;
	}

	fwrite(&m_hintsOn, 1, sizeof(m_hintsOn), file);
	fwrite(&m_soundOn, 1, sizeof(m_soundOn), file);
	fwrite(&m_musicOn, 1, sizeof(m_musicOn), file);

	fclose(file);
}

void cg::SettingsSaveInfo::Load() {
	FILE* file = nullptr;
	file       = fopen("settings.sav", "rb");
	if(file) {
		fread(&m_hintsOn, 1, sizeof(m_hintsOn), file);
		fread(&m_soundOn, 1, sizeof(m_soundOn), file);
		fread(&m_musicOn, 1, sizeof(m_musicOn), file);

		fclose(file);
	} else {
		Reset();
	}
}

void cg::SettingsSaveInfo::Reset() {
	m_hintsOn = true;
	m_soundOn = true;
	m_musicOn = true;
}