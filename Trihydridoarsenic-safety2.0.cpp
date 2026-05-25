#include <windows.h>
#include <ctime>
#include <cmath>
#include <cstdlib>
#define M_PI   3.14159265358979323846264338327950288 
#pragma comment(lib, "winmm.lib")
DWORD WINAPI shader1(LPVOID lpParam) {
    int startTime = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), 
                                           MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    
    // Seed using GetTickCount (already available, no extra include needed)
    srand(GetTickCount());

    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);

        int v = 0;
        BYTE byte = 0;
        if ((GetTickCount() - startTime) > 60000)
            byte = rand() % 0xff;

        for (int x = 0; x < w * h; x++) {          // renamed to avoid shadowing
            if (x % h == 0 && rand() % 100 == 0)
                v = rand() % 50;

            *((BYTE*)data + 4 * x + v) = ((BYTE*)(data + x + v))[v] ^ byte;
        }

        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);

        DeleteObject(hbm);
        DeleteObject(hdcdc);
        ReleaseDC(NULL, desk);
    }
    return 0;
}
DWORD WINAPI shader2(LPVOID lpParam) {
	int ticks = GetTickCount(), w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {

		HDC hdc = GetDC(0), hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcMem, hbm);
		BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0;BYTE bt = 0;
		if ((GetTickCount() - ticks) > 60000) bt = rand() & 0xffffff;
		for (int i = 0; w * h > i; i++) {
			if (!(i % h == 0) && !((rand() % 100))) v = rand() % 24;
			*((BYTE*)data + 4 * i + v) -= *((BYTE*)data + v);
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		DeleteObject(hbm); DeleteObject(hdcMem);
		DeleteObject(hdc);
	}
}
DWORD WINAPI sharpen(LPVOID lpParam) {
   while (1) {
      HDC hdc = GetDC(0);
      int sw = GetSystemMetrics(0);
      int sh = GetSystemMetrics(1);
      SetStretchBltMode(hdc, 4);
      StretchBlt(hdc, 1, 1, sw + 2, sh + 2, hdc, 0, 0, sw, sh, SRCCOPY);
      StretchBlt(hdc, -1, -1, sw - 2, sh - 2, hdc, 0, 0, sw, sh, SRCCOPY);
      ReleaseDC(0, hdc);
   }
}
DWORD WINAPI mousedraw(LPVOID lpParam) {
	while(1){
		HDC hdc = GetDC(HWND_DESKTOP);
		int icon_x = GetSystemMetrics(SM_CXICON);
		int icon_y = GetSystemMetrics(SM_CYICON) ;
		POINT cursor;
        GetCursorPos(&cursor);
        DrawIcon(hdc, cursor.x - icon_x, cursor.y - icon_y, LoadIcon(NULL, IDI_ERROR));
        ReleaseDC(0, hdc);
	}
}
DWORD WINAPI shader3(LPVOID lpParam) {
	int ticks = GetTickCount(), w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	for (int i = 0;; i++, i %= 3) {

		HDC hdc = GetDC(0), hdcMem = CreateCompatibleDC(hdc);HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
		SelectObject(hdcMem, hbm);
		BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		GetBitmapBits(hbm, w * h * 4, data);
		int v = 0;BYTE bt = 0;
		if ((GetTickCount() - ticks) > 60000) bt = rand() & 0xffffff;
		for (int i = 0; w * h > i; i++) {
			if (i % h == 0 && rand() % 100 == 0) v = rand() % 50;
			((BYTE*)(data + i))[v ? 252 : 252] += ((BYTE*)(data + i))[i % 3] ^ bt;
		}
		SetBitmapBits(hbm, w * h * 4, data);
		BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		DeleteObject(hbm); DeleteObject(hdcMem);
		DeleteObject(hdc);
	}
}
DWORD WINAPI BitBlt1(LPVOID lpParam) {
	HDC desktop = GetDC(NULL);
	int xs = GetSystemMetrics(SM_CXSCREEN);
	int ys = GetSystemMetrics(SM_CYSCREEN);
	while (true) {
		desktop = GetDC(NULL);
		BitBlt(desktop, 1,1, xs, ys, desktop, -2,2, SRCERASE);
		ReleaseDC(0, desktop);
	}
}
DWORD WINAPI InvScreenShake(LPVOID lpParam){
	HDC desktop = GetDC(NULL);
	int xs = GetSystemMetrics(SM_CXSCREEN);
	int ys = GetSystemMetrics(SM_CYSCREEN);
	while (true) {
		desktop = GetDC(NULL);
		BitBlt(desktop, rand() % 10,rand() % 10, xs, ys, desktop, rand() % 10,rand() %10, NOTSRCCOPY);
		ReleaseDC(0, desktop);
	}
}
VOID WINAPI ci(int x, int y, int w, int h)
{
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w+x, h+y);
    SelectClipRgn(hdc, hrgn);
    BitBlt(hdc, x, y, w, h, hdc, x, y, NOTSRCCOPY);
    DeleteObject(hrgn);
    ReleaseDC(NULL, hdc);
}
DWORD WINAPI wef(LPVOID lpParam) {
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    int w = rect.right - rect.left - 500, h = rect.bottom - rect.top - 500;
 
    for (int t = 0;; t++)
    {
        const int size = 1000;
        int x = rand()%(w+size)-size/2, y = rand()%(h+size)-size/2;
 
        for (int i = 0; i < size; i+=100)
        {
            ci(x-i/2, y-i/2, i, i);
            Sleep(10);
        }
    }
}
DWORD WINAPI swirl(LPVOID lpParam) {
    HDC desk = GetDC(0);
    int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN), xSize = sh/10, ySize = 9;
    while (1) {
    	HDC desk = GetDC(0);
        for (int i = 0; i < sh*2; i++) {
            int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
            BitBlt(desk, i, 0, 1, sh, desk, i, wave, SRCCOPY);
        }
        for (int i = 0; i < sw*2; i++) {
            int wave = sin(i / ((float)xSize) * M_PI) * (ySize);
            BitBlt(desk, 0, i, sw, 1, desk, wave, i, SRCCOPY);
        }
        ReleaseDC(0, desk);
    }
}
DWORD WINAPI icons(LPVOID lpParam) {
	HDC hDc = GetWindowDC(GetDesktopWindow());
	int x = rand() % GetSystemMetrics(SM_CXSCREEN);
	int y = rand() % GetSystemMetrics(SM_CYSCREEN);
	while (true) {
		HDC hDc = GetWindowDC(GetDesktopWindow());
		x = rand() % GetSystemMetrics(SM_CXSCREEN);
		y = rand() % GetSystemMetrics(SM_CYSCREEN);
		DrawIcon(hDc, x, y, LoadIcon(0, IDI_ERROR));
		ReleaseDC(0, hDc);
	}
}
DWORD WINAPI sines(LPVOID lpParam) {
  HDC desk = GetDC(0); HWND wnd = GetDesktopWindow();
  int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
  double angle = 0;
  while (1) {
    desk = GetDC(0);
    for (float i = 0; i < sw + sh; i += 0.99f) {
      int a = sin(angle) * 20;
      BitBlt(desk, 0, i, sw, 1, desk, a, i, NOTSRCERASE);
      angle += M_PI / 40;
      DeleteObject(&i); DeleteObject(&a);
    }
    ReleaseDC(wnd, desk);
    DeleteDC(desk); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&angle);
  }
}
VOID WINAPI sound1() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t<<1^(t<<1)+(t>>7)&t>>12|t>>4-(1^7&t>>19)|t>>7);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t*((t/2>>10|t%16*t>>8)&8*t>>12&18)|-(t/16)+64);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound3() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(43*(4*t>>43|4*t>>4));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound4() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[22050 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t>>6^t&t>>9^t>>12|(t<<(t>>6)%4^-t&-t>>13)%128^-t>>1);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound5() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t>>6^t&t>>9^t>>12|((t>>6|t<<1)+(t>>5|t<<3|t>>3)|t>>2|t<<1));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound6() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t>>(t%32?4:3)|(t%128?t>>3:t>>3|t>>9));

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
VOID WINAPI sound7() {
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	char buffer[8000 * 30] = {};
	for (DWORD t = 0; t < sizeof(buffer); ++t)
		buffer[t] = static_cast<char>(t*(0xCA98>>(t>>9&14)&15)|t>>8);

	WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	if (MessageBoxW(NULL, L"Run Trihydridoarsenic-safety2.0.exe GDI only.", L"Trihydridoarsenic-safety2.0.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"Are you sure? There's no window shake or Task manager starting up. want to run it press Yes", L"Trihydridoarsenic-safety2.0.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
				Sleep(5000);
				HANDLE thread1 = CreateThread(0, 0, shader1, 0, 0, 0);
				sound1();
				Sleep(30000);
				TerminateThread(thread1, 0);
				CloseHandle(thread1);
    			InvalidateRect(0, 0, 0);
				Sleep(100);
				HANDLE thread2 = CreateThread(0, 0, shader2, 0, 0, 0);
				sound2();
				Sleep(30000);
				TerminateThread(thread2, 0);
				CloseHandle(thread2);
    			InvalidateRect(0, 0, 0);
				Sleep(100);
				HANDLE thread3 = CreateThread(0, 0, sharpen, 0, 0, 0);
				HANDLE thread3dot1 = CreateThread(0, 0, mousedraw, 0, 0, 0);
				sound3();
				Sleep(30000);
				TerminateThread(thread3, 0);
				CloseHandle(thread3);
    			InvalidateRect(0, 0, 0);
				Sleep(100);
    			InvalidateRect(0, 0, 0);
				HANDLE thread4 = CreateThread(0, 0, shader3, 0, 0, 0);
				sound4();
				Sleep(30000);
				TerminateThread(thread4, 0);
				CloseHandle(thread4);
    			InvalidateRect(0, 0, 0);
				Sleep(100);
				HANDLE thread5 = CreateThread(0, 0, BitBlt1, 0, 0, 0);
				sound5();
				Sleep(30000);
				TerminateThread(thread5, 0);
				CloseHandle(thread5);
    			InvalidateRect(0, 0, 0);
				Sleep(100);
				HANDLE thread6 = CreateThread(0, 0, InvScreenShake, 0, 0, 0);
				HANDLE thread7 = CreateThread(0, 0, wef, 0, 0, 0);
				sound6();
				Sleep(30000);
				TerminateThread(thread6, 0);
				CloseHandle(thread6);
    			InvalidateRect(0, 0, 0);
				TerminateThread(thread7, 0);
				CloseHandle(thread7);
    			InvalidateRect(0, 0, 0);
				Sleep(100);
				HANDLE thread8 = CreateThread(0, 0, swirl, 0, 0, 0);
				HANDLE thread9 = CreateThread(0, 0, icons, 0, 0, 0);
				sound7();
				Sleep(15000);
				TerminateThread(thread8, 0);
				CloseHandle(thread8);
    			InvalidateRect(0, 0, 0);
				TerminateThread(thread9, 0);
				CloseHandle(thread9);
    			InvalidateRect(0, 0, 0);
				HANDLE thread10 = CreateThread(0, 0, sines, 0, 0, 0);
				Sleep(15000);
		}
	}
}
