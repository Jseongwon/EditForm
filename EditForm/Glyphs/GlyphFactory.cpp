// GlyphFactory.cpp
/*
파일명칭 : GlyphFactory.cpp
기능 : 글자 팩토리 클래스를 만든다.
작성자 : 정성원
작성일자 : 2020.04.24
*/
#include "GlyphFactory.h"
#include "Glyph.h"
#include "Note.h"
#include "Row.h"
#include "SingleByteLetter.h"
#include "DoubleByteLetter.h"
#include "DummyRow.h"

GlyphFactory::GlyphFactory() {
}

GlyphFactory::~GlyphFactory() {
}

Glyph* GlyphFactory::Create(char(*text), bool onIsRow) {
	Glyph* glyph = 0;
	if (text == 0 || text[0] == '\0') {
		glyph = new Note;
	}
	else if (text[0] == '\r' || text[0] == '\n') {
		if (onIsRow == true) {
			glyph = new Row;
		}
		else {
			glyph = new DummyRow;
		}
	}
	else if (text[0] & 0x80) {
		glyph = new DoubleByteLetter(text);
	}
	else if (!(text[0] & 0x80)) {
		glyph = new SingleByteLetter(text[0]);
	}

	return glyph;
}

Glyph* GlyphFactory::CreateNote(string text) {
	Glyph* note;
	Glyph* row = 0;
	Glyph* glyph;

	Long noteCurrent;
	Long length = (Long)text.length();
	Long i = 0;

	char character[2];

	note = new Note;
	if (length > 0) {
		row = new Row;
		note->Add(row);
		note->First();
	}

	while (i < length) {
		character[0] = text[i];
		if (character[0] == '\r' || character[0] == '\n') {
			glyph = new Row;
			row->Split(glyph, row->GetCurrent());
			row = glyph;
			noteCurrent = note->GetCurrent();
			note->Add(noteCurrent + 1, glyph);
			if (i + 1 < length && text[i] == '\r' && text[i + 1] == '\n') {
				i++;
			}
		}
		else if (character[0] == 0x09 || character[0] >= 0x20 && character[0] < 0x7F) {
			glyph = new SingleByteLetter(character[0]);
			row->Add(row->GetCurrent(), glyph);
		}
		else if (character[0] & 0x80) {
			i++;
			character[1] = text[i];
			glyph = new DoubleByteLetter(character);
			row->Add(row->GetCurrent(), glyph);
		}
		i++;
	}
	return note;
}

Glyph* GlyphFactory::CreateRow(string text) {
	Glyph* row = 0;
	Glyph* glyph;

	Long length = (Long)text.length();
	Long i = 0;

	char character[2];

	if (length > 0) {
		row = new Row;
	}

	while (i < length) {
		character[0] = text[i];
		if (character[0] == 0x09 || character[0] >= 0x20 && character[0] < 0x7F) {
			glyph = new SingleByteLetter(character[0]);
			row->Add(row->GetCurrent(), glyph);
		}
		else if (character[0] & 0x80) {
			i++;
			character[1] = text[i];
			glyph = new DoubleByteLetter(character);
			row->Add(row->GetCurrent(), glyph);
		}
		i++;
	}

	return row;
}