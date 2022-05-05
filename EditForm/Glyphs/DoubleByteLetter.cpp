// DoubleByteLetter.cpp
/*
���ϸ�Ī : DoubleByteLetter.cpp
��� : 2����Ʈ ���� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.06
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