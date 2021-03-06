/*************************************************************************\

Copyright 2018 Zhejiang University.
All Rights Reserved.

Permission to use, copy, modify and distribute this software and its
documentation for educational, research and non-profit purposes, without
fee, and without a written agreement is hereby granted, provided that the
above copyright notice and the following three paragraphs appear in all
copies.

IN NO EVENT SHALL ZHEJIANG UNIVERSITY BE
LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE
USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF ZHEJIANG
UNIVERSITY HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

ZHEJIANG UNIVERSITY SPECIFICALLY DISCLAIM ANY
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE
PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND ZHEJIANG UNIVERSITY
HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT,
UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

The authors may be contacted via:

Mail:             College of Computer Science and Technology
Zhejiang Univeristy
Hangzhou, Zhejiang, China 310027

EMail:            tang_m@zju.edu.cn

\**************************************************************************/
// MoonVisView.cpp : CMoonVisView 类的实现
//

#include "stdafx.h"
#include "MoonVis.h"

#include "MoonVisDoc.h"
#include "MoonVisView.h"
#include "DlgSunSet.h"
#include "DlgGridSet.h"
#include "DlgWizzard.h"

#include "../CSM-src/main.h"
#include "../CSM-src/camera.h"
#include <fstream>
#include <iostream>
using namespace std;
#include <GL/wglext.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern char* depth_tex_filename;
extern char* inf_filename;
extern wchar_t* explorer_filename;
extern wchar_t* explorer_animation_filename;

#define ID_TIMER_ANIMATION 1
#define ID_TIMER_KEY 2
#define ID_TIMER_DRAW 3

// CMoonVisView

IMPLEMENT_DYNCREATE(CMoonVisView, CView)

BEGIN_MESSAGE_MAP(CMoonVisView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SET_SUN, &CMoonVisView::OnSetSun)
	ON_COMMAND(ID_SET_GRID, &CMoonVisView::OnSetGrid)
	ON_COMMAND(ID_ANM_EDIT, &CMoonVisView::OnAnmEdit)
	ON_COMMAND(ID_LOAD_MODEL, &CMoonVisView::OnLoadModel)
	ON_COMMAND(ID_LOAD_TERRAIN, &CMoonVisView::OnLoadTerrain)
	ON_COMMAND(ID_CAMERA_ADD, &CMoonVisView::OnCameraAdd)
	ON_COMMAND(ID_CAMERA_LIST, &CMoonVisView::OnCameraList)
	ON_COMMAND(ID_POINT_LIST, &CMoonVisView::OnPointList)

	ON_COMMAND(ID_RELOAD_ANIMATION, &CMoonVisView::OnReloadAnimation)
	ON_UPDATE_COMMAND_UI(ID_RELOAD_ANIMATION, &CMoonVisView::OnUpdateReloadAnimation)
	ON_COMMAND(ID_VIEW_ANIMATION, &CMoonVisView::OnViewAnimation)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ANIMATION, &CMoonVisView::OnUpdateViewAnimation)
	ON_COMMAND(ID_PAUSE_ANIMATION, &CMoonVisView::OnPauseAnimation)
	ON_UPDATE_COMMAND_UI(ID_PAUSE_ANIMATION, &CMoonVisView::OnUpdatePauseAnimation)
	ON_COMMAND(ID_RESTORE_ANIMATION, &CMoonVisView::OnRestoreAnimation)

	ON_COMMAND(ID_AVI_BEGIN, &CMoonVisView::OnAviBegin)
	ON_COMMAND(ID_AVI_END, &CMoonVisView::OnAviEnd)
	ON_UPDATE_COMMAND_UI(ID_AVI_BEGIN, &CMoonVisView::OnUpdateAviBegin)
	ON_UPDATE_COMMAND_UI(ID_AVI_END, &CMoonVisView::OnUpdateAviEnd)
	ON_COMMAND(ID_BMP_CAPTURE, &CMoonVisView::OnBmpCapture)

	ON_COMMAND(ID_SUN_VIEW, &CMoonVisView::OnShadowRatio)

	ON_COMMAND(ID_VIEW_WINGS, &CMoonVisView::OnViewWings)
	ON_COMMAND(ID_VIEW_CAMERA, &CMoonVisView::OnViewCamera)
	ON_COMMAND(ID_VIEW_FOG, &CMoonVisView::OnViewFog)
	ON_COMMAND(ID_VIEW_WIREFRAME, &CMoonVisView::OnViewWireframe)
	ON_COMMAND(ID_VIEW_DEPTHMAP, &CMoonVisView::OnViewDepthMap)
	ON_COMMAND(ID_VIEW_FPS, &CMoonVisView::OnViewFPS)
	ON_COMMAND(ID_VIEW_GRID, &CMoonVisView::OnViewGrid)

	ON_UPDATE_COMMAND_UI(ID_VIEW_WINGS, &CMoonVisView::OnUpdateViewWings)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAMERA, &CMoonVisView::OnUpdateViewCamera)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FOG, &CMoonVisView::OnUpdateViewFog)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FPS, &CMoonVisView::OnUpdateViewFPS)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WIREFRAME, &CMoonVisView::OnUpdateViewWireframe)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DEPTHMAP, &CMoonVisView::OnUpdateViewDepthMap)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRID, &CMoonVisView::OnUpdateViewGrid)

