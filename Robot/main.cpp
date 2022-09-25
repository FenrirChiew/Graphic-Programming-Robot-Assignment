#include <Windows.h>
#include <Windowsx.h>
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
float camRotation[3] = { 0.0f, 0.0f, 0.0f };
float lastX = 0.0f;
float lastY = 0.0f;

// Robot Attributes
GLint headSlices = 50;
GLint headStacks = 50;
GLint tubeSlices = 20;
GLint tubeStacks = 20;
GLint kunaiSlices = 10;
GLint kunaiStacks = 10;
GLfloat speed = 15.0f;

// Projection Attributes
boolean isOrtho = true;
GLfloat r = -2.5f;
GLfloat angle = 360;
GLfloat eyeX = r * sin(angle * 3.142 / 180.0);
GLfloat eyeY = r;
GLfloat eyeZ = r * cos(angle * 3.142 / 180.0);
GLfloat lookAtX = 0.0f;
GLfloat lookAtY = -4.0f;
GLfloat lookAtZ = 0.0f;
GLfloat upX = 0.0f;
GLfloat upY = 1.0f;
GLfloat upZ = 0.0f;
GLfloat cameraSpeed = 10.0f;

// Light Attributes
boolean onLight = true;
boolean isDay = true;
GLfloat blackLight[4] = { 0, 0, 0, 1 };
GLfloat whiteLight[4] = { 1, 1, 1, 1 };
GLfloat greyLight[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat positionLight[4] = { 0.0f, 0.f, 0.0f, 0.0f };

// Texture Attributes
BITMAP BMP;
HBITMAP hBMP = NULL;
boolean onTexture = true;
GLint textureTheme = 1;
GLint environmentTheme = 1;

// Texture Initiliazation --> Theme 1
GLuint blueStripTexture = 0;
GLuint blackMetalTexture = 0;
GLuint blueLightRayTexture = 0;
GLuint blackWthBlueStripTexture = 0;
GLuint legTipTexture = 0;
GLuint jointMetalTexture = 0;
GLuint bodyTexture = 0;
GLuint wheelReactorTexture = 0;
GLuint darkBrownTexture = 0;
GLuint shoulderTexture = 0;
GLuint darkGreyMetalTexture = 0;
GLuint armorTexture = 0;
GLuint armorSpikeTexture = 0;

// Damage Texture Theme --> Theme 2
GLuint damagebodyTexture = 0;
GLuint damageLegTexture = 0;
GLuint damageShoulderTexture = 0;
GLuint damageLowerShoulderTexture = 0;
GLuint damageEyeTexture = 0;

// Stand 
GLuint upperStandTexture = 0;

// Gun Texture
GLuint gunMetalTexture = 0;
GLuint gunMegazineTexture = 0;
GLuint gunTriggerTexture = 0;
GLuint bulletTexture = 0;

// Finger
GLuint purpleMetalTexture = 0;
GLuint blackPalmTexture = 0;
GLuint goldPalmTexture = 0;
GLuint wristTexture = 0;
GLuint silverMetalTexture = 0;

// Environment
GLuint ruinTexture = 0;
GLuint ruin2Texture = 0;
GLuint ruinFloorTexture = 0;
GLuint ruinFloor2Texture = 0;
GLuint ruinSkyTexture = 0;
GLuint ruinSky2Texture = 0;

// State Attributes
boolean onRest = true;
boolean firstRest = true;
GLfloat restSpeed = 0.0f;
boolean isStop = true;
GLfloat rotateWheel = 0.0f;

// Limb Attributes
GLfloat rotateShoulderJointX = 0.0f;
GLfloat rotateShoulderJointY = 0.0f;
GLfloat rotateShoulderJointZ = -5.0f;
GLfloat rotateUpperArmJointX = 0.0f;
GLfloat rotateUpperArmJointY = 0.0f;
GLfloat rotateUpperArmJointZ = 0.0f;
GLfloat rotateElbowJointX = 30.0f;
GLfloat rotateElbowJointY = 0.0f;
GLfloat rotateElbowJointZ = -90.0f;
GLfloat rotateWristJointX = 0.0f;
GLfloat rotateWristJointY = 5.0f;
GLfloat rotateWristJointZ = 0.0f;
boolean armReturn = false;

// Armor Attributes
boolean onArmor = false;
boolean firstArmor = true;
GLfloat armorSpeed = 0.0f;
GLfloat armorSize = 0.0f;
boolean onSpiky = false;
boolean firstSpiky = true;
GLfloat spikySpeed = 0.0f;
GLfloat spikySize = 0.0f;

// Maglev Punch Attributes
GLfloat rotateFinger = 90.0f;
GLfloat rotateTumb = -45.0f;
boolean onHand = false;
boolean firstHand = true;
GLfloat handSpeed = 0.0f;
GLfloat handSize = 0.0f;
GLfloat rotatePalmX = 0.0f;
GLfloat rotatePalmY = 0.0f;
GLfloat rotatePalmZ = 0.0f;
GLfloat rotateHandX = 0.0f;
GLfloat rotateHandY = 0.0f;
GLfloat rotateHandZ = 0.0f;
GLfloat punchSpeed = 0.0f;
boolean punchReturn = false;

// Gun Attributes
boolean onGun = false;
boolean firstGun = true;
GLfloat gunSize = 0.0f;
GLfloat rotateGunY = 0.0f;
GLfloat gunTranslate = 0.0f;
GLfloat magazineSpeed = 0.0f;
GLfloat isReload = true;
boolean isTriggered = false;
GLint triggerCount = 0;
GLfloat bulletSpeeds[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		switch (wParam) {
		case MK_LBUTTON:
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			camRotation[0] += (yPos - lastY) / 2;
			camRotation[1] += (xPos - lastX) / 2;
			lastX = xPos;
			lastY = yPos;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		lastX = GET_X_LPARAM(lParam);
		lastY = GET_Y_LPARAM(lParam);
		break;
	case WM_MOUSEWHEEL:
		camRotation[2] += GET_WHEEL_DELTA_WPARAM(wParam) / 120.0f;
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		if (wParam == VK_NUMPAD0)
		{
			// MODE 0: Projection Type Control
			mode = 0;
		}
		else if (wParam == VK_NUMPAD1)
		{
			// MODE 1: Projection View Control
			mode = 1;
		}
		else if (wParam == VK_NUMPAD2)
		{
			// MODE 2: Lighting Control
			mode = 2;
		}
		else if (wParam == VK_NUMPAD3)
		{
			// MODE 3: Texture Control
			mode = 3;
		}
		else if (wParam == VK_NUMPAD4)
		{
			// MODE 4: State Control
			mode = 4;
		}
		else if (wParam == VK_NUMPAD5)
		{
			// MODE 5: Limb Control
			mode = 5;
		}
		else if (wParam == VK_NUMPAD6)
		{
			// MODE 6: Weapon Type Control
			mode = 6;
		}
		else if (wParam == VK_NUMPAD7)
		{
			// MODE 7: Weapon Control (Armor)
			mode = 7;
		}
		else if (wParam == VK_NUMPAD8)
		{
			// MODE 8: Weapon Control (Maglev Punch)
			mode = 8;
		}
		else if (wParam == VK_NUMPAD9)
		{
			// MODE 9: Weapon Control (Gun)
			mode = 9;
		}
		else if (wParam == 'M')
		{
			camRotation[0] = 0.0f;
			camRotation[1] = 0.0f;
			camRotation[2] = 0.0f;
			lastX = 0.0f;
			lastY = 0.0f;
		}
		else if (wParam == ' ')	// Reset All
		{
			// Control Attributes
			camRotation[0] = 0.0f;
			camRotation[1] = 0.0f;
			camRotation[2] = 0.0f;
			lastX = 0.0f;
			lastY = 0.0f;

			// Projection Attributes
			r = -2.5f;
			angle = 360.0f;
			eyeX = r * sin(angle * 3.142 / 180.0);
			eyeY = r;
			eyeZ = r * cos(angle * 3.142 / 180.0);
			lookAtX = 0.0f;
			lookAtY = -4.0f;
			lookAtZ = 0.0f;
			upX = 0.0f;
			upY = 1.0f;
			upZ = 0.0f;

			// Light Attributes
			positionLight[0] = 0.0f;
			positionLight[1] = 0.0f;
			positionLight[2] = 0.0f;
			positionLight[3] = 0.0f;
			onLight = true;
			isDay = true;

			// Texture Attributes
			onTexture = true;
			textureTheme = 1;

			// State Attributes
			onRest = true;
			firstRest = true;
			restSpeed = 0.0f;
			isStop = true;
			rotateWheel = 0.0f;

			// Limb Attributes
			rotateShoulderJointX = 0.0f;
			rotateShoulderJointY = 0.0f;
			rotateShoulderJointZ = -5.0f;
			rotateUpperArmJointX = 0.0f;
			rotateUpperArmJointY = 0.0f;
			rotateUpperArmJointZ = 0.0f;
			rotateElbowJointX = 30.0f;
			rotateElbowJointY = 0.0f;
			rotateElbowJointZ = -90.0f;
			rotateWristJointX = 0.0f;
			rotateWristJointY = 5.0f;
			rotateWristJointZ = 0.0f;
			armReturn = false;

			// Armor Attributes
			onArmor = false;
			firstArmor = true;
			armorSpeed = 0.0f;
			armorSize = 0.0f;
			onSpiky = false;
			firstSpiky = true;
			spikySpeed = 0.0f;
			spikySize = 0.0f;

			// Maglev Punch Attributes
			rotateFinger = 90.0f;
			rotateTumb = -45.0f;
			onHand = false;
			firstHand = true;
			handSpeed = 0.0f;
			handSize = 0.0f;
			rotatePalmX = 0.0f;
			rotatePalmY = 0.0f;
			rotatePalmZ = 0.0f;
			rotateHandX = 0.0f;
			rotateHandY = 0.0f;
			rotateHandZ = 0.0f;
			punchSpeed = 0.0f;
			punchReturn = false;

			// Gun Attributes
			onGun = false;
			firstGun = true;
			gunSize = 0.0f;
			rotateGunY = 0.0f;
			gunTranslate = 0.0f;
			magazineSpeed = 0.0f;
			isReload = true;
			isTriggered = false;
			triggerCount = 0;
		}

		// DECS: Use without restriction
		// MODE 0: Projection Type Control
		if (mode == 0)
		{
			// Reset MODE 0
			if (wParam == VK_TAB)
			{
				isOrtho = true;
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(-5, 5, -5, 5, 2, 10);
			}
			else if (wParam == VK_NUMPAD0)
			{
				glMatrixMode(GL_PROJECTION);
				isOrtho = !isOrtho;
				glLoadIdentity();

				if (isOrtho)
				{
					glOrtho(-5, 5, -5, 5, 2, 10);
				}
				else
				{
					gluPerspective(50, 1, 1, 20);
				}
			}
		}

		// MODE 1: Projection View Control
		if (mode == 1)
		{
			// Reset MODE 1
			if (wParam == VK_TAB)
			{
				r = -2.5f;
				angle = 360.0f;
				eyeX = r * sin(angle * 3.142 / 180.0);
				eyeY = r;
				eyeZ = r * cos(angle * 3.142 / 180.0);
				lookAtX = 0.0f;
				lookAtY = -4.0f;
				lookAtZ = 0.0f;
				upX = 0.0f;
				upY = 1.0f;
				upZ = 0.0f;
			}
			// Eye
			else if (wParam == 'D')
			{
				angle += 5;
				eyeX = r * sin(angle * 3.142 / 180.0);
				eyeY = eyeY;
				eyeZ = r * cos(angle * 3.142 / 180.0);
			}
			else if (wParam == 'A')
			{
				angle -= 5;
				eyeX = r * sin(angle * 3.142 / 180.0);
				eyeY = eyeY;
				eyeZ = r * cos(angle * 3.142 / 180.0);
			}
			else if (wParam == 'W')
			{
				if (eyeY > -4.5)
				{
					eyeX = r * sin(angle * 3.142 / 180.0);
					eyeY += eyeY * 0.1f;
					eyeZ = r * cos(angle * 3.142 / 180.0);
				}
			}
			else if (wParam == 'S')
			{
				if (eyeY < -1.0)
				{
					eyeX = r * sin(angle * 3.142 / 180.0);
					eyeY -= eyeY * 0.1f;
					eyeZ = r * cos(angle * 3.142 / 180.0);
				}
			}
			else if (wParam == 'Q')
			{
				r += 0.1f;
				eyeX = r * sin(angle * 3.142 / 180.0);
				eyeY += 0.1f;
				eyeZ = r * cos(angle * 3.142 / 180.0);
			}
			else if (wParam == 'E')
			{
				r -= 0.1f;
				eyeX = r * sin(angle * 3.142 / 180.0);
				eyeY -= 0.1f;
				eyeZ = r * cos(angle * 3.142 / 180.0);
			}
			// Look At
			if (wParam == 'H')
			{
				lookAtX += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'F')
			{
				lookAtX -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'T')
			{
				lookAtY += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'G')
			{
				lookAtY -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'R')
			{
				lookAtZ += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'Y')
			{
				lookAtZ -= cameraSpeed * elapsedSeconds;
			}
			// World Up
			if (wParam == 'L')
			{
				upX += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'J')
			{
				upX -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'I')
			{
				upY += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'K')
			{
				upY -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'U')
			{
				upZ += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'O')
			{
				upZ -= cameraSpeed * elapsedSeconds;
			}
		}

		// MODE 2: Lighting Control
		if (mode == 2)
		{
			// Reset MODE 2
			if (wParam == VK_TAB)
			{
				positionLight[0] = 0.0f;
				positionLight[1] = 0.0f;
				positionLight[2] = 0.0f;
				positionLight[3] = 0.0f;
				onLight = true;
				isDay = true;
			}
			else if (wParam == 'D')	// Right
			{
				// lightX
				positionLight[0] += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'A')	// Left
			{
				// lightX
				positionLight[0] -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'W')	// Up
			{
				// lightY
				positionLight[1] += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'S')	// Down
			{
				// lightY
				positionLight[1] -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'Q')	// Near
			{
				// lightZ
				positionLight[2] += cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'E')	// Far
			{
				// lightZ
				positionLight[2] -= cameraSpeed * elapsedSeconds;
			}
			else if (wParam == 'X')	// On/Off Light
			{
				onLight = !onLight;
			}
			else if (wParam == 'Z')	// On/Off Day
			{
				isDay = !isDay;
			}
		}

		// MODE 3: Texture Control
		if (mode == 3)
		{
			// Reset MODE 3
			if (wParam == VK_TAB)
			{
				onTexture = true;
				textureTheme = 1;
			}
			else if (wParam == 'X')
			{
				onTexture = !onTexture;
			}
			if (wParam == 'Z')
			{
				textureTheme = textureTheme == 1 ? 2 : 1;
			}
			if (wParam == 'C')
			{
				environmentTheme = environmentTheme == 1 ? 2 : 1;
			}
		}

		// MODE 4: State Control
		if (mode == 4)
		{
			// Reset MODE 4
			if (wParam == VK_TAB)
			{
				onRest = true;
				firstRest = true;
				restSpeed = 0.0f;
				isStop = true;
				rotateWheel = 0.0f;

				onArmor = false;
				firstArmor = true;
				armorSize = 0.0f;
				armorSpeed = 0.0f;
				onSpiky = false;
				firstSpiky = true;
				spikySpeed = 0.0f;
				spikySize = 0.0f;

				onHand = false;
				firstHand = true;
				handSize = 0.0f;
				handSpeed = 0.0f;

				onGun = false;
				firstGun = true;
				gunSize = 0.0f;
				rotateGunY = 0.0f;
				gunTranslate = 0.0f;
				magazineSpeed = 0.0f;
				isReload = true;
				isTriggered = false;
				triggerCount = 0;
			}
			// STATE R: Resting State
			else if (wParam == 'R')
			{
				onRest = !onRest;
				isStop = onRest;
				rotateWheel = 0.0f;

				onArmor = false;
				firstArmor = true;
				armorSize = 0.0f;
				armorSpeed = 0.0f;
				onSpiky = false;
				firstSpiky = true;
				spikySpeed = 0.0f;
				spikySize = 0.0f;

				onHand = false;
				firstHand = true;
				handSize = 0.0f;
				handSpeed = 0.0f;

				onGun = false;
				firstGun = true;
				gunSize = 0.0f;
				rotateGunY = 0.0f;
				gunTranslate = 0.0f;
				magazineSpeed = 0.0f;
				isReload = true;
				isTriggered = false;
				triggerCount = 0;
			}
			// STATE: Stop State
			else if (wParam == 'S')
			{
				isStop = !isStop;
				rotateWheel = 0.0f;
			}
		}

		// DECS: Enable only if the robot is not in STATE R
		if (!onRest)
		{
			// MODE 5: Limb Control
			if (mode == 5)
			{
				// Reset MODE 5
				if (wParam == VK_TAB)
				{
					armReturn = false;
					rotateShoulderJointX = 0.0f;
					rotateShoulderJointY = 0.0f;
					rotateShoulderJointZ = -5.0f;
					rotateUpperArmJointX = 0.0f;
					rotateUpperArmJointY = 0.0f;
					rotateUpperArmJointZ = 0.0f;
					rotateElbowJointX = 30.0f;
					rotateElbowJointY = 0.0f;
					rotateElbowJointZ = -90.0f;
					rotateWristJointX = 0.0f;
					rotateWristJointY = 5.0f;
					rotateWristJointZ = 0.0f;
				}
				else if (wParam == 'W')
				{
					if (!armReturn)
					{
						if (rotateShoulderJointX < 10.0f)
						{
							rotateShoulderJointX += 0.5 * speed * elapsedSeconds;
						}
						if (rotateUpperArmJointX < 10.0f)
						{
							rotateUpperArmJointX += 0.5 * speed * elapsedSeconds;
						}
						if (rotateShoulderJointX > 10.0f && rotateUpperArmJointX > 10.0f)
						{
							armReturn = true;
						}
					}
					else
					{
						if (rotateShoulderJointX > -10.0f)
						{
							rotateShoulderJointX -= 0.5 * speed * elapsedSeconds;
						}
						if (rotateUpperArmJointX > -10.0f)
						{
							rotateUpperArmJointX -= 0.5 * speed * elapsedSeconds;
						}
						if (rotateShoulderJointX < -10.0f && rotateUpperArmJointX < -10.0f)
						{
							armReturn = false;
						}
					}
				}
				else if (wParam == 'S')
				{
					if (!armReturn)
					{
						if (rotateShoulderJointX > -10.0f)
						{
							rotateShoulderJointX -= 0.5 * speed * elapsedSeconds;
						}
						if (rotateUpperArmJointX > -10.0f)
						{
							rotateUpperArmJointX -= 0.5 * speed * elapsedSeconds;
						}
						if (rotateShoulderJointX < -10.0f && rotateUpperArmJointX < -10.0f)
						{
							armReturn = true;
						}
					}
					else
					{
						if (rotateShoulderJointX < 10.0f)
						{
							rotateShoulderJointX += 0.5 * speed * elapsedSeconds;
						}
						if (rotateUpperArmJointX < 10.0f)
						{
							rotateUpperArmJointX += 0.5 * speed * elapsedSeconds;
						}
						if (rotateShoulderJointX > 10.0f && rotateUpperArmJointX > 10.0f)
						{
							armReturn = false;
						}
					}
				}
				else if (wParam == 'D')
				{
					if (rotateShoulderJointZ < 10.0f)
					{
						rotateShoulderJointZ += speed * elapsedSeconds;
					}
					if (rotateUpperArmJointY < 10.0f)
					{
						rotateUpperArmJointY += speed * elapsedSeconds;
					}
				}
				else if (wParam == 'A')
				{
					if (rotateShoulderJointZ > -10.0f)
					{
						rotateShoulderJointZ -= speed * elapsedSeconds;
					}
					if (rotateUpperArmJointY > -10.0f)
					{
						rotateUpperArmJointY -= speed * elapsedSeconds;
					}
				}
				else if (wParam == 'Q')
				{
					if (rotateShoulderJointY < 10.0f)
					{
						rotateShoulderJointY += speed * elapsedSeconds;
					}
					if (rotateUpperArmJointZ > 10.0f)
					{
						rotateUpperArmJointZ -= speed * elapsedSeconds;
					}
				}
				else if (wParam == 'E')
				{
					if (rotateShoulderJointY > -10.0f)
					{
						rotateShoulderJointY -= speed * elapsedSeconds;
					}
					if (rotateUpperArmJointZ < -10.0f)
					{
						rotateUpperArmJointZ += speed * elapsedSeconds;
					}
				}
				else if (wParam == 'F')
				{
					if (rotateElbowJointX < 25.0f)
					{
						rotateElbowJointX += speed * elapsedSeconds;
					}
				}
				else if (wParam == 'H')
				{
					if (rotateElbowJointX > 0.0f)
					{
						rotateElbowJointX -= speed * elapsedSeconds;
					}
				}
				else if (wParam == 'T')
				{
					if (rotateElbowJointY < 25.0f)
					{
						rotateElbowJointY += speed * elapsedSeconds;
					}
				}
				else if (wParam == 'G')
				{
					if (rotateElbowJointY > -25.0f)
					{
						rotateElbowJointY -= speed * elapsedSeconds;
					}
				}
				else if (wParam == 'R')
				{
					if (rotateElbowJointZ < 10.0f)
					{
						rotateElbowJointZ += speed * elapsedSeconds;
					}
				}
				else if (wParam == 'Y')
				{
					if (rotateElbowJointZ > -10.0f)
					{
						rotateElbowJointZ -= speed * elapsedSeconds;
					}
				}
				else if (wParam == 'I')
				{
					if (rotateWristJointX < 10.0f)
					{
						rotateWristJointX += speed * elapsedSeconds;
					}
				}
				else if (wParam == 'K')
				{
					if (rotateWristJointX > -10.0f)
					{
						rotateWristJointX -= speed * elapsedSeconds;
					}
				}
				else if (wParam == 'L')
				{
					if (rotateWristJointY < 10.0f)
					{
						rotateWristJointY += speed * elapsedSeconds;
					}
				}
				else if (wParam == 'J')
				{
					if (rotateWristJointY > -10.0f)
					{
						rotateWristJointY -= speed * elapsedSeconds;
					}
				}
				else if (wParam == 'U')
				{
					if (rotateWristJointZ < 10.0f)
					{
						rotateWristJointZ += speed * elapsedSeconds;
					}
				}
				else if (wParam == 'O')
				{
					if (rotateWristJointZ > -10.0f)
					{
						rotateWristJointZ -= speed * elapsedSeconds;
					}
				}
			}

			// MODE 6: Weapon Type Control
			if (mode == 6)
			{
				// Reset MODE 6
				if (wParam == VK_TAB)
				{
					onArmor = false;
					firstArmor = true;
					armorSize = 0.0f;
					armorSpeed = 0.0f;
					onSpiky = false;
					firstSpiky = true;
					spikySpeed = 0.0f;
					spikySize = 0.0f;

					onHand = false;
					firstHand = true;
					handSize = 0.0f;
					handSpeed = 0.0f;

					onGun = false;
					firstGun = true;
					gunSize = 0.0f;
					rotateGunY = 0.0f;
					gunTranslate = 0.0f;
					magazineSpeed = 0.0f;
					isReload = true;
					isTriggered = false;
					triggerCount = 0;
				}
				// WEAPON A: Armor
				else if (wParam == 'A')
				{
					onArmor = !onArmor;
				}
				// WEAPON P: Maglev Punch
				else if (wParam == 'P')
				{
					onHand = !onHand;
					rotateFinger = 90.0f;
					rotateTumb = -45.0f;
					rotatePalmX = 0.0f;
					rotatePalmY = 0.0f;
					rotatePalmZ = 0.0f;
					rotateHandX = 0.0f;
					rotateHandY = 0.0f;
					rotateHandZ = 0.0f;
					punchSpeed = 0.0f;
					punchReturn = false;
				}
				// WEAPON G: Gun
				else if (wParam == 'G')
				{
					onGun = !onGun;
					rotateGunY = 0.0f;
					magazineSpeed = 0.0f;
					isReload = true;
					isTriggered = false;
					triggerCount = 0;
				}
			}

			// MODE 7: Weapon Control (Armor)
			if (mode == 7)
			{
				// Reset MODE 7
				if (wParam == VK_TAB)
				{
					onSpiky = false;
					firstSpiky = true;
					spikySpeed = 0.0f;
					spikySize = 0.0f;
				}
				else if (wParam == 'X')
				{
					onSpiky = !onSpiky;
				}
			}

			// MODE 8: Weapon Control (Maglev Punch)
			if (mode == 8)
			{
				// Reset MODE 8
				if (wParam == VK_TAB)
				{
					rotateFinger = 90.0f;
					rotateTumb = -45.0f;
					rotatePalmX = 0.0f;
					rotatePalmY = 0.0f;
					rotatePalmZ = 0.0f;
					rotateHandX = 0.0f;
					rotateHandY = 0.0f;
					rotateHandZ = 0.0f;
					punchSpeed = 0.0f;
					punchReturn = false;
				}
				else if (wParam == 'W')
				{
					rotatePalmX += speed * elapsedSeconds;
				}
				else if (wParam == 'S')
				{
					rotatePalmX -= speed * elapsedSeconds;
				}
				else if (wParam == 'D')
				{
					rotatePalmZ += speed * elapsedSeconds;
				}
				else if (wParam == 'A')
				{
					rotatePalmZ -= speed * elapsedSeconds;
				}
				else if (wParam == 'Q')
				{
					rotatePalmY += speed * elapsedSeconds;
				}
				else if (wParam == 'E')
				{
					rotatePalmY -= speed * elapsedSeconds;
				}
				else if (wParam == 'T')
				{
					if (rotateHandX > -100.0f)
					{
						rotatePalmX += speed * elapsedSeconds;
						rotateHandX -= speed * elapsedSeconds;
					}
				}
				else if (wParam == 'G')
				{
					if (rotateHandX < 100.0f)
					{
						rotatePalmX -= speed * elapsedSeconds;
						rotateHandX += speed * elapsedSeconds;
					}
				}
				else if (wParam == 'H')
				{
					if (rotateHandZ < 100.0f)
					{
						rotatePalmZ -= speed * elapsedSeconds;
						rotateHandZ += speed * elapsedSeconds;
					}
				}
				else if (wParam == 'F')
				{
					if (rotateHandZ > -100.0f)
					{
						rotatePalmZ += speed * elapsedSeconds;
						rotateHandZ -= speed * elapsedSeconds;
					}
				}
				else if (wParam == 'R')
				{
					rotatePalmY += speed * elapsedSeconds;
					rotateHandY -= speed * elapsedSeconds;
				}
				else if (wParam == 'Y')
				{
					rotatePalmY -= speed * elapsedSeconds;
					rotateHandY += speed * elapsedSeconds;
				}
				else if (wParam == 'Z')
				{
					if (rotateFinger < 90.0f)
					{
						rotateFinger += 2 * speed * elapsedSeconds;
					}
					if (rotateTumb > -45.0f)
					{
						rotateTumb -= speed * elapsedSeconds;
					}
				}
				else if (wParam == 'X')
				{
					if (rotateFinger > 0.0f)
					{
						rotateFinger -= 2 * speed * elapsedSeconds;
					}
					if (rotateTumb < 0.0f)
					{
						rotateTumb += speed * elapsedSeconds;
					}
				}
				else if (wParam == 'C')
				{
					if (!punchReturn)
					{
						if (punchSpeed < 0.5f)
						{
							punchSpeed += 0.025 * speed * elapsedSeconds;
						}
						else
						{
							punchReturn = true;
						}
					}
					else
					{
						if (punchSpeed > -0.5f)
						{
							punchSpeed -= 0.025 * speed * elapsedSeconds;
						}
						else
						{
							punchReturn = false;
						}
					}
				}
			}

			// MODE 9: WEAPON Control (Gun)
			if (mode == 9)
			{
				// Reset MODE 9
				if (wParam == VK_TAB)
				{
					rotateGunY = 0.0f;
					isReload = true;
					magazineSpeed = 0.0f;
					isTriggered = false;
					triggerCount = 0;
				}
				else if (wParam == 'D')
				{
					if (rotateGunY < 180.0f)
					{
						rotateGunY += speed * elapsedSeconds;
					}
				}
				else if (wParam == 'A')
				{
					if (rotateGunY > 0.0f)
					{
						rotateGunY -= speed * elapsedSeconds;
					}
				}
				else if (wParam == 'Z')
				{
					isReload = true;
					magazineSpeed = 0.0f;
					isTriggered = false;
					triggerCount = 0;
				}
				else if (wParam == 'X')
				{
					isReload = false;
					magazineSpeed += speed;

					if (magazineSpeed == 45.0f)
					{
						isTriggered = true;
					}
					else if (magazineSpeed == 135.0f)
					{
						isTriggered = true;
					}
					else if (magazineSpeed == 225.0f)
					{
						isTriggered = true;
					}
					else if (magazineSpeed == 315.0f)
					{
						isTriggered = true;
					}
					else
					{
						isTriggered = false;
					}
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

void drawNormalizedVertex(GLfloat x, GLfloat y, GLfloat z)
{
	glNormal3f(x, y, z);
	glVertex3f(x, y, z);
}

void setMaterial(GLfloat ambientMaterial[4], GLfloat diffuseMaterial[4], GLfloat specularMaterial[4])
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterial);
}

void draw4PointedStarStrip(GLfloat radius, GLfloat depth)
{
	// PLY: GL_QUAD_STRIP
	glBegin(GL_QUAD_STRIP);
	{
		glTexCoord2f(1.0f, 1.0f);
		drawNormalizedVertex(0.0f, radius, -depth / 2);
		glTexCoord2f(1.0f, 0.0f);
		drawNormalizedVertex(0.0f, radius, depth / 2);

		glTexCoord2f(0.0f, 1.0f);
		drawNormalizedVertex(-radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, -depth / 2);
		glTexCoord2f(0.0f, 0.0f);
		drawNormalizedVertex(-radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, depth / 2);

		glTexCoord2f(1.0f, 1.0f);
		drawNormalizedVertex(-radius, 0.0f, -depth / 2);
		glTexCoord2f(1.0f, 0.0f);
		drawNormalizedVertex(-radius, 0.0f, depth / 2);

		glTexCoord2f(0.0f, 0.0f);
		drawNormalizedVertex(-radius * cos(45 * PI / 180) / 2, -radius * sin(45 * PI / 180) / 2, -depth / 2);
		glTexCoord2f(0.0f, 1.0f);
		drawNormalizedVertex(-radius * cos(45 * PI / 180) / 2, -radius * sin(45 * PI / 180) / 2, depth / 2);

		glTexCoord2f(1.0f, 1.0f);
		drawNormalizedVertex(0.0f, -radius, -depth / 2);
		glTexCoord2f(1.0f, 0.0f);
		drawNormalizedVertex(0.0f, -radius, depth / 2);

		glTexCoord2f(0.0f, 0.0f);
		drawNormalizedVertex(radius * cos(45 * PI / 180) / 2, -radius * sin(45 * PI / 180) / 2, -depth / 2);
		glTexCoord2f(0.0f, 1.0f);
		drawNormalizedVertex(radius * cos(45 * PI / 180) / 2, -radius * sin(45 * PI / 180) / 2, depth / 2);

		glTexCoord2f(1.0f, 1.0f);
		drawNormalizedVertex(radius, 0.0f, -depth / 2);
		glTexCoord2f(1.0f, 0.0f);
		drawNormalizedVertex(radius, 0.0f, depth / 2);

		glTexCoord2f(0.0f, 0.0f);
		drawNormalizedVertex(radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, -depth / 2);
		glTexCoord2f(0.0f, 1.0f);
		drawNormalizedVertex(radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, depth / 2);

		glTexCoord2f(1.0f, 1.0f);
		drawNormalizedVertex(0.0f, radius, -depth / 2);
		glTexCoord2f(1.0f, 0.0f);
		drawNormalizedVertex(0.0f, radius, depth / 2);
	}
	glEnd();
}

void draw4PointedStar(GLfloat outerRadius, GLfloat innerRadius, GLfloat outerDepth, GLfloat innerDepth)
{
	GLfloat interval = innerDepth - outerDepth;

	if (onTexture)
	{
		glEnable(GL_TEXTURE_2D);
	}
	{
		glBindTexture(GL_TEXTURE_2D, blueStripTexture);

		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, interval);
			draw4PointedStarStrip(outerRadius, outerDepth);
		}
		glPopMatrix();

		draw4PointedStarStrip(innerRadius, innerDepth);

		// PLY: GL_QUAD_STRIP
		glBegin(GL_QUAD_STRIP);
		{
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(0.0f, outerRadius, -outerDepth / 2 + interval);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(0.0f, innerRadius, -innerDepth / 2);

			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(-outerRadius * cos(45 * PI / 180) / 2, outerRadius * sin(45 * PI / 180) / 2, -outerDepth / 2 + interval);
			glTexCoord2f(0, 0);
			drawNormalizedVertex(-innerRadius * cos(45 * PI / 180) / 2, innerRadius * sin(45 * PI / 180) / 2, -innerDepth / 2);

			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(-outerRadius, 0.0f, -outerDepth / 2 + interval);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(-innerRadius, 0.0f, -innerDepth / 2);

			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(-outerRadius * cos(45 * PI / 180) / 2, -outerRadius * sin(45 * PI / 180) / 2, -outerDepth / 2 + interval);
			glTexCoord2f(0, 0);
			drawNormalizedVertex(-innerRadius * cos(45 * PI / 180) / 2, -innerRadius * sin(45 * PI / 180) / 2, -innerDepth / 2);

			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(0.0f, -outerRadius, -outerDepth / 2 + interval);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(0.0f, -innerRadius, -innerDepth / 2);

			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(outerRadius * cos(45 * PI / 180) / 2, -outerRadius * sin(45 * PI / 180) / 2, -outerDepth / 2 + interval);
			glTexCoord2f(0, 0);
			drawNormalizedVertex(innerRadius * cos(45 * PI / 180) / 2, -innerRadius * sin(45 * PI / 180) / 2, -innerDepth / 2);

			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(outerRadius, 0.0f, -outerDepth / 2 + interval);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(innerRadius, 0.0f, -innerDepth / 2);

			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(outerRadius * cos(45 * PI / 180) / 2, outerRadius * sin(45 * PI / 180) / 2, -outerDepth / 2 + interval);
			glTexCoord2f(0, 0);
			drawNormalizedVertex(innerRadius * cos(45 * PI / 180) / 2, innerRadius * sin(45 * PI / 180) / 2, -innerDepth / 2);

			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(0.0f, outerRadius, -outerDepth / 2 + interval);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(0.0f, innerRadius, -innerDepth / 2);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

void drawKunaiStrip(GLfloat radius, GLfloat depth)
{
	// PLY: GL_QUAD_STRIP
	glBegin(GL_QUAD_STRIP);
	{
		glTexCoord2f(1.0, 0);
		drawNormalizedVertex(0.0f, radius, -depth / 2);
		glTexCoord2f(1.0, 1.0);
		drawNormalizedVertex(0.0f, radius, depth / 2);

		glTexCoord2f(0, 1.0);
		drawNormalizedVertex(-radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, -depth / 2);
		glTexCoord2f(0, 0);
		drawNormalizedVertex(-radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, depth / 2);

		glTexCoord2f(1.0, 0);
		drawNormalizedVertex(0.0f, -radius, -depth / 2);
		glTexCoord2f(1.0, 1.0);
		drawNormalizedVertex(0.0f, -radius, depth / 2);

		glTexCoord2f(0, 1.0);
		drawNormalizedVertex(radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, -depth / 2);
		glTexCoord2f(0, 0);
		drawNormalizedVertex(radius * cos(45 * PI / 180) / 2, radius * sin(45 * PI / 180) / 2, depth / 2);

		glTexCoord2f(1.0, 0);
		drawNormalizedVertex(0.0f, radius, -depth / 2);
		glTexCoord2f(1.0, 1.0);
		drawNormalizedVertex(0.0f, radius, depth / 2);
	}
	glEnd();
}

void drawKunai(GLfloat gripRadius, GLfloat gripLength, GLfloat outerRadius, GLfloat innerRadius, GLfloat outerDepth, GLfloat innerDepth)
{
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	GLfloat interval = innerDepth - outerDepth;

	if (onTexture)
	{
		glEnable(GL_TEXTURE_2D);
	}
	{
		glBindTexture(GL_TEXTURE_2D, blueStripTexture);
		glPushMatrix();
		{
			glTranslatef(0.0f, innerRadius + gripLength, 0.0f);
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			gluQuadricTexture(quad, GL_TRUE);
			// PLY: gluCylinder()
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

		// PLY: GL_QUAD_STRIP
		glBegin(GL_QUAD_STRIP);
		{
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(0.0f, outerRadius, -outerDepth / 2 + interval);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(0.0f, innerRadius, -innerDepth / 2);

			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(-outerRadius * cos(45 * PI / 180) / 2, outerRadius * sin(45 * PI / 180) / 2, -outerDepth / 2 + interval);
			glTexCoord2f(0, 0);
			drawNormalizedVertex(-innerRadius * cos(45 * PI / 180) / 2, innerRadius * sin(45 * PI / 180) / 2, -innerDepth / 2);

			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(0.0f, -outerRadius, -outerDepth / 2 + interval);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(0.0f, -innerRadius, -innerDepth / 2);

			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(outerRadius * cos(45 * PI / 180) / 2, outerRadius * sin(45 * PI / 180) / 2, -outerDepth / 2 + interval);
			glTexCoord2f(0, 0);
			drawNormalizedVertex(innerRadius * cos(45 * PI / 180) / 2, innerRadius * sin(45 * PI / 180) / 2, -innerDepth / 2);

			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(0.0f, outerRadius, -outerDepth / 2 + interval);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(0.0f, innerRadius, -innerDepth / 2);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
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
	gluQuadricDrawStyle(quad, GLU_FILL);

	glPushMatrix();
	{
		glTranslatef(0.0f, 0.0f, eyeRadius / 2);

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			if (textureTheme == 1)
			{

				glBindTexture(GL_TEXTURE_2D, blueLightRayTexture);
			}
			else if (textureTheme == 2)
			{
				glBindTexture(GL_TEXTURE_2D, damageEyeTexture);
			}

			gluQuadricTexture(quad, GL_TRUE);
			// PLY: gluSphere()
			gluSphere(quad, eyeRadius, tubeSlices, tubeStacks);
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
		glBindTexture(GL_TEXTURE_2D, blackMetalTexture);
		gluQuadricTexture(quad, GL_TRUE);
		// PLY: gluCylinder()
		gluCylinder(quad, radius, radius, height, tubeSlices, tubeStacks);

		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, height);
			gluQuadricTexture(quad, GL_TRUE);
			// PLY: gluDisk()
			gluDisk(quad, 0.0f, radius, tubeSlices, tubeStacks);
		}
		glPopMatrix();
		
		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, height);
			gluQuadricTexture(quad, GL_TRUE);
			// PLY: gluDisk()
			gluDisk(quad, 0.0f, radius, tubeSlices, tubeStacks);
		}
		glPopMatrix();

		gluQuadricTexture(quad, GL_TRUE);
		// PLY: gluDisk()
		gluDisk(quad, radius * 3 / 4, radius, tubeSlices, tubeStacks);
		drawEyeFrame(0.05f, 0.2f, 0.15f, 0.01f, 0.015f, 0.01f, 0.05f, 0.065f, 0.045f, 0.01f, 0.015f, 0.125f, 0.125f);
	}
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	{
		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glTranslatef(0.0f, 0.0f, height / 6);
			glBindTexture(GL_TEXTURE_2D, darkGreyMetalTexture);
			gluQuadricTexture(quad, GL_TRUE);
			// PLY: gluCylinder()
			gluCylinder(quad, radius + 0.01, radius + 0.01, height * 5 / 6, tubeSlices, tubeStacks);
			// PLY: gluDisk()
			gluDisk(quad, radius, radius + 0.01, tubeSlices, tubeStacks);
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

void drawStandFoot(GLfloat standFootHeight, GLfloat maxStandFootWidth)
{
	GLfloat standFootDepth = standFootHeight / 4;

	glPushMatrix();
	{
		glRotatef(45.0f, 1.0f, 0.0f, 0.0f);

		// PLY: GL_QUADS
		glBegin(GL_QUADS);
		{
			// Top Face
			glTexCoord2f(0, 0);
			drawNormalizedVertex(-maxStandFootWidth / 2, standFootHeight / 2, standFootDepth / 2);
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(-maxStandFootWidth / 2, standFootHeight / 2, -standFootDepth / 2);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(maxStandFootWidth / 2, standFootHeight / 2, -standFootDepth / 2);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(maxStandFootWidth / 2, standFootHeight / 2, standFootDepth / 2);
		}
		glEnd();

		// PLY: GL_TRIANGLE_FAN
		glBegin(GL_TRIANGLE_FAN);
		{
			// Center
			glTexCoord2f(0, 0);
			drawNormalizedVertex(0.0f, -standFootHeight / 2, 0.0f);

			// Front Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(-maxStandFootWidth / 2, standFootHeight / 2, -standFootDepth / 2);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(maxStandFootWidth / 2, standFootHeight / 2, -standFootDepth / 2);

			// Right Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(maxStandFootWidth / 2, standFootHeight / 2, -standFootDepth / 2);
			glTexCoord2f(0, 0);
			drawNormalizedVertex(maxStandFootWidth / 2, standFootHeight / 2, standFootDepth / 2);

			// Back Face
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(maxStandFootWidth / 2, standFootHeight / 2, standFootDepth / 2);
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(-maxStandFootWidth / 2, standFootHeight / 2, standFootDepth / 2);

			// Left Face
			glTexCoord2f(0, 0);
			drawNormalizedVertex(-maxStandFootWidth / 2, standFootHeight / 2, standFootDepth / 2);
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(-maxStandFootWidth / 2, standFootHeight / 2, -standFootDepth / 2);
		}
		glEnd();
	}
	glPopMatrix();
}

void drawStand(GLfloat standHeight)
{
	GLfloat standWidth = standHeight / 2;
	GLfloat standFootHeight = standHeight / 3;

	if (onTexture)
	{
		glEnable(GL_TEXTURE_2D);
	}
	{
		glBindTexture(GL_TEXTURE_2D, upperStandTexture);
		// PLY: GL_QUADS
		glBegin(GL_QUADS);
		{
			// Top Face
			glTexCoord2f(0, 0);
			drawNormalizedVertex(-standWidth, standHeight / 2, standWidth);
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(-standWidth, standHeight / 2, -standWidth);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(standWidth, standHeight / 2, -standWidth);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(standWidth, standHeight / 2, standWidth);

			// Bottom Face
			glTexCoord2f(0, 0);
			drawNormalizedVertex(0.0f, -standFootHeight / 2, standWidth);
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(-standWidth, -standFootHeight / 2, 0.0f);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(0.0f, -standFootHeight / 2, -standWidth);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(standWidth, -standFootHeight / 2, 0.0f);
		}
		glEnd();

		// PLY: GL_TRIANGLES
		glBegin(GL_TRIANGLES);
		{
			// Front Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(-standWidth, standHeight / 2, -standWidth);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(0.0f, -standFootHeight / 2, -standWidth);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(standWidth, standHeight / 2, -standWidth);

			// Front Right Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(standWidth, standHeight / 2, -standWidth);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(0.0f, -standFootHeight / 2, -standWidth);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(standWidth, -standFootHeight / 2, 0.0f);

			// Right Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(standWidth, standHeight / 2, -standWidth);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(standWidth, -standFootHeight / 2, 0.0f);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(standWidth, standHeight / 2, standWidth);

			// Back Right Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(standWidth, standHeight / 2, standWidth);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(standWidth, -standFootHeight / 2, 0.0f);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(0.0f, -standFootHeight / 2, standWidth);

			// Back Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(standWidth, standHeight / 2, standWidth);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(0.0f, -standFootHeight / 2, standWidth);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(-standWidth, standHeight / 2, standWidth);

			// Back Left Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(-standWidth, standHeight / 2, standWidth);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(0.0f, -standFootHeight / 2, standWidth);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(-standWidth, -standFootHeight / 2, 0.0f);

			// Left Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(-standWidth, standHeight / 2, standWidth);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(-standWidth, -standFootHeight / 2, 0.0f);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(-standWidth, standHeight / 2, -standWidth);

			// Front Left Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(-standWidth, standHeight / 2, -standWidth);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(-standWidth, -standFootHeight / 2, 0.0f);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(0.0f, -standFootHeight / 2, -standWidth);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);

	if (onTexture)
	{
		glEnable(GL_TEXTURE_2D);
	}
	{
		glBindTexture(GL_TEXTURE_2D, darkBrownTexture);

		// Front Right Stand Foot
		glPushMatrix();
		{
			glTranslatef(-standWidth * 10 / 16, -standFootHeight + standFootHeight / 4, -standWidth * 10 / 16);
			glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
			drawStandFoot(standFootHeight, standWidth * 1.5);
		}
		glPopMatrix();

		// Front Left Stand Foot
		glPushMatrix();
		{
			glTranslatef(standWidth * 10 / 16, -standFootHeight + standFootHeight / 4, -standWidth * 10 / 16);
			glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
			drawStandFoot(standFootHeight, standWidth * 1.5);
		}
		glPopMatrix();

		// Back Right Stand Foot
		glPushMatrix();
		{
			glTranslatef(-standWidth * 10 / 16, -standFootHeight + standFootHeight / 4, standWidth * 10 / 16);
			glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			drawStandFoot(standFootHeight, standWidth * 1.5);
		}
		glPopMatrix();

		// Back Right Stand Foot
		glPushMatrix();
		{
			glTranslatef(standWidth * 10 / 16, -standFootHeight + standFootHeight / 4, standWidth * 10 / 16);
			glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
			drawStandFoot(standFootHeight, standWidth * 1.5);
		}
		glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);
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
				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					if (textureTheme == 1)
					{
						glBindTexture(GL_TEXTURE_2D, bodyTexture);
					}
					else if (textureTheme == 2)
					{
						glBindTexture(GL_TEXTURE_2D, damagebodyTexture);
					}
					gluQuadricTexture(quad, GL_TRUE);

					glEnable(GL_CLIP_PLANE0);
					{
						glClipPlane(GL_CLIP_PLANE0, headTrim);

						// 2. Translate the head up 'radius - trimRadius', trim 'trimRadius' of the front face
						glTranslatef(0.0f, radius - trimRadius, 0.0f);
						// 1. Turn the front face of the head to bottom before trim
						glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
						// PLY: gluSphere()
						gluSphere(quad, radius, headSlices, headStacks);
					}
					glDisable(GL_CLIP_PLANE0);
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();

	// Eye Tube
	glPushMatrix();
	{
		glTranslatef(translatex, translatey + trimRadius * 4, translatez - trimRadius - radius);
		drawEyeTube(sqrt(pow(radius, 2) - pow(radius - trimRadius, 2)), radius);
	}
	glPopMatrix();
}

void drawFinger(GLfloat baseRadius, GLfloat topRadius, GLfloat totalLength, boolean isTumb, boolean isRight)
{
	// Finger: S1=C1=S2=C2=S3
	// S1, S2, S3 = sphere joints
	// C1 + C2 = cylinder finger
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	GLfloat part1Length = totalLength * 3 / 5;
	GLfloat part2Length = totalLength * 2 / 5;
	GLfloat centerRadius = (baseRadius + topRadius) / 2;

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

			if (onTexture)
			{
				glEnable(GL_TEXTURE_2D);
			}
			{
				glBindTexture(GL_TEXTURE_2D, bulletTexture);
				gluQuadricTexture(quad, GL_TRUE);
				// PLY: gluSphere()
				gluSphere(quad, baseRadius, tubeSlices, tubeStacks);
			}
			glDisable(GL_TEXTURE_2D);

			glPushMatrix();
			{
				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					// C1
					glRotatef(rotateFinger, 1.0f, 0.0f, 0.0f);
					glTranslatef(0.0f, 0.0f, -part1Length);
					glBindTexture(GL_TEXTURE_2D, blackMetalTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluCylinder()
					gluCylinder(quad, centerRadius, baseRadius, part1Length, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, purpleMetalTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// S2
					// PLY: gluSphere()
					gluSphere(quad, centerRadius, tubeSlices, tubeStacks);

					glPushMatrix();
					{
						// C2
						glRotatef(rotateFinger, 1.0f, 0.0f, 0.0f);
						glTranslatef(0.0f, 0.0f, -part2Length);
						// PLY: gluCylinder()
						gluCylinder(quad, topRadius, centerRadius, part2Length, tubeSlices, tubeStacks);
						// S3
						// PLY: gluSphere()
						gluSphere(quad, topRadius, tubeSlices, tubeStacks);
					}
					glPopMatrix();
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawPalm(GLfloat basedRadius, GLfloat topRadius, GLfloat halfLength, boolean isRight)
{
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	GLfloat leftBasedRadius;
	GLfloat rightBasedRadius;

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
	if (isRight)
	{
		glPushMatrix();
		{
			glTranslatef(-basedRadius, halfLength, 0.0f);

			if (onTexture)
			{
				glEnable(GL_TEXTURE_2D);
			}
			{
				glBindTexture(GL_TEXTURE_2D, wristTexture);
				gluQuadricTexture(quad, GL_TRUE);
				// PLY: gluSphere()
				gluSphere(quad, basedRadius, tubeSlices, tubeStacks);
			}
			glDisable(GL_TEXTURE_2D);

			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

			if (onTexture)
			{
				glEnable(GL_TEXTURE_2D);
			}
			{
				glBindTexture(GL_TEXTURE_2D, silverMetalTexture);
				gluQuadricTexture(quad, GL_TRUE);
				// PLY: gluCylinder()
				gluCylinder(quad, basedRadius, basedRadius * 1.25, basedRadius * 1.25, tubeSlices, tubeStacks);
				// PLY: gluCylinder()
				gluCylinder(quad, basedRadius, basedRadius * 1.50, basedRadius * 1.25, tubeSlices, tubeStacks);

				glPushMatrix();
				{
					glTranslatef(0.0f, 0.0f, basedRadius * 1.25);
					// PLY: gluDisk()
					gluDisk(quad, basedRadius * 1.25, basedRadius * 1.50, tubeSlices, tubeStacks);
				}
				glPopMatrix();
			}
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();

		// Upper Palm
		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glBindTexture(GL_TEXTURE_2D, blackPalmTexture);

			// PLY: GL_QUADS
			glBegin(GL_QUADS);
			{
				// Front Face
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(leftBasedRadius, halfLength, -basedRadius);
				glTexCoord2f(0, 0);
				drawNormalizedVertex(leftBasedRadius, halfLength * 2 / 3, -basedRadius);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(rightBasedRadius, halfLength * 2 / 3, -basedRadius);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(rightBasedRadius - basedRadius, halfLength, -basedRadius);

				// Back Face
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(leftBasedRadius, halfLength, basedRadius);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(leftBasedRadius, halfLength * 2 / 3, basedRadius);
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(rightBasedRadius, halfLength * 2 / 3, basedRadius);
				glTexCoord2f(0, 0);
				drawNormalizedVertex(rightBasedRadius - basedRadius, halfLength, basedRadius);

				// Left Face
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(leftBasedRadius, halfLength, basedRadius);
				glTexCoord2f(0, 0);
				drawNormalizedVertex(leftBasedRadius, halfLength * 2 / 3, basedRadius);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(leftBasedRadius, halfLength * 2 / 3, -basedRadius);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(leftBasedRadius, halfLength, -basedRadius);

				// Right Face
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(rightBasedRadius - basedRadius, halfLength, basedRadius);
				glTexCoord2f(0, 0);
				drawNormalizedVertex(rightBasedRadius, halfLength * 2 / 3, basedRadius);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(rightBasedRadius, halfLength * 2 / 3, -basedRadius);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(rightBasedRadius - basedRadius, halfLength, -basedRadius);

				// Top Face
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(leftBasedRadius, halfLength, basedRadius);
				glTexCoord2f(0, 0);
				drawNormalizedVertex(leftBasedRadius, halfLength, -basedRadius);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(rightBasedRadius - basedRadius, halfLength, -basedRadius);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(rightBasedRadius - basedRadius, halfLength, basedRadius);
			}
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glPushMatrix();
		{
			glTranslatef(basedRadius, halfLength, 0.0f);

			if (onTexture)
			{
				glEnable(GL_TEXTURE_2D);
			}
			{
				glBindTexture(GL_TEXTURE_2D, wristTexture);
				gluQuadricTexture(quad, GL_TRUE);
				// PLY: gluSphere()
				gluSphere(quad, basedRadius, tubeSlices, tubeStacks);
			}
			glDisable(GL_TEXTURE_2D);

			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

			if (onTexture)
			{
				glEnable(GL_TEXTURE_2D);
			}
			{
				glBindTexture(GL_TEXTURE_2D, silverMetalTexture);
				gluQuadricTexture(quad, GL_TRUE);
				// PLY: gluCylinder()
				gluCylinder(quad, basedRadius, basedRadius * 1.25, basedRadius * 1.25, tubeSlices, tubeStacks);
				// PLY: gluCylinder()
				gluCylinder(quad, basedRadius, basedRadius * 1.50, basedRadius * 1.25, tubeSlices, tubeStacks);

				glPushMatrix();
				{
					glTranslatef(0.0f, 0.0f, basedRadius * 1.25);
					// PLY: gluDisk()
					gluDisk(quad, basedRadius * 1.25, basedRadius * 1.50, tubeSlices, tubeStacks);
				}
				glPopMatrix();
			}
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();

		// Upper Palm
		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glBindTexture(GL_TEXTURE_2D, blackPalmTexture);

			// PLY: GL_QUADS
			glBegin(GL_QUADS);
			{
				// Front Face
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(leftBasedRadius + basedRadius, halfLength, -basedRadius);
				glTexCoord2f(0, 0);
				drawNormalizedVertex(leftBasedRadius, halfLength * 2 / 3, -basedRadius);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(rightBasedRadius, halfLength * 2 / 3, -basedRadius);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(rightBasedRadius, halfLength, -basedRadius);

				// Back Face
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(leftBasedRadius + basedRadius, halfLength, basedRadius);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(leftBasedRadius, halfLength * 2 / 3, basedRadius);
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(rightBasedRadius, halfLength * 2 / 3, basedRadius);
				glTexCoord2f(0, 0);
				drawNormalizedVertex(rightBasedRadius, halfLength, basedRadius);

				// Left Face
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(leftBasedRadius + basedRadius, halfLength, basedRadius);
				glTexCoord2f(0, 0);
				drawNormalizedVertex(leftBasedRadius, halfLength * 2 / 3, basedRadius);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(leftBasedRadius, halfLength * 2 / 3, -basedRadius);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(leftBasedRadius + basedRadius, halfLength, -basedRadius);

				// Right Face
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(rightBasedRadius, halfLength, basedRadius);
				glTexCoord2f(0, 0);
				drawNormalizedVertex(rightBasedRadius, halfLength * 2 / 3, basedRadius);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(rightBasedRadius, halfLength * 2 / 3, -basedRadius);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(rightBasedRadius, halfLength, -basedRadius);

				// Top Face
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(leftBasedRadius + basedRadius, halfLength, basedRadius);
				glTexCoord2f(0, 0);
				drawNormalizedVertex(leftBasedRadius + basedRadius, halfLength, -basedRadius);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(rightBasedRadius, halfLength, -basedRadius);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(rightBasedRadius, halfLength, basedRadius);
			}
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}

	// Lower Palm
	if (onTexture)
	{
		glEnable(GL_TEXTURE_2D);
	}
	{
		glBindTexture(GL_TEXTURE_2D, goldPalmTexture);

		// PLY: GL_QUADS
		glBegin(GL_QUADS);
		{
			// Back Face
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(leftBasedRadius, halfLength * 2 / 3, basedRadius);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(leftBasedRadius, 0.0f, basedRadius);
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(rightBasedRadius, 0.0f, basedRadius);
			glTexCoord2f(0, 0);
			drawNormalizedVertex(rightBasedRadius, halfLength * 2 / 3, basedRadius);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);

	if (onTexture)
	{
		glEnable(GL_TEXTURE_2D);
	}
	{
		glBindTexture(GL_TEXTURE_2D, blackPalmTexture);

		// PLY: GL_QUADS
		glBegin(GL_QUADS);
		{
			// Front Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(leftBasedRadius, halfLength * 2 / 3, -basedRadius);
			glTexCoord2f(0, 0);
			drawNormalizedVertex(leftBasedRadius, 0.0f, -basedRadius);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(rightBasedRadius, 0.0f, -basedRadius);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(rightBasedRadius, halfLength * 2 / 3, -basedRadius);

			// Left Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(leftBasedRadius, halfLength * 2 / 3, basedRadius);
			glTexCoord2f(0, 0);
			drawNormalizedVertex(leftBasedRadius, 0.0f, basedRadius);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(leftBasedRadius, 0.0f, -basedRadius);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(leftBasedRadius, halfLength * 2 / 3, -basedRadius);

			// Right Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(rightBasedRadius, halfLength * 2 / 3, basedRadius);
			glTexCoord2f(0, 0);
			drawNormalizedVertex(rightBasedRadius, 0.0f, basedRadius);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(rightBasedRadius, 0.0f, -basedRadius);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(rightBasedRadius, halfLength * 2 / 3, -basedRadius);

			// Bottom Face
			glTexCoord2f(0, 1.0);
			drawNormalizedVertex(leftBasedRadius, 0.0f, basedRadius);
			glTexCoord2f(0, 0);
			drawNormalizedVertex(leftBasedRadius, 0.0f, -basedRadius);
			glTexCoord2f(1.0, 0);
			drawNormalizedVertex(rightBasedRadius, 0.0f, -basedRadius);
			glTexCoord2f(1.0, 1.0);
			drawNormalizedVertex(rightBasedRadius, 0.0f, basedRadius);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);

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

void drawArmorPlane(GLfloat radius, GLfloat bulge)
{
	// PLY: GL_QUADS
	glBegin(GL_QUADS);
	{
		glTexCoord2f(1.0, 0);
		drawNormalizedVertex(radius * cos(90 * PI / 180), radius * sin(90 * PI / 180), -bulge / 2);
		glTexCoord2f(1.0, 1.0);
		drawNormalizedVertex(radius * cos(120 * PI / 180) * 3 / 4, radius * sin(120 * PI / 180) * 3 / 4, bulge / 2);
		glTexCoord2f(0, 1.0);
		drawNormalizedVertex(radius * cos(225 * PI / 180) * 3 / 4, radius * sin(225 * PI / 180) * 3 / 4, bulge / 2);
		glTexCoord2f(0, 0);
		drawNormalizedVertex(radius * cos(270 * PI / 180) * 3 / 4, radius * sin(270 * PI / 180) * 3 / 4, -bulge / 2);

		glTexCoord2f(1.0, 0);
		drawNormalizedVertex(radius * cos(90 * PI / 180), radius * sin(90 * PI / 180), -bulge / 2);
		glTexCoord2f(1.0, 1.0);
		drawNormalizedVertex(radius * cos(270 * PI / 180) * 3 / 4, radius * sin(270 * PI / 180) * 3 / 4, -bulge / 2);
		glTexCoord2f(0, 1.0);
		drawNormalizedVertex(radius * cos(315 * PI / 180) * 3 / 4, radius * sin(315 * PI / 180) * 3 / 4, bulge / 2);
		glTexCoord2f(0, 0);
		drawNormalizedVertex(radius * cos(60 * PI / 180) * 3 / 4, radius * sin(60 * PI / 180) * 3 / 4, bulge / 2);
	}
	glEnd();
}

void drawArmor(GLfloat radius, GLfloat totalDepth)
{
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);

	glPushMatrix();
	{
		glTranslatef(0.0f, 0.0f, totalDepth / 2);

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glBindTexture(GL_TEXTURE_2D, blackWthBlueStripTexture);

			// PLY: GL_QUAD_STRIP
			glBegin(GL_QUAD_STRIP);
			{
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(0.0f, radius, -totalDepth * 1 / 5);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(0.0f, radius, -totalDepth * 1 / 10);

				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(-radius * cos(60 * PI / 180) * 3 / 4, radius * sin(60 * PI / 180) * 3 / 4, 0.0f);
				glTexCoord2f(0, 0);
				drawNormalizedVertex(-radius * cos(60 * PI / 180) * 3 / 4, radius * sin(60 * PI / 180) * 3 / 4, totalDepth * 1 / 10);

				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(-radius * cos(45 * PI / 180) * 3 / 4, -radius * sin(45 * PI / 180) * 3 / 4, 0.0f);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(-radius * cos(45 * PI / 180) * 3 / 4, -radius * sin(45 * PI / 180) * 3 / 4, totalDepth * 1 / 10);

				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(0.0f, -radius * 3 / 4, -totalDepth * 1 / 5);
				glTexCoord2f(0, 0);
				drawNormalizedVertex(0.0f, -radius * 3 / 4, -totalDepth * 1 / 10);

				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(radius * cos(45 * PI / 180) * 3 / 4, -radius * sin(45 * PI / 180) * 3 / 4, 0.0f);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(radius * cos(45 * PI / 180) * 3 / 4, -radius * sin(45 * PI / 180) * 3 / 4, totalDepth * 1 / 10);

				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(radius * cos(60 * PI / 180) * 3 / 4, radius * sin(60 * PI / 180) * 3 / 4, 0.0f);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(radius * cos(60 * PI / 180) * 3 / 4, radius * sin(60 * PI / 180) * 3 / 4, totalDepth * 1 / 10);

				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(0.0f, radius, -totalDepth * 1 / 5);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(0.0f, radius, -totalDepth * 1 / 10);
			}
			glEnd();

			// Armor Faces
			drawArmorPlane(radius, totalDepth * 1 / 5);

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, -totalDepth * 1 / 10);
				drawArmorPlane(radius, totalDepth * 1 / 5);
			}
			glPopMatrix();
		}
		glDisable(GL_TEXTURE_2D);

		// Spikes
		glPushMatrix();
		{
			glTranslatef(0.0f, -radius / 4, -totalDepth);

			glPushMatrix();
			{
				if (onSpiky)
				{
					if (spikySize < 1.0f)
					{
						spikySize += 0.01f;
					}
				}
				else
				{
					if (firstSpiky)
					{
						spikySize = 0.0f;
						firstSpiky = false;
					}
					else
					{
						if (spikySize > 0.0f)
						{
							spikySize -= 0.01f;
						}
					}
				}

				glScalef(spikySize, spikySize, spikySize);

				glPushMatrix();
				{
					glTranslatef(0.0f, 0.0f, -totalDepth / 5);

					if (onTexture)
					{
						glEnable(GL_TEXTURE_2D);
					}
					{
						glBindTexture(GL_TEXTURE_2D, blueLightRayTexture);
						// PLY: gluSphere()
						gluSphere(quad, radius / 4, tubeSlices, tubeStacks);
					}
					glDisable(GL_TEXTURE_2D);
				}
				glPopMatrix();

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, armorSpikeTexture);
					gluQuadricTexture(quad, GL_TRUE);

					glPushMatrix();
					{
						glTranslatef(0.0f, 0.0f, -totalDepth * 4 / 5);
						glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
						glTranslatef(0.0f, 0.0f, totalDepth * 4 / 5);
						// PLY: gluCylinder()
						gluCylinder(quad, 0.0f, radius / 4, totalDepth * 4 / 5, tubeSlices, tubeStacks);
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(0.0f, 0.0f, -totalDepth * 4 / 5);
						glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
						glTranslatef(0.0f, 0.0f, totalDepth * 4 / 5);
						// PLY: gluCylinder()
						gluCylinder(quad, 0.0f, radius / 4, totalDepth * 4 / 5, tubeSlices, tubeStacks);
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(0.0f, 0.0f, -totalDepth * 4 / 5);
						glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
						glTranslatef(0.0f, 0.0f, totalDepth * 4 / 5);
						// PLY: gluCylinder()
						gluCylinder(quad, 0.0f, radius / 4, totalDepth * 4 / 5, tubeSlices, tubeStacks);
					}
					glPopMatrix();

					glPushMatrix();
					{
						glTranslatef(0.0f, 0.0f, -totalDepth * 4 / 5);
						glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);
						glTranslatef(0.0f, 0.0f, totalDepth * 4 / 5);
						// PLY: gluCylinder()
						gluCylinder(quad, 0.0f, radius / 4, totalDepth * 4 / 5, tubeSlices, tubeStacks);
					}
					glPopMatrix();
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();

			if (onTexture)
			{
				glEnable(GL_TEXTURE_2D);
			}
			{
				glBindTexture(GL_TEXTURE_2D, armorTexture);
				gluQuadricTexture(quad, GL_TRUE);
				// PLY: gluCylinder()
				gluCylinder(quad, 0.0f, radius / 4, totalDepth * 4 / 5, tubeSlices, tubeStacks);
			}
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawWheel(GLfloat radius, GLfloat height)
{
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);

	if (!isStop)
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

			if (onTexture)
			{
				glEnable(GL_TEXTURE_2D);
			}
			{
				glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
				gluQuadricTexture(quad, GL_TRUE);
				// PLY: gluDisk()
				gluDisk(quad, 0.0f, radius, tubeSlices, tubeStacks);

			}
			glDisable(GL_TEXTURE_2D);

			if (onTexture)
			{
				glEnable(GL_TEXTURE_2D);
			}
			{
				glBindTexture(GL_TEXTURE_2D, wheelReactorTexture);
				gluQuadricTexture(quad, GL_TRUE);
				// PLY: gluCylinder()
				gluCylinder(quad, radius, radius, height, tubeSlices, tubeStacks);

				glPushMatrix();
				{
					glTranslatef(0.0f, 0.0f, height);
					// PLY: gluDisk()
					gluDisk(quad, 0.0f, radius, tubeSlices, tubeStacks);
				}
				glPopMatrix();
			}
			glDisable(GL_TEXTURE_2D);
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

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glBindTexture(GL_TEXTURE_2D, darkBrownTexture);
			gluQuadricTexture(quad, GL_TRUE);

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, -height * 9 / 16);
				// PLY: gluDisk()
				gluDisk(quad, 0.0f, radius * 0.9, tubeSlices, tubeStacks);
				// PLY: gluCylinder()
				gluCylinder(quad, radius * 0.9, radius * 0.9, height / 16, tubeSlices, tubeStacks);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, -height / 2);
				// PLY: gluDisk()
				gluDisk(quad, 0.0f, radius * 1.1, tubeSlices, tubeStacks);
				// PLY: gluCylinder()
				gluCylinder(quad, radius * 1.1, radius * 1.1, height / 6, tubeSlices, tubeStacks);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, -height / 3);
				// PLY: gluDisk()
				gluDisk(quad, radius, radius * 1.1, tubeSlices, tubeStacks);
			}
			glPopMatrix();
		}
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, -height / 6);
			glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
			drawWheel(radius, height / 3);
		}
		glPopMatrix();

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glBindTexture(GL_TEXTURE_2D, darkBrownTexture);
			gluQuadricTexture(quad, GL_TRUE);
			// PLY: gluCylinder()
			gluCylinder(quad, radius / 2, radius / 2, height / 4, tubeSlices, tubeStacks);
		}
		glDisable(GL_TEXTURE_2D);

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

void drawGun(GLfloat gunLength)
{
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	GLfloat gunHeight = gunLength / 3;

	if (onTexture)
	{
		glEnable(GL_TEXTURE_2D);
	}
	{
		glBindTexture(GL_TEXTURE_2D, gunMetalTexture);
		gluQuadricTexture(quad, GL_TRUE);
		// Top Front Barrel
		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, -gunLength * 5 / 8);
			// PLY: gluCylinder()
			gluCylinder(quad, gunHeight / 4, gunHeight / 4, gunLength * 7 / 8, tubeSlices, tubeStacks);
		}
		glPopMatrix();

		// Middle Top Front Barrel Disk
		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, -gunLength * 3 / 8);
			// PLY: gluDisk()
			gluDisk(quad, 0.0f, gunHeight / 4, tubeSlices, tubeStacks);
		}
		glPopMatrix();

		// Back Top Front Barrel Disk
		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, gunLength / 4);
			// PLY: gluDisk()
			gluDisk(quad, 0.0f, gunHeight / 4, tubeSlices, tubeStacks);
		}
		glPopMatrix();

		// Bottom Front Barrel
		glPushMatrix();
		{
			glTranslatef(0.0f, -gunHeight / 4, -gunLength / 4);
			// PLY: gluCylinder()
			gluCylinder(quad, gunHeight / 8, gunHeight / 8, gunLength / 2, tubeSlices, tubeStacks);
		}
		glPopMatrix();

		// Middle Bottom Front Barrel Disk
		glPushMatrix();
		{
			glTranslatef(0.0f, -gunHeight / 4, -gunLength * 3 / 16);
			// PLY: gluDisk()
			gluDisk(quad, 0.0f, gunHeight / 8, tubeSlices, tubeStacks);
		}
		glPopMatrix();

		// Back Bottom Front Barrel Disk
		glPushMatrix();
		{
			glTranslatef(0.0f, -gunHeight / 4, gunLength / 4);
			// PLY: gluDisk()
			gluDisk(quad, 0.0f, gunHeight / 8, tubeSlices, tubeStacks);
		}
		glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);

	if (onTexture)
	{
		glEnable(GL_TEXTURE_2D);
	}
	{
		glBindTexture(GL_TEXTURE_2D, gunTriggerTexture);
		gluQuadricTexture(quad, GL_TRUE);
		// Outer Trigger
		glPushMatrix();
		{
			glTranslatef(-gunHeight / 4, -gunHeight / 4, gunLength / 8);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			// PLY: gluCylinder()
			gluCylinder(quad, gunLength / 8, gunLength / 8, gunHeight / 2, tubeSlices, tubeStacks);
		}
		glPopMatrix();

		// Inner Trigger
		glPushMatrix();
		{
			glTranslatef(-gunHeight / 4, -gunHeight / 4 + 0.0015f, gunLength / 8);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			// PLY: gluCylinder()
			gluCylinder(quad, gunLength / 8 - 0.0015, gunLength / 8 - 0.0015, gunHeight / 2, tubeSlices, tubeStacks);
		}
		glPopMatrix();

		// Right Trigger Disk
		glPushMatrix();
		{
			glTranslatef(-gunHeight / 4, -gunHeight / 4, gunLength / 8);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			// PLY: gluPartialDisk()
			gluPartialDisk(quad, gunLength / 8 - 0.0015, gunLength / 8, tubeSlices, tubeStacks, 90.0f, 270.0f);
		}
		glPopMatrix();

		// Left Trigger Disk
		glPushMatrix();
		{
			glTranslatef(gunHeight / 4, -gunHeight / 4, gunLength / 8);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			// PLY: gluPartialDisk()
			gluPartialDisk(quad, gunLength / 8 - 0.0015, gunLength / 8, tubeSlices, tubeStacks, 90.0f, 270.0f);
		}
		glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);

	// Handle
	glPushMatrix();
	{
		glTranslatef(0.0f, -gunHeight * 7 / 16, gunHeight * 7 / 16 + gunLength / 32);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -gunHeight * 7 / 16);

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glBindTexture(GL_TEXTURE_2D, gunMetalTexture);
			gluQuadricTexture(quad, GL_TRUE);
			// PLY: gluCylinder()
			gluCylinder(quad, gunLength / 16, gunLength / 16, gunHeight * 7 / 8, tubeSlices, tubeStacks);
		}
		glDisable(GL_TEXTURE_2D);

		glTranslatef(0.0f, 0.0f, gunHeight * 7 / 8);

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
			gluQuadricTexture(quad, GL_TRUE);
			// PLY: gluSphere()
			gluSphere(quad, gunLength / 16, tubeSlices, tubeStacks);
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();

	glRotatef(magazineSpeed, 0.0f, 0.0f, 1.0f);

	// Top Right Magazine
	glPushMatrix();
	{
		glTranslatef(gunHeight / 8, gunHeight / 8, 0.0f);

		// Top Right Magazine Bullet
		glPushMatrix();
		{
			if (triggerCount >= 1)
			{
				bulletSpeeds[0] += 0.01f;
				glTranslatef(0.0f, 0.0f, -bulletSpeeds[0]);
				glTranslatef(-gunHeight / 8, -gunHeight / 8, 0.0f);
			}

			if (isReload)
			{
				glTranslatef(0.0f, 0.0f, bulletSpeeds[0]);
				bulletSpeeds[0] = 0.0f;
			}

			if (isTriggered && triggerCount == 0)
			{
				triggerCount = 1;
				isTriggered = false;
			}

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, -gunLength / 10 + gunHeight / 5);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, bulletTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluSphere()
					gluSphere(quad, gunHeight / 5, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluCylinder()
					gluCylinder(quad, gunHeight / 5, gunHeight / 5, (gunLength - gunHeight) / 5, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, gunLength / 10);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluDisk()
					gluDisk(quad, 0.0f, gunHeight / 5, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();
		}
		glPopMatrix();

		glTranslatef(0.0f, 0.0f, -gunLength / 8);

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glBindTexture(GL_TEXTURE_2D, gunMegazineTexture);
			gluQuadricTexture(quad, GL_TRUE);

			// PLY: gluDisk()
			gluDisk(quad, 0.0f, gunHeight / 4, tubeSlices, tubeStacks);
			// PLY: gluCylinder()
			gluCylinder(quad, gunHeight / 4, gunHeight / 4, gunLength / 4, tubeSlices, tubeStacks);

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, gunLength / 4);
				// PLY: gluDisk()
				gluDisk(quad, 0.0f, gunHeight / 4, tubeSlices, tubeStacks);
			}
			glPopMatrix();
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();

	// Bottom Right Magazine
	glPushMatrix();
	{
		glTranslatef(gunHeight / 8, -gunHeight / 8, 0.0f);

		// Bottom Right Magazine Bullet
		glPushMatrix();
		{
			if (triggerCount >= 2)
			{
				bulletSpeeds[1] += 0.01f;
				glTranslatef(0.0f, 0.0f, -bulletSpeeds[1]);
				glTranslatef(-gunHeight / 8, gunHeight / 8, 0.0f);
			}

			if (isReload)
			{
				glTranslatef(0.0f, 0.0f, bulletSpeeds[1]);
				bulletSpeeds[1] = 0.0f;
			}

			if (isTriggered && triggerCount == 1)
			{
				triggerCount = 2;
				isTriggered = false;
			}

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, -gunLength / 10 + gunHeight / 5);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, bulletTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluSphere()
					gluSphere(quad, gunHeight / 5, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluCylinder()
					gluCylinder(quad, gunHeight / 5, gunHeight / 5, (gunLength - gunHeight) / 5, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, gunLength / 10);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluDisk()
					gluDisk(quad, 0.0f, gunHeight / 5, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();
		}
		glPopMatrix();

		glTranslatef(0.0f, 0.0f, -gunLength / 8);

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glBindTexture(GL_TEXTURE_2D, gunMegazineTexture);
			gluQuadricTexture(quad, GL_TRUE);

			// PLY: gluDisk()
			gluDisk(quad, 0.0f, gunHeight / 4, tubeSlices, tubeStacks);
			// PLY: gluCylinder()
			gluCylinder(quad, gunHeight / 4, gunHeight / 4, gunLength / 4, tubeSlices, tubeStacks);

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, gunLength / 4);
				// PLY: gluDisk()
				gluDisk(quad, 0.0f, gunHeight / 4, tubeSlices, tubeStacks);
			}
			glPopMatrix();
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();

	// Bottom Left Magazine
	glPushMatrix();
	{
		glTranslatef(-gunHeight / 8, -gunHeight / 8, 0.0f);

		// Bottom Left Magazine Bullet
		glPushMatrix();
		{
			if (triggerCount >= 3)
			{
				bulletSpeeds[2] += 0.01f;
				glTranslatef(0.0f, 0.0f, -bulletSpeeds[2]);
				glTranslatef(gunHeight / 8, gunHeight / 8, 0.0f);
			}

			if (isReload)
			{
				glTranslatef(0.0f, 0.0f, bulletSpeeds[2]);
				bulletSpeeds[2] = 0.0f;
			}

			if (isTriggered && triggerCount == 2)
			{
				triggerCount = 3;
				isTriggered = false;
			}

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, -gunLength / 10 + gunHeight / 5);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, bulletTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluSphere()
					gluSphere(quad, gunHeight / 5, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluCylinder()
					gluCylinder(quad, gunHeight / 5, gunHeight / 5, (gunLength - gunHeight) / 5, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, gunLength / 10);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluDisk()
					gluDisk(quad, 0.0f, gunHeight / 5, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();
		}
		glPopMatrix();

		glTranslatef(0.0f, 0.0f, -gunLength / 8);

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glBindTexture(GL_TEXTURE_2D, gunMegazineTexture);
			gluQuadricTexture(quad, GL_TRUE);

			// PLY: gluDisk()
			gluDisk(quad, 0.0f, gunHeight / 4, tubeSlices, tubeStacks);
			// PLY: gluCylinder()
			gluCylinder(quad, gunHeight / 4, gunHeight / 4, gunLength / 4, tubeSlices, tubeStacks);

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, gunLength / 4);
				// PLY: gluDisk()
				gluDisk(quad, 0.0f, gunHeight / 4, tubeSlices, tubeStacks);
			}
			glPopMatrix();
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();

	// Top Left Magazine
	glPushMatrix();
	{
		glTranslatef(-gunHeight / 8, gunHeight / 8, 0.0f);

		// Top Left Magazine Bullet
		glPushMatrix();
		{
			if (triggerCount >= 4)
			{
				bulletSpeeds[3] += 0.01f;
				glTranslatef(0.0f, 0.0f, -bulletSpeeds[3]);
				glTranslatef(gunHeight / 8, -gunHeight / 8, 0.0f);
			}

			if (isReload)
			{
				glTranslatef(0.0f, 0.0f, bulletSpeeds[3]);
				bulletSpeeds[3] = 0.0f;
			}

			if (isTriggered && triggerCount == 3)
			{
				triggerCount = 4;
				isTriggered = false;
			}

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, -gunLength / 10 + gunHeight / 5);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, bulletTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluSphere()
					gluSphere(quad, gunHeight / 5, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluCylinder()
					gluCylinder(quad, gunHeight / 5, gunHeight / 5, (gunLength - gunHeight) / 5, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, gunLength / 10);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluDisk()
					gluDisk(quad, 0.0f, gunHeight / 5, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();
		}
		glPopMatrix();

		glTranslatef(0.0f, 0.0f, -gunLength / 8);

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glBindTexture(GL_TEXTURE_2D, gunMegazineTexture);
			gluQuadricTexture(quad, GL_TRUE);

			// PLY: gluDisk()
			gluDisk(quad, 0.0f, gunHeight / 4, tubeSlices, tubeStacks);
			// PLY: gluCylinder()
			gluCylinder(quad, gunHeight / 4, gunHeight / 4, gunLength / 4, tubeSlices, tubeStacks);

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, gunLength / 4);
				// PLY: gluDisk()
				gluDisk(quad, 0.0f, gunHeight / 4, tubeSlices, tubeStacks);
			}
			glPopMatrix();
		}
		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
}

