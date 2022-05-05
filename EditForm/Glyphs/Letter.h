// Letter.h
#ifndef _LETTER_H
#define _LETTER_H
#include "Glyph.h"

class Letter : public Glyph {
public:
	Letter();
	virtual ~Letter() = 0;
	virtual void Select(bool select = true);
	virtual bool IsSelected() const;

protected:
	bool onIsSelected;
};

inline bool Letter::IsSelected() const {
	return this->onIsSelected;
}

#endif // _LETTER_H