END_MESSAGE_MAP()

// CMoonVisView 构造/析构

CMoonVisView::CMoonVisView()
{
	// TODO: 在此处添加构造代码

}

CMoonVisView::~CMoonVisView()
{
}

BOOL CMoonVisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

// CMoonVisView 绘制
float cfps = 0;
extern bool show_fps;
int diffLevel;
int totalScore;
unsigned int totalFound;

void CMoonVisView::OnDraw(CDC* /*pDC*/)
{
	static DWORD LastFPSTime = GetTickCount(), LastFrameTime = LastFPSTime;
	static int FPS = 0;

	DWORD Time = GetTickCount();

	float FrameTime = (Time - LastFrameTime) * 0.001f;

	LastFrameTime = Time;

	if (Time - LastFPSTime > 1000)
	{
		cfps = FPS;
		LastFPSTime = Time;
		FPS = 0;
	}
	else
	{
		FPS++;
	}

	CMoonVisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	display();
/*	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2i(0,  1);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2i(-1, -1);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2i(1, -1);
	glEnd();
	glFlush();*/

	SwapBuffers(m_pDC->GetSafeHdc());
	if (m_bIsRecording)
	{
		m_cAviRec.Snap();
	}
	
	// with this, fps will be correct, but much slower than without it ...
	if (show_fps)
		Invalidate(FALSE);
}

// CMoonView 消息处理程序
int CMoonVisView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here

	Init();
	initGL(lpCreateStruct->cx, lpCreateStruct->cy);

	return 0;
}

void CMoonVisView::OnDestroy() 
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	HGLRC hrc;

	if (m_nTimerID)  KillTimer(m_nTimerID);

	hrc = ::wglGetCurrentContext();
	::wglMakeCurrent(NULL, NULL);

	if (hrc)  ::wglDeleteContext(hrc);

	CPalette palDefault; // Select our palette out of the dc 
	palDefault.CreateStockObject(DEFAULT_PALETTE);
	m_pDC->SelectPalette(&palDefault, FALSE);

	if (m_pDC) delete m_pDC;

}

BOOL wglChoosePixelFormatARB(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats){
		typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
		static PFNWGLCHOOSEPIXELFORMATARBPROC pfnChoosePixelFormatARB = 0;

		if(!pfnChoosePixelFormatARB)
			pfnChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));

		return pfnChoosePixelFormatARB(hdc, piAttribIList, pfAttribFList, nMaxFormats, piFormats, nNumFormats);
	}

