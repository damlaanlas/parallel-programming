#include <windows.h>  
#include <stdlib.h>
#include <random>
#include "chmat.h"
#include <math.h>
#include <string>
using namespace std;
#include <iostream>
#include <cstring>
#include<mmsystem.h>

#define IDI_ICON1 101
#define IDC_OPEN	3000
#define IDC_BUTTON1 3001
#define IDC_BUTTON2 3002
#define IDC_BUTTON3 3003

HINSTANCE hInst;   // current instance
HWND     hWnd, hEdit, HButton1, HButton2, HButton3, Hmainbmp;    //parent window
HANDLE hTimerQueue = NULL;
HANDLE hElmaTimer = NULL;
HANDLE hMuzTimer = NULL;
HANDLE hHamburgerTimer = NULL;

LPCTSTR lpszAppName = "GEN32";
LPCTSTR lpszTitle = "SAGLIKLI BESLENME OYUNU";
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static int total = 0;
int i = 0;

CHMAT m(500,300, CHMAT_INT);

int keypressed;
int boxlocation = 10; // sepetin başlangıç konumu

//meyveleri yakalayan sepet oluşturuldu
VOID CALLBACK SlidingBox(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
	//sepetin hareketi doğrultusunda sepetin rengini siler 
	for (int i = 0; i < 20; i++)
	{
		m(boxlocation + i, 30) = 0;
		m(boxlocation + i, 31) = 0;
		m(boxlocation + i, 32) = 0;
		m(boxlocation + i, 33) = 0;
		m(boxlocation + i, 34) = 0;
		m(boxlocation + i, 35) = 0;
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
		
	}

	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
	//sepetin sağa sola hareketine göre konumunu değiştirir
	if (keypressed == 37) boxlocation -= 2;//sol
	if (keypressed == 39) boxlocation += 2;//sağ

	//sepetin hareketi doğrultusunda sepeti çizer
	for (int i = 0; i < 20; i++)
	{
		m(boxlocation + i, 30) = 0xffffff;
		m(boxlocation + i, 31) = 0xffffff;
		m(boxlocation + i, 32) = 0xffffff;
		m(boxlocation + i, 33) = 0xffffff;
		m(boxlocation + i, 34) = 0xffffff;
		m(boxlocation + i, 35) = 0xffffff;
		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
		
	}
	 
	
}
void AppendText(HWND hEditWnd, LPCTSTR Text) {
	int idx = GetWindowTextLength(hEditWnd);
	SendMessage(hEditWnd, EM_SETSEL, (WPARAM)idx, (LPARAM)idx);
	SendMessage(hEditWnd, EM_REPLACESEL, 0, (LPARAM)Text);
}

//elma düşmesi için thread oluşturuldu
void* elma(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	
	int x, y, xcor = 150;
	// yukarıdan random elma düşmesini sağlar
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(30, 290); // düşecek alanın koordinatlarını sınırlandırdık
    xcor = dist6(rng);

	Sleep(i);
	for (y = 290; y > 1; y--) // düşecek elma y ekseninde 290 yüksekliğinden 1 yüksekliğine doğru gitsin
	{
			for (x = 1; x < 20; x++) // aşağı doğru inen elmayı arkasından siler
			{
				m(xcor + x, y) = 0x00;
				m(xcor + x, y + 1) = 0x00;
				m(xcor + x, y + 2) = 0;
				m(xcor + x, y + 3) = 0;
			}
			if (y>25) { // 25 yüksekliğinden sonra aşağıda birikmeyi önlemek için if kontrolune alındı
				for (x = 1; x < 20; x++) // aşağı doğru yeşil çizgi çizer
				{
					m(xcor + x, y) = 0x00ff00;
					m(xcor + x, y + 1) = 0x00ff00;
					m(xcor + x, y + 2) = 0x00ff00;
                    m(xcor + x, y - 1) = 0x00ff00;
				} 
			}
		
			
			if (xcor >= boxlocation-20 && xcor <= boxlocation + 20 && y <= 35 ) { //sepet ile meyve buluşuyorsa

				if (xcor >= boxlocation - 20 && xcor <= boxlocation + 20 && y <= 35 && y >= 30) {
					total = total + 1; // elma yendiğinde 6 puan ekler.
					auto s = std::to_string(total);
					SetWindowText(hEdit, s.c_str());
					AppendText(hEdit, TEXT(" : senin skorun.\r\n "));
				}
				
					for (x = 1; x < 20; x++) // elmayı sil
					{

						m(xcor + x, y) = 0x00;
						m(xcor + x, y + 1) = 0x00;
						m(xcor + x, y + 2) = 0;
						m(xcor + x, y + 3) = 0;
						m(xcor + x, y - 1) = 0;

					}
					
			}
			
					SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
					
		Sleep(10); // düşme hızını belirliyor
	}
	return 0;
}

