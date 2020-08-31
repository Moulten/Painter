// Figures.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Figures.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
RECT ClientRect;
HBITMAP background;

FullScreen full_screen;                         //Основной экран
                       //Экран отображения
HWND CIRCLE_PARAMS, RECT_PARAMS;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FIGURES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FIGURES));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FIGURES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FIGURES);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   background = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   GetClientRect(hWnd, &ClientRect);

     //Инициализируем основной экран
   full_screen = FullScreen(ClientRect, CreatePatternBrush(background));
 
  //Создаем кнопки и ввод для отображения круга и квадрата
   HWND DISCRIPTION = CreateWindow(
       TEXT("STATIC"),
       TEXT(R"(
Ввод всех значений через пробел

Треугольник: позиция Х позиция Y Радиус

Квадрат: позиция X позиция Y Ширина Высота

Delete удаляет последнюю фигуру
 )" ),
       WS_CHILD | WS_VISIBLE | WS_BORDER,
       20, 30, 240, 200, hWnd, 0, hInst, NULL
   );
   full_screen.AddWindow("Discription", DISCRIPTION);

   HWND CREATCIRCLE = CreateWindow(
       TEXT("BUTTON"), TEXT("Create Circle"), WS_CHILD | WS_VISIBLE,
       150, ClientRect.bottom/2 - 70, 100, 30, hWnd, (HMENU)1001, hInst, NULL
   );
   full_screen.AddWindow("Button_Create_Circle", CREATCIRCLE);

   HWND CIRCLE_PARAMS = CreateWindow(TEXT("edit"), TEXT("0"),
       WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 20, ClientRect.bottom / 2 - 70, 140, 30,
       hWnd, 0, hInst, NULL);
   full_screen.AddWindow("Input_Circle_params", CIRCLE_PARAMS);

   HWND CREATRECT = CreateWindow(
       TEXT("BUTTON"), TEXT("Create Rect"), WS_CHILD | WS_VISIBLE,
       150, ClientRect.bottom / 2 + 70, 100, 30, hWnd, (HMENU)1002, hInst, NULL
   );

   full_screen.AddWindow("Button_Create_Rect", CREATRECT);

   HWND RECT_PARAMS = CreateWindow(TEXT("edit"), TEXT("0"),
       WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 20, ClientRect.bottom / 2 + 70, 140, 30,
       hWnd, 0, hInst, NULL);

   full_screen.AddWindow("Input_Rect_params", RECT_PARAMS);

   HWND DELETE_BUTTON = CreateWindow(
       TEXT("BUTTON"), TEXT("Delete"), WS_CHILD | WS_VISIBLE,
       150, ClientRect.bottom / 2 + 140, 100, 30, hWnd, (HMENU)1003, hInst, NULL
   );

   full_screen.AddWindow("Button_Undo", DELETE_BUTTON);

   //Инициализируем экран отображения
   FigureParams params = { 850,330, {1100,600},WHITE_BRUSH};
                  
   full_screen.AddScreen(
       "PaintScreen", 
       std::make_unique<PaintScreen>(params)       //Экран отображения
   );
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int Len;
    TCHAR input_text[100];
    std::wstring ws;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case 1001:    //Добавить круг
            {
                Len = GetWindowText(full_screen.GetWindow("Input_Circle_params"), input_text, 100);
                ws = std::wstring(input_text, Len);
                full_screen.GetScreen("PaintScreen").AddPicture(std::string(ws.begin(), ws.end()), FigureType::CIRCLE);
                RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
                break;
                
            }
            case 1002:     //Добавить квадрат
            {
                Len = GetWindowText(full_screen.GetWindow("Input_Rect_params"), input_text, 200);
                ws = std::wstring(input_text, Len);
                full_screen.GetScreen("PaintScreen").AddPicture(std::string(ws.begin(), ws.end()), FigureType::RECT);
                RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
                break;
            }
            case 1003:
            {
                full_screen.GetScreen("PaintScreen").DeleteSub();
                RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
                break;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps); // Add RAII?
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
			//FullScreen new_screen(ClientRect, CreatePatternBrush(background));
            //new_screen.Paint(hdc);
            full_screen.Paint(hdc);
			// Вывод на экран 
			//SetStretchBltMode(hdc, COLORONCOLOR);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
