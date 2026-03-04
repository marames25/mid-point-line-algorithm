#include <windows.h>
#include <vector>
#include <cmath>

using namespace std;

vector<POINT> points;
bool triangleReady = false;

// ================= Midpoint Line Algorithm (All Slopes) =================
void DrawMidPointLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    //If moving right --> +1 If moving left --> -1
    int sx = (dx >= 0) ? 1 : -1;
    int sy = (dy >= 0) ? 1 : -1;

    dx = abs(dx);
    dy = abs(dy);

    int x = x1;
    int y = y1;

    SetPixel(hdc, x, y, color);

    // |slope| <= 1
    if (dx >= dy) //horizontal movement
    {
        int d = 2 * dy - dx; // initial
        int dE = 2 * dy; // right
        int dNE = 2 * (dy - dx); // up and right

        for (int i = 0; i < dx; i++)// only in x direction
        {
            if (d <= 0)
            {
                d += dE;
                x += sx;
            }
            else // diagonally
            {
                d += dNE;
                x += sx;
                y += sy;
            }

            SetPixel(hdc, x, y, color);
        }
    }
        // |slope| > 1
    else // vertical movement
    {
        int d = 2 * dx - dy;
        int dN = 2 * dx;
        int dNE = 2 * (dx - dy);

        for (int i = 0; i < dy; i++)// only in y direction
        {
            if (d <= 0)
            {
                d += dN;
                y += sy;
            }
            else
            {
                d += dNE;
                x += sx;
                y += sy;
            }

            SetPixel(hdc, x, y, color);
        }
    }
}

// ================= Window Procedure =================
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_LBUTTONDOWN:
        {

            POINT p;
            p.x = LOWORD(lParam);
            p.y = HIWORD(lParam);
            points.push_back(p);

            if (points.size() == 3)
                triangleReady = true;

            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (triangleReady)
            {
                COLORREF colors[3] = {
                        RGB(160, 32, 240),   // Bright Purple
                        RGB(255, 182, 193),  // Light Pink
                        RGB(255, 215, 0)     // Golden Yellow
                };

                for (int i = 0; i < 3; i++)
                {
                    POINT p1 = points[i];
                    POINT p2 = points[(i + 1) % 3];

                    DrawMidPointLine(hdc, p1.x, p1.y, p2.x, p2.y, colors[i]);
                }
            }

            EndPaint(hwnd, &ps);
        }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// ================= WinMain =================
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE,
                   LPSTR,
                   int nCmdShow)
{
    const char CLASS_NAME[] = "TriangleWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
            0,
            CLASS_NAME,
            "Triangle Shape Drawing - Midpoint",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
            NULL,
            NULL,
            hInstance,
            NULL
    );

    if (hwnd == NULL)
        return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
