// SingleByteLetter.cpp
/*
���ϸ�Ī : SingleByteLetter.cpp
��� : 1����Ʈ ���� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.06
*/
#include "SingleByteLetter.h"
#include "GlyphVisitors.h"

SingleByteLetter::SingleByteLetter() {
	this->content = '\0';
}

SingleByteLetter::SingleByteLetter(char content) {
	this->content = content;
}

SingleByteLetter::SingleByteLetter(const SingleByteLetter& source) {
	this->content = source.content;
	this->onIsSelected = source.onIsSelected;
}

SingleByteLetter::~SingleByteLetter() {

}

SingleByteLetter& SingleByteLetter::operator =(const SingleByteLetter& source) {
	this->content = source.content;
	this->onIsSelected = source.onIsSelected;

	return* this;
}

Glyph* SingleByteLetter::Clone() {
	return new SingleByteLetter(*this);
}

void SingleByteLetter::Accept(GlyphVisitor* glyphVisitor) {
	glyphVisitor->Visit(this);
}

string SingleByteLetter::GetString() {
	string str;
	str.push_back(this->content);

	return str;
}