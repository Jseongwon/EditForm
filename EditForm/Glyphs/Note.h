// Note.h
#ifndef _NOTE_H
#define _NOTE_H
#include "Composite.h"

class Note : public Composite {
public:
	Note(Long capacity = 256);
	Note(const Note& source);
	virtual ~Note();
	Note& operator =(const Note& source);

	virtual Long Add(Long current, Glyph* glyph);
	virtual void Paste(Glyph* dummyNote);
	virtual void Paste(string text);
	virtual bool RemoveSelect();
	virtual void Combine(Long currentRow);
	virtual void CombineSelectedRows();
	virtual void CombineAll();
	virtual void Select(Long startRowCurrent, Long startNoteCurrent, Long endRowCurrent, Long endNoteCurrent, bool onIsSelect = true);
	virtual void SelectCount(Long startRowCurrent, Long startNoteCurrent, Long textCount);
	virtual void SelectAll(bool onIsSelect = true);
	virtual bool FindPrevious(string strFindWhat, Long strFindWhatLen, bool onIsMatchCase = false);
	virtual bool FindNext(string strFindWhat, Long strFindWhatLen, bool onIsMatchCase = false);
	virtual Long Previous();
	virtual Long Next();
	virtual Long Last();
	virtual void MoveActualCurrent(Long rowCurrent, Long noteCurrent);
	virtual Long MoveToFirstSelectedCurrent();
	virtual Long MoveToLastSelectedCurrent();
	virtual void GetCombineCurrent(Long* lineCurrent, Long* noteCurrent);
	virtual void GetSelectRange(Long* startRowCurrent, Long* startNoteCurrent, Long* endRowCurrent, Long* endNoteCurrent);
	virtual Glyph* Clone();
	virtual void Accept(GlyphVisitor* glyphVisitor);
	virtual string GetString();
	virtual string GetSelectString();
	virtual bool IsSelected() const;
};

#endif // _NOTE_H