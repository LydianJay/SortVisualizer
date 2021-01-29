
#ifndef RENDER_SYS_H
#define RENDER_SYS_H
#include <Windows.h>
#define MOUSE_ON_CLICK 1
#define MOUSE_ON_HELD 2
#define MOUSE_ON_RELEASED 3

#define RENDERER_MOUSE_LEFT 0b01
#define RENDERER_MOUSE_RIGTH 0b1

#define WINDOW_NO_RESIZE WS_VISIBLE | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU
#define WINDOW_DEFAULT WS_VISIBLE | WS_OVERLAPPEDWINDOW

#define KEY_PRESS 1
#define KEY_RELEASED 2

#ifdef _DEBUG
	#include <iostream>
#endif // _DEBUG

#include <vector>


#include <gl\GL.h>
#include <gl\GLU.h>

#if defined (_WIN32) && defined(__cplusplus)
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glu32.lib")
#else
#error "For C++ Windows only!!!"
#endif


/*
-------LOG-----------
	Dec-23-2020
		-Added Mouse support
	Dec-27-2020
		-Remove some mouse functions
		-Remove some mouse related variables
		-Fix wrong mouse coordinates
		-Remove some debugging variables and functions
	January-5-2020
		-Added a blocking process message
		-Minor changes to function calls
		-Added GetButtonMessage function
		-Added Get text function and Get text length function
		-Added set text function
				*NOTE-TO-SELF: ButtonCall must be remove
	January-6-2020
		-Window is has a resizeable option
		-Added GetWindowSize function
	January-27-2020
		-Window title function

--------END----------
*/


class CWindow
{
private:
	
	HINSTANCE m_hInstance = nullptr;
	WNDCLASS m_wWndcls = {};
	bool m_bQuitstate = false;
	HWND m_hWinhwn = nullptr;
	
	static CWindow * m_wWindow;
	std::vector<int> m_vButtonmsg;
	


	PIXELFORMATDESCRIPTOR m_pxlform = { 0 };
	HDC m_hdc;
	int m_iPxlFormat;
	HGLRC m_hglrc;
	static bool m_bMouseDrag;
	MSG m_msg;
	std::vector<HWND> m_vChildwindow;
	unsigned int m_iWidth, m_iHeigth;
	int m_ButtonMessage = -1;
public:
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	static void(*KeyCall)(CWindow * window,unsigned char key);
	static void(*MouseCall)(CWindow *, float x, float y, unsigned char status, unsigned char mouse);
	static void(*ButtonOut)(int msg);
	static void(*ResizeCall)(int newWidth, int newHeigth);

public:
	
	CWindow(unsigned int width, unsigned int height, const char * windowname, unsigned long winmode);
	CWindow(const CWindow&) = delete;
	~CWindow();
	
	
	/*----------for openGL-----------*/
	void CreateOpenGLContext();
	void DeleteOpenGLContext();
	/*-------------------------------*/


	/*---------Win32 windowing functions-------------*/


	void ShowMyWindow();
	void HideWindow();

	void ProcessMessage();
	void ProcessMessageB();
	
	bool WindowShouldClose();
	void SwapWindowBuffers();
	
	void SetKeyCallFunc(void(*keycallback)(CWindow *,unsigned char));
	void SetButtonCallFunc(void(*buttonout)(int msg));
	void SetResizeWindowCallFunc(void(*resizecall)(int, int));
	void SetMouseCallFunc(void(*mcall) (CWindow *, float x, float y, unsigned char, unsigned char));
	void SetTextInTextBox(unsigned int textboxid, const char * text);
	
	void CreateButton(unsigned int width, unsigned int height, int x, int y,const char * but, int msg);
	void CreateStaticTextBox(unsigned int width, unsigned int height, int x, int y, const char * but);
	void CreateInputTextBox(unsigned int width, unsigned int height, int x, int y, const char * but);
	
	const int GetButtonMessage();
	void GetTextFromTextBox(unsigned int textboxid, size_t buffsize, char * buffer);
	size_t GetTextLengthFromTextBox(unsigned int);
	void GetWindowSize(int *, int *);
	void SetWindowTitle(const char *title);
	/*----------------------------------------------*/
};




#endif