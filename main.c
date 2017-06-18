//  
// Copyright (c) 2017 Konjahin A.A. <koniahin.ant@yandex.ru>
// Licensed under the MIT License. See LICENSE file in the project root for full license information.  
//

#include <windows.h>
#include "keyboard.h"


#define BUTTON_LEFT   1
#define BUTTON_RIGHT  2
#define BUTTON_ESCAPE 3


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch(msg) {
  case WM_CREATE:
    HWND hwndButton = CreateWindow("BUTTON", "<", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
				   10, 10, 40, 40, hwnd, (HMENU) BUTTON_LEFT,
				   (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
    hwndButton = CreateWindow("BUTTON", "Esc", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
			      60, 10, 40, 40, hwnd, (HMENU) BUTTON_ESCAPE,
			      (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
    hwndButton = CreateWindow("BUTTON", ">", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
			      110, 10, 40, 40, hwnd, (HMENU) BUTTON_RIGHT,
			      (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
    break;
	    
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
    
    INPUT input;
    switch(LOWORD(wParam)) {
    case BUTTON_LEFT:
      press(&input, VK_LEFT);
      break;
    case BUTTON_RIGHT:
      press(&input, VK_RIGHT);
      break;
    case BUTTON_ESCAPE:
      press(&input, VK_ESCAPE);
      break;
    }
    break;
    
    // move window by user drag
  case WM_NCHITTEST:
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


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  const char className[] = "keyTuneWindowClass";
  WNDCLASSEX wc = {0};
  
  wc.cbSize        = sizeof(WNDCLASSEX);
  wc.lpfnWndProc   = WndProc;
  wc.hInstance     = hInstance;
  wc.hbrBackground = 1;
  wc.lpszClassName = className;
  wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
  
  RegisterClassEx(&wc);
  
  HWND hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_THICKFRAME,
			     className, "Keytune", WS_POPUP | WS_VISIBLE | WS_SYSMENU, CW_USEDEFAULT,
			     CW_USEDEFAULT, 160, 60, NULL, NULL, hInstance, NULL);
  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);
  
  MSG Msg;
  while(GetMessage(&Msg, NULL, 0, 0) > 0) {
    TranslateMessage(&Msg);
    DispatchMessage(&Msg);
  }
  
  return Msg.wParam;
}
