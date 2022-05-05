//CommandFactory.h
#ifndef _COMMANDFACTORY_H
#define _COMMANDFACTORY_H
#include<string>
using namespace std;
typedef signed long int Long;
class Command;
class GlyphSubject;
class CommandFactory {
public:
	CommandFactory(GlyphSubject* glyphSubject);
	~CommandFactory();
	Command* Create(Long id);
public:
	GlyphSubject* glyphSubject;
};


#endif // !_COMMANDFACTORY_H