void drawShoulder(GLfloat shoulderHeight, GLfloat armorDepth, boolean isRight)
{
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	GLfloat gunLength = shoulderHeight * 3 / 2;

	glPushMatrix();
	{
		if (!isRight)
		{
			glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		}

		glPushMatrix();
		{
			glTranslatef(-shoulderHeight * 3 / 4, -shoulderHeight * 1 / 6, 0.0f);

			if (onTexture)
			{
				glEnable(GL_TEXTURE_2D);
			}
			{
				glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
				gluQuadricTexture(quad, GL_TRUE);
				// PLY: gluSphere()
				gluSphere(quad, shoulderHeight * 3 / 10, tubeSlices, tubeStacks);
			}
			glDisable(GL_TEXTURE_2D);
		}
		glPopMatrix();

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			if (textureTheme == 1)
			{
				glBindTexture(GL_TEXTURE_2D, shoulderTexture);
			}
			else if (textureTheme == 2)
			{

				glBindTexture(GL_TEXTURE_2D, damageShoulderTexture);
			}

			// PLY: GL_QUADS
			glBegin(GL_QUADS);
			{
				// Top Face
				glTexCoord2f(0, 0);
				drawNormalizedVertex(-shoulderHeight / 2, shoulderHeight / 2, shoulderHeight * 3 / 8);
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(-shoulderHeight / 2, shoulderHeight / 2, -shoulderHeight * 3 / 8);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(shoulderHeight / 4, shoulderHeight / 2, -shoulderHeight * 3 / 8);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(shoulderHeight / 4, shoulderHeight / 2, shoulderHeight * 3 / 8);

				// Front Top Slope
				glTexCoord2f(0, 0);
				drawNormalizedVertex(-shoulderHeight / 2, shoulderHeight / 2, -shoulderHeight * 3 / 8);
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, shoulderHeight * 1 / 6, -shoulderHeight / 2);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(shoulderHeight / 2, shoulderHeight * 1 / 6, -shoulderHeight / 2);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(shoulderHeight / 4, shoulderHeight / 2, -shoulderHeight * 3 / 8);

				// Back Top Slope
				glTexCoord2f(0, 0);
				drawNormalizedVertex(-shoulderHeight / 2, shoulderHeight / 2, shoulderHeight * 3 / 8);
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, shoulderHeight * 1 / 6, shoulderHeight / 2);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(shoulderHeight / 2, shoulderHeight * 1 / 6, shoulderHeight / 2);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(shoulderHeight / 4, shoulderHeight / 2, shoulderHeight * 3 / 8);

				// Left Top Slope
				glTexCoord2f(0, 0);
				drawNormalizedVertex(-shoulderHeight / 2, shoulderHeight / 2, shoulderHeight * 3 / 8);
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, shoulderHeight * 1 / 6, shoulderHeight / 2);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, shoulderHeight * 1 / 6, -shoulderHeight / 2);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(-shoulderHeight / 2, shoulderHeight / 2, -shoulderHeight * 3 / 8);

				// Left Face
				glTexCoord2f(0, 0);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, shoulderHeight * 1 / 6, shoulderHeight / 2);
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, -shoulderHeight / 2, shoulderHeight / 2);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, -shoulderHeight / 2, -shoulderHeight / 2);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, shoulderHeight * 1 / 6, -shoulderHeight / 2);

				// Right Bottom Slope
				glTexCoord2f(0, 0);
				drawNormalizedVertex(shoulderHeight * 3 / 4, -shoulderHeight * 1 / 6, -shoulderHeight / 2);
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(shoulderHeight / 4, -shoulderHeight / 2, -shoulderHeight / 2);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(shoulderHeight / 4, -shoulderHeight / 2, shoulderHeight / 2);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(shoulderHeight * 3 / 4, -shoulderHeight * 1 / 6, shoulderHeight / 2);

				// Bottom Face
				glTexCoord2f(0, 0);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, -shoulderHeight / 2, shoulderHeight / 2);
				glTexCoord2f(0, 1.0);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, -shoulderHeight / 2, -shoulderHeight / 2);
				glTexCoord2f(1.0, 1.0);
				drawNormalizedVertex(shoulderHeight / 4, -shoulderHeight / 2, -shoulderHeight / 2);
				glTexCoord2f(1.0, 0);
				drawNormalizedVertex(shoulderHeight / 4, -shoulderHeight / 2, shoulderHeight / 2);
			}
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			if (textureTheme == 1)
			{
				glBindTexture(GL_TEXTURE_2D, blackMetalTexture);
			}
			else if (textureTheme == 2)
			{

				glBindTexture(GL_TEXTURE_2D, damageLowerShoulderTexture);
			}

			// PLY: GL_POLYGON
			glBegin(GL_POLYGON);
			{
				// Front Face
				glTexCoord2f(0.0f, 1.0f);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, shoulderHeight * 1 / 6, -shoulderHeight / 2);
				glTexCoord2f(0.0f, 0.0f);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, -shoulderHeight / 2, -shoulderHeight / 2);
				glTexCoord2f(0.75f, 0.0f);
				drawNormalizedVertex(shoulderHeight / 4, -shoulderHeight / 2, -shoulderHeight / 2);
				glTexCoord2f(1.0f, 0.5f);
				drawNormalizedVertex(shoulderHeight * 3 / 4, -shoulderHeight * 1 / 6, -shoulderHeight / 2);
				glTexCoord2f(0.85f, 1.0f);
				drawNormalizedVertex(shoulderHeight / 2, shoulderHeight * 1 / 6, -shoulderHeight / 2);
			}
			glEnd();

			// PLY: GL_POLYGON
			glBegin(GL_POLYGON);
			{
				// Back Face
				glTexCoord2f(0.0f, 1.0f);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, shoulderHeight * 1 / 6, shoulderHeight / 2);
				glTexCoord2f(0.0f, 0.0f);
				drawNormalizedVertex(-shoulderHeight * 3 / 4, -shoulderHeight / 2, shoulderHeight / 2);
				glTexCoord2f(0.75f, 0.0f);
				drawNormalizedVertex(shoulderHeight / 4, -shoulderHeight / 2, shoulderHeight / 2);
				glTexCoord2f(1.0f, 0.5f);
				drawNormalizedVertex(shoulderHeight * 3 / 4, -shoulderHeight * 1 / 6, shoulderHeight / 2);
				glTexCoord2f(0.85f, 1.0f);
				drawNormalizedVertex(shoulderHeight / 2, shoulderHeight * 1 / 6, shoulderHeight / 2);
			}
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			if (textureTheme == 1)
			{
				glBindTexture(GL_TEXTURE_2D, shoulderTexture);
			}
			else if (textureTheme == 2)
			{

				glBindTexture(GL_TEXTURE_2D, damageShoulderTexture);
			}

			// PLY: GL_POLYGON
			glBegin(GL_POLYGON);
			{
				// Right Top Slope
				glTexCoord2f(0.0f, 1.0f);
				drawNormalizedVertex(shoulderHeight / 4, shoulderHeight / 2, -shoulderHeight * 3 / 8);
				glTexCoord2f(0.0f, 0.5f);
				drawNormalizedVertex(shoulderHeight / 2, shoulderHeight * 1 / 6, -shoulderHeight / 2);
				glTexCoord2f(0.0f, 0.0f);
				drawNormalizedVertex(shoulderHeight * 3 / 4, -shoulderHeight * 1 / 6, -shoulderHeight / 2);
				glTexCoord2f(1.0f, 0.0f);
				drawNormalizedVertex(shoulderHeight * 3 / 4, -shoulderHeight * 1 / 6, shoulderHeight / 2);
				glTexCoord2f(1.0f, 0.5f);
				drawNormalizedVertex(shoulderHeight / 2, shoulderHeight * 1 / 6, shoulderHeight / 2);
				glTexCoord2f(1.0f, 1.0f);
				drawNormalizedVertex(shoulderHeight / 4, shoulderHeight / 2, shoulderHeight * 3 / 8);
			}
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);

		// 4 Guns
		glPushMatrix();
		{
			if (isRight)
			{
				glRotatef(-rotateGunY, 0.0f, 1.0f, 0.0f);
			}
			else
			{
				glRotatef(rotateGunY, 0.0f, 1.0f, 0.0f);
			}

			if (onGun)
			{
				if (gunSize < 1.0f)
				{
					gunSize += 0.01f;
				}

				if (gunTranslate < shoulderHeight * 3 / 2 + shoulderHeight / 7)
				{
					gunTranslate += 0.001 * speed * elapsedSeconds;
				}
			}
			else
			{
				if (firstGun)
				{
					gunSize = 0.0f;
					firstGun = false;
				}
				else
				{
					if (gunSize > 0.0f)
					{
						gunSize -= 0.01f;
					}
				}
			}

			glScalef(gunSize, gunSize, gunSize);

			glPushMatrix();
			{
				glTranslatef(0.0f, gunTranslate, 0.0f);

				if (isRight)
				{
					glTranslatef(-shoulderHeight / 7, 0.0f, -shoulderHeight / 4);
				}
				else
				{
					glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
					glTranslatef(shoulderHeight / 7, 0.0f, -shoulderHeight / 4);
				}

				drawGun(gunLength);
			}
			glPopMatrix();
		}
		glPopMatrix();

		glPushMatrix();
		{
			if (isRight)
			{
				glTranslatef(-shoulderHeight / 3, -shoulderHeight / 7, -shoulderHeight / 2);
			}
			else
			{
				glTranslatef(-shoulderHeight / 3, -shoulderHeight / 7, shoulderHeight / 2);
			}

			drawWheel(shoulderHeight / 4, shoulderHeight / 6);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(-shoulderHeight / 8, shoulderHeight * 7 / 8, 0.0f);
			drawNail(shoulderHeight * 3 / 8, shoulderHeight * 3 / 4);
		}
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	{
		if (onArmor)
		{
			if (armorSize < 1.0f)
			{
				armorSize += 0.01f;
			}
		}
		else
		{
			if (firstArmor)
			{
				armorSize = 0.0f;
				firstArmor = false;
			}
			else
			{
				if (armorSize > 0.0f)
				{
					armorSize -= 0.01f;
				}
			}
		}

		glScalef(armorSize, armorSize, armorSize);

		if (isRight)
		{
			glTranslatef(shoulderHeight * 3 / 4 + armorDepth / 3, armorDepth / 2, 0.0f);
			glRotatef(-135.0, 0.0f, 0.0f, 1.0f);
			glRotatef(90.0, 0.0f, 1.0f, 0.0f);
		}
		else
		{
			glTranslatef(-shoulderHeight * 3 / 4 - armorDepth / 3, armorDepth / 2, 0.0f);
			glRotatef(135.0, 0.0f, 0.0f, 1.0f);
			glRotatef(-90.0, 0.0f, 1.0f, 0.0f);
		}

		drawArmor(shoulderHeight, armorDepth);
	}
	glPopMatrix();
}

