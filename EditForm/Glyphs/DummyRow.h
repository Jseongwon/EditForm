// DummyRow.h
#ifndef _DUMMYROW_H
#define _DUMMYROW_H
#include "Row.h"

class DummyRow : public Row {
public:
	DummyRow(Long capacity = 256, bool onIsRow = false);
	DummyRow(const DummyRow& source);
	virtual ~DummyRow();
	virtual Glyph* Clone();
	DummyRow& operator =(const DummyRow& source);
};

#endif // _DUMMYROW_H