//  
// Copyright (c) 2017 Konjahin A.A. <koniahin.ant@yandex.ru>
// Licensed under the MIT License. See LICENSE file in the project root for full license information.  
//

#include "keyboard.h"

void setUpInput(INPUT *ip) {
  ip->type = INPUT_KEYBOARD;
  ip->ki.wScan = 0;
  ip->ki.time = 0;
  ip->ki.dwExtraInfo = 0;
}

void hold(INPUT *ip, WORD keyCode) {
  setUpInput(ip);
  ip->ki.wVk = keyCode;
  ip->ki.dwFlags = 0;
  SendInput(1, ip, sizeof(INPUT));
}

void release(INPUT *ip, WORD keyCode) {
  setUpInput(ip);
  ip->ki.wVk = keyCode;
  ip->ki.dwFlags = KEYEVENTF_KEYUP;
  SendInput(1, ip, sizeof(INPUT));
}

void press(INPUT *ip, WORD keyCode) {
  hold(ip, keyCode);
  release(ip, keyCode);
}

void getCharForKey(WORD keyCode, wchar_t* buffer, size_t bufferLength) {
  HKL keyboardLayout = GetKeyboardLayout(0);
  UINT scanCode = MapVirtualKeyEx(keyCode, MAPVK_VK_TO_VSC, keyboardLayout);
  
  BYTE keyState[256] = {0};
  GetKeyboardState(keyState);
  
  ToUnicodeEx(keyCode, scanCode, keyState, buffer, bufferLength, 0, keyboardLayout);
}
