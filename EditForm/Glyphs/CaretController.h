// CaretController.h
#ifndef _CARETCONTROLLER_H
#define _CARETCONTROLLER_H
#include "Observer.h"

class GlyphSubject;
class Caret;
class CaretController : public Observer {
public:
	GlyphSubject* glyphSubject;
	bool onIsCaret;

public:
	CaretController(GlyphSubject* glyphSubject = 0);
	virtual ~CaretController();
	virtual void Update();
	virtual Observer* Clone();

private:
	Caret* caret;
};

#endif // _CARETCONTROLLER_H