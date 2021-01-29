#include "Renderer.h"


bool CWindow::m_bMouseDrag = false;
CWindow * CWindow::m_wWindow = nullptr;
void(*CWindow::ButtonOut)(int msg) = nullptr;
void(*CWindow::KeyCall)(CWindow * window, unsigned char key) = nullptr;
void(*CWindow::MouseCall)(CWindow *, float x, float y, unsigned char status, unsigned char mouse) = nullptr;
void(*CWindow::ResizeCall)(int newWidth, int newHeigth) = nullptr;

CWindow::CWindow(unsigned int width, unsigned int height, const char * windowname, unsigned long winmode)
{

	this->m_iHeigth = height;
	this->m_iWidth = width;

	m_bQuitstate = false;

	m_hInstance = GetModuleHandle(NULL);

	m_wWndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	m_wWndcls.hInstance = m_hInstance;
	m_wWndcls.lpfnWndProc = this->WndProc;
	m_wWndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wWndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	m_wWndcls.lpszClassName = windowname;
	m_wWndcls.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClass(&m_wWndcls);

	this->m_hWinhwn = CreateWindow(windowname, windowname, winmode, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, m_hInstance, nullptr);
	CWindow::m_wWindow = this;
	ShowWindow(m_hWinhwn, SW_SHOW);
}





CWindow::~CWindow()
{
	this->DeleteOpenGLContext();
}

void CWindow::ShowMyWindow()
{
	ShowWindow(this->m_hWinhwn, SW_SHOW);
}

void CWindow::HideWindow()
{
	ShowWindow(this->m_hWinhwn, SW_HIDE);
}

void CWindow::CreateOpenGLContext()
{
	m_hdc = GetDC(this->m_hWinhwn);
	m_pxlform = {
		sizeof(PIXELFORMATDESCRIPTOR),              // Size Of This Pixel Format Descriptor
		1,                                          // Version Number
		PFD_DRAW_TO_WINDOW |                        // Format Must Support CWindow
		PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,                           // Must Support Double Buffering
		PFD_TYPE_RGBA,                              // Request An RGBA Format
		32,                                         // Select Our Color Depth
		0, 0, 0, 0, 0, 0,                           // Color Bits Ignored
		8,                                          // No Alpha Buffer
		0,                                          // Shift Bit Ignored
		0,                                          // No Accumulation Buffer
		0, 0, 0, 0,                                 // Accumulation Bits Ignored
		16,                                         // 16Bit Z-Buffer (Depth Buffer)  
		0,                                          // No Stencil Buffer
		0,                                          // No Auxiliary Buffer
		PFD_MAIN_PLANE,                             // Main Drawing Layer
		0,                                          // Reserved
		0, 0, 0                                     // Layer Masks Ignored
	};


	m_iPxlFormat = ChoosePixelFormat(m_hdc, &m_pxlform);
	SetPixelFormat(m_hdc, m_iPxlFormat, &m_pxlform);
	m_hglrc = wglCreateContext(m_hdc);
	
	wglMakeCurrent(m_hdc, m_hglrc);
	
	
}

void CWindow::DeleteOpenGLContext()
{
	wglMakeCurrent(0, 0);
	wglDeleteContext(m_hglrc);
	ReleaseDC(CWindow::m_hWinhwn, m_hdc);
}




bool CWindow::WindowShouldClose()
{
	return m_bQuitstate;
}



void CWindow::SwapWindowBuffers()
{
	SwapBuffers(m_hdc);
}
void CWindow::SetKeyCallFunc(void(*keycallback)(CWindow *,unsigned char))
{
	KeyCall = keycallback;
}
void CWindow::CreateButton(unsigned int width, unsigned int height, int x, int y, const char * buttext, int m_msg)
{
	m_vButtonmsg.push_back(m_msg);
	m_vChildwindow.push_back(CreateWindow("Button", buttext, WS_CHILD | WS_VISIBLE | WS_BORDER, x, y, width, height, this->m_hWinhwn, (HMENU)m_msg, nullptr, nullptr));
}

