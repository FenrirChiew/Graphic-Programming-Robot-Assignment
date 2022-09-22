#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <ctime>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"
#define PI 3.141592f

// Time Attributes
clock_t currentTicks;
clock_t elapsedTicks;
float elapsedSeconds = 0.0f;
float frameNo = 100.0f;

// Control Attributes
int mode = 0;
int direction = 0;

// Projection Attributes
boolean isOrtho = true;
float r = 2.0f;
float angle = 90.0f;
float eyeX = r * sin(angle * 3.142 / 180.0);
float eyeY = r;
float eyeZ = r * cos(angle * 3.142 / 180.0);
float lookAtX = 0.0f;
float lookAtY = 0.0f;
float lookAtZ = 0.0f;
float upX = 0.0f;
float upY = 1.0f;
float upZ = 0.0f;
float cameraSpeed = 10.0f;

// Robot Attributes
GLint headSlices = 50;
GLint headStacks = 50;
GLint tubeSlices = 20;
GLint tubeStacks = 20;
GLint kunaiSlices = 10;
GLint kunaiStacks = 10;
boolean isGrab = false;
float rotateFinger = 90.0f;
float rotateTumb = -45.0f;
float rotateWheel = 0.0f;
float rotateShoulderJointX = 0.0f;
float rotateShoulderJointY = 0.0f;
float rotateShoulderJointZ = 0.0f;
float rotateUpperArmJointX = 0.0f;
float rotateUpperArmJointY = 0.0f;
float rotateUpperArmJointZ = 0.0f;
float rotateElbowJointX = 30.0f;
float rotateElbowJointY = 0.0f;
float rotateElbowJointZ = -90.0f;
float rotateWristJointX = 0.0f;
float rotateWristJointY = 5.0f;
float rotateWristJointZ = 0.0f;
float speed = 15.0f;

// Rotate Attributes
float objectRotateX = 0.0f;
float objectRotateY = 0.0f;
float objectRotateZ = 0.0f;

// Light Attributes
GLfloat lightX = 0.0f;
GLfloat lightY = 10.0f;
GLfloat lightZ = 0.0f;
GLfloat positionLight0[4] = { lightX, lightY, lightZ, 0.0f };
boolean isOn = true;

// Texture Attributes
int materialType = 1;
GLuint texTexture = 0;
LPCSTR texFile = "tex.bmp";
BITMAP BMP;
HBITMAP hBMP = NULL;
boolean onTexture = true;

