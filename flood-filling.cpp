void FloodFill_Recursive(HDC hdc,Point p,COLORREF bc,COLORREF fc){
    COLORREF c = GetPixel(hdc,p.x,p.y);
    if(c==bc || c==fc)return;
    SetPixel(hdc,p.x,p.y,fc);
    FloodFill_Recursive(hdc,Point(p.x+1,p.y),bc,fc);
    FloodFill_Recursive(hdc,Point(p.x-1,p.y),bc,fc);
    FloodFill_Recursive(hdc,Point(p.x,p.y+1),bc,fc);
    FloodFill_Recursive(hdc,Point(p.x,p.y-1),bc,fc);
}

void FloodFill_NonRecursive(HDC hdc,Point p,COLORREF bc,COLORREF fc){
    std::queue<Point> s;
    s.push(p);
    while(!s.empty()){
        Point mp = s.front();
        s.pop();
        COLORREF c = GetPixel(hdc,mp.x,mp.y);
        if(c==bc || c==fc)continue;
        SetPixel(hdc,mp.x,mp.y,fc);
        s.push(Point(mp.x+1,mp.y));
        s.push(Point(mp.x-1,mp.y));
        s.push(Point(mp.x,mp.y+1));
        s.push(Point(mp.x,mp.y-1));
    }
}
