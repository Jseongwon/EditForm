// GlyphDirector.cpp
/*
���ϸ�Ī : GlyphDirector.cpp
��    �� : ������ ������ ������ �۸��� ������ Ŭ������ �����.
�ۼ����� : 2022.01.13
*/
#include "GlyphDirector.h"
#include "GlyphSubject.h"
#include "Glyph.h"

GlyphDirector::GlyphDirector(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

GlyphDirector::~GlyphDirector() {

}

void GlyphDirector::GlyphChanged(Glyph* note) {

}

void GlyphDirector::FileChanged(File* file) {
	// 1. �ӽ� ������ �����.

	// 2. ���ϸ��� ������
}