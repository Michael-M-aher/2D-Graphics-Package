const int SCREENHEIGHT = 600;
typedef struct{
    int xLeft,xRight;
} Table[SCREENHEIGHT];

void Init_Table(Table t){
    for(int i=0;i<SCREENHEIGHT;i++){
        t[i].xLeft = INT_MAX;
        t[i].xRight = -INT_MAX;
    }
}

void Edge2Table(Point p1,Point p2,Table t){
    if(p1.y==p2.y)return;
    if(p1.y>p2.y)swap(p1,p2);
    double mi = (double)(p2.x-p1.x)/(p2.y-p1.y);
    double x = p1.x;
    int y = p1.y;
    while(y<p2.y){
        if(x<t[y].xLeft)t[y].xLeft=ceil(x);
        if(x>t[y].xRight)t[y].xRight=floor(x);
        y++;
        x+=mi;
    }
}

void Polygon2Table(mPolygon p,Table t){
    VertexList v = p.getVertexList();
    int sz = p.getNumVerices();
    Point v1 = v[sz-1];
    for(int i=0; i<sz;i++){
        Point v2 = v[i];
        Edge2Table(v1,v2,t);
        v1=v2;
    }
}

void Draw_Table(HDC hdc,Table t,COLORREF color){
    for(int y=0;y<SCREENHEIGHT;y++){
        int xs = t[y].xLeft;
        int xe = t[y].xRight;
        if(xs<xe)
            for(int x = xs;x<=xe;x++){
                SetPixel(hdc,x,y,color);
            }
    }
}

void ConvexFill(HDC hdc,mPolygon p,COLORREF color){
    Table t;
    Init_Table(t);
    Polygon2Table(p,t);
    Draw_Table(hdc,t,color);
}

struct EdgeRec
{
    double x;
    double mi;
    int ymax;
    bool operator<(EdgeRec r)
    {
        return x < r.x;
    }
};
typedef std::list<EdgeRec> EdgeList;

EdgeRec InitEdgeRec(Point &v1, Point &v2)
{
    if (v1.y > v2.y)
        swap(v1, v2);
    EdgeRec rec;
    rec.x = v1.x;
    rec.ymax = v2.y;
    rec.mi = (double)(v2.x - v1.x) / (v2.y - v1.y);
    return rec;
}

void InitEdgeTable(mPolygon p, std::unordered_map<int, EdgeList> &table)
{
    VertexList v = p.getVertexList();
    int sz = p.getNumVerices();
    Point v1 = v[sz - 1];
    for (int i = 0; i < sz; i++)
    {
        Point v2 = v[i];
        if (v1.y == v2.y)
        {
            v1 = v2;
            continue;
        }
        EdgeRec rec = InitEdgeRec(v1, v2);
        table[v1.y].push_back(rec);
        v1 = v[i];
    }
}

void NonConvexFill(HDC hdc,mPolygon p,COLORREF color){
    std::unordered_map<int, EdgeList> table;
    InitEdgeTable(p, table);
    int y = INT_MAX;
    for(auto i:table){
        if(i.first<y)
            y = i.first;
    }
    EdgeList active = table[y];
    while(!active.empty()){
        active.sort();
        for(EdgeList::iterator it = active.begin(); it != active.end(); it++){
            int x1 = ceil(it->x);
            it++;
            int x2 = floor(it->x);
            for(int x = x1;x<x2;x++)
                SetPixel(hdc,x,y,color);
        }
        y++;
        EdgeList::iterator it = active.begin();
        while (it != active.end()){
            if (y == it->ymax)
                it = active.erase(it);
            else
                it++;
        }
        for (EdgeList::iterator it = active.begin(); it != active.end(); it++)
                it->x += it->mi;
        active.insert(active.end(),table[y].begin(),table[y].end());
        }
}
