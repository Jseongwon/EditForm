//ScrollController.h
#ifndef _SCROLLCONTROLLER_H
#define _SCROLLCONTROLLER_H
#include"Observer.h"

class GlyphSubject;
class Scroll;
class ScrollController : public Observer {
public:
	GlyphSubject* glyphSubject;
	Scroll* horizontalScroll;
	Scroll* verticalScroll;

public:
	ScrollController(GlyphSubject* glyphSubject = 0);
	virtual ~ScrollController();
	virtual void Update();
	virtual Observer* Clone();
	Scroll* GetHorizontalScroll();
	Scroll* GetVerticalScroll();
};

#endif // !_SCROLLCONTROLLER_H