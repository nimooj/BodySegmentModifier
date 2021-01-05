
// BodySegmentModifierView.h : interface of the CBodySegmentModifierView class
//

#pragma once

#include <conio.h>
// #define _USE_MATH_DEFINES
// #include <cmath>
#include "MeshObject/HumanObject.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define M_PI 3.14159265


class CBodySegmentModifierView : public CView
{
public : // mj::
	CDC *m_pDC;
	HDC m_hDC = 0;
	HGLRC m_hRC;

	glm::vec3 m_ViewCenter;
	bool m_Bound;

	enum { ORTHO = 0, PERS } m_CameraMode;

	float m_Width, m_Height;

	float m_Fovy, m_ViewSize, m_ViewRatio, m_Near, m_Far;

	glm::mat4 m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix;
	glm::mat4 m_ModelViewMatrix;

	CPoint m_Start, m_Curr, m_End;
	glm::vec3 m_ModelStart, m_ModelEnd;

	void RenderScene();
	void RenderBoudingBox(CDC* pDC);
	void RenderBackground();

	void SetupViewport(float w, float h);
	void SetupFrustum();
	void SetupOffset();


	void UnProjection(float x, float y, float z, double *wx, double *wy, double *wz);

	void InitializeGL();
	// void SetMaterial();

protected: // create from serialization only
	CBodySegmentModifierView() noexcept;
	DECLARE_DYNCREATE(CBodySegmentModifierView)

// Attributes
public:
	CBodySegmentModifierDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CBodySegmentModifierView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBound();
	afx_msg void OnUpdateBound(CCmdUI *pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // debug version in BodySegmentModifierView.cpp
inline CBodySegmentModifierDoc* CBodySegmentModifierView::GetDocument() const
   { return reinterpret_cast<CBodySegmentModifierDoc*>(m_pDocument); }
#endif

