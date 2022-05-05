// Subject.h
#ifndef _SUBJECT_H
#define _SUBJECT_H
#include "Array.h"

class Observer;
class Subject {
public:
	Subject(Long capacity = 24);
	Subject(const Subject& source);
	~Subject();
	void Attach(Observer* o);
	void Detach(Observer* o);
	void Notify();
	Long Find(int type);
	Observer* GetAt(Long index);
	Long GetCapacity() const;
	Long GetLength() const;
	Subject& operator =(const Subject& source);

protected:
	Array<Observer*> observers;
	Long capacity;
	Long length;
};

inline Observer* Subject::GetAt(Long index) {
	return const_cast<Subject*>(this)->observers.GetAt(index);
}
inline Long Subject::GetCapacity() const {
	return this->capacity;
}
inline Long Subject::GetLength() const {
	return this->length;
}

int CompareObservers(void* one, void* other);

#endif // _SUBJECT_H