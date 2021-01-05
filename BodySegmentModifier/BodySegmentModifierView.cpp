
// BodySegmentModifierView.cpp : implementation of the CBodySegmentModifierView class
//
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BodySegmentModifier.h"
#endif

#include "BodySegmentModifierDoc.h"
#include "BodySegmentModifierView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#ifdef __glad_h_
#undef __glad_h_
#endif

#include "GL/freeglut.h"
#include "glm/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

// CBodySegmentModifierView

IMPLEMENT_DYNCREATE(CBodySegmentModifierView, CView)

BEGIN_MESSAGE_MAP(CBodySegmentModifierView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_COMMAND(ID_BOUND, &CBodySegmentModifierView::OnBound)
	ON_UPDATE_COMMAND_UI(ID_BOUND, &CBodySegmentModifierView::OnUpdateBound)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CBodySegmentModifierView diagnostics

#ifdef _DEBUG
void CBodySegmentModifierView::AssertValid() const
{
	CView::AssertValid();
}

void CBodySegmentModifierView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBodySegmentModifierDoc* CBodySegmentModifierView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBodySegmentModifierDoc)));
	return (CBodySegmentModifierDoc*)m_pDocument;
}
#endif //_DEBUG

void CBodySegmentModifierView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


BOOL CBodySegmentModifierView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

int CBodySegmentModifierView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pDC = new CClientDC(this);
	m_hDC = m_pDC->GetSafeHdc();

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			24,
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			32,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
	};
	int pixelformat;
	pixelformat = ChoosePixelFormat(m_hDC, &pfd);
	SetPixelFormat(m_hDC, pixelformat, &pfd);

	m_hRC = wglCreateContext(m_hDC);

	wglMakeCurrent(m_hDC, m_hRC);

	InitializeGL();

	wglMakeCurrent(0, 0);

	return 0;
}


void CBodySegmentModifierView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	if (::wglDeleteContext(m_hRC) == FALSE)
		AfxMessageBox(_T("Failed to delete wglDeleteContext."));
}


BOOL CBodySegmentModifierView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	// return CView::OnEraseBkgnd(pDC);
	return TRUE;
}



// CBodySegmentModifierView construction/destruction

CBodySegmentModifierView::CBodySegmentModifierView() noexcept
{
	// TODO: add construction code here
	m_pDC = 0;
	m_hRC = 0;

	m_Bound = false;

	m_CameraMode = PERS;

	m_Width = m_Height = 0;

	m_Fovy = 85.0f;
	m_Near = 0.1f;
	m_Far = 100.0f;

	m_ViewCenter = glm::vec3(0.0, 0.0, -m_Far);
}

CBodySegmentModifierView::~CBodySegmentModifierView()
{
	if (m_pDC != NULL)
		delete m_pDC;
}

/******************************************************************************************************/
/***										OPENGL SET UP											***/
/******************************************************************************************************/

void CBodySegmentModifierView::InitializeGL() {
	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);

	// other initialize routine can be added to following....
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_SMOOTH);
	// glEnable(GL_NORMALIZE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// glLineWidth(0.0001f);
	// glPointSize(0.0001f);
}


void CBodySegmentModifierView::SetupViewport(float w, float h) {
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	glViewport(0, 0, w, h);
	wglMakeCurrent(0, 0);
}


void CBodySegmentModifierView::SetupFrustum()
{
	// Projection Matrix
	m_ViewSize = -m_ViewCenter[2] * tan((m_Fovy * M_PI / 360.0f));

	switch (m_CameraMode)
	{
	case ORTHO:
		if (m_Width > m_Height)
			m_ProjectionMatrix = glm::ortho(-m_ViewSize * m_ViewRatio, m_ViewSize * m_ViewRatio, -m_ViewSize, m_ViewSize, -m_Far, m_Far);
		else
			m_ProjectionMatrix = glm::ortho(-m_ViewSize, m_ViewSize, -m_ViewSize * m_ViewRatio, m_ViewSize * m_ViewRatio, -m_Far, m_Far);
		break;

	case PERS:
	default:
		m_ProjectionMatrix = glm::perspective(glm::radians(m_Fovy), m_ViewRatio, m_Near, m_Far);
		break;
	}
	glLoadMatrixf(&m_ProjectionMatrix[0][0]);
	
}


