#include "stdafx.h"
#include "eGRIM_GUI.h"
#include "SampleGenerator.h"

//=============================================================================
//! @subsection Global Macros
//!
//! @brief Defines all variable macros which associate a recognizable string 
//! with a constant value.
//=============================================================================

//! Define the largest size for the load string value.
#define MAX_LOADSTRING 100

//=============================================================================
//! @subsection Global Variables
//!
//! @brief Declares all variables which are both publically accessible and 
//! modifiable.
//=============================================================================

//! A handle to the current object instance.
HINSTANCE hInst;

//! A pointer to the sample generation and transmission object.
SampleGenerator* generator = NULL;

//! A character string representing the title bars.
WCHAR szTitle[MAX_LOADSTRING];

//! A character string representing the window class name.
WCHAR szWindowClass[MAX_LOADSTRING];

//=============================================================================
//! @subsection Forward Declarations
//!
//! @brief Forward declarations of functions included within the module.
//=============================================================================

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK eGrimProc(HWND);

//=============================================================================
//! @fn wWinMain
//! 
//! @addtogroup eGRIM_GUI
//=============================================================================

//-----------------------------------------------------------------------------
//! @brief A method to initialize the datastructure, create the GUI, and create
//! the socket and threads.
//! @param hInstance A handle to the main object instance.
//! @param hPrevInstance A handle to the previous object instance.
//! @param lpCmdLine Any command line arguments passed.
//! @param nCmdShow A command to show the window.
//! @return A message parameter executed within the main loop.
//-----------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE 
	hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

	// Declare all relevant variables.
	HACCEL hAccelTable;
	MSG msg;

	// Establish the unreferenced parameters.
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize the global strings within the application.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_EGRIMGUI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) {
		return FALSE;
	}

	// Load the accelerators within the object instance.
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(
		IDC_EGRIMGUI));

	// Execute the main message loop.
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// Return the message parameters.
	return (int) msg.wParam;
}

//-----------------------------------------------------------------------------
//! @brief Registers the main window class.
//! @param hInstance A handle to the main object instance.
//! @return A handle to the registered window class.
//-----------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance) {

	// Declare all relevant variables.
	WNDCLASSEXW wcex;

	// Populate the parameters of the window class object.
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EGRIMGUI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_EGRIMGUI);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	// Return the registered class object.
	return RegisterClassExW(&wcex);
}

