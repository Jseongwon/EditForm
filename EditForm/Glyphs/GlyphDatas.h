#pragma once
/*
GlyphData�� �߰��� ���
1. Letter, SingleByteLetter, DoubleByteLetter, Row(Row), Note, Clipboard
2. GlyphScroll, GlyphVerticalScroll, GlyphHorizontalScroll GlyphExtent, GlyphFont, GlyphFile

������ ���
1. GlyphScrollActions, GlyphCommands(Memento, State ...) And FileCommands
2. CommonDialogs
3. GlyphVisitors -> GlyphVisitor(TextOutVisitor, PrintingVisitor, PreviewVisitor), PasteVisitor, CutVisitor, CopyVisitor,
4. ExternalClipboard
5. GlyphSubject, FileForm �и�

����غ� ����(������Ʈ�� Format�� ������ ��� GlyphExtent�� GlyphFont, CDC�� �־�߸� �ڵ����� ������ ������ �� �ִ�.)
CDC��� ���ٴ� ���ڵ��� ũ��� ȭ�� ������ �˰� ������ �ڵ����� ������ ���� �� �ִ�.
1. Compositor->TexCompositor

�����ϱ� ���� ����
1. ������ ���ÿ� cdc�� �Է� �޾ƾ� �ϰ�, ��Ʈ�� �����, ���� ����� �߰��ϰ�, ��Ʈ�� Extent�� �����Ͽ� ��� �־�� �Ѵ�.
������ ��������ִ� �����͸� �����.?
SettingVisitor

���� ���������� ��� Glyph�� ��ü �����

+ DeviceName���� ������ ���� �ٸ��� DMPAPER�� A4�� �������ֱ�
*/