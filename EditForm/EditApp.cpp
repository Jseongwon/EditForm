// EditApp.cpp
/*
���ϸ�Ī : EditApp.cpp
��� : �޸���ö �� Ŭ������ �����.
�ۼ��� : ������
�ۼ����� : 2020.04.14
*/
#include "EditApp.h"
#include "TestForm.h"
#include <afxdisp.h>

EditApp notepadApp;

BOOL EditApp::InitInstance() {
	// Remainder of function definition omitted.
	CWinApp::InitInstance();

	CRect rect(400, 100, 900, 500);

	TestForm* testForm;
	testForm = new TestForm;

	this->m_pMainWnd = testForm;

	//SetRegistryKey("GlyphSubject");
	//LoadStdProfileSettings(8);

	testForm->Create(NULL, "�׽�Ʈ");
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CoInitialize(NULL);
	AfxEnableControlContainer();
	testForm->MoveWindow(400, 100, 900, 500);
	testForm->ShowWindow(SW_SHOW);
	testForm->UpdateWindow();

	return TRUE;
}