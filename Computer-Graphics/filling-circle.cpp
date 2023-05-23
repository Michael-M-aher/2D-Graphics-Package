int circleShift = 5;
void FillFirstQuarterLines(HDC hdc, Point pCenter, int r, COLORREF color)
{
    int r2 = r * r;
    for (int x = pCenter.x + 1; x <= pCenter.x + r; x += 1){
        int xLen = x - pCenter.x;
        int xLen2 = xLen * xLen;
        int y = -sqrt(r2 - xLen2) + pCenter.y;
        Line_Midpoint(hdc,pCenter,Point(x,y),color);
    }
}

void FillSecondQuarterLines(HDC hdc, Point pCenter, int r, COLORREF color)
{
    int r2 = r * r;
    for (int x = pCenter.x + 1; x <= pCenter.x + r; x += 1){
        int xLen = x - pCenter.x;
        int xLen2 = xLen * xLen;
        int y = sqrt(r2 - xLen2) + pCenter.y;
        Line_Midpoint(hdc,pCenter,Point(x,y),color);
    }
}


void FillThirdQuarterLines(HDC hdc, Point pCenter, int r, COLORREF color)
{
    int r2 = r * r;
    for (int x = pCenter.x - 1; x >= pCenter.x - r; x -= 1){
        int xLen = x - pCenter.x;
        int xLen2 = xLen * xLen;
        int y = sqrt(r2 - xLen2) + pCenter.y;
        Line_Midpoint(hdc,pCenter,Point(x,y),color);
    }
}

void FillFourthQuarterLines(HDC hdc, Point pCenter, int r, COLORREF color)
{
    int r2 = r * r;
    for (int x = pCenter.x - 1; x >= pCenter.x - r; x -= 1){
        int xLen = x - pCenter.x;
        int xLen2 = xLen * xLen;
        int y = -sqrt(r2 - xLen2) + pCenter.y;
        Line_Midpoint(hdc,pCenter,Point(x,y),color);
    }
}



void drawCircleFirstQuarter(HDC hdc, Point pCenter, int r, COLORREF color)
{
    int x = 0;
    int y = r;
    int d = 1-r;
    int c1 = 5-2*r;
    int c2 = 3;
    while(x<=y){
        SetPixel(hdc, pCenter.x + x, pCenter.y - y, color);
        SetPixel(hdc, pCenter.x + y, pCenter.y - x, color);
        if(d>=0){
            d+=c1;
            c1+=4;
            y--;
        }else{
            d+=c2;
            c1+=2;
        }
        c2+=2;
        x++;
    }

}


void drawCircleSecondQuarter(HDC hdc, Point pCenter, int r, COLORREF color)
{
    int x = 0;
    int y = r;
    int d = 1-r;
    int c1 = 5-2*r;
    int c2 = 3;
    while(x<=y){
        SetPixel(hdc, pCenter.x + x, pCenter.y + y, color);
        SetPixel(hdc, pCenter.x + y, pCenter.y + x, color);
        if(d>=0){
            d+=c1;
            c1+=4;
            y--;
        }else{
            d+=c2;
            c1+=2;
        }
        c2+=2;
        x++;
    }
}

void drawCircleThirdQuarter(HDC hdc, Point pCenter, int r, COLORREF color)
{
    int x = 0;
    int y = r;
    int d = 1-r;
    int c1 = 5-2*r;
    int c2 = 3;
    while(x<=y){
        SetPixel(hdc, pCenter.x - x, pCenter.y + y, color);
        SetPixel(hdc, pCenter.x - y, pCenter.y + x, color);
        if(d>=0){
            d+=c1;
            c1+=4;
            y--;
        }else{
            d+=c2;
            c1+=2;
        }
        c2+=2;
        x++;
    }
}

void drawCircleFourthQuarter(HDC hdc, Point pCenter, int r, COLORREF color)
{
    int x = 0;
    int y = r;
    int d = 1-r;
    int c1 = 5-2*r;
    int c2 = 3;
    while(x<=y){
        SetPixel(hdc, pCenter.x - x, pCenter.y - y, color);
        SetPixel(hdc, pCenter.x - y, pCenter.y - x, color);
        if(d>=0){
            d+=c1;
            c1+=4;
            y--;
        }else{
            d+=c2;
            c1+=2;
        }
        c2+=2;
        x++;
    }
}


void FillFirstQuarterCircles(HDC hdc, Point pCenter, int r, COLORREF color)
{
    for (int i = 0; i < r; i += circleShift){
        drawCircleFirstQuarter(hdc, pCenter, i, color);
    }
}

void FillSecondQuarterCircles(HDC hdc, Point pCenter, int r, COLORREF color)
{
    for (int i = 0; i < r; i += circleShift){
        drawCircleSecondQuarter(hdc, pCenter, i, color);
    }
}

void FillThirdQuarterCircles(HDC hdc, Point pCenter, int r, COLORREF color)
{
    for (int i = 0; i < r; i += circleShift){
        drawCircleThirdQuarter(hdc, pCenter, i, color);
    }
}

void FillFourthQuarterCircles(HDC hdc, Point pCenter, int r, COLORREF color)
{
    for (int i = 0; i < r; i += circleShift){
        drawCircleFourthQuarter(hdc, pCenter, i, color);
    }
}
