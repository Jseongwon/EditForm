// Note.cpp
/*
파일명칭 : Note.cpp
기능 : 메모지 클래스를 만든다.
작성자 : 정성원
작성일자 : 2021.01.20
*/
#include "Note.h"
#include "Glyph.h"
#include "GlyphVisitors.h"
#include "GlyphFactory.h"

Note::Note(Long capacity)
	: Composite(capacity) {
}

Note::Note(const Note& source)
	: Composite(source) {
}

Note::~Note() {
}

Note& Note::operator =(const Note& source) {
	Composite::operator =(source);

	return* this;
}

Long Note::Add(Long current, Glyph* glyph) {
	Long index;
	index = this->glyphs.Insert(current, glyph);
	this->capacity++;
	this->length++;
	this->current = index;

	return this->current;
}

void Note::Paste(Glyph* dummyNote) {
	GlyphFactory glyphFactory;
	Glyph* dummyRow;
	Glyph* currentRow;
	Glyph* splitRow;
	Glyph* letter;
	Glyph* letterCopy;

	bool onIsSelected;
	
	Long i = 0;
	Long j;

	currentRow = this->glyphs.GetAt(this->current);
	splitRow = glyphFactory.Create((char*)"\r");
	currentRow->Split(splitRow, currentRow->GetCurrent());

	while (i < dummyNote->GetLength()) {
		dummyRow = dummyNote->GetAt(i);
		j = 0;
		while (j < dummyRow->GetLength()) {
			letter = dummyRow->GetAt(j);
			letterCopy = letter->Clone();
			currentRow->Add(currentRow->GetCurrent(), letterCopy);
			j++;
		}
		onIsSelected = dummyRow->IsSelected();
		currentRow->Select(onIsSelected);
		if (i + 1 < dummyNote->GetLength()) {
			currentRow = glyphFactory.Create((char*)"\r");
			this->glyphs.Insert(this->current + 1, currentRow);
			this->capacity++;
			this->length++;
			this->current++;
		}
		i++;
	}

	if (splitRow->GetLength() > 0) {
		currentRow->Combine(splitRow);
		currentRow->Select(false);
	}
	if (splitRow != 0) {
		delete splitRow;
	}
}

void Note::Paste(string text) {
	GlyphFactory glyphFactory;
	Glyph* currentRow = 0;
	Glyph* glyph;

	char character[2];

	Long rowCurrent;
	Long length = text.length();
	Long i = 0;

	currentRow = this->glyphs.GetAt(this->current);
	while (i < length) {
		character[0] = text[i];
		if (character[0] == '\r' || character[0] == '\n') {
			glyph = glyphFactory.Create(character);
			currentRow->Split(glyph, currentRow->GetCurrent());
			currentRow = glyph;
			this->current = this->glyphs.Insert(this->current + 1, glyph);
			this->capacity++;
			this->length++;
			if (i + 1 < length && text[i] == '\r' && text[i + 1] == '\n') {
				i++;
			}
		}
		else if (character[0] == 0x09 || character[0] >= 0x20 && character[0] < 0x7F) {
			glyph = glyphFactory.Create(character);
			rowCurrent = currentRow->GetCurrent();
			currentRow->Add(rowCurrent, glyph);
		}
		else if (character[0] & 0x80) {
			i++;
			character[1] = text[i];
			glyph = glyphFactory.Create(character);
			rowCurrent = currentRow->GetCurrent();
			currentRow->Add(rowCurrent, glyph);
		}
		i++;
	}
}

