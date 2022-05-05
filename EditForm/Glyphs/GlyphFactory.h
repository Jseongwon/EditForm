// GlyphFactory.h
#ifndef _GLYPHFACTORY_H
#define _GLYPHFACTORY_H
#include <string>
typedef signed long int Long;
using namespace std;

class Glyph;
class GlyphFactory {
public:
	enum { NOTE = 0, ROW = 1, DUMMYROW = 2, SINGLEBYTECHARACTER = 3, DOUBLECHARACTER = 4 };

public:
	GlyphFactory();
	~GlyphFactory();
	Glyph* Create(char(*text), bool onIsRow = true);
	Glyph* CreateNote(string text);
	Glyph* CreateRow(string text);
};

#endif // _GLYPHFACTORY_H