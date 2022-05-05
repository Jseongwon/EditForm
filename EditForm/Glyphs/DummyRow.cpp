// DummyRow.cpp
/*
���ϸ�Ī : DummyRow.cpp
��� : ������ Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2020.06.18
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