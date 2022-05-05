// DummyRow.cpp
/*
파일명칭 : DummyRow.cpp
기능 : 더미줄 클래스를 만든다.
작성자 : 정성원
작성일자 : 2020.06.18
*/
#include "DummyRow.h"

DummyRow::DummyRow(Long capacity, bool onIsRow)
	: Row(capacity, onIsRow) {
}

DummyRow::DummyRow(const DummyRow& source)
	: Row(source) {
}

DummyRow::~DummyRow() {
}

Glyph* DummyRow::Clone() {
	return new DummyRow(*this);
}

DummyRow& DummyRow::operator =(const DummyRow& source) {
	Row::operator =(source);

	return* this;
}