// GlyphDirector.h
#ifndef _GLYPHDIRECTOR_H
#define _GLYPHDIRECTOR_H

class GlyphSubject;
class Glyph;
class File;
class GlyphDirector {
public:
	GlyphSubject* glyphSubject;

public:
	GlyphDirector(GlyphSubject* glyphSubject);
	virtual ~GlyphDirector();

	virtual void GlyphChanged(Glyph* note);
	virtual void FileChanged(File* file);
};

#endif // _GLYPHDIRECTOR_H