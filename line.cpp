void Line_Direct(HDC hdc,Point p1,Point p2,COLORREF color){
    int dy = p2.y - p1.y;
    int dx = p2.x - p1.x;
    double m = (double)dy/dx;
    double mi = (double)dx/dy;
    if(abs(dx)>=abs(dy)){
        if(p2.x<p1.x)swap(p1,p2);
        double x = p1.x;
        double y = p1.y;
        while(x<=p2.x){
            SetPixel(hdc,x,Round(y),color);
            x++;
            y = p1.y+(x-p1.x)*m;
        }
    }else{
        if(dy<0)swap(p1,p2);
        int y = p1.y;
        double x = p1.x;
        while(y<=p2.y){
            SetPixel(hdc,Round(x),y,color);
            y++;
            x = p1.x+(y-p1.y)*mi;
        }
    }
}


void Line_DDA(HDC hdc,Point p1,Point p2,COLORREF color){
    int dy = p2.y - p1.y;
    int dx = p2.x - p1.x;
    double m = (double)dy/dx;
    double mi = (double)dx/dy;
    if(abs(dx)>=abs(dy)){
        if(p2.x<p1.x)swap(p1,p2);
        double x = p1.x;
        double y = p1.y;
        while(x<=p2.x){
            SetPixel(hdc,x,Round(y),color);
            x++;
            y+=m;
        }
    }else{
        if(dy<0)swap(p1,p2);
        double x = p1.x;
        int y = p1.y;
        while(y<=p2.y){
            SetPixel(hdc,Round(x),y,color);
            y++;
            x+=mi;
        }
    }
}

void Line_Midpoint(HDC hdc,Point p1,Point p2,COLORREF color){
    int dy = p2.y - p1.y;
    int dx = p2.x - p1.x;
    if(abs(dx)>=abs(dy)){
        int d = abs(dx)-2*abs(dy);
        int change1 = 2*abs(dx)-2*abs(dy);
        int change2 = -(2*abs(dy));
        if(p2.x<p1.x)swap(p1,p2);
        int x = p1.x;
        int y = p1.y;
        while(x<=p2.x){
            SetPixel(hdc,x,y,color);
            if(p2.y>=p1.y){
                x++;
                if(d<=0){
                    y++;
                    d+=change1;
                }else{
                    d+=change2;
                }
            }else{
                x++;
                if(d>=0){
                    y--;
                    d-=change1;
                }else{

                    d-=change2;
                }
            }
        }
    }else{
        int d = 2*abs(dx)-abs(dy);
        int change1 = 2*abs(dx)-2*abs(dy);
        int change2 = 2*abs(dx);
        if(dy<0)swap(p1,p2);
        int x = p1.x;
        int y = p1.y;
        while(y<=p2.y){
            SetPixel(hdc,x,y,color);
            if(p2.x>=p1.x){
                y++;
                if(d>=0){
                    x++;
                    d+=change1;
                }else{
                    d+=change2;
                }
            }else{
                y++;
                if(d<=0){
                    x--;
                    d-=change1;
                }else{
                    d-=change2;
                }
            }
        }
    }
}

void Line_Parametric(HDC hdc, Point p1, Point p2, COLORREF color)
{
    int dy=p2.y-p1.y;
    int dx=p2.x-p1.x;
    for (double t = 0; t <= 1; t += 0.001)
    {
        double x = Round(p1.x + (t * (dx)));
        double y = Round(p1.y + (t * (dy)));
        SetPixel(hdc, Round(x), Round(y), color);
    }
}