bool Note::RemoveSelect() {
	bool ret = false;
	bool onIsFound = false;
	bool onIsRemoved = false;
	bool onIsCurrentSelected = false;

	Long noteCurrent = 0;
	Long j;
	Long i = 0;

	Glyph* currentRow = 0;
	Glyph* row = 0;
	Glyph* character = 0;
	// 1. 종이의 마지막 줄보다 작고, 메모지 위치를 찾을 동안 반복한다.(메모지의 선택된 시작 위치를 찾는다)
	while (i < this->length && onIsFound != true) {
		// 1.1. 줄을 읽는다.
		row = this->glyphs.GetAt(i);
		j = 0;
		// 1.2. 줄의 마지막 칸보다 작고, 메모지 위치를 찾을 동안 반복한다.
		while (j < row->GetLength() && onIsFound != true) {
			// 1.2.1. 글자를 읽는다.
			character = row->GetAt(j);
			// 1.2.2. 글자가 선택되어 있으면 메모지 위치를 구한다.
			if (character->IsSelected() == true) {
				noteCurrent = i;
				onIsFound = true;
			}
			j++;
		}
		// 1.3. 줄의 마지막 칸이고 현재 줄이 선택되어 있고 찾지 못했으면 메모지 위치를 구한다.
		if (j >= row->GetLength() && row->IsSelected() == true && onIsFound != true) {
			noteCurrent = i;
			onIsFound = true;
		}
		i++;
	}
	// 2. 위치를 찾았으면 메모지에서 현재 위치로 이동하고, 현재 줄을 구한다.
	if (onIsFound == true) {
		ret = onIsFound;
		this->current = noteCurrent;
		currentRow = this->glyphs.GetAt(this->current);
		onIsCurrentSelected = currentRow->IsSelected();
	}
	// 3. 위치를 찾았으면 줄에서 선택된 글자들을 지운다.
	i = noteCurrent;
	if (i < this->length && onIsFound == true) {
		row = this->glyphs.GetAt(i);
		row->RemoveSelect();
		i++;
	}
	// 4. 마지막 줄보다 작고, 마지막 개행문자가 선택되어 있으면 반복한다.
	while (i < this->length && onIsCurrentSelected == true) {
		// 4.1. 줄을 읽는다.
		row = this->glyphs.GetAt(i);
		// 4.2. 줄에서 지운다.
		row->RemoveSelect();
		// 4.3. 현재줄의 개행문자가 선택되어 있지 않으면 선택 여부를 없음으로 한다.
		if (row->IsSelected() != true) {
			onIsCurrentSelected = false;
		}
		// 4.4. 현재 줄에서 줄을 합친다.
		currentRow->Combine(row);
		delete this->glyphs.GetAt(i);
		this->glyphs.Delete(i);
		this->capacity--;
		this->length--;
	}

	return ret;
}

void Note::Combine(Long current) { // 현재 줄의 자동개행된 줄들을 합치는 연산
	Glyph* row = this->glyphs.GetAt(current);
	Glyph* nextRow = 0;

	Long noteCurrent;
	Long rowCurrent = 0;
	Long i = 0;
	while (i <= current) {// 1. 현재 줄의 실제(onIsRow이 true인) 줄을 구한다.
		row = this->glyphs.GetAt(i);
		if (row->GetOnIsRow() == true) {
			noteCurrent = i;
		}
		i++;
	}

	i = noteCurrent;
	while (i < current) { // 2. 합치고 난 뒤 이동할 칸을 미리 구한다.
		row = this->glyphs.GetAt(i);
		rowCurrent += row->GetLength();
		i++;
	}
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		rowCurrent += row->GetCurrent();
	}

	i = noteCurrent; // 3. 현재 줄의 자동개행된 줄만 합쳐준다.
	if (this->length > i + 1) {
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);
	}
	while (this->length > i + 1 && nextRow->GetOnIsRow() != true) {
		row->Combine(nextRow);
		row->Select(nextRow->IsSelected()); // 다음 줄의 선택여부를 확인하여 선택한다.(2021.01.18)
		delete nextRow;
		this->glyphs.Delete(i + 1);
		nextRow = this->glyphs.GetAt(i + 1);
		this->capacity--;
		this->length--;
	}
	// 4. 합치고 난 뒤 위치로 이동한다.
	this->current = noteCurrent;
	this->glyphs.GetAt(this->current)->Move(rowCurrent);
}

