/*
* PlayLib - V1.4
*
* Description: Biblioteca grafica 2D.
*
* Author: Edirlei Soares de Lima (VisionLab - PUC-Rio)
* Web: http://www.inf.puc-rio.br/~elima/
* Email: elima@inf.puc-rio.br
*
*/
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "freeglut.h"
#include <math.h>

//Teclas Especiais
#define  KEY_F1 128
#define  KEY_F2 129
#define  KEY_F3 130
#define  KEY_F4 131
#define  KEY_F5 132
#define  KEY_F6 133
#define  KEY_F7 134
#define  KEY_F8 135
#define  KEY_F9 136
#define  KEY_F10 137
#define  KEY_F11 138
#define  KEY_F12 139
#define  KEY_LEFT 227
#define  KEY_UP 228
#define  KEY_RIGHT 229
#define  KEY_DOWN 230
#define  KEY_PAGE_UP 231
#define  KEY_PAGE_DOWN 232
#define  KEY_HOME 233
#define  KEY_END 234
#define  KEY_INSERT 235
#define  KEY_ESC 27
#define  KEY_ENTER 13
#define  KEY_BACKSPACE 8
#define  KEY_LEFTCTRL 241
#define  KEY_RIGHTCTRL 242
#define  KEY_LEFTSHIFT 239
#define  KEY_RIGHTSHIFT 240
#define  KEY_LEFTALT 243
#define  KEY_RIGHTALT 244
#define  KEY_TAB 9

//Botoes do Mouse
#define  MOUSE_LEFT_BUTTON 0x0000
#define  MOUSE_MIDDLE_BUTTON 0x0001
#define  MOUSE_RIGHT_BUTTON 0x0002

//Estados dos botões do mouse
#define  MOUSE_STATE_DOWN 0x0000
#define  MOUSE_STATE_UP 0x0001

//Estados das teclas
#define  KEY_STATE_DOWN 0x0000
#define  KEY_STATE_UP 0x0001

//Font Weights
#define FONT_WEIGHT_NONE 0
#define FONT_WEIGHT_THIN 100
#define FONT_WEIGHT_EXTRALIGHT 200
#define FONT_WEIGHT_LIGHT 300
#define FONT_WEIGHT_NORMAL 400
#define FONT_WEIGHT_MEDIUM 500
#define FONT_WEIGHT_SEMIBOLD 600
#define FONT_WEIGHT_BOLD 700
#define FONT_WEIGHT_EXTRABOLD 800
#define FONT_WEIGHT_HEAVY 900

typedef void (*MainLoolCallback)();
typedef void (*MouseMotionCallback)(int x, int y);
typedef void (*MouseClickCallback)(int button, int state, int x, int y);
typedef void (*KeyboardInputCallback)(int key, int state, int x, int y);

namespace PlayLib
{	
	class Image
	{
	public:
		Image();
		unsigned int image;
		int x;
		int y;
		int width;
		int height;
		bool active;
		typedef void (*OnClickCallback)(int button, int state, int xx, int yy);
		OnClickCallback onclickcallback;
		void LoadPNGImage(char *filename);
		void SetOnClick(void (*callback)(int button, int state, int xx, int yy));
		void SetPosition(int px, int py, int pwidth, int pheight);
	private:		
		void OnClick(int button, int state, int xx, int yy);
			
	};
		
    class Graphics
    {
    public:
		Graphics();
        void CreateMainWindow(int sizeX, int sizeY, char title[]);
		void SetMainLoop(void (*callback)());
		void SetKeyboardInput(void (*callback)(int key, int state, int x, int y));
		void SetMouseClickInput(void (*callback)(int button, int state, int x, int y));
		void SetMouseMotionInput(void (*callback)(int x, int y));
		void StartMainLoop(void);
		void DrawText2D(int x, int y, const char *text, ...);
		void SetTextFont(const char *font_name, int size, int weight, bool italic, bool underline);
		void DrawImage2D(Image image);
		void DrawImage2D(int x, int y, int width, int height, Image image);
		void DrawImage2D(int x, int y, int width, int height, int crop_x, int crop_y, int crop_width, int crop_height, Image image);
		void DrawLine2D(int x1, int y1, int x2, int y2);
		void DrawPoint2D(int x, int y);
		void RotateBegin(float angle);
		void RotateEnd();
		void DrawCircle2D(int x, int y, int radius);
		void FillCircle2D(int x, int y, int radius, int segments);
		void DrawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3);
		void FillTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3);
		void FillRectangle2D(int x1, int y1, int x2, int y2);
		void DrawRectangle2D(int x1, int y1, int x2, int y2);
		void SetBackgroundColor(float r, float g, float b);
		void SetColor(float r, float g, float b);
		void SetLineWidth(float width);
		void SetFullscreen(bool enable);
		float GetFPS();
		float GetElapsedTime();
		int GetScreenWidth();
		int GetScreenHeight();
	private:
		bool is_rotating;
		float rotation_angle;
    };	
}

using namespace PlayLib;

#endif