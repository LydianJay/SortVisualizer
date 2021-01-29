#include "Renderers\GLRenderer.h"
#include <thread>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>
/*
Bubble Sort: 100
5.04018
5.0339
4.99023
5.03289
5.02241
Average: 5.023922s
Bubble Sort: 200
19.9936
20.0032
19.9594
19.965
19.9742
Bubble Sort: 500



Insertion Sort: 100
5.1304
5.14292
5.10653
5.1638
5.100068
Average: 5.12873136s

Insertion Sort: 200
20.1812
20.1603
20.2312
20.13173
20.2105


*/

struct Box
{
	float x;
	float y;
	float heigth;
	float width;
	
	Box(float l, float w, float xpos, float ypos) :x(xpos), y(ypos), heigth(l), width(w){}
	Box() { x = 0; y = 0; heigth = 0; width = 0; }
};

class CApp : public CGLRenderer
{
private:
	static const int m_TileCount = 250;
	const int m_delay = 10;
	Box m_Boxes[m_TileCount];
	int m_width;
	int m_heigth;
	bool m_isFinished = false;
	bool m_isThreadCreated = false;
	std::thread t1;
	
	float m_ftimer = 0;
	int m_nCurrentIndex = 0;
public:
	CApp(CWindow * window)
	{
		
		srand(time(0));

		this->window = window;
		window->GetWindowSize(&m_width, &m_heigth);
		window->CreateOpenGLContext();
		glClearColor(0.5, 0.5, 0.5, 0);
		gluOrtho2D(0, m_width, m_heigth, 0);
		for (size_t i = 0; i < m_TileCount; i++)
		{
			m_Boxes[i].width = m_width / (m_TileCount + 45 );
			m_Boxes[i].heigth = (m_heigth / m_TileCount) * (i+1);
			
			m_Boxes[i].x = ((double)m_width/m_TileCount * i);
			m_Boxes[i].y = (m_heigth - m_Boxes[i].heigth);
		}

		Shuffle();

	}
	~CApp()
	{
		
	}

	void DrawBoxes()
	{
		
		for (size_t i = 0; i < m_TileCount; i++)
		{
			
			if (i == m_nCurrentIndex)
				glColor3f(0,1,0);
			else
				glColor3f((float)1 - (float)(i + 1) / m_TileCount, 0, (float)(i + 1) / m_TileCount);
			
			DrawQuad(m_Boxes[i].x, m_Boxes[i].y, m_Boxes[i].width, m_Boxes[i].heigth, 0);
		}
	}

	void Shuffle()
	{
		for (size_t i = 0; i < m_TileCount; i++)
		{
			int index = rand() % m_TileCount;

			if (index != i)
			{
				float temp = m_Boxes[i].x;
				m_Boxes[i].x = m_Boxes[index].x;
				m_Boxes[index].x = temp;
			}

		}
	}

	
	void bubbleSort()
	{
		
		

		for (size_t i = 0; i < m_TileCount; i++)
		{
			for (size_t j = i+1; j < m_TileCount; j++)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(m_delay));
				if (m_Boxes[i].x > m_Boxes[j].x)
				{
					m_nCurrentIndex = j;
					float temp = m_Boxes[i].x;
					m_Boxes[i].x = m_Boxes[j].x;
					m_Boxes[j].x = temp;	
				}
			}
			
		}

		

		m_isFinished = true;

	}

	void SelectionSort()
	{
		

		for (size_t i = 0; i < m_TileCount; i++)
		{
			int minindex = i;
			for (size_t j = i+1; j < m_TileCount; j++)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(m_delay));
				m_nCurrentIndex = j;
				if (m_Boxes[minindex].x > m_Boxes[j].x)
				{
					minindex = j;
				}
				
			}
			
			float temp = m_Boxes[i].x;
			m_Boxes[i].x = m_Boxes[minindex].x;
			m_Boxes[minindex].x = temp;
			
		}
		m_isFinished = true;

	}

	void InsertionSort()
	{
		int r = 1;
		for (size_t i = 0; i < m_TileCount; i++)
		{

			int a = i;
			while (m_Boxes[a].x < m_Boxes[a - 1].x && a > 0)
			{
				m_nCurrentIndex = a;
				std::this_thread::sleep_for(std::chrono::microseconds(m_delay));
				float temp = m_Boxes[a].x;
				m_Boxes[a].x = m_Boxes[a - 1].x;
				m_Boxes[a - 1].x = temp;
				a--;
			}

		}
		m_isFinished = true;
	}

	void ShellSort()
	{
		int gap = m_TileCount / 2;

		for (size_t k = 0; k < m_TileCount / 2; k++)
		{

			for (size_t i = 0; i < m_TileCount - gap; i++)
			{
				m_nCurrentIndex = i;
				std::this_thread::sleep_for(std::chrono::microseconds(m_delay));
				if (m_Boxes[i].x > m_Boxes[i + gap].x)
				{
					float temp = m_Boxes[i].x;
					m_Boxes[i].x = m_Boxes[i + gap].x;
					m_Boxes[i + gap].x = temp;
				}
			}
			gap /= 2;
			if (gap < 1)gap = 1;
		}

	}

	void Update(double fElapseTime)override
	{
		m_ftimer += fElapseTime;
		if (!m_isThreadCreated)
		{
			t1 = std::thread(&CApp::ShellSort, this);
			m_isThreadCreated = true;
		}

		if(m_isFinished)
		{
			t1.join();
			std::stringstream ss;
			ss << "Time: " << m_ftimer << "s";

			m_isFinished = false;
			MessageBox(NULL, ss.str().c_str(), "SORTED", MB_OK);
			
		}

		DrawBoxes();
	}

};



int main()
{
	CWindow window(1100, 540, "Shell Sort", WINDOW_NO_RESIZE);
	CApp app(&window);
	
	app.MainLoop();

	
	return 0;
}