void CWindow::CreateStaticTextBox(unsigned int width, unsigned int height, int x, int y, const char * buttext)
{

	m_vChildwindow.push_back(CreateWindow("Static", buttext, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, x, y, width, height, this->m_hWinhwn, nullptr, nullptr, nullptr));
}

void CWindow::CreateInputTextBox(unsigned int width, unsigned int height, int x, int y, const char * text)
{
	m_vChildwindow.push_back(CreateWindowA("Edit", text, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_WANTRETURN, x, y, width, height, m_hWinhwn, nullptr, NULL, nullptr));
}

void CWindow::SetButtonCallFunc(void(*buttnout)(int msg))
{
	m_wWindow->ButtonOut = buttnout;
}

void CWindow::SetResizeWindowCallFunc(void(*resizecall)(int, int))
{
	ResizeCall = resizecall;
}

void CWindow::SetMouseCallFunc(void(*mcall)(CWindow *, float x, float y, unsigned char, unsigned char))
{
	CWindow::MouseCall = mcall;
}

const int CWindow::GetButtonMessage()
{
	return m_ButtonMessage;
}

void CWindow::GetTextFromTextBox(unsigned int textboxid, size_t buffsize, char * buffer)
{
	if (m_vChildwindow.size() > 0 && textboxid < m_vChildwindow.size())
		GetWindowText(m_vChildwindow[textboxid], buffer, buffsize);
}

void CWindow::SetTextInTextBox(unsigned int textboxid, const char * text)
{
	if (m_vChildwindow.size() > 0 && textboxid < m_vChildwindow.size())
		SetWindowText(m_vChildwindow[textboxid], text);
}

size_t CWindow::GetTextLengthFromTextBox(unsigned int textboxid)
{
	if (m_vChildwindow.size() > 0 && textboxid < m_vChildwindow.size())
		return GetWindowTextLength(m_vChildwindow[textboxid]);
	else
		return 0;

}

void CWindow::GetWindowSize(int * width, int * heigth)
{
	if (width != nullptr && heigth != nullptr)
	{
		*width = m_iWidth;
		*heigth = m_iHeigth;
	}
}

void CWindow::SetWindowTitle(const char * title)
{
	SetWindowText(CWindow::m_hWinhwn, title);
}


//non-blocking
void CWindow::ProcessMessage()
{
	
	PeekMessage(&m_msg, this->m_hWinhwn, 0, 0, PM_REMOVE);
	TranslateMessage(&m_msg);
	DispatchMessage(&m_msg);
		
	if (m_msg.message == WM_QUIT)
		m_wWindow->m_bQuitstate = true;
}


//with blocking
void CWindow::ProcessMessageB()
{
	GetMessage(&m_msg,m_hWinhwn, 0, 0);
	TranslateMessage(&m_msg);
	DispatchMessage(&m_msg);
}









