/*
 *  IScrapbookChapter.h
 *  Bumble Tales
 *
 *  Created by Robert Shoemate on 7/4/09.
 *  Copyright 2009 Conjured Realms LLC. All rights reserved.
 *
 */
export module CR.Game.IScrapbookChapter;

import std;
import std.compat;

export namespace CR::Game {
	enum ScrapbookChapter { CharactersChapter, RecordsChapter, TrophiesChapter };

	class IScrapbookChapter /*: public Input_Object */ {
	  public:
		virtual ~IScrapbookChapter() {};

		virtual void Update() = 0;
		virtual void Render() = 0;

		virtual void NextPage() = 0;
		virtual void PrevPage() = 0;

		virtual ScrapbookChapter GetChapterType() = 0;

		// TODO: Input_Object from HPT engine, doesn't exist anymore, need to redo the input code
		// virtual void TouchesBeganImpl(Touch& _touches) {}
		// virtual void TouchesMovedImpl(Touch& _touches) {}
		// virtual void TouchesEndedImpl(Touch& _touches) {}

		virtual void Reset() {};
		virtual void FreeResources() {};

	  protected:
		IScrapbookChapter() {};
	};
}    // namespace CR::Game

module :private;
