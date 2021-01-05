
// BodySegmentModifierDoc.cpp : implementation of the CBodySegmentModifierDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BodySegmentModifier.h"
#endif

#include "BodySegmentModifierDoc.h"
#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBodySegmentModifierDoc

IMPLEMENT_DYNCREATE(CBodySegmentModifierDoc, CDocument)

BEGIN_MESSAGE_MAP(CBodySegmentModifierDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CBodySegmentModifierDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CBodySegmentModifierDoc::OnFileSave)
END_MESSAGE_MAP()


// CBodySegmentModifierDoc construction/destruction

CBodySegmentModifierDoc::CBodySegmentModifierDoc() noexcept
{
	// TODO: add one-time construction code here
	isHuman = false;
	Human = new HumanObject();
}

CBodySegmentModifierDoc::~CBodySegmentModifierDoc()
{
	Human = 0;
}

BOOL CBodySegmentModifierDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CBodySegmentModifierDoc serialization

void CBodySegmentModifierDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CBodySegmentModifierDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CBodySegmentModifierDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CBodySegmentModifierDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CBodySegmentModifierDoc diagnostics

#ifdef _DEBUG
void CBodySegmentModifierDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBodySegmentModifierDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBodySegmentModifierDoc commands


void CBodySegmentModifierDoc::OnFileOpen()
{
	// TODO: Add your command handler code here
	LPCTSTR filter = _T("obj file(ascii)(*.obj)|*.obj||All Files (*.*)|*.*||");
	CFileDialog FileDlg(true, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (FileDlg.DoModal() != IDOK)
		return;

	m_FileName = CT2A(FileDlg.GetPathName());

	BeginWaitCursor();

	if (Human->LoadObj(LPSTR(LPCTSTR(m_FileName)))) {
		isHuman = true;
	}


	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	pFrame->m_InfoDialog.SendMessage(WM_UPDATE_VERTEX_LIST, 0, 0);

	EndWaitCursor();


	UpdateAllViews(NULL);
}


void CBodySegmentModifierDoc::OnFileSave()
{
	// TODO: Add your command handler code here
	
	// CHECK :: Segment region에 속한 정점의 인덱스들을 저장 ? 
	LPCTSTR filter = _T("obj file(ascii)(*.obj)|*.obj||");

	CFileDialog FileDlg(false, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (FileDlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	EndWaitCursor();
}
