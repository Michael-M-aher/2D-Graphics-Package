void Draw8Points(HDC hdc,Point pCenter,int x,int y,COLORREF color){
    SetPixel(hdc, pCenter.x+x, pCenter.y+y, color);
    SetPixel(hdc, pCenter.x+x, pCenter.y-y, color);
    SetPixel(hdc, pCenter.x-x, pCenter.y+y, color);
    SetPixel(hdc, pCenter.x-x, pCenter.y-y, color);
    SetPixel(hdc, pCenter.x+y, pCenter.y+x, color);
    SetPixel(hdc, pCenter.x+y, pCenter.y-x, color);
    SetPixel(hdc, pCenter.x-y, pCenter.y+x, color);
    SetPixel(hdc, pCenter.x-y, pCenter.y-x, color);
}

void Circle_Cartesian(HDC hdc,Point pCenter,int r, COLORREF color){
    int x = 0;
    int y = r;
    int r2 = r*r;
    while(x<=y){
        Draw8Points(hdc,pCenter,x,y,color);
        x++;
        y = Round(sqrt((r2-x*x)));
    }
}

void Circle_Polar(HDC hdc,Point pCenter,int r, COLORREF color){
    int x = r;
    int y = 0;
    double theta=0,dtheta=1.0/r;
    while(x>=y){
        Draw8Points(hdc,pCenter,x,y,color);
        theta+=dtheta;
        x = Round(r*cos(theta));
        y = Round(r*sin(theta));
    }
}

void Circle_Polar_Iterative(HDC hdc,Point pCenter,int r, COLORREF color){
    int x = r;
    int y = 0;
    double dtheta=1.0/r;
    double ct = cos(dtheta),st = sin(dtheta);
    while(x>=y){
        Draw8Points(hdc,pCenter,Round(x),Round(y),color);
        double xtemp = x*ct - y*st;
        y = y*ct + x*st;
        x = xtemp;
    }
}

void Circle_Midpoint(HDC hdc,Point pCenter,int r, COLORREF color){
    int x = 0;
    int y = r;
    int d = 1-r;
    while(x<=y){
        Draw8Points(hdc,pCenter,Round(x),Round(y),color);
        if(d>=0){
            d+=(2*(x-y)+5);
            y--;
        }else{
            d+=(2*x+3);
        }
        x++;
    }
}

void Circle_Midpoint_Improved(HDC hdc,Point pCenter,int r, COLORREF color){
    int x = 0;
    int y = r;
    int d = 1-r;
    int c1 = 5-2*r;
    int c2 = 3;
    while(x<=y){
        Draw8Points(hdc,pCenter,Round(x),Round(y),color);
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