int chooseMultisampleFormat(HDC dc, int * formatNum) {  

	int pixelFormat;
	int valid;
	unsigned int numFormats;
	float fAttributes[] = {
		0.0f,
		0.0f
	};
	int iAttributes[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 24,
		WGL_ALPHA_BITS_ARB, 8, 
		WGL_DEPTH_BITS_ARB, 16,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 8,
		0, 0
	};

	// get our pixel format
	// first we check to see if we can get a pixel format for 8 samples
	valid = wglChoosePixelFormatARB(dc, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
	if (valid && numFormats >= 1) {
		*formatNum = pixelFormat;	
		return 1;
	}

	// our pixel format with 8 samples failed, test for 6 samples
	iAttributes[19] = 6;
	valid = wglChoosePixelFormatARB(dc, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
	if (valid && numFormats >= 1) {
		*formatNum = pixelFormat;	
		return 1;
	}

	// our pixel format with 6 samples failed, test for 4 samples
	iAttributes[19] = 4;
	valid = wglChoosePixelFormatARB(dc, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
	if (valid && numFormats >= 1) {
		*formatNum = pixelFormat;	
		return 1;
	}

	// our pixel format with 4 samples failed, test for 2 samples
	iAttributes[19] = 2;
	valid = wglChoosePixelFormatARB(dc, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
	if (valid && numFormats >= 1) {
		*formatNum = pixelFormat;	
		return 1;
	}
	
	cout << "No suitable multisample pixel format" << endl;
	return 0;
}

int vgsetPixelFormat(HDC dc, PIXELFORMATDESCRIPTOR * desc )
{
	int index = ChoosePixelFormat(dc, desc);
	if (index == 0)
		return 0;

	SetPixelFormat(dc, index, desc);
	return 1;
}

static PFNWGLGETEXTENSIONSSTRINGARBPROC _wglewGetExtensionsStringARB = NULL;
static PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglewGetExtensionsStringEXT = NULL;

void loadProcs()
{
	_wglewGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	_wglewGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");
}


int wglisExtensionSupported(const char *extension, HDC dc)
{
	const size_t extlen = strlen(extension);
	const char *supported = NULL;
	const char* p = NULL;
	// Try To Use wglGetExtensionStringARB On Current DC, If Possible

	loadProcs();

	if (_wglewGetExtensionsStringARB)
		supported = _wglewGetExtensionsStringARB(dc);

	if (supported == NULL && _wglewGetExtensionsStringEXT)
		supported = _wglewGetExtensionsStringEXT();

	// If That Failed, Try Standard Opengl Extensions String
	if (supported == NULL)
		supported = (char*)glGetString(GL_EXTENSIONS);

	// If That Failed Too, Must Be No Extensions Supported
	if (supported == NULL)
		return 0;

	printf("\n\nWGL extensions: \n%s\n\n", supported);
	
	// Begin Examination At Start Of String, Increment By 1 On False Match
	for (p = supported; ; p++)
	{
		// Advance p Up To The Next Possible Match
		p = strstr(p, extension);

		if (p == NULL)
			return 0;						// No Match

		// Make Sure That Match Is At The Start Of The String Or That
		// The Previous Char Is A Space, Or Else We Could Accidentally
		// Match "wglFunkywglExtension" With "wglExtension"

		// Also, Make Sure That The Following Character Is Space Or NULL
		// Or Else "wglExtensionTwo" Might Match "wglExtension"
		if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
			return 1;						// Match
	}
}

int checkMultisampling(PIXELFORMATDESCRIPTOR * desc, int * formatNum)
{
	WNDCLASS wc;
	HINSTANCE hInstance;
	HWND hWnd = 0;
	HDC dc = 0;
	HGLRC rc = 0;
	int index;
	int result = 0;
	// register window class

  /* register window class */
	memset(&wc, 0, sizeof(WNDCLASS));

	wc.style =/* CS_OWNDC | */CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"GLEW";;
	if (0 == RegisterClass(&wc)) return 0;

	hInstance = GetModuleHandle(NULL);

  /* create window */
	hWnd = CreateWindow(L"GLEW", L"GLEW", 0, CW_USEDEFAULT, CW_USEDEFAULT, 
                          CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, 
                          GetModuleHandle(NULL), NULL);
	if (!hWnd)
		goto FAILED;

	dc = GetDC(hWnd);

	if (!vgsetPixelFormat(dc, desc))
		goto FAILED;

	rc = wglCreateContext(dc);
	if(!rc) 
		goto FAILED;

	/* Make the context current */
	if(!wglMakeCurrent(dc, rc)) goto FAILED;

	// see if the string exists in WGL
	if (!wglisExtensionSupported("WGL_ARB_multisample",dc)) {
		cout << "WGL_ARB_multisample extension not supported" << endl;
		return 0;
	}

	result = chooseMultisampleFormat(dc, formatNum);

FAILED:
  /* do necessary cleanup */
	if(rc) wglDeleteContext(rc);
	if (hWnd) {
		if(dc) 
			ReleaseDC(hWnd, dc);
		DestroyWindow(hWnd);
	}

	return result;
}

BOOL CMoonVisView::Init()
{
	//m_nTimerID = SetTimer(1, 100, NULL);
	m_pDC= new CClientDC(this);
	HDC hDC = m_pDC->GetSafeHdc();

	PIXELFORMATDESCRIPTOR pixelDesc={
		sizeof(PIXELFORMATDESCRIPTOR),			//size of pdf
		1,										//edition NO.
		PFD_DRAW_TO_WINDOW|						//support drawing in window
		PFD_SUPPORT_OPENGL|						//support OpenGL
		PFD_DOUBLEBUFFER,						//double-buffer mode
		PFD_TYPE_RGBA,							//RGBA color mode
		24,										//24bit color depth
		0,0,0,0,0,0,							//ignore color bit
		0,										//no none-transparent buffer
		0,										//ignore transform bit
		0,										//no progression bit
		0,0,0,0,								//ignore progression bit
		32,										//32bit depth buffer
		0,										//no template buffer
		0,										//no accessorial buffer
		PFD_MAIN_PLANE,							//main plane
		0,										//remain
		0,0,0									//ignore plane£¬visibility and destroy mask
	};
	int index = ChoosePixelFormat(hDC,&pixelDesc);

	if (checkMultisampling(&pixelDesc, &index)) {
		cout << "multisample for new window is spported" << endl;
	}

	if(index == 0) // Choose default
	{
		index = 1;
		if(DescribePixelFormat(hDC,index,
			sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
			return FALSE;
	}	
	if(!SetPixelFormat(hDC,index,&pixelDesc))
		return FALSE;

	HGLRC hrc = wglCreateContext(hDC);
	if(hrc==NULL) return FALSE;
	if(wglMakeCurrent(hDC,hrc)==FALSE)
		return FALSE;
	return TRUE;
}

void CMoonVisView::EnableRecording(BOOL flag, const wchar_t *fname/* = NULL*/)
{
	if (m_bIsRecording == flag)
		return;

	if (flag) {
		if (m_cAviRec.Begin(15, this, fname) == false)
		{
			m_bIsRecording = FALSE;
			return;
		}
		setAnmFrameRate(15);
	} else	{
		m_cAviRec.End();
		setAnmFrameRate(0);
	}

	m_bIsRecording = flag;
}

BOOL CMoonVisView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMoonVisView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO:
	reshape(cx, cy);
}

void CMoonVisView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case ID_TIMER_DRAW:
		{
			Invalidate();
		}
		break;
	case ID_TIMER_ANIMATION:
		{
			long curTimeTag = clock();
			float dt = (float)(curTimeTag - m_anmTimeTag) / 1000.0f;
			m_anmTimeTag = curTimeTag;
			
			if (updateAnimation(dt))
			{
				toggleAnimation();
				KillTimer(ID_TIMER_ANIMATION);
			}
			Invalidate();
		}
		break;
	case ID_TIMER_KEY:
		{
			CMDIFrameWnd *pFrame = 
				(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;

			// Get the active MDI child window.
			CMDIChildWnd *pChild = 
				(CMDIChildWnd *) pFrame->GetActiveFrame();

			// or CMDIChildWnd *pChild = pFrame->MDIGetActive();

			// Get the active view attached to the active MDI child
			// window.
			CMoonVisView *pView = (CMoonVisView *) pChild->GetActiveView();

			if (this == pView && AfxGetApp()->m_pMainWnd == GetActiveWindow())
			{
				//keys();
			}
		}
		break;
	default:
		break;
	}

	CView::OnTimer(nIDEvent);
}

void CMoonVisView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
}

// CMoonVisView 打印

BOOL CMoonVisView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMoonVisView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMoonVisView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMoonVisView 诊断

#ifdef _DEBUG
void CMoonVisView::AssertValid() const
{
	CView::AssertValid();
}

void CMoonVisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMoonVisDoc* CMoonVisView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMoonVisDoc)));
	return (CMoonVisDoc*)m_pDocument;
}
#endif //_DEBUG


