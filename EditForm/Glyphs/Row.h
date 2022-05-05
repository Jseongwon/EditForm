// Row.h
#ifndef _ROW_H
#define _ROW_H
#include "Composite.h"

class Row : public Composite {
public:
	Row(Long capacity = 256, bool onIsRow = true);
	Row(const Row& source);
	virtual ~Row();
	Row& operator =(const Row& source);

	virtual Long Add(Long current, Glyph* glyph);
	virtual bool RemoveSelect();
	virtual void Split(Glyph* row, Long current);
	virtual void Combine(Glyph* row);
	virtual void Combine(Long index, Glyph* row);
	virtual void Select(bool onIsSelect = true);
	virtual void Select(Long start, Long end, bool onIsSelect = true);
	virtual void SelectAll(bool onIsSelect = true);
	virtual Long Previous();
	virtual Long PreviousWord();
	virtual Long Next();
	virtual Long NextWord();
	virtual Long Last();
	virtual Long MoveToFirstSelectedCurrent();
	virtual Long MoveToLastSelectedCurrent();
	virtual string GetString();
	virtual string GetString(Long startXPos, Long endXPos);
	virtual string GetSelectString();
	virtual Glyph* Clone();
	virtual void Accept(GlyphVisitor* glyphVisitor);
	virtual bool GetOnIsRow() const;
	virtual bool IsSelected() const;

protected:
	bool onIsRow;
	bool onIsSelected;
};

inline bool Row::GetOnIsRow() const {
	return this->onIsRow;
}

inline bool Row::IsSelected() const {
	return this->onIsSelected;
}

#endif // _ROW_H