// muz düşmesi için thread oluşturduk
void* muz(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{

	int x, y, xcor = 150;
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(30, 290); 

	xcor = dist6(rng);
	Sleep(i);
	for (y = 290; y > 1; y--)
	{
		for (x = 1; x < 20; x++)
		{
			
			m(xcor + x, y) = 0x00;
			m(xcor + x, y + 1) = 0x00;
			m(xcor + x, y + 2) = 0;
			m(xcor + x, y + 3) = 0;

		}
		if (y > 25) {
			for (x = 1; x < 20; x++)
			{   
				
				m(xcor + x, y) = 0xffff00;
				m(xcor + x, y + 1) = 0xffff00;
				m(xcor + x, y + 2) = 0xffff00;
				m(xcor + x, y - 1) = 0xffff00;
                
			}
		}

		if (xcor >= boxlocation - 20 && xcor <= boxlocation + 20 && y <= 35 && y >= 0) {

			if (xcor >= boxlocation - 20 && xcor <= boxlocation + 20 && y <= 35 && y >= 30) {
				total = total + 2; // muz yendiğinde 12 puan ekler.
				auto s = std::to_string(total);
				SetWindowText(hEdit, s.c_str());
				AppendText(hEdit, TEXT(" : senin skorun.\r\n "));
			}
			for (x = 1; x < 20; x++)
			{
                
				m(xcor + x, y) = 0x00;
				m(xcor + x, y + 1) = 0x00;
				m(xcor + x, y + 2) = 0;
				m(xcor + x, y + 3) = 0;
				m(xcor + x, y - 1) = 0;
			}
			 
			
		}
	

		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);

		Sleep(10); // düşme hızını belirliyor.
	}
	return 0;
}