void Note::CombineSelectedRows() {
	Glyph* row;
	Glyph* nextRow;
	Glyph* glyph;

	bool onIsStop = false;
	bool onIsSelected = false;
	bool onIsCurrentRowSelected = false;

	Long startSelectedtNoteCurrent = -1;
	Long noteCurrentForRowCurrent;
	Long noteCurrent = -1;
	Long rowCurrent = 0;
	Long j;
	Long i = 0;
	// 1. 선택된 줄의 시작 위치를 찾는다.
	while (i < this->length && onIsStop != true) {
		row = this->glyphs.GetAt(i);
		j = 0;
		while (j < row->GetLength() && onIsStop != true) {
			glyph = row->GetAt(j);
			if (glyph->IsSelected() == true) {
				startSelectedtNoteCurrent = i;
				onIsStop = true;
			}
			j++;
		}
		if (row->IsSelected() == true) {
			startSelectedtNoteCurrent = i;
			onIsStop = true;
		}
		i++;
	}

	// 2. 합치고 난 뒤의 칸을 구하기 위한 줄의 위치와 / 합치고 난뒤의 줄 위치를 / 미리 구한다.
	i = 0;
	while (i <= this->current) {
		row = this->glyphs.GetAt(i);
		if (row->GetOnIsRow() == true) {
			noteCurrentForRowCurrent = i;
			noteCurrent++;
		}
		i++;
	}

	// 3. 합치고 난 뒤의 칸을 구한다.
	i = noteCurrentForRowCurrent;
	while (i < this->current) {
		row = this->glyphs.GetAt(i);
		rowCurrent += row->GetLength();
		i++;
	}
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		rowCurrent += row->GetCurrent();
	}

	// 4. 선택된 자동개행된 줄들을 합친다.
	// ===================================
	// 1. 선택된 첫 번째 줄이 있으면
	i = startSelectedtNoteCurrent;
	if (startSelectedtNoteCurrent >= 0 && i < this->length) {
		// 1.1. 선택된 줄과 다음줄을 구한다.
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);

		// 1.2. 다음줄을 합친다.
		while (this->length > i + 1 && nextRow->GetOnIsRow() != true) {
			row->Combine(nextRow);
			onIsSelected = nextRow->IsSelected();
			row->Select(onIsSelected);
			delete nextRow;
			this->glyphs.Delete(i + 1);
			nextRow = this->glyphs.GetAt(i + 1);
			this->capacity--;
			this->length--;
		}
		// 1.3. 합쳐진 줄의 선택 여부를 구한다.
		onIsCurrentRowSelected = row->IsSelected();
	}

	// 2. 선택된 줄이 있을동안 줄을 합친다.
	while (i < this->length - 1 && onIsCurrentRowSelected == true) {
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);
		while (this->length > i + 1 && nextRow->GetOnIsRow() != true) {
			row->Combine(nextRow);
			onIsSelected = nextRow->IsSelected();
			row->Select(onIsSelected);
			delete nextRow;
			this->glyphs.Delete(i + 1);
			nextRow = this->glyphs.GetAt(i + 1);
			this->capacity--;
			this->length--;
		}
		onIsCurrentRowSelected = row->IsSelected();
		i++;
	}
	// ===================================
	// 5. 구한 위치로 이동한다.
	this->current = noteCurrent;
	row = this->glyphs.GetAt(this->current);
	row->Move(rowCurrent);
}

void Note::CombineAll() { // 모든 줄의 자동개행된 줄들을 합친다.
	Glyph* row;
	Glyph* nextRow;

	Long noteCurrentForRowCurrent;
	Long noteCurrent = -1;
	Long rowCurrent = 0;
	Long i = 0;
	while (i <= this->current) {// 1. 합치고 난 뒤의 칸을 구하기 위한 줄의 위치와 / 합치고 난뒤의 줄 위치를 / 미리 구한다.
		row = this->glyphs.GetAt(i);
		if (row->GetOnIsRow() == true) {
			noteCurrentForRowCurrent = i;
			noteCurrent++;
		}
		i++;
	}

	i = noteCurrentForRowCurrent;// 2. 합치고 난 뒤의 칸을 구한다.
	while (i < this->current) {
		row = this->glyphs.GetAt(i);
		rowCurrent += row->GetLength();
		i++;
	}
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		rowCurrent += row->GetCurrent();
	}

	i = 0;
	while (i < this->length - 1) {// 3. 모든 자동개행된 줄을 합친다.
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);
		while (this->length > i + 1 && nextRow->GetOnIsRow() != true) {
			row->Combine(nextRow); // 알아서 할당해제 해준다.
			row->Select(nextRow->IsSelected()); // 다음 줄의 선택여부를 확인하여 선택한다.(2021.01.18)
			delete nextRow;
			this->glyphs.Delete(i + 1);
			nextRow = this->glyphs.GetAt(i + 1);
			this->capacity--;
			this->length--;
		}
		i++;
	}
	// 4. 합치고 난 뒤 위치로 이동한다.
	this->current = noteCurrent;
	this->glyphs.GetAt(this->current)->Move(rowCurrent);
}

