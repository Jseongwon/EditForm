// GlyphSubject.h
#ifndef _GLYPHSTATE_H
#define _GLYPHSTATE_H
#include "Subject.h"

class Glyph;
class GlyphState {
	GlyphState();
	GlyphState(const GlyphState& source);
	~GlyphState();
	GlyphState& operator=(const GlyphState& source);

	Glyph* GetNote() const;
	Long GetRowCurrent() const;
	Long GetNoteCurrent() const;

private:
	Glyph* note;
	Long rowCurrent;
	Long noteCurrent;
};

inline Glyph* GlyphState::GetNote() const {
	return const_cast<Glyph*>(this->note);
}
inline Long GlyphState::GetRowCurrent() const {
	return this->rowCurrent;
}
inline Long GlyphState::GetNoteCurrent() const {
	return this->noteCurrent;
}

#endif // _GLYPHSTATE_H