void CBodySegmentModifierView::SetupOffset() {
	// View Matrix
	m_ViewMatrix = glm::lookAt(m_ViewCenter, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Model Matrix
	m_ModelMatrix = glm::mat4(1.0);

	// Model-View Matrix = View * Model
	m_ModelViewMatrix = m_ViewMatrix * m_ModelMatrix;

	glLoadMatrixf(&m_ModelViewMatrix[0][0]);
}


void CBodySegmentModifierView::UnProjection(float x, float y, float z, double *wx, double *wy, double *wz) {
	GLint viewport[4];
	GLdouble modelviewMat[16], projectionMat[16];
	float real_y;

	// Opengl Support Mode.
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	{
		// get viewport
		glGetIntegerv(GL_VIEWPORT, viewport);
		// get model view matrix.
		glGetDoublev(GL_MODELVIEW_MATRIX, modelviewMat);

		glGetDoublev(GL_PROJECTION_MATRIX, projectionMat);

		real_y = (float) viewport[3] - (float) y;

		gluUnProject(x, real_y, z, modelviewMat, projectionMat, viewport, wx, wy, wz);
	}
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
}


/******************************************************************************************************/
/***											RENDER												***/
/******************************************************************************************************/

void CBodySegmentModifierView::OnDraw(CDC* /*pDC*/) {
	CBodySegmentModifierDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
		// OpenGL 렌더링 모드로 진입한다.
	wglMakeCurrent(m_hDC, m_hRC);
	{

		// 칼라 버퍼를 초기화 한다.
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		SetupFrustum();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		RenderBackground();
		SetupOffset();

		if (pDoc->isHuman) {
			glPushMatrix();
			RenderScene();
			glPopMatrix();
		}

		if (m_Bound) {
			glPushMatrix();
			RenderBoudingBox(m_pDC);
			glPopMatrix();
		}

		glFinish();
		glFlush();

		SwapBuffers(m_hDC);
	}
	wglMakeCurrent(NULL, NULL);

}



void CBodySegmentModifierView::RenderBackground() {
	float dx, dy, dz;

	// 폭이 더 넓을 때, x(width)를 View Size * View Ratio
	if (m_Width > m_Height) {
		dx = m_ViewSize * m_ViewRatio;
		dy = m_ViewSize;
		dz = -m_Far;
	}
	else {
		dx = m_ViewSize;
		dy = m_ViewSize * m_ViewRatio;
		dz = -m_Far;
	}

	glBegin(GL_POLYGON);

	// Left Top
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(-dx, dy, dz);

	// Left Bottom
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-dx, -dy, dz);

	// Right Botton
	glVertex3f(dx, -dy, dz);


	// Right Top
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex3f(dx, dy, dz);

	glEnd();
}


void CBodySegmentModifierView::RenderScene() {
	CBodySegmentModifierDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->Human->m_VertBuf.empty()) {
		return;
	}

	glPushMatrix();

	glBegin(GL_TRIANGLES);

	std::vector<float> tmp = pDoc->Human->m_VertBuf["default"];

	for (int i = 0; i < tmp.size(); i += 11) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(tmp[i], tmp[i + 1], 0.0);
	}

	glEnd();
	glPopMatrix();
}