void Note::Select(Long startRowCurrent, Long startNoteCurrent, Long endRowCurrent, Long endNoteCurrent, bool onIsSelect) {
	Long tempRowCurrent = startRowCurrent;
	Long tempNoteCurrent = startNoteCurrent;

	Glyph* row;

	if (startNoteCurrent > endNoteCurrent || (startNoteCurrent == endNoteCurrent && startRowCurrent > endRowCurrent)) {
		startRowCurrent = endRowCurrent; //lLeft = lRight;
		startNoteCurrent = endNoteCurrent; //lTop = lBottom;
		endRowCurrent = tempRowCurrent; //lRight = left;
		endNoteCurrent = tempNoteCurrent; //lBottom = top;
	}

	row = this->glyphs.GetAt(startNoteCurrent);
	if (startNoteCurrent != endNoteCurrent) {
		row->Select(startRowCurrent, row->GetLength(), onIsSelect);
		row->Select();
	}
	else {
		row->Select(startRowCurrent, endRowCurrent, onIsSelect);
	}

	Long i = startNoteCurrent + 1;
	while (i < endNoteCurrent) {
		row = this->glyphs.GetAt(i);
		row->Select(0, row->GetLength(), onIsSelect);
		row->Select();
		i++;
	}

	if (startNoteCurrent != endNoteCurrent && i <= endNoteCurrent) {
		row = this->glyphs.GetAt(i);
		row->Select(0, endRowCurrent, onIsSelect);
	}
}

void Note::SelectCount(Long startRowCurrent, Long startNoteCurrent, Long textCount) {
	Glyph* row;
	Glyph* nextRow;
	Glyph* glyph;

	bool onIsFirst;
	
	Long endRowCurrent = startRowCurrent;
	Long endNoteCurrent = startNoteCurrent;
	Long i;

	// 2.1. 이전에 선택된 글자들을 모두 선택 해제 한다.
	i = 0;
	while (i < this->length) {
		row = this->glyphs.GetAt(i);
		row->SelectAll(false);
		i++;
	}

	// 2.2. 시작 위치부터 글자 개수만큼 선택한다.
	onIsFirst = true;
	i = startNoteCurrent;
	while (i < this->length && textCount > 0) {
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);
		endRowCurrent = 0;
		if (onIsFirst == true) {
			endRowCurrent = startRowCurrent;
			onIsFirst = false;
		}
		while (endRowCurrent < row->GetLength() && textCount > 0) {
			glyph = row->GetAt(endRowCurrent);
			glyph->Select(true);
			textCount--;
			endRowCurrent++;
		}
		if (endRowCurrent >= row->GetLength() && i < this->length - 1 && textCount > 0) {
			row->Select(true);
		}
		if (i < this->length - 1 && nextRow->GetOnIsRow() == true) {
			textCount--;
		}
		endNoteCurrent = i;
		i++;
	}
	// 2.3. 마지막 위치로 이동한다.
	this->current = endNoteCurrent;
	row = this->glyphs.GetAt(this->current);
	row->Move(endRowCurrent);
}

void Note::SelectAll(bool onIsSelect) {
	Glyph* row;

	Long i = 0;
	while (i < this->length - 1) {
		row = this->glyphs.GetAt(i);
		row->SelectAll(onIsSelect);
		i++;
	}
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		row->Select(0, row->GetLength(), onIsSelect);
	}
}

