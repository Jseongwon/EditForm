// DoubleByteLetter.cpp
/*
파일명칭 : DoubleByteLetter.cpp
기능 : 2바이트 글자 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.06
*/
#include "DoubleByteLetter.h"
#include "GlyphVisitors.h"

DoubleByteLetter::DoubleByteLetter(){
	this->content[0] = '\0';
	this->content[1] = '\0';
}

DoubleByteLetter::DoubleByteLetter(char(*content)) {
	this->content[0] = content[0];
	this->content[1] = content[1];
}

DoubleByteLetter::DoubleByteLetter(const DoubleByteLetter& source) {
	this->content[0] = source.content[0];
	this->content[1] = source.content[1];
	this->onIsSelected = source.onIsSelected;
}

DoubleByteLetter::~DoubleByteLetter() {

}

DoubleByteLetter& DoubleByteLetter::operator =(const DoubleByteLetter& source) {
	this->content[0] = source.content[0];
	this->content[1] = source.content[1];
	this->onIsSelected = source.onIsSelected;

	return* this;
}

Glyph* DoubleByteLetter::Clone() {
	return new DoubleByteLetter(*this);
}

void DoubleByteLetter::Accept(GlyphVisitor* glyphVisitor) {
	glyphVisitor->Visit(this);
}

string DoubleByteLetter::GetString() {
	char content[3];
	content[0] = this->content[0];
	content[1] = this->content[1];
	content[2] = '\0';

	return content;
}