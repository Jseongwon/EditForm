// GlyphPtr.h
#ifndef _GLYPHPTR_H
#define _GLYPHPTR_H
#include <string>
using namespace std;

class Glyph;
class GlyphPtr {
public:
	GlyphPtr();
	GlyphPtr(const GlyphPtr& source);
	~GlyphPtr();
	GlyphPtr& operator =(const GlyphPtr& source);
	Glyph* operator->();

	bool Open(string pathFileName);
	bool Open(string pathName, string fileName);
	bool Save();
	bool SaveAs(string pathFileName);
	bool SaveAs(string pathName, string fileName);
	
	bool Add(size_t index, char(*text));
	bool Remove(size_t index, size_t size);

private:
	Glyph* note;
	size_t offSet;
	string pathFileName;
	string tempFileName;
};

#endif // _GLYPHPTR_H