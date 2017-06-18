//  
// Copyright (c) 2017 Konjahin A.A. <koniahin.ant@yandex.ru>
// Licensed under the MIT License. See LICENSE file in the project root for full license information.  
//

#include <windows.h>
#include "keyboard.h"


typedef struct {
  char *title;
  WORD keyCode;
} Key;

typedef struct {
  int padding;
  int width;
  int height;
  int rows;
  int columns;
  HBRUSH background;
  Key *keys;
} Layout;


static Key* currentKeys = NULL;

WORD getActionForKey(int keyHandler) {
  return currentKeys[keyHandler].keyCode;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch(msg) {

  case WM_CLOSE:
    DestroyWindow(hwnd);
    break;
    
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
    
  case WM_COMMAND:
    // processed only button's clicks
    if (HIWORD(wParam) != BN_CLICKED) {
      break;
    }
    
    INPUT input = {0};
    press(&input, getActionForKey(LOWORD(wParam)));
    break;
    
  case WM_NCHITTEST:
    // move window by user drag
    LRESULT hit = DefWindowProc(hwnd, msg, wParam, lParam);
    if (hit == HTCLIENT) hit = HTCAPTION;
    return hit;
    
  case WM_WINDOWPOSCHANGING:
    // forbid window maximization
    ((WINDOWPOS*) lParam)->flags |= SWP_NOSIZE;
    return 1;
    
  default:
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  
  return 0;
}


HWND createMainWindow(Layout *layout, HINSTANCE hInstance, int nCmdShow) {
  const char className[] = "keyTuneWindowClass";
  WNDCLASSEX wc = {0};

  int width = (layout->width + layout->padding) * layout->columns + layout->padding;
  int height = (layout->height + layout->padding) * layout->rows + layout->padding;
  
  wc.cbSize        = sizeof(WNDCLASSEX);
  wc.lpfnWndProc   = WndProc;
  wc.hInstance     = hInstance;
  wc.hbrBackground = layout->background;
  wc.lpszClassName = className;
  wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
  
  RegisterClassEx(&wc);
  
  HWND hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_THICKFRAME,
			     className, "Keytune", WS_POPUP | WS_VISIBLE | WS_SYSMENU, CW_USEDEFAULT,
			     CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);
  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  return hwnd;
}


HWND createButton(Layout *layout, HWND window, int x, int y) {
  int index = x + y * layout->columns;
  Key key = layout->keys[index];

  if (key.keyCode == NULL) {
    return NULL;
  }
  
  int positionX = layout->padding + (layout->padding + layout->width) * x;
  int positionY = layout->padding + (layout->padding + layout->height) * y;
  return CreateWindow("BUTTON", key.title, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
		      positionX, positionY, layout->width, layout->height, window, (HMENU) index,
		      (HINSTANCE)GetWindowLong(window, GWL_HINSTANCE), NULL);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

  Key keys[] = {
    {.title = "<", .keyCode = VK_LEFT},
    {.title = "Esc", .keyCode = VK_ESCAPE},
    {.title = ">", .keyCode = VK_RIGHT},
    {0}
  };
  
  Layout layout = {
    .padding = 10,
    .width = 60,
    .height = 40,
    .rows = 1,
    .columns = 4,
    .background = (HBRUSH) 0,
    .keys = keys
  };

  currentKeys = layout.keys;

  HWND hwnd = createMainWindow(&layout, hInstance, nCmdShow);

  for (int x = 0; x < layout.columns; x++) {
    for (int y = 0; y < layout.rows; y++) {
      createButton(&layout, hwnd, x, y);
    }
  }

  MSG Msg;
  while(GetMessage(&Msg, NULL, 0, 0) > 0) {
    TranslateMessage(&Msg);
    DispatchMessage(&Msg);
  }
  
  return Msg.wParam;
}
