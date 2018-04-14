
#include <Windows.h>
#include <time.h>
#include <thread>

#include "naturesim_main.h"

#define WIN_TITLE "Nature Simulator"

struct win32_screen_buffer {
    BITMAPINFO Info;
    void *Memory;
    int BytesPerPixel;
    int Width;
    int Height;
};

struct win32_window_dimension {
    int Width;
    int Height;
};

// --------------------------------------------------------------------------------------

LRESULT CALLBACK
WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

win32_window_dimension
Win32GetWindowDimension(HWND hWnd);

void
Win32ResizeDIBSection(win32_screen_buffer *Buffer, int Width, int Height);

void
Win32DisplayBufferInWindow(win32_screen_buffer *Buffer,
    HDC DeviceContext, win32_window_dimension *WindowSize);

// --------------------------------------------------------------------------------------

input GameKeyboardMouse;
screen_buffer GameScreenBuffer;

int running;
win32_screen_buffer ScreenBuffer;

// --------------------------------------------------------------------------------------

int CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdLine, int nCmdShow)
{
    {
        UNREFERENCED_PARAMETER(hPrevInstance);
        UNREFERENCED_PARAMETER(CmdLine);
        UNREFERENCED_PARAMETER(nCmdShow);
    }

    srand((unsigned)time(0));

    HWND hWnd = NULL;
    MSG Msg = {};

    CHAR szWindowName[] = WIN_TITLE;
    CHAR szWindowClass[] = "Window Class";

    WNDCLASSEX MainWindowClass = {};

    MainWindowClass.cbSize = sizeof(WNDCLASSEX);
    MainWindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    MainWindowClass.lpfnWndProc = WndProc;
    MainWindowClass.hInstance = hInstance;
    MainWindowClass.lpszClassName = szWindowClass;
    MainWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassEx(&MainWindowClass);

    // Get width and geight in pixels of the monitor
    HMONITOR hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = { sizeof(mi) };
    if (GetMonitorInfo(hmon, &mi) == 0) return EXIT_FAILURE;

    hWnd = CreateWindowEx(0, szWindowClass, szWindowName, WS_POPUP | WS_VISIBLE,
                                mi.rcMonitor.left, mi.rcMonitor.top, 
            mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
                                    NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
        return EXIT_FAILURE;
    }

    running = 1;

    // Render and display image
    int updated = 0;
    std::thread game (render, &running, &updated, &GameScreenBuffer, &GameKeyboardMouse);

    while (running) {
        // Redraw and process any other messages
        if (updated) {
            InvalidateRect(hWnd, NULL, TRUE);
            updated = 0;
        }

        // Clear up message queue...
        while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
            if (Msg.message == WM_QUIT) {
                break;
            }
        
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }

        // We will be doing 60 fps, we need to keep the window working nicely...
#define FPS 60
        Sleep(1000 / FPS);
    }

    game.join();

    return EXIT_SUCCESS;
}

// --------------------------------------------------------------------------------------

win32_window_dimension
Win32GetWindowDimension(HWND hWnd)
{
    win32_window_dimension Dimensions;

    RECT ClientRect;
    GetClientRect(hWnd, &ClientRect);
    Dimensions.Width = ClientRect.right - ClientRect.left;
    Dimensions.Height = ClientRect.bottom - ClientRect.top;

    return Dimensions;
}

void
Win32ResizeDIBSection(win32_screen_buffer *Buffer, int Width, int Height)
{
    delete Buffer->Memory;

    Buffer->Width = Width;
    Buffer->Height = Height;

    int BytesPerPixel = 4;
    Buffer->BytesPerPixel = BytesPerPixel;

    // NOTE: biHeight is negative for top-down approach, first byte is top-left pixel
    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;

    int BitmapMemorySize = (Buffer->Width * Buffer->Height) * BytesPerPixel;
    Buffer->Memory = new char[BitmapMemorySize]();
}

void
Win32DisplayBufferInWindow(win32_screen_buffer *Buffer, HDC DeviceContext,
    win32_window_dimension *WindowSize)
{
    StretchDIBits(DeviceContext,
        0, 0, WindowSize->Width, WindowSize->Height,
        0, 0, Buffer->Width, Buffer->Height,
        Buffer->Memory,
        &Buffer->Info,
        DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT Result = 0;

    switch (uMsg) {
        case WM_SIZE: 
        {
            win32_window_dimension Dimensions = Win32GetWindowDimension(hWnd);
            Win32ResizeDIBSection(&ScreenBuffer, Dimensions.Width, Dimensions.Height);

            GameScreenBuffer.memory = ScreenBuffer.Memory;
            GameScreenBuffer.width = ScreenBuffer.Width;
            GameScreenBuffer.height = ScreenBuffer.Height;
            GameScreenBuffer.bpp = ScreenBuffer.BytesPerPixel;
        } break;

        case WM_PAINT:
        {
            PAINTSTRUCT Paint = {};
            HDC hDC = BeginPaint(hWnd, &Paint);

            win32_window_dimension Dimensions = Win32GetWindowDimension(hWnd);
            Win32DisplayBufferInWindow(&ScreenBuffer, hDC, &Dimensions);

            EndPaint(hWnd, &Paint);
        } break;

        case WM_MOUSEHOVER: 
        {
            // TODO: See dragging and things :)...

            // X, Y from the upper-left of the clien area
            GameKeyboardMouse.Mouse.XPos = lParam & ~((WORD)0);
            GameKeyboardMouse.Mouse.YPos = lParam & ((~((WORD)0)) << (sizeof(WORD) * 8));
        } break;

        case WM_KEYDOWN:
        {
            WPARAM VKCode = wParam;

            switch (VKCode) 
            {
                case 'W':
                {
                    GameKeyboardMouse.Keyboard.W++;
                } break;
                
                case 'P':
                {
                    GameKeyboardMouse.Keyboard.P++;
                } break;

                case 'S':
                {
                    GameKeyboardMouse.Keyboard.S++;
                } break;

                case 'H':
                {
                    GameKeyboardMouse.Keyboard.H++;
                } break;
                
                case 'Z':
                {
                    GameKeyboardMouse.Keyboard.Z++;
                } break;

                case 'C':
                {
                    GameKeyboardMouse.Keyboard.C++;
                } break;


                case VK_ADD: 
                {
                    GameKeyboardMouse.Keyboard.PlusSign++;
                } break;
                
                case VK_SUBTRACT:
                {
                    GameKeyboardMouse.Keyboard.MinusSign++;
                } break;


                case VK_ESCAPE: 
                {
                    running = false;
                } break;
            }
        } break;

        case WM_DESTROY:
        {
            PostQuitMessage(EXIT_SUCCESS);
            running = 0;
        } break;

        default:
        {
            Result = DefWindowProc(hWnd, uMsg, wParam, lParam);
        } break;
    }

    return Result;
}