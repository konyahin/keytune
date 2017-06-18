//  
// Copyright (c) 2017 Konjahin A.A. <koniahin.ant@yandex.ru>
// Licensed under the MIT License. See LICENSE file in the project root for full license information.  
//

#include <windows.h>
#include <winuser.h>

/**
 * Send press event for key with virtual-key codes
 * Arguments:
 *  ip - INPUT structure from winuser.h
 *  keyCode - WORD with virtual key-code from https://msdn.microsoft.com/en-us/library/dd375731(v=vs.85).aspx
 */
void hold(INPUT *ip, WORD keyCode);

/**
 * Send release event for key with virtual-key codes
 * Arguments:
 *  ip - INPUT structure from winuser.h
 *  keyCode - WORD with virtual key-code from https://msdn.microsoft.com/en-us/library/dd375731(v=vs.85).aspx
 */
void release(INPUT *ip, WORD keyCode);

/**
 * Send press and release event for key with virtual-key codes
 * Arguments:
 *  ip - INPUT structure from winuser.h
 *  keyCode - WORD with virtual key-code from https://msdn.microsoft.com/en-us/library/dd375731(v=vs.85).aspx
 */
void press(INPUT *ip, WORD keyCode);

/**
 * Put in buffer with wide chars text representation of keyboard's key
 * with virtual-key codes in current keyboard layout.
 * Arguments:
 *  keyCode - WORD with virtual key-code from https://msdn.microsoft.com/en-us/library/dd375731(v=vs.85).aspx
 *  buffer - wxhar_t* wide chars buffer for output
 *  bufferLength size_t size of buffer 
 */
void getCharForKey(WORD keyCode, wchar_t* buffer, size_t bufferLength);