//-----------------------------------------------------------------------------
//! @brief Saves the instance handle in a global variable, creates the main 
//! program window, and displays it.
//! @param hInstance A handle to the main object instance.
//! @param nCmdShow A command to show the window.
//! @return True, if successfully created, false otherwise.
//-----------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {

	// Declare all relevant variables.
	HWND hwnd;
	HWND qlen;
	HWND txrt;
	HWND rtst;
	HWND rtrt;
	HWND ipad;
	HWND port;
	HWND bttn;

	// Establish a global handle to the object instance.
	hInst = hInstance;

	// Create the main window object, returning false if nothing exists.
	hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 230, 225, NULL, NULL, hInstance, NULL);
	if (!hwnd) {
		return FALSE;
	}

	// Create all line editing windows, associated labels, and relevant 
	// pushbutton objects.
	CreateWindow(TEXT("static"), TEXT("Queue Length: "), WS_CHILD | WS_VISIBLE
		| ES_LEFT | WS_BORDER, 10, 10, 100, 20, hwnd, NULL, hInstance, NULL);
	qlen = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT("100.0"),
		WS_VISIBLE | WS_CHILD, 110, 10, 95, 20, hwnd, (HMENU) IDC_QUEUE_LEN, 
		hInstance, NULL);
	CreateWindow(TEXT("static"), TEXT("Trans. Rate: "), WS_CHILD | WS_VISIBLE
		| ES_LEFT | WS_BORDER, 10, 30, 100, 20, hwnd, NULL, hInstance, NULL);
	txrt = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT("0.083"),
		WS_VISIBLE | WS_CHILD, 110, 30, 95, 20, hwnd, (HMENU) IDC_TRX_RATE, 
		hInstance, NULL);
	CreateWindow(TEXT("static"), TEXT("Rotation Start: "), WS_CHILD | WS_VISIBLE
		| ES_LEFT | WS_BORDER, 10, 50, 100, 20, hwnd, NULL, hInstance, NULL);
	rtst = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT("0.00"),
		WS_VISIBLE | WS_CHILD, 110, 50, 95, 20, hwnd, (HMENU)IDC_ROT_RATE,
		hInstance, NULL);
	CreateWindow(TEXT("static"), TEXT("Rotation Rate: "), WS_CHILD | WS_VISIBLE
		| ES_LEFT | WS_BORDER, 10, 70, 100, 20, hwnd, NULL, hInstance, NULL);
	rtrt = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT("60.00"),
		WS_VISIBLE | WS_CHILD, 110, 70, 95, 20, hwnd, (HMENU) IDC_ROT_RATE, 
		hInstance, NULL);
	CreateWindow(TEXT("static"), TEXT("IP Address: "), WS_CHILD | WS_VISIBLE
		| ES_LEFT | WS_BORDER, 10, 90, 100, 20, hwnd, NULL, hInstance, NULL);
	ipad = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT("224.0.0.0"),
		WS_VISIBLE | WS_CHILD, 110, 90, 95, 20, hwnd, (HMENU) IDC_IP_ADDR, 
		hInstance, NULL);
	CreateWindow(TEXT("static"), TEXT("Comm. Port: "), WS_CHILD | WS_VISIBLE
		| ES_LEFT | WS_BORDER, 10, 110, 100, 20, hwnd, NULL, hInstance, NULL);
	port = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("edit"), TEXT("1024"),
		WS_VISIBLE | WS_CHILD, 110, 110, 95, 20, hwnd, (HMENU) IDC_PORT, 
		hInstance, NULL);
	bttn = CreateWindow(TEXT("button"), TEXT("Generate"), WS_VISIBLE |
		WS_CHILD, 110, 140, 95, 20, hwnd, (HMENU) IDC_GEN_BUTTON, hInstance, 
		NULL);
	if (!qlen || !txrt || !rtrt || !ipad || !port || !bttn) {
		return false;
	}

	// Present the object window and update its contents.
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Return successful.
	return TRUE;
}

//-----------------------------------------------------------------------------
//! @brief Process all messages for the primary application window.
//! @param hwnd A handle to the main window object.
//! @param message A message to be processed.
//! @param wParam A word parameter value.
//! @param lParam A pointer parameter value.
//! @return A zero value, if successful, negative otherwise.
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM 
        lParam) {

	// Different message types require difference processing techniques. Each 
	// type is handled as part of a switch statement.
	switch (message) {

	// Process a requested command, either to open an about window, handle a 
	// button press, or exit from the application.
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
	
		// Parse the menu selections:
		switch (wmId) {
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
			break;
		case IDC_GEN_BUTTON:
			return eGrimProc(hwnd);
		case IDM_EXIT:
			DestroyWindow(hwnd);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		break;
	}

	// Process a paint message to update a portion of the application window.
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	}

	// Process a message to close the window and its children.
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}

	// By default, provide default processing for any window messages that the 
	// application does not process.
	default: {
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	}

	// Return successful.
	return 0;
}

