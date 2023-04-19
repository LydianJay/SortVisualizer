/*
* 
* Date: 02/05/23 MM/DD/YY
* Author: LydianJay
* 
* Visualization of some sorting algorithms
* 
* 
*/

#include "GLSimp2DGraphics.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <sstream>


class App {
	
public:
	App() {

		m_window = new s2d::S2DWindow(m_w, m_h, "[SELECTION SORT] Sorting Algorithm Visualization", S2D_WINDOW_NO_RESIZE); // create the window
		m_graphics = new s2d::S2DGraphics(*m_window); // create the graphics rendering

		m_randEng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count()); // set the seed for the rng
		m_rand = std::uniform_int_distribution<uint32_t>(0, m_maxValueCount - 1); // set a uniform distribution for rng

		init();
		m_graphics->initGlyphs(s2d::PXLCLR_GREEN, s2d::PXLCLR_BLACK);
		
	}
	~App() { // cleanup
		m_window->DeleteOpenGLContext();
		delete m_graphics;
		delete m_window;
	}

	void appStart() { // main loop of the program
		
		uint64_t timeElapse = 0, time = 0; // time stuff
		while (isAppRunning && !m_window->WindowShouldClose()) {
			auto t1 = std::chrono::high_resolution_clock::now();

			s2d::KeyState ks = m_window->GetKeyState();

			if (ks.state == S2D_KEY_PRESSED) {
				switch (ks.key) {
				
				case VK_SPACE: // when space is pressed the sorting begins
					if (!m_isSorting && !m_isSorted) {
						time = 0;
						m_isSorting = true;
					}

					break;
				case VK_ESCAPE: // when escape key is pressed it shuffles/randomize the data
					if (!m_isSorting) {
						randomizeValues();
						reset();
						m_isSorted = false;
					}
					break;


				case 'S':	// when 'S' key is pressed the program stops from sorting
					m_isSorting = !m_isSorting;
					break;

				case 'E':
					if (!m_isSorting && !m_isSorted) {
						selectionSort();
						time = 0;
					}
					break;
				
				default:
					break;
				}
			}

			else if (ks.state == S2D_KEY_RELEASED) {
				
				
				switch (ks.key) { // control the speed of sorting
				case VK_UP:
					m_delay++;
					break;
				case VK_DOWN:
					if(m_delay > 0)
					m_delay--;

					break;
			
				}

			}
			

			displayLines(); // displays the rectangles/lines in the screen

			if (m_isSorting && time >= m_delay) { // executes one iteration of the sorting algorithm in accordance to the delay
				
				selectionSort(); 
				time = 0;
			}
	
			if (!m_isSorting && !m_isSorted)
				m_graphics->drawString("paused", 0, 42, 20, 20);
			else if(m_isSorting)
				m_graphics->drawString("sorting", 0, 42, 20, 20);
			else
				m_graphics->drawString("sorted", 0, 42, 20, 20);

			std::stringstream ss;
			ss << "delay " << m_delay << "ms            " << "Data size: " << m_maxValueCount;
			m_graphics->drawString(ss.str(), 0, 20, 20, 20);
			std::stringstream ss2;
			
			m_graphics->flushBuffer(); // boiler plate stuff, rendering
			m_window->ProcessMessage();
			m_window->SwapWindowBuffers();

			auto t2 = std::chrono::high_resolution_clock::now();
			
			timeElapse = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
			if(m_isSorting)
				time += timeElapse;
		}

	}

	



