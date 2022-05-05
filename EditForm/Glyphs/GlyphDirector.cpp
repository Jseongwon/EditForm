// GlyphDirector.cpp
/*
파일명칭 : GlyphDirector.cpp
기    능 : 중재자 패턴을 응용한 글리프 관리자 클래스를 만든다.
작성일자 : 2022.01.13
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
	// 1. 임시 파일을 만든다.

	// 2. 파일명이 있으면
}