#include "GLRenderer.h"





void CGLRenderer::DrawQuad(float x, float y, float width, float heigth, float fGap)
{
	glBegin(GL_QUADS);
	glVertex2d(x + fGap, y + fGap);
	glVertex2d(x + width - fGap, y + fGap);
	glVertex2d(x + width - fGap, y + heigth - fGap);
	glVertex2d(x + fGap, y + heigth - fGap);
	glEnd();


	
}

void CGLRenderer::DrawTiles(int width, int heigth, float fGap, int box)
{
	for (int y = 0; y < heigth; y++)
		for (int x = 0; x < width; x++)
		{
			DrawQuad(box * x, box * y, box, box, fGap);
		}
}

void CGLRenderer::MainLoop()
{
	
	
	while (!m_bQuit && !window->WindowShouldClose())
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		glClear(GL_COLOR_BUFFER_BIT);
		Update(m_chElapseTime.count());
		window->SwapWindowBuffers();
		window->ProcessMessage();
		auto t2 = std::chrono::high_resolution_clock::now();
		m_chElapseTime = t2 - t1;
	}
	//window.DeleteOpenGLContext();
}

CGLRenderer::CGLRenderer()
{
	
}



CGLRenderer::~CGLRenderer()
{

}
