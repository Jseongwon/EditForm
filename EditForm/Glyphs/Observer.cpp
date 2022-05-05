// Observer.cpp
/*
파일명칭 : Observer.cpp
기능 : 옵저버 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.07
*/
#include "Observer.h"
#include "GlyphSubject.h"

Observer::Observer(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

Observer::~Observer() {
}