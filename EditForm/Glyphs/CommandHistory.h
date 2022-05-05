//CommandHistory.h
#ifndef _COMMANDHISTORY_H
#define _COMMANDHISTORY_H

class GlyphSubject;
class CommandStack;
class Command;
class CommandHistory {
public:
	GlyphSubject* glyphSubject;

public:
	CommandHistory(GlyphSubject* glyphSubject = 0);
	~CommandHistory();
	void Save(Command* command);
	void Undo();
	void Redo();

private:
	CommandStack* undoStack;
	CommandStack* redoStack;
};

#endif // !_COMMANDHISTORY_H
