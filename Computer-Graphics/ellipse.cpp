void Draw4Points(HDC hdc,Point pCenter,int x,int y,COLORREF color){
    SetPixel(hdc, pCenter.x+x, pCenter.y+y, color);
    SetPixel(hdc, pCenter.x+x, pCenter.y-y, color);
    SetPixel(hdc, pCenter.x-x, pCenter.y+y, color);
    SetPixel(hdc, pCenter.x-x, pCenter.y-y, color);
}
void Ellipse_Cartesian(HDC hdc,Point pCenter,int a,int b, COLORREF color){
    int x = 0;
    int y = b;
    int a2 = a*a;
    int b2 = b*b;
    while(b2*x<=a2*y){
        Draw4Points(hdc,pCenter,x,y,color);
        x++;
        int x2 = x*x;
        y = Round(sqrt((a2*b2-b2*x2)/a2));
    }
    while(y>=0){
        Draw4Points(hdc,pCenter,x,y,color);
        y--;
        int y2 = y*y;
        x = Round(sqrt((a2*b2-a2*y2)/b2));
    }
}

void Ellipse_Polar(HDC hdc,Point pCenter,int a,int b, COLORREF color){
    int x = a;
    int y = 0;
    double theta = 0,dtheta=1.0/std::max(a,b);
    while(x>=0){
        Draw4Points(hdc,pCenter,x,y,color);
        theta += dtheta;
        x = Round(a*cos(theta));
        y = Round(b*sin(theta));
    }
}

void Ellipse_Midpoint(HDC hdc,Point pCenter,int a,int b, COLORREF color){
    int x = 0;
    int y = b;
    int a2 = a*a;
    int b2 = b*b;
    int d = b2-a2*b+0.25*a2;
    while(b2*x<=a2*y){
        Draw4Points(hdc,pCenter,x,y,color);
        if(d>=0){
            d += (2*x*b2 + 3*b2 - 2*y*a2 +2*a2);
            y--;
        }else{
            d += (2*x*b2 + 3*b2);
        }
        x++;
    }
    d = b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2;
    while(y>=0){
        Draw4Points(hdc,pCenter,x,y,color);
        if(d<=0){
            d += (2*x*b2 + 3*b2 - 2*y*a2 +2*a2);
            x++;
        }else{
            d += (2*a2 - 2*y*a2);
        }
        y--;
    }
}