void drawArm(GLfloat baseRadius, GLfloat topRadius, GLfloat totalLength, boolean isRight)
{
	// Arm: S1=C1=S2=C2=S3=C3
	// S1, S2, S3 = sphere joints
	// C1 + C2 = cylinder arms
	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	GLfloat partLength = totalLength / 2;
	GLfloat centerRadius = (baseRadius + topRadius) / 2;

	if (!isRight)
	{
		glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
	}

	glPushMatrix();
	{
		// S1
		glTranslatef(0.0f, 0.0f, -partLength);

		if (!firstRest)
		{
			glRotatef(restSpeed, 0.0f, 1.0f, 0.0f);
		}

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
			gluQuadricTexture(quad, GL_TRUE);
			// PLY: gluSphere()
			gluSphere(quad, baseRadius, tubeSlices, tubeStacks);
		}
		glDisable(GL_TEXTURE_2D);

		//C1
		glRotatef(rotateUpperArmJointX, 1.0f, 0.0f, 0.0f);
		glRotatef(rotateUpperArmJointY, 0.0f, 1.0f, 0.0f);
		glRotatef(rotateUpperArmJointZ, 0.0f, 0.0f, 1.0f);

		if (!isRight)
		{
			glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
		}

		if (onTexture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		{
			if (textureTheme == 1)
			{
				glBindTexture(GL_TEXTURE_2D, blackWthBlueStripTexture);
			}
			else if (textureTheme == 2)
			{
				glBindTexture(GL_TEXTURE_2D, damageLegTexture);
			}

			gluQuadricTexture(quad, GL_TRUE);
			// PLY: gluCylinder()
			gluCylinder(quad, baseRadius, centerRadius, partLength * 3 / 4, tubeSlices, tubeStacks);
		}
		glDisable(GL_TEXTURE_2D);

		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, partLength * 3 / 4);
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);

			if (onTexture)
			{
				glEnable(GL_TEXTURE_2D);
			}
			{
				glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
				gluQuadricTexture(quad, GL_TRUE);
				// S2
				// PLY: gluSphere()
				gluSphere(quad, centerRadius, tubeSlices, tubeStacks);
			}
			glDisable(GL_TEXTURE_2D);

			// C2
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
			}

			if (onTexture)
			{
				glEnable(GL_TEXTURE_2D);
			}
			{
				if (textureTheme == 1)
				{
					glBindTexture(GL_TEXTURE_2D, blackWthBlueStripTexture);
				}
				else if (textureTheme == 2)
				{
					glBindTexture(GL_TEXTURE_2D, damageLegTexture);
				}

				gluQuadricTexture(quad, GL_TRUE);
				// PLY: gluCylinder()
				gluCylinder(quad, centerRadius, topRadius, partLength, tubeSlices, tubeStacks);
			}
			glDisable(GL_TEXTURE_2D);

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, partLength);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, jointMetalTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// S3
					// PLY: gluSphere()
					gluSphere(quad, topRadius, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);

				// C3
				glRotatef(rotateWristJointX, 1.0f, 0.0f, 0.0f);
				glRotatef(rotateWristJointY, 0.0f, 1.0f, 0.0f);
				glRotatef(rotateWristJointZ, 0.0f, 0.0f, 1.0f);

				if (onTexture)
				{
					glEnable(GL_TEXTURE_2D);
				}
				{
					glBindTexture(GL_TEXTURE_2D, legTipTexture);
					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluCylinder()
					gluCylinder(quad, topRadius, 0.0f, partLength / 4, tubeSlices, tubeStacks);
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void drawLimbs(GLfloat shoulderHeight, GLfloat armorDepth, GLfloat armLength, GLfloat wristJointRadius, boolean isRight)
{
	GLfloat shouderJointRadius = shoulderHeight / 4;

	// Shoulders
	glPushMatrix();
	{
		if (onRest)
		{
			if (firstRest)
			{
				restSpeed = 0.0f;
				firstRest = false;
			}
			else
			{
				if (restSpeed > 0.0f)
				{
					restSpeed -= 0.175f;
				}
			}
		}
		else
		{
			if (restSpeed < 35.0f)
			{
				restSpeed += 0.175f;
			}
		}

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

		glPushMatrix();
		{
			if (isRight)
			{
				glRotatef(restSpeed / 32 * 5.0f, 0.0f, 0.0f, 1.0f);
			}
			else
			{
				glRotatef(-restSpeed / 32 * 5.0f, 0.0f, 0.0f, 1.0f);
			}

			drawShoulder(shoulderHeight, armorDepth, isRight);
		}
		glPopMatrix();

		// Arm
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

void drawRobot(GLfloat mainRadius, GLfloat headRotate, GLfloat wristJointRadius, GLfloat fingerTipRadius, GLfloat palmLength)
{
	GLfloat trimRadius = mainRadius / 13;
	GLdouble headTrim[4] = { 0.0, 1.0, 0.0, 0.0 };
	GLfloat shoulderHeight = mainRadius / 2;
	GLfloat armLength = mainRadius * 2;
	GLfloat armorDepth = shoulderHeight / 0.65f;

	if (onRest)
	{
		if (firstRest)
		{
			restSpeed = 0.0f;
			firstRest = false;
		}
		else
		{
			if (restSpeed > 0.0f)
			{
				restSpeed -= 0.175f;
			}
		}
	}
	else
	{
		if (restSpeed < 35.0f)
		{
			restSpeed += 0.175f;
		}
	}

	glPushMatrix();
	{
		glTranslatef(0.0f, restSpeed / 35.0 * 0.1f, 0.0f);
		// Head
		drawHead(mainRadius, trimRadius, 0.0f, 0.0f, 0.0f, headRotate, 1.0f, 0.0f, 0.0f, headTrim);

		// Stand
		glTranslatef(0.0f, -mainRadius * 1.5 + restSpeed / 35.0 * mainRadius * 1.5, 0.0f);
		glScalef(1.0f - restSpeed / 35.0, 1.0f - restSpeed / 35.0, 1.0f - restSpeed / 35.0);
		drawStand(mainRadius);
	}
	glPopMatrix();

	// 4 Arms
	//	Right Front Arm
	glPushMatrix();
	{
		glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.325f + 0.325 / 4, 0.0f, -0.325f / 4);
		drawLimbs(shoulderHeight, armorDepth, armLength, wristJointRadius, true);
	}
	glPopMatrix();

	//	Right Back Arm
	glPushMatrix();
	{
		glRotatef(-25.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.325f + 0.325 / 4, 0.0f, 0.325f / 4);
		drawLimbs(shoulderHeight, armorDepth, armLength, wristJointRadius, true);
	}
	glPopMatrix();

	//	Left Front Arm
	glPushMatrix();
	{
		glRotatef(-25.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(-0.325f - 0.325 / 4, 0.0f, -0.325f / 4);
		drawLimbs(shoulderHeight, armorDepth, armLength, wristJointRadius, false);
	}
	glPopMatrix();

	//	Left Back Arm
	glPushMatrix();
	{
		glRotatef(25.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(-0.325f - 0.325 / 4, 0.0f, 0.325f / 4);
		drawLimbs(shoulderHeight, armorDepth, armLength, wristJointRadius, false);
	}
	glPopMatrix();

	// Hands
	glPushMatrix();
	{
		if (onHand)
		{
			if (handSize < 1.0f)
			{
				handSize += 0.01f;
			}
		}
		else
		{
			if (firstHand)
			{
				handSize = 0.0f;
				firstHand = false;
			}
			else
			{
				if (handSize > 0.0f)
				{
					handSize -= 0.01f;
				}
			}
		}

		glScalef(handSize, handSize, handSize);

		glPushMatrix();
		{
			glRotatef(rotateHandX, 1.0f, 0.0f, 0.0f);
			glRotatef(rotateHandY, 0.0f, 1.0f, 0.0f);
			glRotatef(rotateHandZ, 0.0f, 0.0f, 1.0f);

			//	Upper Left Hand
			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, punchSpeed);
				glTranslatef(mainRadius / 2, mainRadius * 2.5, -palmLength);
				glRotatef(rotatePalmX, 1.0f, 0.0f, 0.0f);
				glRotatef(rotatePalmY, 0.0f, 1.0f, 0.0f);
				glRotatef(rotatePalmZ, 0.0f, 0.0f, 1.0f);
				glTranslatef(-mainRadius / 2, -mainRadius * 2.5, palmLength);
				glTranslatef(mainRadius / 2, mainRadius * 2.5, -palmLength);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				glRotatef(180.0, 0.0, 1.0, 0.0);
				drawPalm(wristJointRadius, fingerTipRadius, palmLength, true);
			}
			glPopMatrix();

			//	Upper Right Hand
			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, -punchSpeed);
				glTranslatef(-mainRadius / 2, mainRadius * 2.5, -palmLength);
				glRotatef(rotatePalmX, 1.0f, 0.0f, 0.0f);
				glRotatef(rotatePalmY, 0.0f, 1.0f, 0.0f);
				glRotatef(rotatePalmZ, 0.0f, 0.0f, 1.0f);
				glTranslatef(mainRadius / 2, -mainRadius * 2.5, palmLength);
				glTranslatef(-mainRadius / 2, mainRadius * 2.5, -palmLength);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				glRotatef(180.0f, 0.0, 1.0, 0.0);
				drawPalm(wristJointRadius, fingerTipRadius, palmLength, false);
			}
			glPopMatrix();

			//	Lower Left Hand
			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, -punchSpeed);
				glTranslatef(mainRadius, mainRadius * 2, 0.0f);
				glRotatef(rotatePalmX, 1.0f, 0.0f, 0.0f);
				glRotatef(rotatePalmY, 0.0f, 1.0f, 0.0f);
				glRotatef(rotatePalmZ, 0.0f, 0.0f, 1.0f);
				glTranslatef(-mainRadius, -mainRadius * 2, 0.0f);
				glTranslatef(mainRadius, mainRadius * 2, 0.0f);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				glRotatef(180.0, 0.0, 1.0, 0.0);
				drawPalm(wristJointRadius, fingerTipRadius, palmLength, true);
			}
			glPopMatrix();

			//	Lower Right Hand
			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, punchSpeed);
				glTranslatef(-mainRadius, mainRadius * 2, 0.0f);
				glRotatef(rotatePalmX, 1.0f, 0.0f, 0.0f);
				glRotatef(rotatePalmY, 0.0f, 1.0f, 0.0f);
				glRotatef(rotatePalmZ, 0.0f, 0.0f, 1.0f);
				glTranslatef(mainRadius, -mainRadius * 2, 0.0f);
				glTranslatef(-mainRadius, mainRadius * 2, 0.0f);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				glRotatef(180.0f, 0.0, 1.0, 0.0);
				drawPalm(wristJointRadius, fingerTipRadius, palmLength, false);
			}
			glPopMatrix();
		}
		glPopMatrix();
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
	glMatrixMode(GL_MODELVIEW);

	GLUquadricObj* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	GLfloat mainRadius = 0.325f;
	GLfloat headRotate = 20.0f;
	GLfloat wristJointRadius = 0.02625f;
	GLfloat fingerTipRadius = 0.01875f;
	GLfloat palmLength = 0.1275f;
	GLfloat environmentRadius = 5.0f;
	//eyeX = r * sin(angle * 3.142 / 180.0);
	//eyeZ = r * cos(angle * 3.142 / 180.0);

	glColor3f(0.0f, 0.0f, 0.0f);

	glPushMatrix();
	{
		gluLookAt(
			eyeX, eyeY, eyeZ,	// Eye
			lookAtX, lookAtY, lookAtZ,	// Look At
			upX, upY, upZ);	// World Up

		glRotatef(camRotation[0], 1.0f, 0.0f, 0.0f);
		glRotatef(camRotation[1], 0.0f, 1.0f, 0.0f);
		glRotatef(camRotation[2], 0.0f, 0.0f, 1.0f);

		if (onLight)
		{
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHTING);
		}
		else
		{
			glDisable(GL_LIGHT0);
			glDisable(GL_LIGHTING);
		}

		if (isDay)
		{
			glLightfv(GL_LIGHT0, GL_AMBIENT, whiteLight);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, blackLight);
			glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
		}
		else
		{
			glLightfv(GL_LIGHT0, GL_AMBIENT, blackLight);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
			glLightfv(GL_LIGHT0, GL_SPECULAR, blackLight);
		}

		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, positionLight);

		setMaterial(whiteLight, whiteLight, whiteLight);

		glPushMatrix();
		{
			// Sky
			glPushMatrix();
			{
				glTranslatef(0.0f, environmentRadius, 0.0f);
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

				glEnable(GL_TEXTURE_2D);
				{
					if (environmentTheme == 1)
					{
						glBindTexture(GL_TEXTURE_2D, ruinSkyTexture);
					}
					else if (environmentTheme == 2)
					{
						glBindTexture(GL_TEXTURE_2D, ruinSky2Texture);
					}

					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluDisk()
					gluDisk(quad, 0.0f, environmentRadius, headSlices, headStacks);
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();

			// Ruin
			glPushMatrix();
			{
				glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
				glTranslatef(0.0f, 0.0f, -environmentRadius);

				glEnable(GL_TEXTURE_2D);
				{
					if (environmentTheme == 1)
					{
						glBindTexture(GL_TEXTURE_2D, ruinTexture);
					}
					else if (environmentTheme == 2)
					{
						glBindTexture(GL_TEXTURE_2D, ruin2Texture);
					}

					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluSphere()
					gluCylinder(quad, environmentRadius, environmentRadius, environmentRadius * 2.0, headSlices, headStacks);
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();

			// Floor
			glPushMatrix();
			{
				glTranslatef(0.0f, -environmentRadius, 0.0f);
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

				glEnable(GL_TEXTURE_2D);
				{
					if (environmentTheme == 1)
					{
						glBindTexture(GL_TEXTURE_2D, ruinFloorTexture);
					}
					else if (environmentTheme == 2)
					{
						glBindTexture(GL_TEXTURE_2D, ruinFloor2Texture);
					}

					gluQuadricTexture(quad, GL_TRUE);
					// PLY: gluDisk()
					gluDisk(quad, 0.0f, environmentRadius, headSlices, headStacks);
				}
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();

			// Robot Name: Ruin Araneid
			glPushMatrix();
			{
				glTranslatef(0.0f, -environmentRadius + mainRadius * 2, 0.0f);
				setMaterial(greyLight, greyLight, greyLight);
				drawRobot(mainRadius, headRotate, wristJointRadius, fingerTipRadius, palmLength);
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}

void setupCamera()
{
	glMatrixMode(GL_PROJECTION);
	glOrtho(-5, 5, -5, 5, 2, 10);
}

void setupLighting()
{
	if (isDay)
	{
		glLightfv(GL_LIGHT0, GL_AMBIENT, whiteLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, blackLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
	}
	else
	{
		glLightfv(GL_LIGHT0, GL_AMBIENT, blackLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, blackLight);
	}
}

void setupGLProperties()
{
	glEnable(GL_DEPTH_TEST);
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
	setupTextures("blueStrip.bmp", &blueStripTexture);
	setupTextures("blackMetal.bmp", &blackMetalTexture);
	setupTextures("blueLightRay.bmp", &blueLightRayTexture);
	setupTextures("jointMetal.bmp", &jointMetalTexture);
	setupTextures("blackWthBlueStrip.bmp", &blackWthBlueStripTexture);
	setupTextures("legTip.bmp", &legTipTexture);
	setupTextures("body.bmp", &bodyTexture);
	setupTextures("wheelReactor.bmp", &wheelReactorTexture);
	setupTextures("darkBrown.bmp", &darkBrownTexture);
	setupTextures("shoulder.bmp", &shoulderTexture);
	setupTextures("darkGreyMetal.bmp", &darkGreyMetalTexture);
	setupTextures("armor.bmp", &armorTexture);
	setupTextures("armorSpike.bmp", &armorSpikeTexture);
	setupTextures("damageEye.bmp", &damageEyeTexture);
	setupTextures("damageBody.bmp", &damagebodyTexture);
	setupTextures("damageLeg.bmp", &damageLegTexture);
	setupTextures("damageShoulder.bmp", &damageShoulderTexture);
	setupTextures("damageLowerShoulder.bmp", &damageLowerShoulderTexture);

	setupTextures("upperStand.bmp", &upperStandTexture);

	setupTextures("gunMetal.bmp", &gunMetalTexture);
	setupTextures("gunMagazine.bmp", &gunMegazineTexture);
	setupTextures("gunTrigger.bmp", &gunTriggerTexture);
	setupTextures("bullet.bmp", &bulletTexture);

	setupTextures("purpleMetal.bmp", &purpleMetalTexture);
	setupTextures("blackPalm.bmp", &blackPalmTexture);
	setupTextures("goldPalm.bmp", &goldPalmTexture);
	setupTextures("wrist.bmp", &wristTexture);
	setupTextures("silverMetal.bmp", &silverMetalTexture);

	setupTextures("ruin.bmp", &ruinTexture);
	setupTextures("ruin2.bmp", &ruin2Texture);
	setupTextures("ruinFloor.bmp", &ruinFloorTexture);
	setupTextures("ruinFloor2.bmp", &ruinFloor2Texture);
	setupTextures("ruinSky.bmp", &ruinSkyTexture);
	setupTextures("ruinSky2.bmp", &ruinSky2Texture);

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