private:

	s2d::S2DGraphics* m_graphics = nullptr;
	s2d::S2DWindow* m_window = nullptr;
	const int m_w = 850, m_h = 850; // width and height of the window
	const uint32_t m_maxValueCount = 50; // the number of data
	bool isAppRunning = true; // boolean flag for the program state
	bool m_isSorted = true; // boolean flag if the data is sorted
	bool m_isSorting = false; // boolean flag if program is currently sorting

	uint32_t m_prevI = 0, m_prevJ = 0, m_currentMin = 0; // some data required for sorting, this are iterators of data array
	uint32_t m_delay = 15; // delay in milliseconds
	std::vector<uint32_t> m_vData; // the array that contains the data
	std::default_random_engine m_randEng; // the randomizer
	std::uniform_int_distribution<uint32_t> m_rand; // the uniform distribution rng instance
private:


	void reset() { // reset the iterators
		m_isSorting = false;
		m_prevI = 0, m_prevJ = 0, m_currentMin = 0;
	}
	void init() {
		uint32_t n = m_maxValueCount;
		uint32_t offset = m_h / n;
		for (uint32_t i = 1; i <= n; i++) { // initialize the data
			m_vData.push_back(i * offset);
		}
	}
	void randomizeValues() { // shuffles or randomize the data array
		

		for (uint32_t i = 0; i < m_maxValueCount; i++) {

			uint32_t idx1 = m_rand(m_randEng);
			uint32_t idx2 = m_rand(m_randEng);

			if (idx1 == idx2)continue;

			uint32_t temp = m_vData[idx2];
			// swap
			m_vData[idx2] = m_vData[idx1];
			m_vData[idx1] = temp;


		}


	}


	
	
	
	
	/*void bubbleSort() {

		if (m_isSorted)return;
		
		uint32_t i = m_prevI, j = m_prevJ;
		
		
		if (m_vData[i] < m_vData[j]) {

			uint32_t temp = m_vData[i];
			m_vData[i] = m_vData[j];
			m_vData[j] = temp;

		}

		if (j < m_maxValueCount) {
			j++;
			
		}
		else {
			j = 0;
			
			if (i < m_maxValueCount) {
				i++;
				
			}
			else {
				
				m_isSorted = true;
				m_isSorting = false;
			}
		}

		
		m_prevI = i;
		m_prevJ = j;
		

		
		if (m_prevJ >= m_maxValueCount) {
			
			m_prevJ = 0;
			m_prevI++;
		}
		if (m_prevI >= m_maxValueCount) {
			m_prevI = 0;
			m_isSorted = true;
			m_isSorting = false;
		}
	}*/



	// runs the Selection Sort in a single iteration
	void selectionSort() {
		
		uint32_t i = m_prevI, j = m_prevJ, m = m_currentMin; // load the previous iteration data

		if (m_vData[j] < m_vData[m]) {
			m = j;
		}
		
		j++;

		if (j >= m_maxValueCount) {
			
			uint32_t temp = m_vData[i];
			m_vData[i] = m_vData[m];
			m_vData[m] = temp;


			i++;
			m = i;
			j = i;
		}


		if (i >= m_maxValueCount) {
			m_isSorted = true;
			m_isSorting = false;
			i = 0;
			j = 0;
			m = 0;
		}
		m_prevI = i, m_prevJ = j, m_currentMin = m;
	}


	void displayLines() {
		const float offsetX = (float)m_w / m_maxValueCount;
		const float offsetY = (float)m_h / m_maxValueCount;

		
		for (uint32_t i = 0; i < m_maxValueCount; i++) {
			float x = offsetX * i;
			float y = (m_h - m_vData[i]);


			s2d::Color color = s2d::PXLCLR_WHITE;

			if (m_vData[m_prevJ] == m_vData[i]) color = s2d::PXLCLR_BLUE;
			if (m_vData[m_prevI] == m_vData[i]) color = s2d::PXLCLR_RED;
			if (m_vData[m_currentMin] == m_vData[i]) color = { 255, 128 ,128 };
			if (i < m_prevI) color = s2d::PXLCLR_GREEN;

			m_graphics->drawRect(x, y, (offsetX - 2.0f), (float)m_vData[i], color); // draws rect or bar graph


		}

	}

};






int main() {
	App app;
	app.appStart();
}