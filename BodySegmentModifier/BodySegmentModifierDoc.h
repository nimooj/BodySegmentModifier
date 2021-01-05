
// BodySegmentModifierDoc.h : interface of the CBodySegmentModifierDoc class
//


#pragma once

#include "MeshObject/HumanObject.h"

#define WM_UPDATE_VERTEX_LIST (WM_USER + 1)

class CBodySegmentModifierDoc : public CDocument
{
public : // mj ::
	bool isHuman;
	CString m_FileName;

	HumanObject *Human = 0;

protected: // create from serialization only
	CBodySegmentModifierDoc() noexcept;
	DECLARE_DYNCREATE(CBodySegmentModifierDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CBodySegmentModifierDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
};