LRESULT CALLBACK CWindow::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	

	switch (msg)
	{

	case WM_CREATE:

		break;

	case WM_QUIT:
		
		break;
	case WM_SIZE:
	{
		if (m_wWindow != nullptr)
		{
			m_wWindow->m_iWidth = LOWORD(lp);
			m_wWindow->m_iHeigth = HIWORD(lp);
			if (m_wWindow->ResizeCall != nullptr)ResizeCall(m_wWindow->m_iWidth, m_wWindow->m_iHeigth);
		}

	}
	break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		m_wWindow->m_bQuitstate = true;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	

	case WM_KEYDOWN:
		if (m_wWindow->KeyCall != nullptr)
			m_wWindow->KeyCall(m_wWindow, (unsigned char)wp);
		break;
	case WM_KEYUP:
		if(m_wWindow->KeyCall!=nullptr)
			m_wWindow->KeyCall(m_wWindow, (unsigned char)wp);
		break;

	case WM_COMMAND:
		
		for (int i = 0; i < m_wWindow->m_vButtonmsg.size(); i++)
		{
			if (m_wWindow->m_vButtonmsg[i] == wp && m_wWindow->ButtonOut != nullptr)
			{
				m_wWindow->ButtonOut(m_wWindow->m_vButtonmsg[i]);
				
			}
		}
		m_wWindow->m_ButtonMessage = wp;

		break;
	
	
		
	case WM_LBUTTONDOWN:
	{
		
		POINT pt = { LOWORD(lp), HIWORD(lp) };
		if (DragDetect(hwnd, pt))
		{
			m_wWindow->m_bMouseDrag = true;
		}

		if (MouseCall != nullptr)
		{
			float fx = pt.x + (20 * (float)pt.x / m_wWindow->m_iWidth);
			float fy = pt.y + (40 * (float)pt.y / m_wWindow->m_iHeigth);


			if (m_wWindow->m_bMouseDrag)MouseCall(m_wWindow, fx , fy, MOUSE_ON_HELD, RENDERER_MOUSE_LEFT);
			else MouseCall(m_wWindow, fx, fy, MOUSE_ON_CLICK, RENDERER_MOUSE_LEFT);
		}

	}
	
	case WM_MOUSEMOVE:
		if (m_wWindow->m_bMouseDrag)
		{
			POINT pt = { LOWORD(lp), HIWORD(lp) };
			float fx = pt.x + (20 * (float)pt.x / m_wWindow->m_iWidth);
			float fy = pt.y + (40 * (float)pt.y / m_wWindow->m_iHeigth);

			if (MouseCall != nullptr)
			MouseCall(m_wWindow, fx, fy, MOUSE_ON_HELD, RENDERER_MOUSE_LEFT | RENDERER_MOUSE_RIGTH);

		}
		break;
	case WM_LBUTTONUP:
	{
		if (m_wWindow->m_bMouseDrag)m_wWindow->m_bMouseDrag = false;

		if (MouseCall != nullptr) 
		{ 
			POINT pt = { LOWORD(lp), HIWORD(lp) };
			float fx = pt.x + (20 * (float)pt.x / m_wWindow->m_iWidth);
			float fy = pt.y + (40 * (float)pt.y / m_wWindow->m_iHeigth);
			MouseCall(m_wWindow, fx, fy, MOUSE_ON_RELEASED, RENDERER_MOUSE_LEFT);
		}
	}
		break;
	case WM_RBUTTONDOWN:
	{
		
		POINT pt = { LOWORD(lp), HIWORD(lp) };
		if (DragDetect(hwnd, pt))
		{
			m_wWindow->m_bMouseDrag = true;
		}

		if (MouseCall != nullptr)
		{
			float fx = pt.x + (20 * (float)pt.x / m_wWindow->m_iWidth);
			float fy = pt.y + (40 * (float)pt.y / m_wWindow->m_iHeigth);
			if (m_wWindow->m_bMouseDrag)MouseCall(m_wWindow, fx, fy, MOUSE_ON_HELD, RENDERER_MOUSE_RIGTH);
			else MouseCall(m_wWindow, fx, fy, MOUSE_ON_CLICK, RENDERER_MOUSE_RIGTH);
		}

	}
	case WM_RBUTTONUP:
	{
		if (m_wWindow->m_bMouseDrag)m_wWindow->m_bMouseDrag = false;

		if (MouseCall != nullptr)
		{
			POINT pt = { LOWORD(lp), HIWORD(lp) };
			float fx = pt.x + (20 * (float)pt.x / m_wWindow->m_iWidth);
			float fy = pt.y + (40 * (float)pt.y / m_wWindow->m_iHeigth);
			MouseCall(m_wWindow, fx, fy, MOUSE_ON_RELEASED, RENDERER_MOUSE_RIGTH);
		}
	}

	break;
	default:
		if(m_wWindow != nullptr)
			m_wWindow->m_ButtonMessage = -1;
		return DefWindowProc(hwnd, msg, wp, lp);
		break;
	}



	return DefWindowProc(hwnd, msg, wp, lp);
}