GLuint starStripTexture = 0;
GLuint headTexture = 0;
GLuint eyeTexture = 0;
GLuint eyeTubeTexture = 0;
GLuint lavaStoneTexture = 0;
GLuint lavaStone2Texture = 0;
GLuint metalTexture = 0;
GLuint metal2Texture = 0;
GLuint legTexture = 0;
GLuint leg2Texture = 0;
GLuint leg3Texture = 0;
GLuint legTipTexture = 0;
GLuint neckTexture = 0;
GLuint metal3Texture = 0;
GLuint metal4Texture = 0;
GLuint mothEyeTexture = 0;
GLuint mothEye2Texture = 0;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		if (wParam == VK_NUMPAD0)	// Projection
		{
			mode = 0;
		}
		else if (wParam == VK_NUMPAD1)	// Eye
		{
			mode = 1;
		}
		else if (wParam == VK_NUMPAD2)	// Light
		{
			mode = 2;
		}
		else if (wParam == VK_NUMPAD3)	// LookAt
		{
			mode = 3;
		}
		else if (wParam == VK_NUMPAD4)	// Up
		{
			mode = 4;
		}
		else if (wParam == VK_NUMPAD5)	// Texture
		{
			mode = 5;
		}
		else if (wParam == VK_NUMPAD6)	// Robot Finger
		{
			mode = 6;
		}
		else if (wParam == VK_NUMPAD7)	// Robot Arms
		{
			mode = 7;
		}
		//else if (wParam == VK_DECIMAL)	// Rotate
		//{
		//	mode = -1;
		//}
		else if (wParam == ' ')
		{
			materialType = 1;
			r = 2.0f;
			angle = 90.0f;
			//objectRotateX = 0.0f;
			//objectRotateY = 0.0f;
			//objectRotateZ = 0.0f;
			eyeX = r * sin(angle * 3.142 / 180.0);
			eyeY = r;
			eyeZ = r * cos(angle * 3.142 / 180.0);
			lookAtX = 0.0f;
			lookAtY = 0.0f;
			lookAtZ = 0.0f;
			upX = 0.0f;
			upY = 1.0f;
			upZ = 0.0f;
			rotateFinger = 90.0f;
			rotateTumb = -45.0f;
			rotateWheel = 0.0f;
			rotateShoulderJointX = 0.0f;
			rotateShoulderJointY = 0.0f;
			rotateShoulderJointZ = 0.0f;
			rotateUpperArmJointX = 0.0f;
			rotateUpperArmJointY = 0.0f;
			rotateUpperArmJointZ = 0.0f;
			rotateElbowJointX = 30.0f;
			rotateElbowJointY = 0.0f;
			rotateElbowJointZ = -90.0f;
			rotateWristJointX = 0.0f;
			rotateWristJointY = 5.0f;
			rotateWristJointZ = 0.0f;
			// lightX
			positionLight0[0] = 0.0f;
			// lightY
			positionLight0[1] = 10.0f;
			// lightZ
			positionLight0[2] = 0.0f;
			isOn = true;
		}

		if (mode == 0)
		{
			if (wParam == VK_NUMPAD0)
			{
				glMatrixMode(GL_PROJECTION);
				isOrtho = !isOrtho;
				glLoadIdentity();

				if (isOrtho)
				{
					glOrtho(-3, 3, -3, 3, -3, 3);
				}
				else
				{
					gluPerspective(50, 1, 1, 20);
				}
			}
		}
		if (mode == 1)
		{
			if (wParam == 'D')	// Right
			{
				angle += 5;
				eyeX = r * sin(angle * 3.142 / 180.0);
				eyeY = eyeY;
				eyeZ = r * cos(angle * 3.142 / 180.0);
				//eyeX += cameraSpeed * elapsedSeconds;
				//rotatex += 0.5f * elapsedSeconds;
			}
			else if (wParam == 'A')	// Left
			{
				angle -= 5;
				eyeX = r * sin(angle * 3.142 / 180.0);
				eyeY = eyeY;
				eyeZ = r * cos(angle * 3.142 / 180.0);
				//eyeX -= cameraSpeed * elapsedSeconds;
				//rotatex -= 0.5f * elapsedSeconds;
			}
			else if (wParam == 'W')	// Up
			{
				eyeX = r * sin(angle * 3.142 / 180.0);
				eyeY += eyeY * 0.1f;
				eyeZ = r * cos(angle * 3.142 / 180.0);
				//eyeY += cameraSpeed * elapsedSeconds;
				//rotatey += 0.5f * elapsedSeconds;
			}
			else if (wParam == 'S')	// Down
			{
				eyeX = r * sin(angle * 3.142 / 180.0);
				eyeY -= eyeY * 0.1f;
				eyeZ = r * cos(angle * 3.142 / 180.0);
				//eyeY -= cameraSpeed * elapsedSeconds;
				//rotatey -= 0.5f * elapsedSeconds;
			}
			else if (wParam == 'Q')	// Near
			{
				r += 0.1f;
				eyeX = r * sin(angle * 3.142 / 180.0);
				eyeY += 0.1f;
				eyeZ = r * cos(angle * 3.142 / 180.0);
				//eyeZ -= cameraSpeed * elapsedSeconds;
				//rotatez -= 0.5f * elapsedSeconds;
			}
			else if (wParam == 'E')	// Far
			{
				r -= 0.1f;
				eyeX = r * sin(angle * 3.142 / 180.0);
				eyeY -= 0.1f;
				eyeZ = r * cos(angle * 3.142 / 180.0);
				//eyeZ += cameraSpeed * elapsedSeconds;
				//rotatez += 0.5f * elapsedSeconds;
			}
		}
		if (mode == 2)
		{
			if (wParam == 'D')	// Right
			{
				// lightX
				positionLight0[0] += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'A')	// Left
			{
				// lightX
				positionLight0[0] -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'W')	// Up
			{
				// lightY
				positionLight0[1] += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'S')	// Down
			{
				// lightY
				positionLight0[1] -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'Q')	// Near
			{
				// lightZ
				positionLight0[2] += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'E')	// Far
			{
				// lightZ
				positionLight0[2] -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'X')	// On/Off Light
			{
				isOn = !isOn;
			}
		}
		if (mode == 3)
		{
			if (wParam == 'D')
			{
				lookAtX += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'A')
			{
				lookAtX -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'W')
			{
				lookAtY += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'S')
			{
				lookAtY -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'Q')
			{
				lookAtZ += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'E')
			{
				lookAtZ -= cameraSpeed * elapsedSeconds;
			}
		}
		if (mode == 4)
		{
			if (wParam == 'D')
			{
				upX += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'A')
			{
				upX -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'W')
			{
				upY += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'S')
			{
				upY -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'Q')
			{
				upZ += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'E')
			{
				upZ -= cameraSpeed * elapsedSeconds;
			}
		}
		if (mode == 6)
		{
			if (wParam == 'W')
			{
				if (rotateFinger < 90.0f)
				{
					rotateFinger += speed * elapsedSeconds;
				}
				if (rotateTumb > -45.0f)
				{
					rotateTumb -= speed / 2 * elapsedSeconds;
				}
			}
			else if (wParam == 'S')
			{
				if (rotateFinger > 0.0f)
				{
					rotateFinger -= speed * elapsedSeconds;
				}
				if (rotateTumb < 0.0f)
				{
					rotateTumb += speed / 2 * elapsedSeconds;
				}
			}
		}
		if (mode == 7)
		{
			if (wParam == 'W')
			{
				if (rotateShoulderJointX < 5.0f)
				{
					rotateShoulderJointX += speed * elapsedSeconds;
				}
				if (rotateUpperArmJointX < 5.0f)
				{
					rotateUpperArmJointX += speed * elapsedSeconds;
				}
				//if (rotateArmJoint < 45.0f)
				//{
				//	rotateArmJoint += speed / 2 * elapsedSeconds;
				//}
			}
			else if (wParam == 'S')
			{
				if (rotateShoulderJointX > -5.0f)
				{
					rotateShoulderJointX -= speed * elapsedSeconds;
				}
				if (rotateUpperArmJointX > -5.0f)
				{
					rotateUpperArmJointX -= speed * elapsedSeconds;
				}
			}
			else if (wParam == 'A')
			{
				if (rotateShoulderJointZ < 5.0f)
				{
					rotateShoulderJointZ += speed * elapsedSeconds;
				}
				if (rotateUpperArmJointY < 5.0f)
				{
					rotateUpperArmJointY += speed * elapsedSeconds;
				}
			}
			else if (wParam == 'D')
			{
				if (rotateShoulderJointZ > -5.0f)
				{
					rotateShoulderJointZ -= speed * elapsedSeconds;
				}
				if (rotateUpperArmJointY > -5.0f)
				{
					rotateUpperArmJointY -= speed * elapsedSeconds;
				}
			}
			else if (wParam == 'E')
			{
				if (rotateShoulderJointY < 5.0f)
				{
					rotateShoulderJointY += speed * elapsedSeconds;
				}
				if (rotateUpperArmJointZ > 5.0f)
				{
					rotateUpperArmJointZ -= speed * elapsedSeconds;
				}
			}
			else if (wParam == 'Q')
			{
				if (rotateShoulderJointY > -5.0f)
				{
					rotateShoulderJointY -= speed * elapsedSeconds;
				}
				if (rotateUpperArmJointZ < -5.0f)
				{
					rotateUpperArmJointZ += speed * elapsedSeconds;
				}
			}
			else if (wParam == 'H')
			{
				if (rotateElbowJointX < 45.0f)
				{
					rotateElbowJointX += speed * elapsedSeconds;
				}
			}
			else if (wParam == 'F')
			{
				if (rotateElbowJointX > -0.0f)
				{
					rotateElbowJointX -= speed * elapsedSeconds;
				}
			}
			else if (wParam == 'T')
			{
				if (rotateElbowJointY < 45.0f)
				{
					rotateElbowJointY += speed * elapsedSeconds;
				}
			}
			else if (wParam == 'G')
			{
				if (rotateElbowJointY > -45.0f)
				{
					rotateElbowJointY -= speed * elapsedSeconds;
				}
			}
			else if (wParam == 'Y')
			{
				if (rotateElbowJointZ < 5.0f)
				{
					rotateElbowJointZ += speed * elapsedSeconds;
				}
			}
			else if (wParam == 'R')
			{
				if (rotateElbowJointZ > -5.0f)
				{
					rotateElbowJointZ -= speed * elapsedSeconds;
				}
			}
			else if (wParam == 'I')
			{
				if (rotateWristJointX < 5.0f)
				{
					rotateWristJointX += speed * elapsedSeconds;
				}
			}
			else if (wParam == 'K')
			{
				if (rotateWristJointX > -5.0f)
				{
					rotateWristJointX -= speed * elapsedSeconds;
				}
			}
			else if (wParam == 'J')
			{
				if (rotateWristJointY < 5.0f)
				{
					rotateWristJointY += speed * elapsedSeconds;
				}
			}
			else if (wParam == 'L')
			{
				if (rotateWristJointY > -5.0f)
				{
					rotateWristJointY -= speed * elapsedSeconds;
				}
			}
			else if (wParam == 'O')
			{
				if (rotateWristJointZ < 5.0f)
				{
					rotateWristJointZ += speed * elapsedSeconds;
				}
			}
			else if (wParam == 'U')
			{
				if (rotateWristJointZ > -5.0f)
				{
					rotateWristJointZ -= speed * elapsedSeconds;
				}
			}
		}
		if (mode != 0)
		{
			rotateWheel = 0.0f;
		}
		//else if (mode == -1)
		//{
		//	if (wParam == VK_UP) {
		//		objectRotateX += cameraSpeed * elapsedSeconds;
		//		objectRotateY += cameraSpeed * elapsedSeconds;
		//		objectRotateZ += cameraSpeed * elapsedSeconds;
		//	}
		//	else if (wParam == VK_DOWN) {
		//		objectRotateX -= cameraSpeed * elapsedSeconds;
		//		objectRotateY -= cameraSpeed * elapsedSeconds;
		//		objectRotateZ -= cameraSpeed * elapsedSeconds;
		//	}
		//}

		//switch (wParam)
		//{
		//case 'X':
		//	objectRotateX += 45.0f * elapsedSeconds;
		//	break;
		//case 'Y':
		//	objectRotateY += 45.0f * elapsedSeconds;
		//	break;
		//case 'Z':
		//	objectRotateZ += 45.0f * elapsedSeconds;
		//	break;
		//case ' ':
		//	objectRotateX = 0.0f;
		//	objectRotateY = 0.0f;
		//	objectRotateZ = 0.0f;
		//	break;
		//}
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

void drawS(GLfloat coordX, GLfloat coordY, GLfloat coordZ, GLfloat radiusX, GLfloat radiusY, GLfloat radiusZ, GLfloat increment, GLfloat r, GLfloat g, GLfloat b)
{
	glColor3f(r, g, b);
	float theta;
	int iteration = int(radiusZ / increment);
	float incrementX = radiusX / iteration;
	float incrementY = radiusY / iteration;
	float accumX = 0.0f;
	float accumY = 0.0f;
	float accumZ = radiusZ;
	float x;
	float y;
	float z;
	do
	{
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++)
		{
			theta = i * PI / 180;
			x = accumX * cos(theta) + coordX;
			y = accumY * sin(theta) + coordY;
			z = accumZ + coordZ;
			glColor3f(0, 0, 0);
			glVertex3f(x, y, z);
		}
		glEnd();
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++)
		{
			theta = i * PI / 180;
			glColor3f(r, g, b);
			x = -accumX * cos(theta) + coordX;
			y = -accumY * sin(theta) + coordY;
			z = -accumZ + coordZ;
			glVertex3f(x, y, z);
		}
		glEnd();

		accumX += incrementX;
		accumY += incrementY;
		accumZ -= increment;
	} while (accumZ >= 0.0f);
}