bool Note::FindPrevious(string strFindWhat, Long strFindWhatLen, bool onIsMatchCase) {
	bool ret = false;
	bool onIsFirst = true;
	bool onIsPrevious = false;
	bool onIsMatching = false;

	Glyph* row = 0;
	Glyph* nextRow;
	Glyph* glyph;
	string character;
	char findText[3];

	Long findTextLength = (Long)strFindWhat.length();
	Long matchCount = 0;
	Long textCount = strFindWhatLen;
	Long startRowCurrent;
	Long startNoteCurrent;
	Long endRowCurrent;
	Long endNoteCurrent;
	Long k = (Long)strFindWhat.length() - 1;
	Long j;
	Long i = this->current;

	// 1. 현재 위치의 줄부터 마지막 줄보다 작고, 매칭되는 개수와 찾는 글자의 개수가 같지 않을 동안 반복한다.
	while (i >= 0 && matchCount != strFindWhatLen) {
		// 1.1. 줄을 읽는다.
		row = this->glyphs.GetAt(i);
		j = row->GetLength() - 1;
		// 1.2. 처음이면 현재 위치의 줄의 현재 칸부터 시작한다.
		if (onIsFirst == true) {
			j = row->GetCurrent() - 1;
			onIsFirst = false;
		}
		// 1.3. 줄의 마지막 칸보다 작고, 매칭되는 개수와 찾는 글자의 개수가 같지 않을 동안 반복한다.
		while (j >= 0 && row->GetLength() > 0 && matchCount != strFindWhatLen) {
			// 1.3.1. 글자를 읽는다.
			character = row->GetAt(j)->GetString();
			onIsMatching = false;
			// 1.3.2. 찾는 글자의 한 글자를 따온다.
			if (strFindWhat[k] & 0x80) {
				findText[0] = strFindWhat[k - 1];
				findText[1] = strFindWhat[k];
				findText[2] = '\0';
				k -= 2;
			}
			else {
				findText[0] = strFindWhat[k];
				findText[1] = '\0';
				k--;
			}
			// 1.3.3. 찾는 한 글자와 현재 글자가 같으면 매칭 여부를 참으로 한다.
			if ((character[0] & 0x80 && character.compare(findText) == 0) ||
				(onIsMatchCase == true && character.compare(findText) == 0)) {
				onIsMatching = true;
			}
			// 1.3.4. 대/소문자 구분여부가 체크되어 있지 않고 찾는 한 글자와 현재 글자를 대/소문자 구분없이 비교해서 같으면
			// 매칭 여부를 참으로 한다.
			else if (onIsMatchCase != true && _stricmp(character.c_str(), findText) == 0) {
				onIsMatching = true;
			}
			// 1.3.5. 매칭 여부가 참이면 매칭 개수를 증가시킨다.
			if (onIsMatching == true) {
				matchCount++;
				onIsPrevious = true;
			}
			// 1.3.6. 매칭 여부가 거짓이면 매칭개수를 원위치 시킨다.
			else {
				matchCount = 0;
				k = (Long)strFindWhat.length() - 1;
				onIsPrevious = false;
			}
			j--;
		}
		// 1.4. 찾는 글자를 읽는다.
		if (k > 0) {
			findText[0] = strFindWhat[k];
			findText[1] = '\0';
		}

		// 1.5. 글자가 개행문자이면 매칭개수를 증가한다.
		if (findText[0] == '\n') {
			k--;
			matchCount++;
			onIsPrevious = true;
		}
		// 1.6. 현재 줄이 실제 줄이고, 이전으로 이동한 적이 있고, 매칭 개수가 찾는 글자 개수와 같지 않으면 매칭개수를 원위치 시킨다.
		// 자동개행을 풀지 않고 찾을 수 있게 해 놓음.
		else if (row->GetOnIsRow() == true && onIsPrevious == true && matchCount != strFindWhatLen) {
			matchCount = 0;
			k = (Long)strFindWhat.length() - 1;
			onIsPrevious = false;
		}
		
		i--;
	}
	// 2. 매칭개수와 찾는 글자 개수가 같으면(찾았으면)
	if (matchCount == strFindWhatLen && onIsFirst != true) {
		// 2.1. 시작 위치를 구한다.
		startRowCurrent = j + 1;
		startNoteCurrent = i + 1;

		// 2.2. 이전에 선택된 글자들을 모두 선택 해제 한다.
		i = 0;
		while (i < this->length) {
			row = this->glyphs.GetAt(i);
			row->SelectAll(false);
			i++;
		}

		// 2.3. 시작 위치부터 글자 개수만큼 선택한다.
		onIsFirst = true;
		i = startNoteCurrent;
		while (i < this->length && textCount > 0) {
			row = this->glyphs.GetAt(i);
			nextRow = this->glyphs.GetAt(i + 1);
			endRowCurrent = 0;
			if (onIsFirst == true) {
				endRowCurrent = startRowCurrent;
				onIsFirst = false;
			}
			while (endRowCurrent < row->GetLength() && textCount > 0) {
				glyph = row->GetAt(endRowCurrent);
				glyph->Select(true);
				textCount--;
				endRowCurrent++;
			}
			if (endRowCurrent >= row->GetLength() && i < this->length - 1 && textCount > 0) {
				row->Select(true);
			}
			if (i < this->length - 1 && nextRow->GetOnIsRow() == true) {
				textCount--;
			}
			endNoteCurrent = i;
			i++;
		}
		// 2.4. 마지막 위치로 이동한다.
		this->current = endNoteCurrent;
		row = this->glyphs.GetAt(this->current);
		row->Move(endRowCurrent);

		ret = true;
	}

	return ret;
}

