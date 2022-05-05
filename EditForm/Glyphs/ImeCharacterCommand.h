// ImeCharacterCommand.h
#ifndef _IMECHARACTERCOMMAND_H
#define _IMECHARACTERCOMMAND_H
#include "WriteCommand.h"

class ImeCharacterCommand : public WriteCommand {
public:
	ImeCharacterCommand(GlyphSubject* glyphSubject);
	virtual ~ImeCharacterCommand();
	virtual void Execute();
	virtual void UnExecute();
	virtual void ReExecute();
	virtual Long GetRowCurrent();
	virtual Long GetNoteCurrent();
};

#endif // _IMECHARACTERCOMMAND_H