void draw4PointedStarLine(GLfloat radius)
{
	glBegin(GL_LINE_LOOP);
	{
		for (int i = 0; i < 8; i++) {
			int j = i * 360 / 8;
			if (i % 2 == 1) {
				glVertex3f(radius * cos(j * PI / 180) / 2, radius * sin(j * PI / 180) / 2, 0.0f);
			}
			else {
				glVertex3f(radius * cos(j * PI / 180), radius * sin(j * PI / 180), 0.0f);
			}
		}
	}
	glEnd();
}

void draw4PointedStarStrip(GLfloat radius, GLfloat depth)
{
	glBegin(GL_QUAD_STRIP);
	{
		//glColor3f(1, 0, 1);
		//top back
		glTexCoord2f(1.0, 0); //right bottom 
		glVertex3f(0.0f, radius, depth / 2); //Front
		glTexCoord2f(1.0, 1.0); //right top 
		glVertex3f(0.0f, radius, -depth / 2); //Back


		//glColor3f(1, 1, 1);
		//bottom back
		glTexCoord2f(0, 1.0); //left top
		glVertex3f(-radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, -depth / 2); //Back
		glTexCoord2f(0, 0); //left bottom
		glVertex3f(-radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, depth / 2); //Front 

		//glColor3f(1, 0, 1);
		//right back
		glTexCoord2f(1.0, 0); //right bottom 
		glVertex3f(-radius, 0.0f, depth / 2);
		glTexCoord2f(1.0, 1.0); //right top 
		glVertex3f(-radius, 0.0f, -depth / 2);

		//glColor3f(1, 1, 1);
		//left back
		glTexCoord2f(0, 1.0); //left top
		glVertex3f(-radius * cos(45 * PI / 180) / 2, -radius * sin(45 * PI / 180) / 2, depth / 2);
		glTexCoord2f(0, 0); //left bottom
		glVertex3f(-radius * cos(45 * PI / 180) / 2, -radius * sin(45 * PI / 180) / 2, -depth / 2);

		//glColor3f(0, 0, 0);
		//bottom back
		glTexCoord2f(1.0, 0); //right bottom 
		glVertex3f(0.0f, -radius, depth / 2);
		glTexCoord2f(1.0, 1.0); //right top 
		glVertex3f(0.0f, -radius, -depth / 2);

		//glColor3f(1, 0, 0);
		//Left back
		glTexCoord2f(0, 1.0); //left top
		glVertex3f(radius * cos(45 * PI / 180) / 2, -radius * sin(45 * PI / 180) / 2, depth / 2);
		glTexCoord2f(0, 0); //left bottom
		glVertex3f(radius * cos(45 * PI / 180) / 2, -radius * sin(45 * PI / 180) / 2, -depth / 2);

		glTexCoord2f(1.0, 0); //right bottom 
		glVertex3f(radius, 0.0f, depth / 2);
		glTexCoord2f(1.0, 1.0); //right top 
		glVertex3f(radius, 0.0f, -depth / 2);
		glTexCoord2f(0, 1.0); //left top
		glVertex3f(radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, depth / 2);
		glTexCoord2f(0, 0); //left bottom
		glVertex3f(radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, -depth / 2);

		//glColor3f(0, 0, 1);
		glTexCoord2f(1.0, 0); //right bottom 
		glVertex3f(0.0f, radius, depth / 2);
		glTexCoord2f(1.0, 1.0); //right top 
		glVertex3f(0.0f, radius, -depth / 2);
	}
	glEnd();
}

