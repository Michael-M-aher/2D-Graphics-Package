#include <tchar.h>
#include <string>
#include <windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <list>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <math.h>

#include "helper.cpp"
#include "line.cpp"
#include "circle.cpp"
#include "ellipse.cpp"
#include "circle-filling.cpp"
#include "flood-filling.cpp"
#include "polygon-clipping.cpp"
#include "polygon-filling.cpp"
#include "circleIntersection-filling.cpp"
#include "cardinal-splines.cpp"
#include "curve-filling.cpp"
#include "save.cpp"
#include "load.cpp"

static COLORREF color = RGB(0, 0, 0);
static int RC =0 , GC = 0 , BC=0;


void ClearScreen(HDC hdc, int width, int height)
{
    std::ofstream myFile2("temp.txt", std::ofstream::out | std::ofstream::trunc);
    myFile2.close();
    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
}

int windowWidth, windowHeight;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void addMenus(HWND);

HMENU hmenu;
HMENU hmenu2;

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("Computer Graphics");

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
{
    HWND hwnd;        /* This is the handle for our window */
    MSG messages;     /* Here messages to the application are saved */
    WNDCLASSEX wincl; /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure; /* This function is called by windows */
    wincl.style = CS_DBLCLKS;            /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_CROSS);
    wincl.lpszMenuName = NULL; /* No menu */
    wincl.cbClsExtra = 0;      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
        0,                                       /* Extended possibilites for variation */
        szClassName,                             /* Classname */
        _T("Computer Graphics"), /* Title Text */
        WS_OVERLAPPEDWINDOW,                     /* default window */
        CW_USEDEFAULT,                           /* Windows decides the position */
        CW_USEDEFAULT,                           /* where the window ends up on the screen */
        544,                                     /* The programs width */
        375,                                     /* and height in pixels */
        HWND_DESKTOP,                            /* The window is a child-window to desktop */
        NULL,                                    /* No menu */
        hThisInstance,                           /* Program Instance handler */
        NULL                                     /* No Window Creation data */
    );

    CreateWindow("BUTTON", "Clear", WS_VISIBLE | WS_CHILD, windowWidth - 100, 10, 80, 25, hwnd, (HMENU)0, hThisInstance, NULL);
    // UpdateWindow(hwnd);

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);
    std::ofstream myFile2("temp.txt", std::ofstream::out | std::ofstream::trunc);
    myFile2.close();

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);

    static int circleShift = 5;
    static bool polygonCompleted = false;

    static VertexList v;
    // polygon
    static mPolygon m = mPolygon();
    static int selected = -1;
    switch (message) /* handle the messages */
    {
    case WM_CREATE:
        addMenus(hwnd);
        RECT rect;
        GetClientRect(hwnd, &rect);
        windowWidth = (rect.right - rect.left);
        windowHeight = rect.bottom - rect.top;
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case 0:
            ClearScreen(hdc, windowWidth, windowHeight);
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            break;
        case 1:
            color = RGB(0, 0, 0);
            RC = 0 ;
            GC = 0 ;
            BC = 0;
            break;
        case 2:
            color = RGB(255, 255, 255);
            RC = 255 ;
            GC = 255 ;
            BC = 255;
            break;
        case 3:
            color = RGB(255, 0, 0);
            RC = 255 ;
            GC = 0 ;
            BC = 0;
            break;
        case 4:
            color = RGB(0, 255, 0);
            RC = 0 ;
            GC = 255 ;
            BC = 0;
            break;
        case 5:
            color = RGB(0, 0, 255);
            RC = 0 ;
            GC = 0 ;
            BC = 255;
            break;
        case 40:
            saveTemp();
            break;
        case 41:
            readLoadFromFile(hdc);
            break;
        case 42:
            clearSave();
            break;
        default:
        {
            if (wParam >= 6 && wParam <= 34)
            {
                v.clear();
                selected = wParam;
            }
            break;
        }
        }
        break;
    case WM_LBUTTONDBLCLK:
    {
        switch (selected)
        {
        case 6:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                Line_DDA(hdc, Point(v[0].x, v[0].y), Point(v[1].x, v[1].y), color);
                save("dl" ,v[0].x , v[0].y ,v[1].x , v[1].y ,RC , GC , BC );
                v.clear();
            }
            break;
        }
        case 7:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                Line_Midpoint(hdc, Point(v[0].x, v[0].y), Point(v[1].x, v[1].y), color);
                save("dl" ,v[0].x , v[0].y ,v[1].x , v[1].y ,RC , GC , BC );
                v.clear();
            }
            break;
        }
        case 8:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                Line_Parametric(hdc, Point(v[0].x, v[0].y), Point(v[1].x, v[1].y), color);
                save("dl" ,v[0].x , v[0].y ,v[1].x , v[1].y ,RC , GC , BC );
                v.clear();
            }
            break;
        }
        case 9:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int r = distance(v[0], v[1]);
                Circle_Cartesian(hdc, Point(v[0].x, v[0].y), r, color);
                save("dc" ,v[0].x , v[0].y , r , RC , GC ,BC );
                v.clear();
            }
            break;
        }
        case 10:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int r = distance(v[0], v[1]);
                Circle_Polar(hdc, Point(v[0].x, v[0].y), r, color);
                save("dc" ,v[0].x , v[0].y , r , RC , GC ,BC );
                v.clear();
            }
            break;
        }
        case 11:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int r = distance(v[0], v[1]);
                Circle_Polar_Iterative(hdc, Point(v[0].x, v[0].y), r, color);
                save("dc" ,v[0].x , v[0].y , r , RC , GC ,BC );
                v.clear();
            }
            break;
        }
        case 12:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int r = distance(v[0], v[1]);
                Circle_Midpoint(hdc, Point(v[0].x, v[0].y), r, color);
                save("dc" ,v[0].x , v[0].y , r , RC , GC ,BC );
                v.clear();
            }
            break;
        }
        case 13:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int r = distance(v[0], v[1]);
                Circle_Midpoint_Improved(hdc, Point(v[0].x, v[0].y), r, color);
                save("dc" ,v[0].x , v[0].y , r , RC , GC ,BC );
                v.clear();
            }
            break;
        }
        case 14:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            break;
        }
        case 15:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 3)
            {
                int a = distance(v[0], v[1]);
                int b = distance(v[0], v[2]);
                Ellipse_Cartesian(hdc, Point(v[0].x, v[0].y), a, b, color);
                save("el" , v[0].x , v[0].y , a , b , RC , GC , BC);
                v.clear();
            }
            break;
        }
        case 16:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 3)
            {
                int a = distance(v[0], v[1]);
                int b = distance(v[0], v[2]);
                Ellipse_Polar(hdc, Point(v[0].x, v[0].y), a, b, color);
                save("el" , v[0].x , v[0].y , a , b , RC , GC , BC);
                v.clear();
            }
            break;
        }
        case 17:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 3)
            {
                int a = distance(v[0], v[1]);
                int b = distance(v[0], v[2]);
                Ellipse_Midpoint(hdc, Point(v[0].x, v[0].y), a, b, color);
                save("el" , v[0].x , v[0].y , a , b , RC , GC , BC);
                v.clear();
            }
            break;
        }

        case 18:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int r = distance(v[0], v[1]);
                FillFirstQuarterLines(hdc, Point(v[0].x, v[0].y), r, color);
                save("l1" , v[0].x , v[0].y , r , RC , GC , BC);
                v.clear();
            }
            break;
        }
        case 19:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int r = distance(v[0], v[1]);
                FillSecondQuarterLines(hdc, Point(v[0].x, v[0].y), r, color);
                save("l2" , v[0].x , v[0].y , r , RC , GC , BC);
                v.clear();
            }
            break;
        }
        case 20:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int r = distance(v[0], v[1]);
                FillThirdQuarterLines(hdc, Point(v[0].x, v[0].y), r, color);
                save("l3" , v[0].x , v[0].y , r , RC , GC , BC);
                v.clear();
            }
            break;
        }
        case 21:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int r = distance(v[0], v[1]);
                FillFourthQuarterLines(hdc, Point(v[0].x, v[0].y), r, color);
                save("l4" , v[0].x , v[0].y , r , RC , GC , BC);
                v.clear();
            }
            break;
        }
        case 22:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int r = distance(v[0], v[1]);
                for (int i = 0; i < r; i += circleShift)
                {
                    FillFirstQuarterCircles(hdc, Point(v[0].x, v[0].y), i, color);
                }
                save("c1" , v[0].x , v[0].y , r , RC , GC , BC);
                v.clear();
            }
            break;
        }
        case 23:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int r = distance(v[0], v[1]);
                for (int i = 0; i < r; i += circleShift)
                {
                    FillSecondQuarterCircles(hdc, Point(v[0].x, v[0].y), i, color);
                }
                save("c2" , v[0].x , v[0].y , r , RC , GC , BC);
                v.clear();
            }
            break;
        }
        case 24:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int r = distance(v[0], v[1]);
                for (int i = 0; i < r; i += circleShift)
                {
                    FillThirdQuarterCircles(hdc, Point(v[0].x, v[0].y), i, color);
                }
                save("c3" , v[0].x , v[0].y , r , RC , GC , BC);
                v.clear();
            }
            break;
        }
        case 25:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int r = distance(v[0], v[1]);
                for (int i = 0; i < r; i += circleShift)
                {
                    FillFourthQuarterCircles(hdc, Point(v[0].x, v[0].y), i, color);
                }
                save("c4" , v[0].x , v[0].y , r , RC , GC , BC);
                v.clear();
            }
            break;
        }
        case 26:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 2)
            {
                int l = distance(v[0], v[1]);
                DrawSquare(hdc, v[0], l, color);
                fillSquareHermiteCurves(hdc, v[0], l, color);
                save("sh" , v[0].x , v[0].y , l , RC , GC , BC);
                v.clear();
            }

            break;
        }
        case 27:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            if (v.size() == 3)
            {
                int l = distance(v[0], v[1]);
                int w = distance(v[0], v[2]);
                drawRectangle(hdc, v[0], w, l, color);
                fillRectangleBezierCurve(hdc, v[0], w, l, color);
                save("rb" , v[0].x , v[0].y , w,  l , RC , GC , BC);
                v.clear();
            }
            break;
        }
        case 28:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            m.add_Vertex(p);
            break;
        }
        case 29:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            m.add_Vertex(p);
            break;
        }
        case 30:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            FloodFill_Recursive(hdc, Point(p.x, p.y), color, color);
            save("fl" , p.x , p.y , RC , GC , BC);
            v.clear();
            break;
        }
        case 31:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            FloodFill_NonRecursive(hdc, Point(p.x, p.y), color, color);
            save("fl" , p.x , p.y , RC , GC , BC);
            v.clear();
            break;
        }
        case 32:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            if (m.getNumVerices() < 1)
            {
                m.add_Vertex(p);
            }
            else if (m.getNumVerices() == 1)
            {
                m.add_Vertex(p);
                Line_DDA(hdc, Point(m.getVertexList()[0].x, m.getVertexList()[0].y), Point(m.getVertexList()[1].x, m.getVertexList()[1].y), color);
                //save("dl" ,m.getVertexList()[0].x , m.getVertexList()[0].y, m.getVertexList()[1].x, m.getVertexList()[1].y,RC , GC , BC );
            }
            else
            {
                v.push_back(p);
                if (v.size() == 4)
                {
                    Line_DDA(hdc, Point(m.getVertexList()[0].x, m.getVertexList()[0].y), Point(m.getVertexList()[1].x, m.getVertexList()[1].y), RGB(255, 255, 255));
                    clipLine(m.getVertexList()[0].x, m.getVertexList()[0].y, m.getVertexList()[1].x, m.getVertexList()[1].y, v[3].x, v[1].x, v[2].y, v[0].y);
                    // clipping window
                    mPolygon clipWindow = mPolygon();
                    clipWindow.add_Vertex(Point(v[3].x, v[2].y));
                    clipWindow.add_Vertex(Point(v[1].x, v[2].y));
                    clipWindow.add_Vertex(Point(v[1].x, v[0].y));
                    clipWindow.add_Vertex(Point(v[3].x, v[0].y));
                    clipWindow.draw(hdc, RGB(0, 0, 0));
                    Line_DDA(hdc, Point(m.getVertexList()[0].x, m.getVertexList()[0].y), Point(m.getVertexList()[1].x, m.getVertexList()[1].y), color);
                    save("dl" ,m.getVertexList()[0].x , m.getVertexList()[0].y, m.getVertexList()[1].x, m.getVertexList()[1].y, RC , GC , BC );
                    v.clear();
                    m.clear();
                }
            }

            break;
        }
        case 33:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            if (polygonCompleted)
            {
                v.push_back(p);
                if (v.size() == 4)
                {
                    mPolygon pol = clipPolygon(m, v[3].x, v[1].x, v[2].y, v[0].y);

                    // clear the polygon
                    m.draw(hdc, RGB(255, 255, 255));
                    // clipping window
                    mPolygon clipWindow = mPolygon();
                    clipWindow.add_Vertex(Point(v[3].x, v[2].y));
                    clipWindow.add_Vertex(Point(v[1].x, v[2].y));
                    clipWindow.add_Vertex(Point(v[1].x, v[0].y));
                    clipWindow.add_Vertex(Point(v[3].x, v[0].y));
                    clipWindow.draw(hdc, RGB(0, 0, 0));
                    // draw the clipped polygon
                    //pol.draw(hdc, color);
                    VertexList vertices = pol.getVertexList();
                    Point v1 = vertices[vertices.size() - 1];
                    for (Point v2 : vertices)
                    {
                    Line_DDA(hdc, Point(v1.x, v1.y), Point(v2.x, v2.y), color);
                    save("dl" ,v1.x , v1.y, v2.x , v2.y , RC ,  GC , BC );
                    v1 = v2;
                    }

                    m.clear();
                    v.clear();
                    polygonCompleted = false;
                }
            }
            else
            {
                m.add_Vertex(p);
            }
            break;
        }
        case 34:
            {
              Point p = {LOWORD(lParam), HIWORD(lParam)};
              v.push_back(p);

              if(v.size() == 4)
              {
                  int r1 = distance(v[0] , v[1]);
                  int r2 = distance(v[2] , v[3]);
                  Circle_Midpoint_Improved(hdc , Point(v[0].x , v[0].y) , r1 , RGB(0 , 0 , 0));
                  save("dc" ,v[0].x , v[0].y , r1 , RC , GC ,BC );
                  Circle_Midpoint_Improved(hdc , Point(v[2].x , v[2].y) , r2 , RGB(0 , 0 , 0));
                  save("dc" ,v[2].x , v[2].y , r2 , RC , GC ,BC );
                  Point p = getPoint(r1 , r2 , v[0].x , v[0].y , v[2].x , v[2].y);
                  FloodFill_NonRecursive(hdc , Point(p.x , p.y) , RGB(0 , 0 , 0) , color);
                  save("fl" , p.x , p.y , RC , GC , BC);
                  v.clear();
              }
              break;
            }

        default:
            break;
        }
        break;
    }

    case WM_RBUTTONDBLCLK:
    {
        switch (selected)
        {
        case 14:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            v.push_back(p);
            Cardinal_Spline(hdc, v, 0.5, 800, color);
            save("cs", v.size() ,v , 0.5 , 800 , RC , GC ,BC );
            v.clear();
            break;
        }
        case 28:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            m.add_Vertex(p);
            ConvexFill(hdc, m, color);
            save("cf" , v.size() , m.getVertexList() , RC , GC , BC);
            m.clear();
            break;
        }
        case 29:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            m.add_Vertex(p);
            NonConvexFill(hdc, m, color);
            save("cf" , v.size() , m.getVertexList() , RC , GC , BC);
            m.clear();
            break;
        }
        case 33:
        {
            Point p = {LOWORD(lParam), HIWORD(lParam)};
            m.add_Vertex(p);
            polygonCompleted = true;
            m.draw(hdc, color);
            break;
        }
        default:
            break;
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0); /* send a WM_QUIT to the message queue */
        break;
    default: /* for messages that we don't deal with */
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
void addMenus(HWND hwnd)
{
    hmenu = CreateMenu();
    HMENU colorMenu = CreateMenu();
    HMENU shapes = CreateMenu();
    HMENU filling = CreateMenu();
    HMENU clipping = CreateMenu();

    HMENU line = CreateMenu();
    HMENU circle = CreateMenu();
    HMENU elipse = CreateMenu();
    HMENU circlesLines = CreateMenu();
    HMENU circlesCircles = CreateMenu();

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)colorMenu, "Color");
    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)shapes, "Shapes");
    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)filling, "Filling");
    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)clipping, "Clipping");
    AppendMenu(hmenu, MF_STRING, 40, "Save");
    AppendMenu(hmenu, MF_STRING, 41, "Load");
    AppendMenu(hmenu, MF_STRING, 42, "Clear Save");

    AppendMenu(colorMenu, MF_STRING, 1, "black");
    AppendMenu(colorMenu, MF_STRING, 2, "white");
    AppendMenu(colorMenu, MF_STRING, 3, "red");
    AppendMenu(colorMenu, MF_STRING, 4, "green");
    AppendMenu(colorMenu, MF_STRING, 5, "blue");

    AppendMenu(shapes, MF_POPUP, (UINT_PTR)line, "Line");
    AppendMenu(line, MF_STRING, 6, "Line DDA");
    AppendMenu(line, MF_STRING, 7, "Line Midpoint");
    AppendMenu(line, MF_STRING, 8, "Line Parametric");

    AppendMenu(shapes, MF_POPUP, (UINT_PTR)circle, "Circle");
    AppendMenu(circle, MF_STRING, 9, "Circle Direct");
    AppendMenu(circle, MF_STRING, 10, "Circle Polar");
    AppendMenu(circle, MF_STRING, 11, "Circle Iterative Polar");
    AppendMenu(circle, MF_STRING, 12, "Circle Midpoint");
    AppendMenu(circle, MF_STRING, 13, "Circle Modified Midpoint");

    AppendMenu(shapes, MF_STRING, 14, "Cardinal Spline");

    AppendMenu(shapes, MF_POPUP, (UINT_PTR)elipse, "Elipse");
    AppendMenu(elipse, MF_STRING, 15, "Elipse Direct");
    AppendMenu(elipse, MF_STRING, 16, "Elipse Polar");
    AppendMenu(elipse, MF_STRING, 17, "Elipse Midpoint");

    AppendMenu(filling, MF_POPUP, (UINT_PTR)circlesLines, "Filling Circle With Lines");
    AppendMenu(circlesLines, MF_STRING, 18, "First Quarter");
    AppendMenu(circlesLines, MF_STRING, 19, "Second Quarter");
    AppendMenu(circlesLines, MF_STRING, 20, "Third Quarter");
    AppendMenu(circlesLines, MF_STRING, 21, "Fourth Quarter");

    AppendMenu(filling, MF_POPUP, (UINT_PTR)circlesCircles, "Filling Cricle With Circles");
    AppendMenu(circlesCircles, MF_STRING, 22, "First Quarter");
    AppendMenu(circlesCircles, MF_STRING, 23, "Second Quarter");
    AppendMenu(circlesCircles, MF_STRING, 24, "Third Quarter");
    AppendMenu(circlesCircles, MF_STRING, 25, "Fourth Quarter");

    AppendMenu(filling, MF_STRING, 26, "Filling Square With Hermite Curve");
    AppendMenu(filling, MF_STRING, 27, "Filling Rectangle With Bezier Curve");
    AppendMenu(filling, MF_STRING, 28, "Convex Filling");
    AppendMenu(filling, MF_STRING, 29, "Non-Convex Filling");
    AppendMenu(filling, MF_STRING, 30, "Recursive FloodFill");
    AppendMenu(filling, MF_STRING, 31, "Non-Recursive FloodFill");
    AppendMenu(filling, MF_STRING, 34, "circleFillIntersection");

    AppendMenu(clipping, MF_STRING, 32, "Line Clipping");
    AppendMenu(clipping, MF_STRING, 33, "Polygon Clipping");

    SetMenu(hwnd, hmenu);
}
