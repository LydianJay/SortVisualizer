#ifndef GL_RENDERER_SYS_H
#define GL_RENDERER_SYS_H
#include "Renderer.h"
#include <chrono>
#include <gl\GL.h>
#include <gl\GLU.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
class CGLRenderer
{
public:
private:
	bool m_bQuit = false;
	
	std::chrono::duration<double> m_chElapseTime = std::chrono::high_resolution_clock::now() - std::chrono::high_resolution_clock::now();
protected:
	CWindow* window =nullptr;
public:
	void MainLoop();
	CGLRenderer();
	~CGLRenderer();
private:
protected:
	void DrawQuad(float x, float y, float width, float heigth, float fGap);
	void DrawTiles(int width, int heigth, float fGap, int box);
	virtual void Update(double fElapseTime) = 0;

};

#endif // !GL_RENDERER_SYS_H