bool Note::FindNext(string strFindWhat, Long strFindWhatLen, bool onIsMatchCase) {
	bool ret = false;

	Glyph* previous = 0;
	Glyph* row = 0;
	Glyph* nextRow;
	Glyph* glyph;
	string character;
	char findText[3];

	bool onIsFirst = true;
	bool onIsNext = false;
	bool onIsMatching = false;

	Long findTextLength = (Long)strFindWhat.length();
	Long textCount = strFindWhatLen;
	Long endRowCurrent;
	Long endNoteCurrent;
	Long matchCount = 0;
	Long k = 0;
	Long j;
	Long i = this->current;

	// 1. 현재 위치의 줄부터 마지막 줄보다 작고, 매칭되는 개수와 찾는 글자의 개수가 같지 않을 동안 반복한다.
	while (i < this->length && matchCount != strFindWhatLen) {
		// 2. 줄과 다음줄을 읽는다.
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);

		j = 0;
		// 1.2. 처음이면 현재 위치의 줄의 현재 칸부터 시작한다.
		if (onIsFirst == true) {
			j = row->GetCurrent();
			onIsFirst = false;
		}
		// 1.3. 줄의 마지막 칸보다 작고, 매칭되는 개수와 찾는 글자의 개수가 같지 않을 동안 반복한다.
		while (j < row->GetLength() && matchCount != strFindWhatLen) {
			// 1.3.1. 글자를 읽는다.
			glyph = row->GetAt(j);
			character = glyph->GetString();
			onIsMatching = false;
			// 1.3.2. 찾는 글자의 한 글자를 따온다.
			if (strFindWhat[k] & 0x80) {
				findText[0] = strFindWhat[k];
				findText[1] = strFindWhat[k + 1];
				findText[2] = '\0';
				k += 2;
			}
			else {
				findText[0] = strFindWhat[k];
				findText[1] = '\0';
				k++;
			}
			// 1.3.3. 찾는 한 글자와 현재 글자가 같으면 매칭 여부를 참으로 한다.
			if ((character[0] & 0x80 && character.compare(findText) == 0) ||
				(onIsMatchCase == true && character.compare(findText) == 0)) {
				onIsMatching = true;
			}
			// 1.3.4. 대/소문자 구분여부가 체크되어 있지 않고 찾는 한 글자와 현재 글자를 대/소문자 구분없이 비교해서 같으면
			// 매칭 여부를 참으로 한다.
			else if (onIsMatchCase != true && _stricmp(character.c_str(), findText) == 0) {
				onIsMatching = true;
			}
			// 1.3.5. 매칭 여부가 참이면 매칭 개수를 증가시킨다.
			if (onIsMatching == true) {
				matchCount++;
				onIsNext = true;
			}
			// 1.3.6. 매칭 여부가 거짓이면 매칭개수를 원위치 시킨다.
			else {
				matchCount = 0;
				k = 0;
				onIsNext = false;
			}
			j++;
		}
		// 1.4. 찾는 글자를 읽는다.
		if (k < findTextLength) {
			findText[0] = strFindWhat[k];
			findText[1] = '\0';
		}
		
		// 1.5. 글자가 개행문자이면 매칭개수를 증가한다.
		if (findText[0] == '\n') {
			k++;
			matchCount++;
			onIsNext = true;
		}
		// 1.6. 찾는 글자가 개행문자가 아니고, 이전줄이 있고, 이전줄이 실제 줄이고, 다음이동 여부가 참이고,  
		// 매칭개수와 찾는 글자개수가 같지 않으면
		// 자동개행을 풀지 않고 찾을 수 있게 해 놓음.
		else if (i < this->length - 1 && nextRow->GetOnIsRow() == true && onIsNext == true && matchCount != strFindWhatLen) {
			matchCount = 0;
			k = 0;
			onIsNext = false;
		}
		i++;
	}

	// 2. 매칭개수와 찾는 글자 개수가 같으면(찾았으면)
	if (matchCount == strFindWhatLen) {
		// 2.1. 마지막 위치를 구한다. 거꾸로 가야됨
		endRowCurrent = j;
		endNoteCurrent = i - 1;

		// 2.2. 이전에 선택된 글자들을 모두 선택 해제 한다.
		i = 0;
		while (i < this->length) {
			row = this->glyphs.GetAt(i);
			row->SelectAll(false);
			i++;
		}

		// 2.3. 마지막 위치부터 글자 개수만큼 선택한다.
		onIsFirst = true;
		i = endNoteCurrent;
		while (i >= 0 && textCount > 0) {
			previous = this->glyphs.GetAt(i - 1);
			row = this->glyphs.GetAt(i);
			j = row->GetLength() - 1;
			if (onIsFirst == true) {
				j = endRowCurrent - 1;
				onIsFirst = false;
			}
			while (j >= 0 && textCount > 0) {
				glyph = row->GetAt(j);
				glyph->Select(true);
				textCount--;
				j--;
			}
			if (j < 0 && i > 0 && textCount > 0) {
				previous->Select(true);
			}
			if (row->GetOnIsRow() == true) {
				textCount--;
			}
			i--;
		}

		// 2.4. 마지막 위치로 이동한다.
		this->current = endNoteCurrent;
		row = this->glyphs.GetAt(this->current);
		row->Move(endRowCurrent);

		ret = true;
	}

	return ret;
}

