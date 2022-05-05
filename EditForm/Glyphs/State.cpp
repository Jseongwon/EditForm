// State.cpp
/*
파일명칭 : State.cpp
기능 : 상태 정보 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.26
*/
#include "State.h"
#include "GlyphSubject.h"
#include "Glyph.h"
#include "GlyphFactory.h"

State::State() {
	this->note = 0;
}

State::State(Long rowCurrent, Long noteCurrent, string text) {
	GlyphFactory glyphFactory;
	this->rowCurrent = rowCurrent;
	this->noteCurrent = noteCurrent;
	this->note = glyphFactory.CreateNote(text);
}

State::State(const State& source) {
	this->rowCurrent = source.rowCurrent;
	this->noteCurrent = source.noteCurrent;
	this->note = source.note->Clone();
}

State::~State() {
	if (this->note != 0) {
		delete this->note;
	}
}

State& State::operator =(const State& source) {
	this->rowCurrent = source.rowCurrent;
	this->noteCurrent = source.noteCurrent;
	if (this->note != 0) {
		delete this->note;
	}
	this->note = source.note->Clone();

	return* this;
}