void draw4PointedStar(GLfloat outerRadius, GLfloat innerRadius, GLfloat outerDepth, GLfloat innerDepth)
{
	GLfloat interval = innerDepth - outerDepth;

	glPushMatrix();
	{
		glTranslatef(0.0f, 0.0f, interval);
		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glBindTexture(GL_TEXTURE_2D, starStripTexture);
			draw4PointedStarStrip(outerRadius, outerDepth);
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();

	if (onTexture)
	{
		glEnable(GL_TEXTURE_2D);
	}
	{
		glBindTexture(GL_TEXTURE_2D, starStripTexture);
		draw4PointedStarStrip(innerRadius, innerDepth);
	}
	glDisable(GL_TEXTURE_2D);

	if (onTexture)
	{
		glEnable(GL_TEXTURE_2D);
	}
	{
		glBindTexture(GL_TEXTURE_2D, starStripTexture);

		glBegin(GL_QUAD_STRIP);
		{
			//glColor3f(0, 0, 1);
			glTexCoord2f(1.0, 0); //right bottom 
			glVertex3f(0.0f, outerRadius, -outerDepth / 2 + interval);
			glTexCoord2f(1.0, 1.0); //right top 
			glVertex3f(0.0f, innerRadius, -innerDepth / 2);

			//glColor3f(0, 0, 0);
			glTexCoord2f(0, 1.0); //left top
			glVertex3f(-outerRadius * cos(45 * PI / 180) / 2, outerRadius * sin(45 * PI / 180) / 2, -outerDepth / 2 + interval);
			glTexCoord2f(0, 0); //left bottom
			glVertex3f(-innerRadius * cos(45 * PI / 180) / 2, innerRadius * sin(45 * PI / 180) / 2, -innerDepth / 2);

			glTexCoord2f(1.0, 0); //right bottom 
			glVertex3f(-outerRadius, 0.0f, -outerDepth / 2 + interval);
			glTexCoord2f(1.0, 1.0); //right top 
			glVertex3f(-innerRadius, 0.0f, -innerDepth / 2);

			//glColor3f(1, 0, 0);
			glTexCoord2f(0, 1.0); //left top
			glVertex3f(-outerRadius * cos(45 * PI / 180) / 2, -outerRadius * sin(45 * PI / 180) / 2, -outerDepth / 2 + interval);
			glTexCoord2f(0, 0); //left bottom
			glVertex3f(-innerRadius * cos(45 * PI / 180) / 2, -innerRadius * sin(45 * PI / 180) / 2, -innerDepth / 2);

			glTexCoord2f(1.0, 0); //right bottom 
			glVertex3f(0.0f, -outerRadius, -outerDepth / 2 + interval);
			glTexCoord2f(1.0, 1.0); //right top 
			glVertex3f(0.0f, -innerRadius, -innerDepth / 2);

			//glColor3f(0, 1, 0);
			glTexCoord2f(0, 1.0); //left top
			glVertex3f(outerRadius * cos(45 * PI / 180) / 2, -outerRadius * sin(45 * PI / 180) / 2, -outerDepth / 2 + interval);
			glTexCoord2f(0, 0); //left bottom
			glVertex3f(innerRadius * cos(45 * PI / 180) / 2, -innerRadius * sin(45 * PI / 180) / 2, -innerDepth / 2);

			glTexCoord2f(1.0, 0); //right bottom 
			glVertex3f(outerRadius, 0.0f, -outerDepth / 2 + interval);
			glTexCoord2f(1.0, 1.0); //right top 
			glVertex3f(innerRadius, 0.0f, -innerDepth / 2);

			//glColor3f(0, 0, 1);
			glTexCoord2f(0, 1.0); //left top
			glVertex3f(outerRadius * cos(45 * PI / 180) / 2, outerRadius * sin(45 * PI / 180) / 2, -outerDepth / 2 + interval);
			glTexCoord2f(0, 0); //left bottom
			glVertex3f(innerRadius * cos(45 * PI / 180) / 2, innerRadius * sin(45 * PI / 180) / 2, -innerDepth / 2);

			glTexCoord2f(1.0, 0); //right bottom 
			glVertex3f(0.0f, outerRadius, -outerDepth / 2 + interval);
			glTexCoord2f(1.0, 1.0); //right top 
			glVertex3f(0.0f, innerRadius, -innerDepth / 2);
		}
	}
	glDisable(GL_TEXTURE_2D);
	glEnd();
}

void drawKunaiStrip(GLfloat radius, GLfloat depth)
{
	glBegin(GL_QUAD_STRIP);
	{
		glColor3f(1, 1, 1);
		glVertex3f(0.0f, radius, -depth / 2);
		glVertex3f(0.0f, radius, depth / 2);

		glColor3f(0, 0, 0);
		glVertex3f(-radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, -depth / 2);
		glVertex3f(-radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, depth / 2);

		glVertex3f(0.0f, -radius, -depth / 2);
		glVertex3f(0.0f, -radius, depth / 2);

		glColor3f(1, 1, 1);
		glVertex3f(radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, -depth / 2);
		glVertex3f(radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, depth / 2);

		glVertex3f(0.0f, radius, -depth / 2);
		glVertex3f(0.0f, radius, depth / 2);
	}
	glEnd();
}

void drawKunai(GLfloat gripRadius, GLfloat gripLength, GLfloat outerRadius, GLfloat innerRadius, GLfloat outerDepth, GLfloat innerDepth)
{
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	GLfloat interval = innerDepth - outerDepth;

	glPushMatrix();
	{
		glTranslatef(0.0f, innerRadius + gripLength, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glColor3f(0, 0, 0);
		gluCylinder(quad, gripRadius, gripRadius, gripLength, kunaiSlices, kunaiStacks);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0.0f, 0.0f, interval);
		drawKunaiStrip(outerRadius, outerDepth);
	}
	glPopMatrix();
	drawKunaiStrip(innerRadius, innerDepth);
	glBegin(GL_QUAD_STRIP);
	{
		glColor3f(1, 1, 1);
		glVertex3f(0.0f, outerRadius, -outerDepth / 2 + interval);
		glVertex3f(0.0f, innerRadius, -innerDepth / 2);

		glColor3f(0, 0, 0);
		glVertex3f(-outerRadius * cos(45 * PI / 180) / 2, outerRadius * sin(45 * PI / 180) / 2, -outerDepth / 2 + interval);
		glVertex3f(-innerRadius * cos(45 * PI / 180) / 2, innerRadius * sin(45 * PI / 180) / 2, -innerDepth / 2);

		glVertex3f(0.0f, -outerRadius, -outerDepth / 2 + interval);
		glVertex3f(0.0f, -innerRadius, -innerDepth / 2);

		glColor3f(1, 1, 1);
		glVertex3f(outerRadius * cos(45 * PI / 180) / 2, outerRadius * sin(45 * PI / 180) / 2, -outerDepth / 2 + interval);
		glVertex3f(innerRadius * cos(45 * PI / 180) / 2, innerRadius * sin(45 * PI / 180) / 2, -innerDepth / 2);

		glVertex3f(0.0f, outerRadius, -outerDepth / 2 + interval);
		glVertex3f(0.0f, innerRadius, -innerDepth / 2);
	}
	glEnd();
}

void draw4Kunais(GLfloat gripRadius, GLfloat gripLength, GLfloat outerRadius, GLfloat innerRadius, GLfloat outerDepth, GLfloat innerDepth, GLfloat intervalx, GLfloat intervaly)
{
	glPushMatrix();
	{
		glTranslatef(-intervalx / 2, intervaly / 2, 0.0f);
		glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
		drawKunai(gripRadius, gripLength, outerRadius, innerRadius, outerDepth, innerDepth);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(-intervalx / 2, -intervaly / 2, 0.0f);
		glRotatef(135.0f, 0.0f, 0.0f, 1.0f);
		drawKunai(gripRadius, gripLength, outerRadius, innerRadius, outerDepth, innerDepth);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(intervalx / 2, -intervaly / 2, 0.0f);
		glRotatef(225.0f, 0.0f, 0.0f, 1.0f);
		drawKunai(gripRadius, gripLength, outerRadius, innerRadius, outerDepth, innerDepth);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(intervalx / 2, intervaly / 2, 0.0f);
		glRotatef(315.0f, 0.0f, 0.0f, 1.0f);
		drawKunai(gripRadius, gripLength, outerRadius, innerRadius, outerDepth, innerDepth);
	}
	glPopMatrix();
}

void drawEyeFrame(GLfloat eyeRadius, GLfloat starOuterRadius, GLfloat starInnerRadius, GLfloat starOuterDepth, GLfloat starInnerDepth, GLfloat gripRadius, GLfloat gripLength, GLfloat kunaiOuterRadius, GLfloat kunaiInnerRadius, GLfloat kunaiOuterDepth, GLfloat kunaiInnerDepth, GLfloat intervalx, GLfloat intervaly)
{
	GLUquadricObj* quad = gluNewQuadric();

	glPushMatrix();
	{
		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glTranslatef(0.0f, 0.0f, eyeRadius / 2);
			//glColor3f(1.0f, 1.0f, 0.0f);
			glBindTexture(GL_TEXTURE_2D, eyeTexture);
			gluQuadricTexture(quad, GL_TRUE);
			gluSphere(quad, eyeRadius, 20, 20);
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0.0f, 0.0f, -starInnerDepth / 2);
		draw4PointedStar(starOuterRadius, starInnerRadius, starOuterDepth, starInnerDepth);
	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(0.0f, 0.0f, -starInnerDepth);
		draw4Kunais(gripRadius, gripLength, kunaiOuterRadius, kunaiInnerRadius, kunaiOuterDepth, kunaiInnerDepth, intervalx, intervaly);
	}
	glPopMatrix();
}