Long Note::Previous() {
	this->current--;
	if (this->current < 0) {
		this->current = 0;
	}

	return this->current;
}

Long Note::Next() {
	this->current++;
	if (this->current >= this->length) {
		this->current = this->length - 1;
	}

	return this->current;
}

Long Note::Last() {
	this->current = this->length - 1;

	return this->current;
}

void Note::MoveActualCurrent(Long rowCurrent, Long noteCurrent) {
	Glyph* row;

	Long rowCount = 0;
	Long findNoteCurrent = 0;
	Long i = 0;

	while (i < this->length && rowCount <= noteCurrent) {// 1. 현재 줄의 실제(onIsRow이 true인) 줄을 구한다.
		row = this->glyphs.GetAt(i);
		if (row->GetOnIsRow() == true) {
			findNoteCurrent = i;
			rowCount++;
		}
		i++;
	}

	row = this->glyphs.GetAt(findNoteCurrent);
	i = findNoteCurrent;
	while (i < this->glyphs.GetLength() - 1 && rowCurrent > row->GetLength()) {
		findNoteCurrent++;
		rowCurrent -= row->GetLength();

		row = this->glyphs.GetAt(i + 1);
		i++;
	}

	this->current = findNoteCurrent;
	row = this->glyphs.GetAt(this->current);
	row->Move(rowCurrent);
}

Long Note::MoveToFirstSelectedCurrent() {
	Glyph* row;

	Long noteCurrent = -1;
	Long rowCurrent = -1;
	Long i = 0;
	while (i < this->length && rowCurrent == -1) {
		row = this->glyphs.GetAt(i);
		rowCurrent = row->MoveToFirstSelectedCurrent();
		i++;
	}
	if (rowCurrent != -1) {
		noteCurrent = i - 1;
		this->current = noteCurrent;
	}

	return noteCurrent;
}

Long Note::MoveToLastSelectedCurrent() {
	Glyph* row;

	bool onIsSelected;

	Long noteCurrent = 0;
	Long rowCurrent = -1;
	Long i = this->length;
	while (i > 0 && rowCurrent == -1) {
		row = this->glyphs.GetAt(i - 1);
		rowCurrent = row->MoveToLastSelectedCurrent();
		onIsSelected = row->IsSelected();
		i--;
	}
	if (rowCurrent != -1) {
		noteCurrent = i;
		if (onIsSelected == true) {
			noteCurrent++;
		}
		this->current = noteCurrent;
	}

	return noteCurrent;
}

void Note::GetCombineCurrent(Long* rowCurrent, Long* noteCurrent) {
	Glyph* row;

	Long noteCurrentForRowCurrent;
	Long findNoteCurrent = -1;
	Long findRowCurrent = 0;
	Long i = 0;
	while (i <= this->current) {// 1. 합치고 난 뒤의 칸을 구하기 위한 줄의 위치와 / 합치고 난뒤의 줄 위치를 / 미리 구한다.
		row = this->glyphs.GetAt(i);
		if (row->GetOnIsRow() == true) {
			noteCurrentForRowCurrent = i;
			findNoteCurrent++;
		}
		i++;
	}

	i = noteCurrentForRowCurrent;// 2. 합치고 난 뒤의 칸을 구한다.
	while (i < this->current) {
		row = this->glyphs.GetAt(i);
		findRowCurrent += row->GetLength();
		i++;
	}
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		findRowCurrent += row->GetCurrent();
	}

	// 3. 미리 구한 위치를 출력한다.
	*noteCurrent = findNoteCurrent;
	*rowCurrent = findRowCurrent;
}