// CMoonVisView 消息处理程序

void CMoonVisView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (nFlags & MK_LBUTTON)
	{
		motion(point.x, point.y);
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}

void CMoonVisView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	keys(nChar, 0, 0);
	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMoonVisView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	mouse(0, 0, point.x, point.y);
	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}

void CMoonVisView::OnSetSun()
{
	CDlgSunSet dlg(this, light_dir[0], light_dir[1], light_dir[2]);
	dlg.DoModal();
}

void CMoonVisView::OnAnmEdit()
{
}

void CMoonVisView::OnSetGrid()
{
	CDlgGridSet dlg(this);
	dlg.DoModal();
}

void CMoonVisView::OnLoadModel()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, 
		_T("WRL文件(*.wrl)|*.wrl||"));
	if (IDOK == dlg.DoModal())
	{
		CString buff = dlg.GetPathName();
		if (NULL != explorer_filename)
			delete[] explorer_filename;
		explorer_filename = new wchar_t[buff.GetLength()+1];
		memcpy(explorer_filename, buff.GetBuffer(), buff.GetLength()*sizeof(wchar_t));
		explorer_filename[buff.GetLength()] = _T('\0');

		buff = buff.Left(buff.GetLength() - 3);
		buff += _T("anm");
		if (NULL != explorer_animation_filename)
			delete[] explorer_animation_filename;
		explorer_animation_filename = new wchar_t[buff.GetLength()+1];
		memcpy(explorer_animation_filename, buff.GetBuffer(), buff.GetLength()*sizeof(wchar_t));
		explorer_animation_filename[buff.GetLength()] = _T('\0');
	}
	ofstream file;
	file.open("../log.txt", ios::app);
	file << "in...initScenewrl" << endl;
	initScene();
	file << "out...initScenewrl" << endl;
}

