// Caret.cpp
/*
파일명칭 : Caret.cpp
기능 : 캐럿 클래스를 만든다.
작성자: 정성원
작성일자 : 2020.04.29
*/
#include "Caret.h"
#include "GlyphSubject.h"
#include "CaretController.h"

Caret::Caret(CaretController* caretController) {
	this->caretController = caretController;
}

Caret::Caret(const Caret& source) {
	this->caretController = source.caretController;
}

Caret::~Caret() {
	DestroyCaret();
}

void Caret::Create(Long width, Long height) {
	this->caretController->glyphSubject->CreateSolidCaret(width, height);
}

void Caret::Move(Long x, Long y) {
	this->caretController->glyphSubject->SetCaretPos(CPoint(x, y));
	this->caretController->glyphSubject->ShowCaret();
}

void Caret::Hide() {
	this->caretController->glyphSubject->HideCaret();
}

Caret& Caret::operator =(const Caret& source) {
	this->caretController = source.caretController;

	return* this;
}