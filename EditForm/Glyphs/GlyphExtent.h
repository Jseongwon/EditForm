// GlyphExtent.h
#ifndef _GLYPHEXTENT_H
#define _GLYPHEXTENT_H
typedef signed long int Long;

class GlyphSubject;
class Glyph;
class GlyphExtent {
public:
	GlyphExtent();
	GlyphExtent(GlyphSubject* glyphSubject);
	GlyphExtent(const GlyphExtent& source);
	~GlyphExtent();
	Long GetWidth(Long index);
	Long GetWidth(char(*text));
	Long GetHeight() const;
	Long GetX(Long current);
	Long GetMaxX();
	Long GetY(Long current);
	Long GetMaxY();
	Long GetDistance(Glyph* line, Long startXPos, Long endXPos);
	Long GetRowCurrent(Long x);
	Long GetNoteCurrent(Long y);
	Long GetSplitCurrent(Glyph* line, Long width);
	Long GetSplitCount(Glyph* line);
	Long GetStringCount(char(*text));
	bool GetOnIsGreaterThanScreen();
	GlyphExtent& operator =(const GlyphExtent& source);

private:
	GlyphSubject* glyphSubject;
	Long(*widths);
	Long height;
};

inline Long GlyphExtent::GetHeight() const {
	return this->height;
}

#endif // _GLYPHEXTENT_H