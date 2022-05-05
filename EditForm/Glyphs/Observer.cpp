// Observer.cpp
/*
���ϸ�Ī : Observer.cpp
��� : ������ Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2021.01.07
*/
#include "Observer.h"
#include "GlyphSubject.h"

Observer::Observer(GlyphSubject* glyphSubject) {
	this->glyphSubject = glyphSubject;
}

Observer::~Observer() {
}