void drawEyeTube(GLfloat radius, GLfloat height)
{
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);

	if (onTexture)
	{
		glEnable(GL_TEXTURE_2D);
	}
	{
		glBindTexture(GL_TEXTURE_2D, lavaStoneTexture);
		gluQuadricTexture(quad, GL_TRUE);
		//glColor3f(0, 0, 0);
		gluCylinder(quad, radius, radius, height, tubeSlices, tubeStacks);

		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, height);
			gluQuadricTexture(quad, GL_TRUE);
			gluDisk(quad, 0.0f, radius, tubeSlices, tubeStacks);
		}
		glPopMatrix();

	}
	glDisable(GL_TEXTURE_2D);


	if (onTexture)
	{
		glEnable(GL_TEXTURE_2D);
	}
	{
		//glColor3f(0, 0, 1);
		glBindTexture(GL_TEXTURE_2D, metal2Texture);
		gluQuadricTexture(quad, GL_TRUE);
		gluDisk(quad, radius * 3 / 4, radius, tubeSlices, tubeStacks);
		gluQuadricTexture(quad, GL_TRUE);
		drawEyeFrame(0.05f, 0.2f, 0.15f, 0.01f, 0.015f, 0.01f, 0.05f, 0.065f, 0.045f, 0.01f, 0.015f, 0.125f, 0.125f);
	}
	glDisable(GL_TEXTURE_2D);


	//glColor3f(0, 1, 1); //Yellow eye 

	glPushMatrix();
	{
		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glTranslatef(0.0f, 0.0f, height / 6);
			glBindTexture(GL_TEXTURE_2D, metalTexture);
			gluQuadricTexture(quad, GL_TRUE);
			gluCylinder(quad, radius + 0.01, radius + 0.01, height * 5 / 6, tubeSlices, tubeStacks);
			//glColor3f(0, 0, 1);

			gluQuadricTexture(quad, GL_TRUE);
			gluDisk(quad, radius, radius + 0.01, tubeSlices, tubeStacks);
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

void drawHead(GLdouble radius, GLfloat trimRadius, GLfloat translatex, GLfloat translatey, GLfloat translatez, GLfloat degree, GLfloat rotatex, GLfloat rotatey, GLfloat rotatez, GLdouble headTrim[4])
{
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);



	// Trimmed Head
	glPushMatrix();
	{
		// 6. Translate the head
		glTranslatef(translatex, translatey, translatez + trimRadius / 2);
		// 5. Rotate the front face
		glRotatef(degree, rotatex, rotatey, rotatez);
		glPushMatrix();
		{
			// 4. Turn the front face of the head to back to front
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			// 3. Translate the head down trimRadius
			glTranslatef(0.0f, -(radius - trimRadius), 0.0f);
			glPushMatrix();
			{
				glEnable(GL_CLIP_PLANE0);
				{
					glClipPlane(GL_CLIP_PLANE0, headTrim);
					// 2. Translate the head up 'radius - trimRadius', trim 'trimRadius' of the front face
					glTranslatef(0.0f, radius - trimRadius, 0.0f);
					// 1. Turn the front face of the head to bottom before trim
					glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
					//gluQuadricDrawStyle(quad, GLU_LINE);
					//glColor3f(1.0f, 0.0f, 0.0f);
					//gluSphere(quad, radius + 0.005, headSlices, headStacks);
					//gluQuadricDrawStyle(quad, GLU_FILL);
					//glColor3f(1.0f, 0.0f, 1.0f);


					if (onTexture)
					{
						glEnable(GL_TEXTURE_2D);
					}
					{
						glBindTexture(GL_TEXTURE_2D, mothEye2Texture);
						gluQuadricTexture(quad, GL_TRUE);
						gluSphere(quad, radius, headSlices, headStacks);
					}
					glDisable(GL_TEXTURE_2D);
				}
				glDisable(GL_CLIP_PLANE0);

			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();


	// Eye Tube
	glPushMatrix();
	{
		gluQuadricTexture(quad, GL_TRUE);
		glTranslatef(translatex, translatey + trimRadius * 4, translatez - trimRadius - radius);
		drawEyeTube(sqrt(pow(radius, 2) - pow(radius - trimRadius, 2)), radius);
	}
	glPopMatrix();
}

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
			// mode == 6 triggered
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
			glColor3f(1, 1, 1);
			gluSphere(quad, baseRadius, tubeSlices, tubeStacks);
			glPushMatrix();
			{
				// C1
				// mode == 6 triggered
				glRotatef(rotateFinger, 1.0f, 0.0f, 0.0f);
				glTranslatef(0.0f, 0.0f, -part1Length);
				glColor3f(0, 0, 0);
				gluCylinder(quad, centerRadius, baseRadius, part1Length, tubeSlices, tubeStacks);
				// S2
				glColor3f(1, 1, 1);
				gluSphere(quad, centerRadius, tubeSlices, tubeStacks);
				glPushMatrix();
				{
					// C2
					// mode == 6 triggered
					glRotatef(rotateFinger, 1.0f, 0.0f, 0.0f);
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
			leftBasedRadius = -basedRadius * 3;
			rightBasedRadius = basedRadius * 2;
		}
		else
		{
			leftBasedRadius = -basedRadius * 2;
			rightBasedRadius = basedRadius * 3;
		}

		// Wrist
		glColor3f(0, 1, 0);
		glBegin(GL_QUADS);
		{
			if (isRight)
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
			else
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
				glTranslatef(basedRadius, 0.0f, 0.0f);
			}
			else
			{
				glTranslatef(-basedRadius, 0.0f, 0.0f);
			}
			drawFinger(basedRadius, topRadius, halfLength * 0.85, false, isRight);
			// Tumb
			if (isRight)
			{
				glTranslatef(basedRadius, halfLength * 0.5, 0.0f);
				glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
				glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			}
			else
			{
				glTranslatef(-basedRadius, halfLength * 0.5, 0.0f);
			}
			drawFinger(basedRadius, topRadius, halfLength * 0.7, true, isRight);
		}
		glPopMatrix();
		glPushMatrix();
		{
			// Ring Finger
			if (isRight)
			{
				glTranslatef(-basedRadius, 0.0f, 0.0f);
			}
			else
			{
				glTranslatef(basedRadius, 0.0f, 0.0f);
			}
			drawFinger(basedRadius, topRadius, halfLength * 0.85, false, isRight);
			// Little Finger
			if (isRight)
			{
				glTranslatef(-basedRadius, 0.0f, 0.0f);
			}
			else
			{
				glTranslatef(basedRadius, 0.0f, 0.0f);
			}
			drawFinger(basedRadius, topRadius, halfLength * 0.7, false, isRight);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawShieldPlane(GLfloat radius, GLfloat bulge)
{
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
	gluQuadricDrawStyle(quad, GLU_FILL);

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

// No use
void drawHand(GLfloat basedRadius, GLfloat topRadius, GLfloat totalLength, boolean isRight)
{
	GLfloat totalDepth = 0.05f;

	drawPalm(basedRadius, topRadius, totalLength, isRight);
	glPushMatrix();
	{
		if (isRight)
		{
			glTranslatef(-basedRadius / 2, totalLength * sin(45 * PI / 180) * 3 / 4, -totalDepth);
		}
		else
		{
			glTranslatef(basedRadius / 2, totalLength * sin(45 * PI / 180) * 3 / 4, -totalDepth);
		}
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(180.0f, 0.0f, 1.0f, 0.05f);
		drawShield(totalLength * 1.25, totalDepth);
	}
	glPopMatrix();
}

void drawWheel(GLfloat radius, GLfloat height)
{
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);

	if (mode != 0)
	{
		if (rotateWheel > 360.0f)
		{
			rotateWheel = 0.0f;
		}
		else
		{
			if (rotateWheel == 0.0f)
			{
				rotateWheel += 0.01f;
			}
			rotateWheel += rotateWheel * elapsedSeconds;
		}
	}
	glPushMatrix();
	{
		glRotatef(rotateWheel, 1.0f, 0.0f, 0.0f);
		glPushMatrix();
		{
			glTranslatef(-height / 2, 0.0f, 0.0f);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			glColor3f(1, 0.8, 0.5);
			gluDisk(quad, 0.0f, radius, tubeSlices, tubeStacks);
			glColor3f(1, 0.9, 0);
			gluCylinder(quad, radius, radius, height, tubeSlices, tubeStacks);
			//glColor3f(1, 1, 1);
			//gluQuadricDrawStyle(quad, GLU_LINE);
			//gluCylinder(quad, radius + 0.005, radius + 0.005, height + 0.005, tubeSlices, tubeStacks);
			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, height);
				glColor3f(1, 0.8, 0.5);
				//gluQuadricDrawStyle(quad, GLU_FILL);
				gluDisk(quad, 0.0f, radius, tubeSlices, tubeStacks);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawNail(GLfloat radius, GLfloat height)
{
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);

	glPushMatrix();
	{
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, -height * 9 / 16);
			glColor3f(1, 0, 0);
			gluDisk(quad, 0.0f, radius * 0.9, tubeSlices, tubeStacks);
			gluCylinder(quad, radius * 0.9, radius * 0.9, height / 16, tubeSlices, tubeStacks);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, -height / 2);
			glColor3f(0, 1, 0);
			gluDisk(quad, 0.0f, radius * 1.1, tubeSlices, tubeStacks);
			gluCylinder(quad, radius * 1.1, radius * 1.1, height / 6, tubeSlices, tubeStacks);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, -height / 3);
			gluDisk(quad, radius, radius * 1.1, tubeSlices, tubeStacks);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, -height / 6);
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
			drawWheel(radius, height / 3);
		}
		glPopMatrix();
		glColor3f(0, 0, 1);
		gluCylinder(quad, radius / 2, radius / 2, height / 4, tubeSlices, tubeStacks);
		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, height * 3 / 8);
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
			drawWheel(radius, height / 4);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawShoulder(GLfloat height, GLfloat shieldDepth, boolean isRight) {

	glPushMatrix();
	{
		if (!isRight)
		{
			//glTranslatef(-height * 7 / 4, 0.0f, 0.0f);
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		}
		else
		{
			//glTranslatef(height * 7 / 4, 0.0f, 0.0f);
		}
		glBegin(GL_QUADS);
		{
			// Top Face
			glColor3f(0, 0, 0);
			glVertex3f(-height / 2, height / 2, height * 3 / 8);
			glVertex3f(-height / 2, height / 2, -height * 3 / 8);
			glVertex3f(height / 4, height / 2, -height * 3 / 8);
			glVertex3f(height / 4, height / 2, height * 3 / 8);

			// Front Top Slope
			glColor3f(0, 0, 1);
			glVertex3f(-height / 2, height / 2, -height * 3 / 8);
			glVertex3f(-height * 3 / 4, height * 1 / 6, -height / 2);
			glVertex3f(height / 2, height * 1 / 6, -height / 2);
			glVertex3f(height / 4, height / 2, -height * 3 / 8);

			// Back Top Slope
			glColor3f(0, 1, 0);
			glVertex3f(-height / 2, height / 2, height * 3 / 8);
			glVertex3f(-height * 3 / 4, height * 1 / 6, height / 2);
			glVertex3f(height / 2, height * 1 / 6, height / 2);
			glVertex3f(height / 4, height / 2, height * 3 / 8);

			// Left Top Slope
			glColor3f(0, 1, 1);
			glVertex3f(-height / 2, height / 2, height * 3 / 8);
			glVertex3f(-height * 3 / 4, height * 1 / 6, height / 2);
			glVertex3f(-height * 3 / 4, height * 1 / 6, -height / 2);
			glVertex3f(-height / 2, height / 2, -height * 3 / 8);

			// Left Face
			glColor3f(1, 0, 0);
			glVertex3f(-height * 3 / 4, height * 1 / 6, height / 2);
			glVertex3f(-height * 3 / 4, -height / 2, height / 2);
			glVertex3f(-height * 3 / 4, -height / 2, -height / 2);
			glVertex3f(-height * 3 / 4, height * 1 / 6, -height / 2);

			// Right Bottom Slope
			glColor3f(1, 0, 1);
			glVertex3f(height * 3 / 4, -height * 1 / 6, -height / 2);
			glVertex3f(height / 4, -height / 2, -height / 2);
			glVertex3f(height / 4, -height / 2, height / 2);
			glVertex3f(height * 3 / 4, -height * 1 / 6, height / 2);

			// Bottom Face
			glColor3f(1, 1, 0);
			glVertex3f(-height * 3 / 4, -height / 2, height / 2);
			glVertex3f(-height * 3 / 4, -height / 2, -height / 2);
			glVertex3f(height / 4, -height / 2, -height / 2);
			glVertex3f(height / 4, -height / 2, height / 2);
		}
		glEnd();

		glBegin(GL_POLYGON);
		{
			// Front Face
			glColor3f(1, 1, 1);
			glVertex3f(-height * 3 / 4, height * 1 / 6, -height / 2);
			glVertex3f(-height * 3 / 4, -height / 2, -height / 2);
			glVertex3f(height / 4, -height / 2, -height / 2);
			glVertex3f(height * 3 / 4, -height * 1 / 6, -height / 2);
			glVertex3f(height / 2, height * 1 / 6, -height / 2);
		}
		glEnd();

		glBegin(GL_POLYGON);
		{
			// Back Face
			glColor3f(0, 0, 0.5);
			glVertex3f(-height * 3 / 4, height * 1 / 6, height / 2);
			glVertex3f(-height * 3 / 4, -height / 2, height / 2);
			glVertex3f(height / 4, -height / 2, height / 2);
			glVertex3f(height * 3 / 4, -height * 1 / 6, height / 2);
			glVertex3f(height / 2, height * 1 / 6, height / 2);
		}
		glEnd();

		glBegin(GL_POLYGON);
		{
			// Right Top Slope
			glColor3f(0.5, 0, 0);
			glVertex3f(height / 4, height / 2, -height * 3 / 8);
			glVertex3f(height / 2, height * 1 / 6, -height / 2);
			glVertex3f(height * 3 / 4, -height * 1 / 6, -height / 2);
			glVertex3f(height * 3 / 4, -height * 1 / 6, height / 2);
			glVertex3f(height / 2, height * 1 / 6, height / 2);
			glVertex3f(height / 4, height / 2, height * 3 / 8);
		}
		glEnd();

		glPushMatrix();
		{
			//glTranslatef(-height / 3, -height / 7, -height / 2);
			if (isRight)
			{
				glTranslatef(-height / 3, -height / 7, -height / 2);
			}
			else
			{
				glTranslatef(-height / 3, -height / 7, height / 2);
			}
			drawWheel(height / 4, height / 6);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(-height / 8, height * 7 / 8, 0.0f);
			drawNail(height * 3 / 8, height * 3 / 4);
		}
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	{
		if (isRight)
		{
			glTranslatef(height * 3 / 4 + shieldDepth / 3, shieldDepth / 2, 0.0f);
			glRotatef(-135.0, 0.0f, 0.0f, 1.0f);
			glRotatef(90.0, 0.0f, 1.0f, 0.0f);
		}
		else
		{
			glTranslatef(-height * 3 / 4 - shieldDepth / 3, shieldDepth / 2, 0.0f);
			glRotatef(135.0, 0.0f, 0.0f, 1.0f);
			glRotatef(-90.0, 0.0f, 1.0f, 0.0f);
		}
		drawShield(height, shieldDepth);
	}
	glPopMatrix();
}

void drawArm(GLfloat baseRadius, GLfloat topRadius, GLfloat totalLength, boolean isRight)
{
	// Arm: S1=C1=S2=C2=S3=C3
	// S1, S2, S3 = shepere joints
	// C1 + C2 = cylinder arms
	GLfloat partLength = totalLength / 2;
	GLfloat centerRadius = (baseRadius + topRadius) / 2;
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);

	if (isRight)
	{
		//glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	}
	else
	{
		glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
	}

	glPushMatrix();
	{
		// S1
		glTranslatef(0.0f, 0.0f, -partLength);
		glRotatef(35.0f, 0.0f, 1.0f, 0.0f);
		glColor3f(1, 1, 1);
		gluSphere(quad, baseRadius, tubeSlices, tubeStacks);
		//C1
		// mode == 7 triggered
		glRotatef(rotateUpperArmJointX, 1.0f, 0.0f, 0.0f);
		glRotatef(rotateUpperArmJointY, 0.0f, 1.0f, 0.0f);
		glRotatef(rotateUpperArmJointZ, 0.0f, 0.0f, 1.0f);
		if (!isRight)
		{
			glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
		}
		glColor3f(0, 0, 0);
		gluCylinder(quad, baseRadius, centerRadius, partLength * 3 / 4, tubeSlices, tubeStacks);
		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, partLength * 3 / 4);
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
			// S2
			glColor3f(1, 1, 1);
			gluSphere(quad, centerRadius, tubeSlices, tubeStacks);
			// C2
			// mode == 7 triggered
			if (isRight)
			{
				glRotatef(rotateElbowJointX, 1.0f, 0.0f, 0.0f);
				glRotatef(rotateElbowJointY, 0.0f, 1.0f, 0.0f);
				glRotatef(rotateElbowJointZ, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				glRotatef(-rotateElbowJointX, 1.0f, 0.0f, 0.0f);
				glRotatef(-rotateElbowJointY, 0.0f, 1.0f, 0.0f);
				glRotatef(-rotateElbowJointZ, 0.0f, 0.0f, 1.0f);
				//glRotatef(-55.0f, 0.0f, 1.0f, 0.0f);
			}
			glColor3f(0, 0, 0);
			gluCylinder(quad, centerRadius, topRadius, partLength, tubeSlices, tubeStacks);
			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, partLength);
				// S3
				glColor3f(1, 1, 1);
				gluSphere(quad, topRadius, tubeSlices, tubeStacks);
				// C3
				// mode == 7 triggered
				glRotatef(rotateWristJointX, 1.0f, 0.0f, 0.0f);
				glRotatef(rotateWristJointY, 0.0f, 1.0f, 0.0f);
				glRotatef(rotateWristJointZ, 0.0f, 0.0f, 1.0f);
				glColor3f(0, 0, 0);
				gluCylinder(quad, topRadius, 0.0f, partLength / 4, tubeSlices, tubeStacks);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawCompleteArms(GLfloat shoulderHeight, GLfloat shieldDepth, GLfloat armLength, GLfloat wristJointRadius, boolean isRight)
{
	// Shoulders
	glPushMatrix();
	{
		// mode == 7 triggered
		//glRotatef(rotateShoulderJointX, 1.0f, 0.0f, 0.0f);
		//glRotatef(rotateShoulderJointY, 0.0f, 1.0f, 0.0f);
		//glRotatef(rotateShoulderJointZ, 0.0f, 0.0f, 1.0f);
		if (isRight)
		{
			glRotatef(rotateShoulderJointX, 1.0f, 0.0f, 0.0f);
			glRotatef(rotateShoulderJointY, 0.0f, 1.0f, 0.0f);
			glRotatef(rotateShoulderJointZ, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			glRotatef(-rotateShoulderJointX, 1.0f, 0.0f, 0.0f);
			glRotatef(-rotateShoulderJointY, 0.0f, 1.0f, 0.0f);
			glRotatef(-rotateShoulderJointZ, 0.0f, 0.0f, 1.0f);
		}
		drawShoulder(shoulderHeight, shieldDepth, isRight);

		// Arm
		GLfloat shouderJointRadius = shoulderHeight / 4; //0.65 / 8
		glPushMatrix();
		{
			if (isRight)
			{
				glTranslatef(-shoulderHeight * 1 / 4, 0.0f, 0.0f);
			}
			else
			{
				glTranslatef(shoulderHeight * 1 / 4, 0.0f, 0.0f);
			}
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0, 0.0, armLength / 2);
			drawArm(shouderJointRadius, wristJointRadius, armLength, isRight);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawBody(GLenum mode, GLfloat radius, int sliceNo, int stackNo)
{
	GLfloat x, y, z, slice, stack;
	for (slice = 0.0; slice < 2 * PI; slice += PI / sliceNo)
	{
		glBegin(mode);
		for (stack = 0.0; stack < 2 * PI; stack += PI / stackNo)
		{
			x = radius * cos(stack) * sin(slice);
			y = radius * sin(stack) * sin(slice);
			z = radius * cos(slice);
			glVertex3f(x, y, z);
			x = radius * cos(stack) * sin((double)slice + PI / stackNo);
			y = radius * sin(stack) * sin((double)slice + PI / sliceNo);
			z = radius * cos((double)slice + PI / sliceNo);
			glVertex3f(x, y, z);
		}
		glEnd();
	}

	// GLU Sphere
	//GLUquadricObj* sphere = gluNewQuadric();
	//gluQuadricDrawStyle(sphere, GLU_LINE);
	//gluSphere(sphere, 0.5, 10, 10);
	//gluDeleteQuadric(sphere);
}

void drawRobot()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	drawBody(GL_LINE_STRIP, 0.5f, 30, 30);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//drawBody(GL_LINE_STRIP, 0.1f, 50, 50);
}

//glBindTexture(GL_TEXTURE_2D, texTexture);
//gluQuadricTexture(obj, GL_TRUE);
void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	{
		gluLookAt(
			eyeX, eyeY, eyeZ,	// Eye Position
			lookAtX, lookAtY, lookAtZ,	// Look At
			upX, upY, upZ);	// World Up

		//glOrtho(2, -2, -2, 2, -2, 2);
		//gluPerspective(fovy, aspect, near, far);
		//glRotatef(0.01f, 1, 1, 1);
		//glBegin(GL_QUADS);
		//glColor3f(0.0f, 0.0f, 0.0f);
		//glVertex3f(-0.25f, 0.25f, 0.25f);
		//glVertex3f(0.25f, 0.25f, 0.25f);
		//glVertex3f(0.25f, -0.25f, 0.25f);
		//glVertex3f(-0.25f, -0.25f, 0.25f);

		//glColor3f(1.0f, 0.0f, 0.0f);
		//glVertex3f(0.25f, 0.25f, 0.25f);
		//glVertex3f(0.25f, 0.25f, -0.25f);
		//glVertex3f(0.25f, -0.25f, -0.25f);
		//glVertex3f(0.25f, -0.25f, 0.25f);

		//glColor3f(1.0f, 1.0f, 0.0f);
		//glVertex3f(0.25f, 0.25f, -0.25f);
		//glVertex3f(-0.25f, 0.25f, -0.25f);
		//glVertex3f(-0.25f, -0.25f, -0.25f);
		//glVertex3f(0.25f, -0.25f, -0.25f);

		//glColor3f(1.0f, 1.0f, 1.0f);
		//glVertex3f(-0.25f, 0.25f, -0.25f);
		//glVertex3f(-0.25f, 0.25f, 0.25f);
		//glVertex3f(-0.25f, -0.25f, 0.25f);
		//glVertex3f(-0.25f, -0.25f, -0.25f);

		//glColor3f(0.0f, 1.0f, 1.0f);
		//glVertex3f(-0.25f, 0.25f, -0.25f);
		//glVertex3f(-0.25f, 0.25f, 0.25f);
		//glVertex3f(0.25f, 0.25f, 0.25f);
		//glVertex3f(0.25f, 0.25f, -0.25f);

		//glColor3f(0.0f, 0.0f, 1.0f);
		//glVertex3f(-0.25f, -0.25f, -0.25f);
		//glVertex3f(0.25f, -0.25f, -0.25f);
		//glVertex3f(0.25f, -0.25f, 0.25f);
		//glVertex3f(-0.25f, -0.25f, 0.25f);
		//glEnd();

		// Clip GLU Sphere
		//GLUquadricObj* sphere = gluNewQuadric();
		//glRotatef(0.01f, 1, 1, 1);
		//gluQuadricDrawStyle(sphere, GLU_FILL);
		//glColor3f(1.0f, 1.0f, 1.0f);
		//gluSphere(sphere, 0.1, 50, 50);
		//GLdouble bodyTrim[4] = { 0.0, 1.0, 0.0, 0.0 };
		//GLdouble eqn2[4] = { 0.0, 0.0, 0.0, 0.0 };
		//GLdouble eqn3[4] = { 0.0, 0.0, 1.0, 0.0 };
		//glPushMatrix();
		//glTranslatef(0.0f, 0.3f, 0.0f);
		//glRotatef(110.0f, 1.0f, 0.0f, 0.0f);
		//glEnable(GL_CLIP_PLANE0);
		//glClipPlane(GL_CLIP_PLANE0, bodyTrim);
		//glTranslatef(0.0f, 0.275f, 0.0f);
		//glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		//glPushMatrix();
		//gluQuadricDrawStyle(sphere, GLU_LINE);
		//glColor3f(1.0f, 0.0f, 0.0f);
		//gluSphere(sphere, 0.305, 50, 50);
		//gluQuadricDrawStyle(sphere, GLU_FILL);
		//glColor3f(1.0f, 1.0f, 1.0f);
		//gluSphere(sphere, 0.3, 50, 50);
		//glDisable(GL_CLIP_PLANE0);
		//glPopMatrix();
		//glPopMatrix();
		//glDisable(GL_CLIP_PLANE0);

		//gluSphere(sphere, 0.5, 50, 50);
		//gluQuadricDrawStyle(sphere, GLU_FILL);
		//gluDeleteQuadric(sphere);
		//drawRobot();

		//glRotatef(1.0f, 1, 1, 1);
		//drawS(0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0001f, 1.0f, 1.0f, 1.0f);

		//glPushMatrix();
		//glRotatef(objectRotateX, 1.0f, 0.0f, 0.0f);
		//glRotatef(objectRotateY, 0.0f, 1.0f, 0.0f);
		//glRotatef(objectRotateZ, 0.0f, 0.0f, 1.0f);
		// Center point
		GLUquadricObj* quad = gluNewQuadric();
		gluQuadricDrawStyle(quad, GLU_FILL);
		glColor3f(1, 1, 1);
		glPushMatrix();
		{
			//glScalef(20, 20, 20);
			gluSphere(quad, 0.005, 20, 20);
		}
		glPopMatrix();

		// Head
		GLdouble headTrim[4] = { 0.0, 1.0, 0.0, 0.0 };
		drawHead(0.325f, 0.025f, 0.0f, 0.0f, 0.0f, 20.0f, 1.0f, 0.0f, 0.0f, headTrim);

		// Eye
		//drawEyeTube(sqrt(pow(0.325f, 2) - pow(0.325f - 0.025f, 2)), 0.35f);
		//drawEye(0.325f, 0.025f, 0.0f, 0.3f, 0.0f);
		//drawEyeFrame(0.2f, 0.15f, 0.01f, 0.015f, 0.01f, 0.05f, 0.065f, 0.045f, 0.01f, 0.015f, 0.125f, 0.125f);

		// Palm
		//drawPalm(0.0175f, 0.0125f, 0.085f);

		// Shield
		//drawShield(0.085f, 0.05f);

		// Hands
		//glPushMatrix();
		//glScalef(5, 5, 5);
		//glPushMatrix();
		//glTranslatef(-0.1f, 0.0f, 0.0f);
		//drawHand(true);
		//glPopMatrix();
		//glPushMatrix();
		//glTranslatef(0.1f, 0.0f, 0.0f);
		//drawHand(false);
		//glPopMatrix();
		//glPopMatrix();

		// Hands
		//GLfloat partLength = 0.325;
		//GLfloat wristJointRadius = 0.0175f;
		//GLfloat fingerTipRadius = 0.0125f;
		//GLfloat plamLength = 0.085f;
		//boolean isRight = false;
		//glPushMatrix();
		//{
		//	//glTranslatef(-0.3f, 0.0f, 0.0f);
		//	// mode == 7 triggered
		//	glRotatef(rotateUpperArmJointX, 1.0f, 0.0f, 0.0f);
		//	glRotatef(rotateUpperArmJointY, 0.0f, 1.0f, 0.0f);
		//	glRotatef(rotateUpperArmJointZ, 0.0f, 0.0f, 1.0f);
		//	glRotatef(rotateElbowJointX, 1.0f, 0.0f, 0.0f);
		//	glRotatef(rotateElbowJointY, 0.0f, 1.0f, 0.0f);
		//	glRotatef(rotateElbowJointZ, 0.0f, 0.0f, 1.0f);
		//	if (isRight)
		//	{
		//		glTranslatef(wristJointRadius, 0.0f, partLength + plamLength);
		//	}
		//	else
		//	{
		//		glTranslatef(-wristJointRadius, 0.0f, partLength + plamLength);
		//	}
		//	glRotatef(90.0, -1.0, 0.0, 0.0);
		//	drawPalm(wristJointRadius, fingerTipRadius, plamLength, isRight);
		//	//drawHand(wristJointRadius, fingerTipRadius, handLength, isRight);
		//}
		//glPopMatrix();

		// Wheel
		//drawWheel(0.325 / 2, 0.1f);

		// Nail
		//drawNail(0.325 / 2, 0.325);

		// Shoulders
		//drawShoulder(0.325, true);
		//drawShoulder(0.325, false);

		// Arm
		//glPushMatrix();
		//glTranslatef(0.0, 0.0, 0.65 / 2);
		//drawArm(0.65f / 8, 0.65f / 8, 0.65f);
		//glPopMatrix();
		//glPopMatrix();

		// 4 Arms
		GLfloat shoulderHeight = 0.325f / 2; // 0.325f
		GLfloat shieldDepth = 0.25f;
		GLfloat armLength = 0.65f;
		GLfloat wristJointRadius = 0.0175f;
		//	Right Front
		glPushMatrix();
		{
			glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.325f + 0.325 / 4, 0.0f, -0.325f / 4);
			drawCompleteArms(shoulderHeight, shieldDepth, armLength, wristJointRadius, true);
		}
		glPopMatrix();

		//	Right Back
		glPushMatrix();
		{
			glRotatef(-25.0f, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.325f + 0.325 / 4, 0.0f, 0.325f / 4);
			drawCompleteArms(shoulderHeight, shieldDepth, armLength, wristJointRadius, true);
		}
		glPopMatrix();

		//	Left Front
		glPushMatrix();
		{
			glRotatef(-25.0f, 0.0f, 1.0f, 0.0f);
			glTranslatef(-0.325f - 0.325 / 4, 0.0f, -0.325f / 4);
			drawCompleteArms(shoulderHeight, shieldDepth, armLength, wristJointRadius, false);
		}
		glPopMatrix();

		//	Left Back
		glPushMatrix();
		{
			glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
			glTranslatef(-0.325f - 0.325 / 4, 0.0f, 0.325f / 4);
			drawCompleteArms(shoulderHeight, shieldDepth, armLength, wristJointRadius, false);
		}
		glPopMatrix();

		//glPushMatrix();
		//glScalef(1.5, 1.5, 1.5);
		//glPopMatrix();
	}
	glPopMatrix();
	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}

void setupCamera()
{
	//#pragma region View to Project
	glMatrixMode(GL_PROJECTION);
	glOrtho(-3, 3, -3, 3, -3, 3);
	//eyeZ -= 5.0f;
	//# pragma endregion
}

GLfloat ambientLight0[4] = { 0, 0, 0, 1 };
GLfloat diffuseLight0[4] = { 1, 1, 1, 1 };
//GLfloat specularLight0[4] = { 0.5, 0.5, 0.5, 1 };
//GLfloat positionLight0[4] = { 0, 10, 0, 0 };
//GLfloat ambientLight1[4] = { 1, 1, 1, 1 };
//GLfloat diffuseLight1[4] = { 1, 1, 1, 1 };
//GLfloat specularLight1[4] = { 0.5, 0.5, 0.5, 1 };
//GLfloat positionLight1[4] = { 0, -10, 0, 0 };

void setupLighting()
{
	//// Q0
	//// Which Light - What Property - What Value for the Property
	//// AMBIENT DEFAULT 0, 0, 0, 1
	//// DIFFUSE DEFAULT 1, 1, 1, 1
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	//glLightfv(GL_LIGHT0, GL_POSITION, specularLight0);
	//// Move Light Position to display() for Furter Control
	//glLightfv(GL_LIGHT0, GL_POSITION, positionLight0);
	//glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, positionLight0);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, positionLight0);
	//glEnable(GL_LIGHT0);

	////glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	////glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	////glLightfv(GL_LIGHT0, GL_POSITION, specularLight1);
	////glLightfv(GL_LIGHT1, GL_POSITION, positionLight1);
	////glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHTING);
	//// Use glColor3f() as material
	////glEnable(GL_COLOR_MATERIAL);

	// Q1
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
}

void setupGLProperties()
{
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonMode(GL_BACK, GL_LINE);
}

void setupTextures(LPCSTR filename, GLuint* texture) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// Load Bitmap
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	//GLuint textures[10];
	//glGenTextures(10, &textures);
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	// GL_CLAMP or GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// GL_LINEAR or GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	DeleteObject(hBMP);
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
	setupLighting();
	setupCamera();
	//setupTextures(texFile, &texTexture);

#pragma region TextureMapping

	setupTextures("starStrip.bmp", &starStripTexture);
	setupTextures("head.bmp", &headTexture);
	setupTextures("eye.bmp", &eyeTexture);
	setupTextures("metal.bmp", &metalTexture);
	setupTextures("metal2.bmp", &metal2Texture);
	setupTextures("metal4.bmp", &metal4Texture);
	setupTextures("lavaStone.bmp", &lavaStoneTexture);
	setupTextures("leg.bmp", &legTexture);
	setupTextures("leg2.bmp", &leg2Texture);
	setupTextures("leg3.bmp", &leg3Texture);
	setupTextures("legTip.bmp", &legTipTexture);
	setupTextures("lavaStone2.bmp", &lavaStone2Texture);
	setupTextures("neck.bmp", &neckTexture);
	setupTextures("mothEye.bmp", &mothEyeTexture);
	setupTextures("mothEye2.bmp", &mothEye2Texture);


#pragma endregion

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