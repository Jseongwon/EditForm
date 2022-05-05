// Observer.h
#ifndef _OBSERVER_H
#define _OBSERVER_H

class GlyphSubject;
class Observer {
public:
	enum { ERROROBSERVER = -1, CARET = 0, SCROLL = 1, MEMORY = 2 };

public:
	GlyphSubject* glyphSubject;

public:
	Observer(GlyphSubject* glyphSubject = 0);
	virtual ~Observer() = 0;
	virtual void Update() = 0;
	virtual Observer* Clone() { return 0; }

	int GetType() const;

protected:
	int type;
};

inline int Observer::GetType() const {
	return this->type;
}

#endif // _OBSERVER_H