void Note::GetSelectRange(Long* startRowCurrent, Long* startNoteCurrent, Long* endRowCurrent, Long* endNoteCurrent) {
	Long j;
	Long i = 0;

	Glyph* currentRow = this->glyphs.GetAt(this->current);
	Glyph* row;
	Glyph* character = 0;

	bool onIsFound = false; // 시작 선택된 위치를 찾기 위한 플래그
	bool onIsFirst = true; // 마지막 선택된 위치를 찾기 위한 플래그

	*startRowCurrent = 0;
	*startNoteCurrent = 0;
	*endRowCurrent = 0;
	*endNoteCurrent = 0;

	// 1. 시작 선택된 위치를 구한다.
	while (i < this->length && onIsFound != true) {
		row = this->glyphs.GetAt(i);
		j = 0;
		while (j < row->GetLength() && onIsFound != true) {
			character = row->GetAt(j);
			if (character->IsSelected() == true) {
				*startRowCurrent = j;
				*startNoteCurrent = i;
				onIsFound = true;
			}
			j++;
		}
		if (j >= row->GetLength() && row->IsSelected() == true && onIsFound != true) {
			*startRowCurrent = j;
			*startNoteCurrent = i;
			onIsFound = true;
		}
		i++;
	}

	if (onIsFound != true) {
		*startRowCurrent = currentRow->GetCurrent();
		*startNoteCurrent = this->current;
	}
	
	bool onIsStop = false;
	// 2. 마지막 선택된 위치를 구한다.
	i =* startNoteCurrent;
	while (i < this->length && onIsStop != true) {
		row = this->glyphs.GetAt(i);
		j = 0;
		if (onIsFirst == true) {
			j =* startRowCurrent;
			onIsFirst = false;
		}
		while (j < row->GetLength() && onIsStop != true) {
			character = row->GetAt(j);
			if (character->IsSelected() == true) {
				*endRowCurrent = j + 1;
				*endNoteCurrent = i;
			}
			else {
				onIsStop = true;
			}
			j++;
		}
		if (j >= row->GetLength() && row->IsSelected() == true) {
			*endRowCurrent = 0;
			*endNoteCurrent = i + 1;
		}
		i++;
	}

	if (onIsFound != true) {
		*endRowCurrent = currentRow->GetCurrent();
		*endNoteCurrent = this->current;
	}
}

Glyph* Note::Clone() {
	return new Note(*this);
}

void Note::Accept(GlyphVisitor* glyphVisitor) {
	glyphVisitor->Visit(this);
}

string Note::GetString() {
	string str;
	string text;

	Glyph* row;
	Glyph* nextRow;
	
	Long j;
	Long i = 0;
	while (i < this->length - 1) {
		row = this->glyphs.GetAt(i);
		nextRow = this->glyphs.GetAt(i + 1);
		j = 0;
		while (j < row->GetLength()) {
			text = row->GetAt(j)->GetString();
			str += text;

			j++;
		}
		if (nextRow->GetOnIsRow() == true) {
			str += "\n";
		}

		i++;
	}
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		j = 0;
		while (j < row->GetLength()) {
			text = row->GetAt(j)->GetString();
			str += text;

			j++;
		}
	}

	return str;
}

string Note::GetSelectString() {
	string str;

	Glyph* row;
	Glyph* nextRow;

	Long i = 0;
	// 1. 마지막 줄 전까지 반복한다.
	while (i < this->length - 1) {
		// 1.1. 줄을 읽는다.
		row = this->glyphs.GetAt(i);
		// 1.2. 다음 줄을 읽는다.
		nextRow = this->glyphs.GetAt(i + 1);

		// 1.3. 줄에서 선택된 글자들을 누적한다.
		str += row->GetSelectString();
		// 1.4. 현재 줄이 선택되어 있고, 다음 줄이 실제 줄이면 개행문자를 누적한다.
		if (row->IsSelected() == true && nextRow->GetOnIsRow() == true) {
			str += "\n";
		}

		i++;
	}
	// 2. 마지막 줄의 선택된 글자들을 누적한다.
	if (i < this->length) {
		row = this->glyphs.GetAt(i);
		str += row->GetSelectString();
	}
	// 3. 누적된 글자들을 출력한다.
	return str;
}

bool Note::IsSelected() const {
	bool onIsSelect = false;

	Glyph* row;
	Glyph* character;

	Long j;
	Long i = 0;
	while (i < this->length && onIsSelect != true) {
		row = const_cast<Note*>(this)->glyphs.GetAt(i);
		j = 0;
		while (j < row->GetLength() && onIsSelect != true) {
			character = row->GetAt(j);
			onIsSelect = character->IsSelected();
			j++;
		}
		if (row->IsSelected() == true) {
			onIsSelect = row->IsSelected();
		}
		i++;
	}

	return onIsSelect;
}