//-----------------------------------------------------------------------------
//! @brief Manages all messages for the 'About' box.
//! @param hDlg A handle to the 'About' dialog window.
//! @param message A message to be processed.
//! @param wParam A word parameter value.
//! @param lParam A pointer parameter value.
//! @return A true value, if successully processed, false otherwise.
//-----------------------------------------------------------------------------
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {

	// Establish the pointer parameter as unreferenced.
	UNREFERENCED_PARAMETER(lParam);

	// Parse either an initialization command, or closure command.
	switch (message) {
	case WM_INITDIALOG: {
		return (INT_PTR)TRUE;
	}
	case WM_COMMAND: {
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	}

	// The command was unsuccessfully parsed, return false.
	return (INT_PTR)FALSE;
}

//-----------------------------------------------------------------------------
//! @brief Processes all pushbutton functionality, managing requested socket 
//! communication, and packet generation.
//! @param hwnd A handle to the main window object.
//! @return A zero value, if successful, negative otherwise.
//-----------------------------------------------------------------------------
LRESULT CALLBACK eGrimProc(HWND hwnd) {

	// Declare all relevant variables.
	TCHAR* message;
	uint32_t queue_len;
	double trx_rate;
	double rot_start;
	double rot_rate;
	TCHAR* ip_tchar;
	char* ip_addr;
	uint32_t comm_port;

	// Initialize various character arrays.
	message = new TCHAR[32];
	ip_tchar = new TCHAR[32];
	ip_addr = new char[32];

	// Retrieve the pushbutton text, establishing whether the button prompt 
	// requests termination or packet generation.
	GetDlgItemText(hwnd, IDC_GEN_BUTTON, message, 32);
	if (!_tcscmp(message, TEXT("Generate"))) {

		// Retrieve the queue length from the figure, checking that it is 
		// within range.
		GetDlgItemText(hwnd, IDC_QUEUE_LEN, message, 32);
		queue_len = _ttoi(message);
		if (queue_len < 1 || queue_len > 100) {
			MessageBox(NULL, TEXT("Acceptable Range: [1, 100]"), TEXT("Queue "
				"Length: Out of Bounds!"), MB_OK | MB_ICONEXCLAMATION);
			return -1;
		}

		// Retrieve the transmission rate from the figure, checking that it is 
		// within range.
		GetDlgItemText(hwnd, IDC_TRX_RATE, message, 32);
		trx_rate = _ttof(message);
		if (trx_rate < 0 || trx_rate > 10) {
			MessageBox(NULL, TEXT("Acceptable Range: [0, 10]"), TEXT("Transmission "
				"Rate: Out of Bounds!"), MB_OK | MB_ICONEXCLAMATION);
			return -1;
		}

		// Retrieve the rotation rate from the figure, checking that it is 
		// within range.
		GetDlgItemText(hwnd, IDC_ROT_START, message, 32);
		rot_start = _ttof(message);
		if (rot_start < 0 || rot_start > 360) {
			MessageBox(NULL, TEXT("Acceptable Range: [0, 360]"), TEXT("Rotation "
				"Rate: Out of Bounds!"), MB_OK | MB_ICONEXCLAMATION);
			return -1;
		}

		// Retrieve the rotation rate from the figure, checking that it is 
		// within range.
		GetDlgItemText(hwnd, IDC_ROT_RATE, message, 32);
		rot_rate = _ttof(message);
		if (rot_rate < 0 || rot_rate > 60) {
			MessageBox(NULL, TEXT("Acceptable Range: [0, 60]"), TEXT("Rotation "
				"Rate: Out of Bounds!"), MB_OK | MB_ICONEXCLAMATION);
			return -1;
        	}

		// Retrieve the IP address from the figure, converting it to a standard 
		// character array.
		GetDlgItemText(hwnd, IDC_IP_ADDR, ip_tchar, 32);
		if (sizeof(TCHAR) != sizeof(char)) {
			wcstombs_s(NULL, ip_addr, 32, (wchar_t*)ip_tchar, 32);
		}
		else {
			strcpy_s(ip_addr, 32, (char*)ip_tchar);
		}

		// Retrieve the communication port from the figure, checking that it is 
		// within range.
		GetDlgItemText(hwnd, IDC_PORT, message, 32);
		comm_port = _ttoi(message);
		if (comm_port < 1024 || comm_port > 49151) {
			MessageBox(NULL, TEXT("Acceptable Range: [1024, 49151]"),
				TEXT("Communication Port: Out of Bounds!"), MB_OK |
				MB_ICONEXCLAMATION);
			return -1;
		}

		// Create a new instance of the packet generator with the given IP 
		// address and communication port.
		generator = new SampleGenerator(ip_addr, comm_port);

		// Initialize the packet generation and transfer threads.
		generator->init(queue_len, trx_rate, rot_start, rot_rate);

		// Change the text on the pushbutton to signify process termination.
		SetDlgItemText(hwnd, IDC_GEN_BUTTON, TEXT("Terminate"));
	}
	else {

		// Terminate the packet generation and transfer threads.
		generator->uninit();

		// Free the existing generator object.
		free(generator);

		// Change the text on the pushbutton to signify starting the process.
		SetDlgItemText(hwnd, IDC_GEN_BUTTON, TEXT("Generate"));
	}

	// Free the arrays of values
	free(message);
	free(ip_tchar);
	free(ip_addr);

	// Return successful.
	return 0;
}
