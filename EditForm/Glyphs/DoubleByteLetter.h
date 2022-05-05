// DoubleByteLetter.h
#ifndef _DOUBLEBYTELETTER_H
#define _DOUBLEBYTELETTER_H
#include "Letter.h"

class DoubleByteLetter : public Letter {
public:
	DoubleByteLetter();
	DoubleByteLetter(char(*content));
	DoubleByteLetter(const DoubleByteLetter& source);
	virtual ~DoubleByteLetter();
	DoubleByteLetter& operator =(const DoubleByteLetter& source);

	virtual Glyph* Clone();
	virtual void Accept(GlyphVisitor* glyphVisitor);
	virtual string GetString();
	char* GetContent() const;

private:
	char content[2];
};

inline char* DoubleByteLetter::GetContent() const {
	return const_cast<char*>(this->content);
}

#endif // _DOUBLEBYTELETTER_H