// Glyph.h
#ifndef _GLYPH_H
#define _GLYPH_H
//#include "atltypes.h"
#include <string>
using namespace std;
typedef signed long int Long;

class GlyphVisitor;
class Glyph {
public:
	Glyph();
	virtual ~Glyph() = 0;
	virtual Long Add(Glyph* glyphLink) { return -1; }
	virtual Long Add(Long current, Glyph* glyph) { return -1; }
	virtual void Paste(Glyph* dummyNote) {  }
	virtual void Paste(string text) {  }
	virtual Long Remove(Long index) { return -1; }
	virtual bool RemoveSelect() { return false; }
	virtual void Split(Glyph* line, Long current) {  }
	virtual void Combine(Glyph* line) {  }
	virtual void Combine(Long index, Glyph* line) {  }
	virtual void Combine(Long currentRow) {  }
	virtual void CombineSelectedRows() {  }
	virtual void CombineAll() {  }
	virtual void Select(bool select = true) {  }
	virtual void Select(Long start, Long end, bool select = true) {  }
	virtual void Select(Long startRowCurrent, Long startNoteCurrent, Long endRowCurrent, Long endNoteCurrent, bool select = true) {  }
	virtual void SelectCount(Long startRowCurrent, Long startNoteCurrent, Long textCount) {  }
	virtual void SelectAll(bool select = true) {  }
	virtual bool FindPrevious(string strFindWhat, Long strFindWhatLen, bool onIsMatchCase = false) { return false; }
	virtual bool FindNext(string strFindWhat, Long strFindWhatLen, bool onIsMatchCase = false) { return false; }
	virtual Long First() { return -1; }
	virtual Long Previous() { return -1; }
	virtual Long PreviousWord() { return -1; }
	virtual Long Next() { return -1; }
	virtual Long NextWord() { return -1; }
	virtual Long Last() { return -1; }
	virtual Long Move(Long index) { return -1; }
	virtual void MoveActualCurrent(Long rowCurrent, Long noteCurrent) {  }
	virtual Long MoveToFirstSelectedCurrent() { return -1; }
	virtual Long MoveToLastSelectedCurrent() { return -1; }
	virtual void GetCombineCurrent(Long* lineCurrent, Long* noteCurrent) {  }
	virtual Glyph* GetAt(Long index) { return 0; }
	virtual void GetSelectRange(Long* startRowCurrent, Long* startNoteCurrent, Long* endRowCurrent, Long* endNoteCurrent) {  }
	virtual void Accept(GlyphVisitor* glyphVisitor) = 0;
	virtual Glyph* Clone() = 0;
	virtual string GetString() = 0;
	virtual string GetString(Long start, Long end) { return "\0"; }
	virtual string GetSelectString() { return "\0"; }
	virtual string GetLowerCaseString() { return "\0"; };
	virtual Long GetCapacity() const { return 0; }
	virtual Long GetLength() const { return 0; }
	virtual Long GetCurrent() const { return -1; }
	virtual bool GetOnIsRow() const { return false; }
	virtual bool IsSelected() const { return false; }
};

#endif // _GLYPH_H

/*
Glyph Formatting

목표 : Glyph 인스턴스 하나로 편집기를 구현하자!
*/