void CBodySegmentModifierView::RenderBoudingBox(CDC *pDC) {
	RECT box;
	box.left = box.right = box.top = box.bottom = 0;

	box.left = m_Start.x;
	box.top = m_Start.y;

	box.right = m_Curr.x;
	box.bottom = m_Curr.y;

	DrawFocusRect(m_hDC, &box);

	// pDC->DrawFocusRect(&box);

	float scale = 1;

	glPointSize(10.0f);
	glLineWidth(1.0f);

	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	glVertex3f(m_ModelStart[0] * scale, m_ModelStart[1] * scale, 0);
	glEnd();

	glBegin(GL_LINES);

	glVertex3f(m_ModelStart[0] * scale, m_ModelStart[1] * scale, 0.0f);

	glVertex3f(m_ModelEnd[0] * scale, m_ModelEnd[1] * scale, 0.0);
	glEnd();

	_cprintf("%f %f to %f %f\n", 
		m_ModelStart[0] * scale, 
		m_ModelStart[1] * scale,
		m_ModelEnd[0] * scale, 
		m_ModelEnd[1] * scale
	);
}

/******************************************************************************************************/
/***											CONTROL												***/
/******************************************************************************************************/

void CBodySegmentModifierView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

/*** 마우스를 움직여서 영역을 선택한다 ***/
void CBodySegmentModifierView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	// 마우스 안눌린 상태면 return
	if (!(nFlags & MK_LBUTTON))
		return;

	m_Curr = point;

	CView::OnMouseMove(nFlags, point);
}


/*** 마우스 왼쪽 버튼을 누른채로 영역을 선택한다  ***/
void CBodySegmentModifierView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	// m_StartPt = m_CurPt = m_StartPos = m_CurPos = point;
	m_Start = m_Curr = point;

	CView::OnLButtonDown(nFlags, point);
}


/*** 마우스 왼쪽 버튼을 뗄 때 선택 영역을 활성화한다  ***/
void CBodySegmentModifierView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	float scale = m_ViewSize * m_ViewRatio / 10.0f;
	double left, right, bottom, top, depth, useless;
	
	depth = 0.0;
	m_End = m_Curr = point;

	UnProjection((float)m_Start.x, (float)m_Start.y, (float)depth, &left, &top, &useless);
	UnProjection((float)m_End.x, (float)m_End.y, (float)depth, &right, &bottom, &useless);


	glm::vec4 modelStartPos = glm::inverse(m_ModelViewMatrix) * glm::inverse(m_ProjectionMatrix) * glm::vec4(left, top, useless, 1);
	glm::vec4 modelEndPos = glm::inverse(m_ModelViewMatrix) * glm::inverse(m_ProjectionMatrix) * glm::vec4(right, bottom, useless, 1);

	m_ModelStart = glm::vec3(-modelStartPos[0] * scale, modelStartPos[1] * scale, 0.0);
	m_ModelEnd = glm::vec3(-modelEndPos[0] * scale, modelEndPos[1] * scale, 0.0);

	_cprintf("model Start : %f %f\n", modelStartPos[0] * scale, modelStartPos[1] * scale);
	_cprintf("model End : %f %f\n\n", modelEndPos[0] * scale, modelEndPos[1] * scale);


	_cprintf("m_ViewSize : %f\n", m_ViewSize);
	_cprintf("m_ViewRatio: %f\n", m_ViewRatio);

	m_Start = m_End = m_Curr = CPoint(0, 0);
	// m_Bound = FALSE;

	Invalidate(false);
	CView::OnLButtonUp(nFlags, point);
}


BOOL CBodySegmentModifierView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta > 0) {
		m_ViewCenter[2] += m_ViewSize;
	}
	else {
		m_ViewCenter[2] -= m_ViewSize;
	}

	Invalidate(true);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CBodySegmentModifierView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_Width = cx;
	m_Height = cy;
	m_ViewRatio = (float) m_Width / (float) m_Height;
	SetupViewport(cx, cy);
}


void CBodySegmentModifierView::OnBound()
{
	// TODO: Add your command handler code here
	m_Bound = !m_Bound;
}


void CBodySegmentModifierView::OnUpdateBound(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_Bound) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}
