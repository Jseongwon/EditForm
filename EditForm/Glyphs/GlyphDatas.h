#pragma once
/*
GlyphData에 추가할 목록
1. Letter, SingleByteLetter, DoubleByteLetter, Row(Row), Note, Clipboard
2. GlyphScroll, GlyphVerticalScroll, GlyphHorizontalScroll GlyphExtent, GlyphFont, GlyphFile

수정할 목록
1. GlyphScrollActions, GlyphCommands(Memento, State ...) And FileCommands
2. CommonDialogs
3. GlyphVisitors -> GlyphVisitor(TextOutVisitor, PrintingVisitor, PreviewVisitor), PasteVisitor, CutVisitor, CopyVisitor,
4. ExternalClipboard
5. GlyphSubject, FileForm 분리

고민해볼 사항(컴포지트의 Format을 나누는 방법 GlyphExtent와 GlyphFont, CDC가 있어야만 자동개행 포멧을 설정할 수 있다.)
CDC라기 보다는 글자들의 크기와 화면 영역을 알고 있으면 자동개행 포멧을 만들 수 있다.
1. Compositor->TexCompositor

설정하기 위한 전략
1. 생성과 동시에 cdc를 입력 받아야 하고, 노트를 만들고, 줄을 만들어 추가하고, 폰트와 Extent를 생성하여 들고 있어야 한다.
포멧을 변경시켜주는 비지터를 만든다.?
SettingVisitor

모든걸 포괄적으로 담는 Glyph의 객체 만들기

+ DeviceName으로 프린터 명이 다르면 DMPAPER를 A4로 변경해주기
*/