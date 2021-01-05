// InfoDialog.cpp : implementation file
//

#include "pch.h"
#include "BodySegmentModifier.h"
#include "MainFrm.h"
#include "InfoDialog.h"
#include "afxdialogex.h"


// InfoDialog dialog

IMPLEMENT_DYNAMIC(CInfoDialog, CPaneDialog)

CInfoDialog::CInfoDialog()
{
}

CInfoDialog::~CInfoDialog()
{
}

CBodySegmentModifierDoc* CInfoDialog::GetDocument() {
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	return (CBodySegmentModifierDoc *)pFrame->GetActiveDocument();
}

void CInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CPaneDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VERTEX_LIST, m_VertexList);
	DDX_Control(pDX, IDC_SEGMENT_NAME, m_SegmentName);
}


BEGIN_MESSAGE_MAP(CInfoDialog, CPaneDialog)
	ON_BN_CLICKED(IDC_SAVE, &CInfoDialog::OnBnClickedSave)
//	ON_WM_ACTIVATE()
ON_MESSAGE(WM_UPDATE_VERTEX_LIST, &CInfoDialog::OnUpdateVertexList)
END_MESSAGE_MAP()


// InfoDialog message handlers


void CInfoDialog::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here

	// Save Segment with vert list
	// CString name = m_SegmentName.
}


afx_msg LRESULT CInfoDialog::OnUpdateVertexList(WPARAM wParam, LPARAM lParam)
{
	CBodySegmentModifierDoc* pDoc = GetDocument();

	CRect rect;
	m_VertexList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_VertexList.GetClientRect(&rect);

	m_VertexList.InsertColumn(0, _T("Vertex Id"), LVCFMT_CENTER, rect.Width() * 0.2);
	m_VertexList.InsertColumn(1, _T("Vertex Position"), LVCFMT_CENTER, rect.Width() * 0.8);

	if (pDoc->isHuman) {
		for (int i = 0; i < pDoc->Human->m_Vertices->size(); i++) {
			CString id;
			id.Format(_T("%d"), (*pDoc->Human->m_Vertices)[i]->m_Idx);
			m_VertexList.InsertItem(i, id);

			CString x, y, z, xyz;
			x.Format(_T("%.3f"), (*pDoc->Human->m_Vertices)[i]->m_Coord->x);
			y.Format(_T("%.3f"), (*pDoc->Human->m_Vertices)[i]->m_Coord->y);
			z.Format(_T("%.3f"), (*pDoc->Human->m_Vertices)[i]->m_Coord->z);
			
			xyz = x + _T(", ") + y + _T(", ") + z;
			m_VertexList.SetItem(i, 1, LVIF_TEXT, xyz, 0, 0, 0, NULL);
		}
	}

	return 0;
}
