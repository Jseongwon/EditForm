// SingleByteLetter.h
#ifndef _SINGLEBYTELETTER_H
#define _SINGLEBYTELETTER_H
#include "Letter.h"

class SingleByteLetter : public Letter {
public:
	SingleByteLetter();
	SingleByteLetter(char content);
	SingleByteLetter(const SingleByteLetter& source);
	virtual ~SingleByteLetter();
	SingleByteLetter& operator =(const SingleByteLetter& source);

	virtual Glyph* Clone();
	virtual void Accept(GlyphVisitor* glyphVisitor);
	virtual string GetString();
	char GetContent() const;

private:
	char content;
};

inline char SingleByteLetter::GetContent() const {
	return this->content;
}

#endif // _SINGLEBYTELETTER_H