void* hamburger(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{

	int x, y, xcor = 150;
	// yukarıdan random hamburger düşmesini sağlar
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(30, 290); // düşecek alanın koordinatlarını sınırlandırdık
	xcor = dist6(rng);

	Sleep(i);
	for (y = 290; y > 1; y--) // düşecek hamburger y ekseninde 290 yüksekliğinden 1 yüksekliğine doğru gitsin
	{
		for (x = 1; x < 20; x++) // aşağı doğru inen hamburgerleri arkasından siler
		{
			m(xcor + x, y) = 0x00;
			m(xcor + x, y + 1) = 0x00;
			m(xcor + x, y + 2) = 0;
			m(xcor + x, y + 3) = 0;
		}
		if (y > 25) { // 25 yüksekliğinden sonra aşağıda birikmeyi önlemek için if kontrolune alındı
			for (x = 1; x < 20; x++) // aşağı doğru kahverengi hamburgeri çizgi olarak çizer
			{
				m(xcor + x, y) = 0x964b00;
				m(xcor + x, y + 1) = 0x964b00;
				m(xcor + x, y + 2) = 0x964b00;
				m(xcor + x, y - 1) = 0x964b00;
			}
		}

		if (xcor >= boxlocation - 20 && xcor <= boxlocation + 20 && y <= 35 && y >= 0) { //sepet ile hamburger buluşuyorsa
			if (xcor >= boxlocation - 20 && xcor <= boxlocation + 20 && y <= 35 && y >= 30) {
				total = total - 4 ; // hamburger yendiğinde 24 puan siler.
				auto s = std::to_string(total); // skoru ekrana yazdırır.
				SetWindowText(hEdit, s.c_str());
				AppendText(hEdit, TEXT(" : senin skorun.\r\n ")); 
				if (total < 0) { // skor 0 ın altına indiğinde game over yapar.
					// yeni meyve düşmesini engeller ve sepeti durdurur.
					
					DeleteTimerQueue(hTimerQueue);
					DeleteTimerQueue(hElmaTimer);
					DeleteTimerQueue(hMuzTimer);
					DeleteTimerQueue(hHamburgerTimer);
					SetWindowText(hEdit, "GAME OVER "); 
					PlaySound(TEXT("go.wav"), NULL, SND_SYNC);
			        Sleep(3000);
			        exit(0);
				}
				
			}
			for (x = 1; x < 20; x++) // hamburgeri sil
			{

				m(xcor + x, y) = 0x00;
				m(xcor + x, y + 1) = 0x00;
				m(xcor + x, y + 2) = 0;
				m(xcor + x, y + 3) = 0;
				m(xcor + x, y - 1) = 0;
			}
		
			
		       
		}

		SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);

		Sleep(3); // düşme hızını belirliyor
	}
	return 0;
}


   int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)
{
	MSG      msg;
	WNDCLASSEX wc;
	HMENU MainMenu;
	MainMenu = CreateMenu();
	
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = lpszAppName;
	wc.lpszClassName = lpszAppName;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = (HICON)LoadImage(hInstance, lpszAppName,
		IMAGE_ICON, 16, 16,
		LR_DEFAULTCOLOR);

	if (!RegisterClassEx(&wc))
		return(FALSE);

	hInst = hInstance;
	hWnd = CreateWindowEx(0, lpszAppName,
		lpszTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		790, 380, // pencerenin büyüklük ölçüleri
		NULL,
		MainMenu,
		hInstance,
		NULL
	);


	if (!hWnd)
		return(FALSE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return(msg.wParam);
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_CREATE:
	{
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "OYUNUN KURALI :Sağ ve sol yön tuşlarını kullanarak beyaz renk sepeti hareket ettirebilir yukarı tuşa basarak sepeti durdurabilir düşen sağlıklı meyveleri yiyebilirsin bu sana puan kazandırır. Yeşil elma ve +6 puandır, sarı muz ve +12 puandır, kahverengi hamburger sağlıksız olduğundan -24 puandır. Puanın SIFIRIN ALTINA DÜŞERSE GAME OVER OLURSUN. ", WS_CHILD | WS_VISIBLE |
			ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL, 515, 22, 200, 310, hWnd, NULL, hInst, NULL);
		HButton1 = CreateWindowEx(NULL, "BUTTON", "START", WS_CHILD | WS_VISIBLE | SS_CENTER,
			92, 2, 80, 20, hWnd, (HMENU)IDC_BUTTON1, hInst, NULL);
		HButton2 = CreateWindowEx(NULL, "BUTTON", "STOP", WS_CHILD | WS_VISIBLE | SS_CENTER,
			172, 2, 80, 20, hWnd, (HMENU)IDC_BUTTON2, hInst, NULL);
		HButton3 = CreateWindowEx(NULL, "BUTTON", "EXIT", WS_CHILD | WS_VISIBLE | SS_CENTER,
			252, 2, 80, 20, hWnd, (HMENU)IDC_BUTTON3, hInst, NULL);
		Hmainbmp = CreateWindowEx(NULL, "STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_THICKFRAME, 1, 23, 510, 310, hWnd, NULL, hInst, NULL);
	}
	break;
	case WM_KEYDOWN:
	{
		static int t, parameter = 5;
		static DWORD dw;
		t = (int)wParam;
		if (t > 36 && t < 41) keypressed = t;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		
		case IDC_BUTTON1:
		{

			HANDLE hTimer = NULL;

			hTimerQueue = CreateTimerQueue();
			if (NULL == hTimerQueue)
			{
				break;
			}
			//starts 1000 ms later calls thread every 30 ms
			CreateTimerQueueTimer(&hTimer, hTimerQueue, (WAITORTIMERCALLBACK)SlidingBox, NULL, 0, 30, 0);

			void* parameter = NULL;
			DWORD dw;

			i = 100;
			parameter = &i;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)elma, &parameter, 0, &dw);
			Sleep(100);
			i = 200;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)muz, &parameter, 0, &dw);
			Sleep(100);
			i = 300;
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)hamburger, &parameter, 0, &dw);

			HANDLE hETimer = NULL;
			hElmaTimer = CreateTimerQueue();
			if (NULL == hElmaTimer)
			{
				break;
			}
			CreateTimerQueueTimer(&hETimer, hElmaTimer, (WAITORTIMERCALLBACK)elma, NULL, 0, 5000, 0);

			HANDLE hMTimer = NULL;
			hMuzTimer = CreateTimerQueue();
			if (NULL == hMuzTimer)
			{
				break;
			}
			CreateTimerQueueTimer(&hMTimer, hMuzTimer, (WAITORTIMERCALLBACK)muz, NULL, 0, 7000, 0);

			HANDLE hSTimer = NULL;
			hHamburgerTimer = CreateTimerQueue();
			if (NULL == hHamburgerTimer)
			{
				break;
			}
			CreateTimerQueueTimer(&hSTimer, hHamburgerTimer, (WAITORTIMERCALLBACK)hamburger, NULL, 0, 3000, 0);
		}
		break;
		case IDC_BUTTON2:
		{
			
			DeleteTimerQueue(hTimerQueue);
			DeleteTimerQueue(hElmaTimer);
			DeleteTimerQueue(hMuzTimer);
			DeleteTimerQueue(hHamburgerTimer);
		}
		break;
		case IDC_BUTTON3:
		{
			PostQuitMessage(0);
		}
		break;
		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
	}

	return(0L);
}

