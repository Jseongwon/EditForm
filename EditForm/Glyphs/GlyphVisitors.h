// GlyphVisitors.h
#ifndef _GLYPHVISITORS_H
#define _GLYPHVISITORS_H
typedef signed long int Long;

class GlyphSubject;
class CDC;
class Note;
class Row;
class SingleByteLetter;
class DoubleByteLetter;
class GlyphVisitor {
public:
	GlyphVisitor();
	virtual ~GlyphVisitor() = 0;
	
	virtual void Visit(Note* note) {  }
	virtual void Visit(Row* row) {  }
	virtual void Visit(SingleByteLetter* singleByteLetter) {  }
	virtual void Visit(DoubleByteLetter* doubleByteLetter) {  }
};

class GlyphDrawingVisitor : public GlyphVisitor {
public:
	GlyphDrawingVisitor(GlyphSubject* glyphSubject = 0, CDC* pDC = 0);
	virtual ~GlyphDrawingVisitor();

	virtual void Visit(Note* note);
	virtual void Visit(Row* row);
	virtual void Visit(SingleByteLetter* singleByteLetter);
	virtual void Visit(DoubleByteLetter* doubleByteLetter);

private:
	GlyphSubject* glyphSubject;
	CDC* pDC;
	CDC* tempDC;
	Long x;
	Long y;
};

#endif // _TEXTDRAWINGVISITOR_H