void CMoonVisView::OnLoadTerrain()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, 
		_T("PNG文件(*.png)|*.png||"));
	if (IDOK == dlg.DoModal())
	{
		CString buff = dlg.GetPathName();
		if (NULL != depth_tex_filename)
			delete[] depth_tex_filename;
		WideCharToMultiByte_M(buff, &depth_tex_filename);
		buff = buff.Left(buff.GetLength() - 3);
		buff += _T("inf");
		if (NULL != inf_filename)
			delete[] inf_filename;
		WideCharToMultiByte_M(buff, &inf_filename);
	}
	AfxMessageBox(_T("in...initScene"));
	initScene();
	AfxMessageBox(_T("out...initScene"));
}

void CMoonVisView::OnCameraAdd()
{
	addCamera();
}

void CMoonVisView::OnCameraList()
{
}

void CMoonVisView::OnPointList()
{
}

void CMoonVisView::OnViewFog()
{
	toggleShowFog();
	Invalidate();
}

void CMoonVisView::OnViewCamera()
{
	toggleShowCamera();
	Invalidate();
}

void CMoonVisView::OnViewWings()
{
	toggleShowWings();
	Invalidate();
}

void CMoonVisView::OnViewGrid()
{
	toggleShowGrid();
	Invalidate();
}

void CMoonVisView::OnReloadAnimation()
{
	reloadAnimation();
	resetAnimation();
}

void CMoonVisView::OnUpdateReloadAnimation( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(!isAnimating());
	pCmdUI->SetCheck(FALSE);
}

