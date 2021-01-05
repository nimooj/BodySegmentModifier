#pragma once


// InfoDialog dialog
#include "BodySegmentModifierDoc.h"

#define WM_UPDATE_VERTEX_LIST (WM_USER + 1)


class CInfoDialog : public CPaneDialog
{
	DECLARE_DYNAMIC(CInfoDialog)

public:
	CInfoDialog();   // standard constructor
	virtual ~CInfoDialog();

	CBodySegmentModifierDoc *GetDocument();

	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) {
		CPaneDialog::OnUpdateCmdUI(pTarget, FALSE);
	}

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INFO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_VertexList;
	CEdit m_SegmentName;
	afx_msg void OnBnClickedSave();
protected:
	afx_msg LRESULT OnUpdateVertexList(WPARAM wParam, LPARAM lParam);
};
