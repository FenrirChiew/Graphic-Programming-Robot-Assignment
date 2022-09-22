#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <ctime>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"
#define PI 3.141592f

clock_t currentTicks;
clock_t elapsedTicks;
float elapsedSeconds = 0.0f;
float frameNo = 100.0f;
int mode = -1;
GLint tubeSlices = 20;
GLint tubeStacks = 20;
float rotateFinger = 90.0f;
float rotateTumb = 45.0f;
float speed = 35.0f;
float objectRotateX = 0.0f;
float objectRotateY = 0.0f;
float objectRotateZ = 0.0f;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else if (wParam == VK_NUMPAD0)
		{
			mode = 0;
		}
		else if (wParam == VK_NUMPAD8)
		{
			objectRotateX += speed * elapsedSeconds;
		}
		else if (wParam == VK_NUMPAD2)
		{
			objectRotateX -= speed * elapsedSeconds;
		}
		else if (wParam == VK_NUMPAD6)
		{
			objectRotateY += speed * elapsedSeconds;
		}
		else if (wParam == VK_NUMPAD4)
		{
			objectRotateY -= speed * elapsedSeconds;
		}
		else if (wParam == VK_NUMPAD9)
		{
			objectRotateZ += speed * elapsedSeconds;
		}
		else if (wParam == VK_NUMPAD7)
		{
			objectRotateZ -= speed * elapsedSeconds;
		}
		else if (wParam == ' ')
		{
			mode = -1;
			rotateFinger = 90.0f;
			rotateTumb = 45.0f;
			objectRotateX = 0.0f;
			objectRotateY = 0.0f;
			objectRotateZ = 0.0f;
		}

		if (mode == 0)
		{
			if (wParam == 'W')
			{
				if (rotateFinger < 90.0f)
				{
					rotateFinger += speed * elapsedSeconds;
				}
				if (rotateTumb < 45.0f)
				{
					rotateTumb += speed / 2 * elapsedSeconds;
				}
			}
			else if (wParam == 'S')
			{
				if (rotateFinger > 0.0f)
				{
					rotateFinger -= speed * elapsedSeconds;
				}
				if (rotateTumb > 0.0f)
				{
					rotateTumb -= speed / 2 * elapsedSeconds;
				}
			}
		}
		break;
	case WM_KEYUP:

		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void drawFinger(GLfloat baseRadius, GLfloat topRadius, GLfloat totalLength, boolean isTumb, boolean isRight)
{
	// Finger: S1=C1=S2=C2=S3
	// S1, S2, S3 = shepere joints
	// C1 + C2 = cylinder finger
	GLfloat part1Length = totalLength * 3 / 5;
	GLfloat part2Length = totalLength * 2 / 5;
	GLfloat centerRadius = (baseRadius + topRadius) / 2;
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);

	glPushMatrix();
	{
		// S1
		if (isTumb)
		{
			glRotatef(90.0f, 0.0f, 1.0f, -1.0f);
			glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
		}
		else
		{
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		}
		glPushMatrix();
		{
			if (mode == 0)
			{
				if (isTumb)
				{
					if (isRight)
					{
						glRotatef(rotateTumb, 0.0f, 1.0f, 0.0f);
					}
					else
					{
						glRotatef(-rotateTumb, 0.0f, 1.0f, 0.0f);
					}
				}
			}
			glColor3f(1, 1, 1);
			gluSphere(quad, baseRadius, tubeSlices, tubeStacks);
			glPushMatrix();
			{
				// C1
				if (mode == 0)
				{
					glRotatef(rotateFinger, 1.0f, 0.0f, 0.0f);
				}
				glTranslatef(0.0f, 0.0f, -part1Length);
				glColor3f(0, 0, 0);
				gluCylinder(quad, centerRadius, baseRadius, part1Length, tubeSlices, tubeStacks);
				// S2
				glColor3f(1, 1, 1);
				gluSphere(quad, centerRadius, tubeSlices, tubeStacks);
				glPushMatrix();
				{
					// C2
					if (mode == 0)
					{
						glRotatef(rotateFinger, 1.0f, 0.0f, 0.0f);
					}
					glTranslatef(0.0f, 0.0f, -part2Length);
					glColor3f(0, 0, 0);
					gluCylinder(quad, topRadius, centerRadius, part2Length, tubeSlices, tubeStacks);
					// S3
					glColor3f(1, 1, 1);
					gluSphere(quad, topRadius, tubeSlices, tubeStacks);
				}
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawPalm(GLfloat basedRadius, GLfloat topRadius, GLfloat halfLength, boolean isRight)
{
	GLfloat leftBasedRadius;
	GLfloat rightBasedRadius;

	glPushMatrix();
	{
		//glScalef(5, 5, 5);
		if (isRight)
		{
			leftBasedRadius = -basedRadius * 2;
			rightBasedRadius = basedRadius * 3;
		}
		else
		{
			leftBasedRadius = -basedRadius * 3;
			rightBasedRadius = basedRadius * 2;
		}

		// Wrist
		glColor3f(0, 1, 0);
		glBegin(GL_QUADS);
		{
			if (isRight)
			{
				// Front Face
				glVertex3f(leftBasedRadius + basedRadius, halfLength, -basedRadius);
				glVertex3f(leftBasedRadius, halfLength * 2 / 3, -basedRadius);
				glVertex3f(rightBasedRadius, halfLength * 2 / 3, -basedRadius);
				glVertex3f(rightBasedRadius, halfLength, -basedRadius);

				// Back Face
				glVertex3f(leftBasedRadius + basedRadius, halfLength, basedRadius);
				glVertex3f(leftBasedRadius, halfLength * 2 / 3, basedRadius);
				glVertex3f(rightBasedRadius, halfLength * 2 / 3, basedRadius);
				glVertex3f(rightBasedRadius, halfLength, basedRadius);

				// Left Face
				glVertex3f(leftBasedRadius + basedRadius, halfLength, basedRadius);
				glVertex3f(leftBasedRadius, halfLength * 2 / 3, basedRadius);
				glVertex3f(leftBasedRadius, halfLength * 2 / 3, -basedRadius);
				glVertex3f(leftBasedRadius + basedRadius, halfLength, -basedRadius);

				// Right Face
				glVertex3f(rightBasedRadius, halfLength, basedRadius);
				glVertex3f(rightBasedRadius, halfLength * 2 / 3, basedRadius);
				glVertex3f(rightBasedRadius, halfLength * 2 / 3, -basedRadius);
				glVertex3f(rightBasedRadius, halfLength, -basedRadius);

				// Top Face
				glVertex3f(leftBasedRadius + basedRadius, halfLength, basedRadius);
				glVertex3f(leftBasedRadius + basedRadius, halfLength, -basedRadius);
				glVertex3f(rightBasedRadius, halfLength, -basedRadius);
				glVertex3f(rightBasedRadius, halfLength, basedRadius);
			}
			else
			{
				// Front Face
				glVertex3f(leftBasedRadius, halfLength, -basedRadius);
				glVertex3f(leftBasedRadius, halfLength * 2 / 3, -basedRadius);
				glVertex3f(rightBasedRadius, halfLength * 2 / 3, -basedRadius);
				glVertex3f(rightBasedRadius - basedRadius, halfLength, -basedRadius);

				// Back Face
				glVertex3f(leftBasedRadius, halfLength, basedRadius);
				glVertex3f(leftBasedRadius, halfLength * 2 / 3, basedRadius);
				glVertex3f(rightBasedRadius, halfLength * 2 / 3, basedRadius);
				glVertex3f(rightBasedRadius - basedRadius, halfLength, basedRadius);

				// Left Face
				glVertex3f(leftBasedRadius, halfLength, basedRadius);
				glVertex3f(leftBasedRadius, halfLength * 2 / 3, basedRadius);
				glVertex3f(leftBasedRadius, halfLength * 2 / 3, -basedRadius);
				glVertex3f(leftBasedRadius, halfLength, -basedRadius);

				// Right Face
				glVertex3f(rightBasedRadius - basedRadius, halfLength, basedRadius);
				glVertex3f(rightBasedRadius, halfLength * 2 / 3, basedRadius);
				glVertex3f(rightBasedRadius, halfLength * 2 / 3, -basedRadius);
				glVertex3f(rightBasedRadius - basedRadius, halfLength, -basedRadius);

				// Top Face
				glVertex3f(leftBasedRadius, halfLength, basedRadius);
				glVertex3f(leftBasedRadius, halfLength, -basedRadius);
				glVertex3f(rightBasedRadius - basedRadius, halfLength, -basedRadius);
				glVertex3f(rightBasedRadius - basedRadius, halfLength, basedRadius);
			}
		}
		glEnd();

		// Palm
		glColor3f(1, 0, 0);
		glBegin(GL_QUADS);
		{
			// Front Face
			glVertex3f(leftBasedRadius, halfLength * 2 / 3, -basedRadius);
			glVertex3f(leftBasedRadius, 0.0f, -basedRadius);
			glVertex3f(rightBasedRadius, 0.0f, -basedRadius);
			glVertex3f(rightBasedRadius, halfLength * 2 / 3, -basedRadius);

			// Back Face
			glVertex3f(leftBasedRadius, halfLength * 2 / 3, basedRadius);
			glVertex3f(leftBasedRadius, 0.0f, basedRadius);
			glVertex3f(rightBasedRadius, 0.0f, basedRadius);
			glVertex3f(rightBasedRadius, halfLength * 2 / 3, basedRadius);

			// Left Face
			glVertex3f(leftBasedRadius, halfLength * 2 / 3, basedRadius);
			glVertex3f(leftBasedRadius, 0.0f, basedRadius);
			glVertex3f(leftBasedRadius, 0.0f, -basedRadius);
			glVertex3f(leftBasedRadius, halfLength * 2 / 3, -basedRadius);

			// Right Face
			glVertex3f(rightBasedRadius, halfLength * 2 / 3, basedRadius);
			glVertex3f(rightBasedRadius, 0.0f, basedRadius);
			glVertex3f(rightBasedRadius, 0.0f, -basedRadius);
			glVertex3f(rightBasedRadius, halfLength * 2 / 3, -basedRadius);

			// Bottom Face
			glVertex3f(leftBasedRadius, 0.0f, basedRadius);
			glVertex3f(leftBasedRadius, 0.0f, -basedRadius);
			glVertex3f(rightBasedRadius, 0.0f, -basedRadius);
			glVertex3f(rightBasedRadius, 0.0f, basedRadius);
		}
		glEnd();

		// Middle Finger
		drawFinger(basedRadius, topRadius, halfLength, false, isRight);
		glPushMatrix();
		{
			// Fore Finger
			if (isRight)
			{
				glTranslatef(-basedRadius, 0.0f, 0.0f);
			}
			else
			{
				glTranslatef(basedRadius, 0.0f, 0.0f);
			}
			drawFinger(basedRadius, topRadius, halfLength * 0.85, false, isRight);
			// Tumb
			if (isRight)
			{
				glTranslatef(-basedRadius, halfLength * 0.5, 0.0f);
			}
			else
			{
				glTranslatef(basedRadius, halfLength * 0.5, 0.0f);
				glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
				glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			}
			drawFinger(basedRadius, topRadius, halfLength * 0.7, true, isRight);
		}
		glPopMatrix();
		glPushMatrix();
		{
			// Ring Finger
			if (isRight)
			{
				glTranslatef(basedRadius, 0.0f, 0.0f);
			}
			else
			{
				glTranslatef(-basedRadius, 0.0f, 0.0f);
			}
			drawFinger(basedRadius, topRadius, halfLength * 0.85, false, isRight);
			// Little Finger
			if (isRight)
			{
				glTranslatef(basedRadius, 0.0f, 0.0f);
			}
			else
			{
				glTranslatef(-basedRadius, 0.0f, 0.0f);
			}
			drawFinger(basedRadius, topRadius, halfLength * 0.7, false, isRight);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawShieldPlane(GLfloat radius, GLfloat bulge)
{
	//glBegin(GL_TRIANGLES);
	//{
	//	glColor3f(1, 0, 0);
	//	glVertex3f(radius * cos(60 * PI / 180) * 3 / 4, radius * sin(60 * PI / 180) * 3 / 4, bulge / 2);
	//	glColor3f(0, 1, 0);
	//	glVertex3f(radius * cos(90 * PI / 180), radius * sin(90 * PI / 180), -bulge / 2);
	//	glColor3f(0, 0, 1);
	//	glVertex3f(radius * cos(120 * PI / 180) * 3 / 4, radius * sin(120 * PI / 180) * 3 / 4, bulge / 2);

	//	glColor3f(1, 0, 0);
	//	glVertex3f(radius * cos(225 * PI / 180) * 3 / 4, radius * sin(225 * PI / 180) * 3 / 4, bulge / 2);
	//	glColor3f(0, 1, 0);
	//	glVertex3f(radius * cos(270 * PI / 180) * 3 / 4, radius * sin(270 * PI / 180) * 3 / 4, -bulge / 2);
	//	glColor3f(0, 0, 1);
	//	glVertex3f(radius * cos(315 * PI / 180) * 3 / 4, radius * sin(315 * PI / 180) * 3 / 4, bulge / 2);
	//}
	//glEnd();

	glBegin(GL_QUADS);
	{
		glColor3f(1, 1, 1);
		glVertex3f(radius * cos(90 * PI / 180), radius * sin(90 * PI / 180), -bulge / 2);
		glVertex3f(radius * cos(120 * PI / 180) * 3 / 4, radius * sin(120 * PI / 180) * 3 / 4, bulge / 2);
		glVertex3f(radius * cos(225 * PI / 180) * 3 / 4, radius * sin(225 * PI / 180) * 3 / 4, bulge / 2);
		glVertex3f(radius * cos(270 * PI / 180) * 3 / 4, radius * sin(270 * PI / 180) * 3 / 4, -bulge / 2);

		glColor3f(0, 0, 0);
		glVertex3f(radius * cos(90 * PI / 180), radius * sin(90 * PI / 180), -bulge / 2);
		glVertex3f(radius * cos(270 * PI / 180) * 3 / 4, radius * sin(270 * PI / 180) * 3 / 4, -bulge / 2);
		glVertex3f(radius * cos(315 * PI / 180) * 3 / 4, radius * sin(315 * PI / 180) * 3 / 4, bulge / 2);
		glVertex3f(radius * cos(60 * PI / 180) * 3 / 4, radius * sin(60 * PI / 180) * 3 / 4, bulge / 2);
	}
	glEnd();
}

void drawShield(GLfloat radius, GLfloat totalDepth)
{
	GLUquadricObj* quad = gluNewQuadric();

	glPushMatrix();
	{
		//glScalef(5, 5, 5);
		glTranslatef(0.0f, 0.0f, totalDepth / 2);
		glBegin(GL_QUAD_STRIP);
		{
			glColor3f(0, 0, 1);
			glVertex3f(0.0f, radius, -totalDepth * 1 / 5);
			glVertex3f(0.0f, radius, -totalDepth * 1 / 10);

			glColor3f(0, 0, 0);
			glVertex3f(-radius * cos(60 * PI / 180) * 3 / 4, radius * sin(60 * PI / 180) * 3 / 4, 0.0f);
			glVertex3f(-radius * cos(60 * PI / 180) * 3 / 4, radius * sin(60 * PI / 180) * 3 / 4, totalDepth * 1 / 10);

			glColor3f(1, 0, 0);
			glVertex3f(-radius * cos(45 * PI / 180) * 3 / 4, -radius * sin(45 * PI / 180) * 3 / 4, 0.0f);
			glVertex3f(-radius * cos(45 * PI / 180) * 3 / 4, -radius * sin(45 * PI / 180) * 3 / 4, totalDepth * 1 / 10);

			glVertex3f(0.0f, -radius * 3 / 4, -totalDepth * 1 / 5);
			glVertex3f(0.0f, -radius * 3 / 4, -totalDepth * 1 / 10);

			glColor3f(0, 1, 0);
			glVertex3f(radius * cos(45 * PI / 180) * 3 / 4, -radius * sin(45 * PI / 180) * 3 / 4, 0.0f);
			glVertex3f(radius * cos(45 * PI / 180) * 3 / 4, -radius * sin(45 * PI / 180) * 3 / 4, totalDepth * 1 / 10);

			glColor3f(0, 0, 1);
			glVertex3f(radius * cos(60 * PI / 180) * 3 / 4, radius * sin(60 * PI / 180) * 3 / 4, 0.0f);
			glVertex3f(radius * cos(60 * PI / 180) * 3 / 4, radius * sin(60 * PI / 180) * 3 / 4, totalDepth * 1 / 10);

			glVertex3f(0.0f, radius, -totalDepth * 1 / 5);
			glVertex3f(0.0f, radius, -totalDepth * 1 / 10);
		}
		glEnd();

		// Shield Faces
		drawShieldPlane(radius, totalDepth * 1 / 5);
		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, -totalDepth * 1 / 10);
			drawShieldPlane(radius, totalDepth * 1 / 5);
		}
		glPopMatrix();

		// Torn
		glColor3f(1, 1, 0);
		glPushMatrix();
		{
			glTranslatef(0.0f, -radius / 4, -totalDepth);
			gluCylinder(quad, 0.0f, radius / 4, totalDepth * 4 / 5, tubeSlices, tubeStacks);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawHand(boolean isRight)
{
	GLfloat basedRadius = 0.0175f;
	GLfloat topRadius = 0.0125f;
	GLfloat totalLength = 0.085f;
	GLfloat totalDepth = 0.05f;

	drawPalm(basedRadius, topRadius, totalLength, isRight);
	glPushMatrix();
	{
		if (isRight)
		{
			glTranslatef(basedRadius / 2, totalLength * sin(45 * PI / 180) * 3 / 4, totalDepth);
		}
		else
		{
			glTranslatef(-basedRadius / 2, totalLength * sin(45 * PI / 180) * 3 / 4, totalDepth);
		}
		glRotatef(180.0f, 0.0f, 1.0f, 0.05f);
		drawShield(totalLength * 1.25, totalDepth);
	}
	glPopMatrix();
}

void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Palm
	//drawPalm(0.0175f, 0.0125f, 0.085f);

	// Shield
	//drawShield(0.085f, 0.05f);

	// Hands
	glPushMatrix();
	glScalef(5, 5, 5);
	glRotatef(objectRotateX, 1.0, 0.0f, 0.0f);
	glRotatef(objectRotateY, 0.0, 1.0f, 0.0f);
	glRotatef(objectRotateZ, 0.0, 0.0f, 1.0f);
	glPushMatrix();
	//glTranslatef(-0.1f, 0.0f, 0.0f);
	drawHand(true);
	glPopMatrix();
	glPopMatrix();
	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}

void setupGLProperties()
{
	glEnable(GL_DEPTH_TEST);
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------


	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	setupGLProperties();

	while (true)
	{
		currentTicks = clock();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);

		elapsedTicks = clock() - currentTicks;
		elapsedSeconds = (float)elapsedTicks * frameNo / 1000;
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------