void CMoonVisView::OnViewAnimation()
{
	if (1) {
		toggleAnimation();
		if (isAnimationPaused())
		{
			togglePauseAnimation();
		}
		KillTimer(ID_TIMER_ANIMATION);
		if (isAnimating())
		{
			resetAnimation();
			m_anmTimeTag = clock();
			SetTimer(ID_TIMER_ANIMATION, 1, NULL);
		}
		else
		{
			resetAnimation();
			Invalidate();
		}
	}
	else {
		flushAnimation();
		Invalidate();
	}
}

void CMoonVisView::OnUpdateViewAnimation( CCmdUI* pCmdUI )
{
	pCmdUI->SetCheck(isAnimating());
}

void CMoonVisView::OnPauseAnimation()
{
	togglePauseAnimation();
	KillTimer(ID_TIMER_ANIMATION);
	if (isAnimating() && !isAnimationPaused())
	{
		m_anmTimeTag = clock();
		SetTimer(ID_TIMER_ANIMATION, 1, NULL);
	}
}

void CMoonVisView::OnUpdatePauseAnimation( CCmdUI* pCmdUI )
{
	pCmdUI->Enable(isAnimating());
	pCmdUI->SetCheck(isAnimationPaused());
}

void CMoonVisView::OnRestoreAnimation()
{
	if (!isAnimating())
	{
		resetAnimation();
		Invalidate();
	}
}

void CMoonVisView::OnAviBegin()
{
	CFileDialog dlgFile(FALSE, 
		L"avi",NULL,OFN_OVERWRITEPROMPT,
		L"视频文件 (*.avi)|*.avi||");

	if (dlgFile.DoModal() == IDOK) {
		CString file_name = dlgFile.GetPathName();
		EnableRecording(TRUE, file_name.GetBuffer());
	}
}

void CMoonVisView::OnAviEnd()
{
	EnableRecording(false);
}

void CMoonVisView::OnUpdateAviBegin(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bIsRecording);
	pCmdUI->SetCheck(m_bIsRecording);
}

void CMoonVisView::OnUpdateViewCamera(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(getShowCamera());
}

void CMoonVisView::OnUpdateViewWings(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(getShowWings());
}

void CMoonVisView::OnUpdateViewGrid(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(getShowGrid());
}

void CMoonVisView::OnUpdateViewFog(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(getShowFog());
}

void CMoonVisView::OnUpdateViewFPS(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(getShowFPS());
}

void CMoonVisView::OnUpdateViewWireframe(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(getShowWireframe());
}

void CMoonVisView::OnUpdateViewDepthMap(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(getShowDepthMap());
}

void CMoonVisView::OnUpdateAviEnd(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsRecording);
	pCmdUI->SetCheck(false);
}

void CMoonVisView::OnShadowRatio()
{
	float ratio, angle;

	computeShadowRatio(ratio, angle);

	CString buffer;

	buffer.Format(L"照射角度：%.2f度\n阴影比例：%.2f%%", (M_PI*0.5-angle)*180/M_PI, ratio*100.f);

	AfxMessageBox(buffer);
}

void CMoonVisView::OnBmpCapture()
{
	CFileDialog dlgFile(FALSE, 
		L"bmp",NULL,OFN_OVERWRITEPROMPT,
		L"位图文件 (*.bmp)|*.bmp||");

	if (dlgFile.DoModal() == IDOK) {
		CString file_name = dlgFile.GetPathName();

		CRect rect;
		GetClientRect(rect);
		int width  = rect.Width();
		int height = rect.Height();
		Invalidate();
		m_cBmpCap.Capture(0, 0, width, height, file_name.GetBuffer());
	}
}

void CMoonVisView::OnViewWireframe()
{
	toggleShowWireframe();
	Invalidate();
}

void CMoonVisView::OnViewDepthMap()
{
	toggleShowDepthMap();
	Invalidate();
}

void CMoonVisView::OnViewFPS()
{
	toggleShowFPS();
	Invalidate();
}

void CMoonVisView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	initScene();

	m_bIsRecording = FALSE;

	//SetTimer(ID_TIMER_DRAW, 16, NULL);
	//SetTimer(ID_TIMER_KEY, 1, NULL);
}


