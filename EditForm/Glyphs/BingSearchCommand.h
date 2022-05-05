// BingSearchCommand.h
#ifndef _BINGSEARCHCOMMAND_H
#define _BINGSEARCHCOMMAND_H
#include "Command.h"

class BingSearchCommand : public Command {
public:
	BingSearchCommand(GlyphSubject* glyphSubject = 0);
	virtual ~BingSearchCommand();
	virtual void Execute();
};

#endif // _BINGSEARCHCOMMAND_H