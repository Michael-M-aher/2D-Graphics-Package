class Constraint
{
public:
    int w, l, xCons, yCons;
    std::string type;
    Constraint(int w = 0, int l = 0, std::string type = "")
    {
        this->w = w;
        this->l = l;
        this->type = type;
        setConsBasedOnType();
    }

    void setConsBasedOnType()
    {
        if (this->type == "R")
        {
            this->xCons = this->w;
            this->yCons = this->l;
        }
        else
        {
            this->xCons = l;
            this->yCons = l;
        }
    }
};

int getxConstraintStart(Constraint cons, Point c)
{
    return c.x - (int)(cons.xCons / 2);
}
int getxConstraintEnd(int ConstraintStart, Constraint cons)
{
    return ConstraintStart + cons.xCons;
}

int getyConstraintStart(Constraint cons, Point c)
{
    return c.y - (int)(cons.yCons / 2);
}
int getyConstraintEnd(int ConstraintStart, Constraint cons)
{
    return ConstraintStart + cons.yCons;
}

void drawHermiteCurveWithConstraints(HDC hdc, Point P0, Point T0, Point P1, Point T1, COLORREF color, Point c, Constraint cons)
{
    double *xCoff = GetHermiteCoeff(P0.x, T0.x, P1.x, T1.x);
    double *yCoff = GetHermiteCoeff(P0.y, T0.y, P1.y, T1.y);

    int xConstraintStart = getxConstraintStart(cons, c);
    int xConstraintEnd = getxConstraintEnd(xConstraintStart, cons);
    int yConstraintStart = getyConstraintStart(cons, c);
    int yConstraintEnd = getyConstraintEnd(yConstraintStart, cons);

    double dt = 1.0 / 200;
    for (double t = 0; t <= 1; t += dt)
    {
        double *vt = new double[4];
        vectorInit(vt);
        vt[3] = 1;
        for (int i = 2; i >= 0; i--)
            vt[i] = vt[i + 1] * t;
        int x = round(dotProduct(xCoff, vt));
        int y = round(dotProduct(yCoff, vt));
        if (x < xConstraintStart || x > xConstraintEnd)
            continue;
        if (y < yConstraintStart || y > yConstraintEnd)
            continue;
        if (t == 0)
            MoveToEx(hdc, x, y, NULL);
        else
            LineTo(hdc, x, y);

        delete[] vt;
    }
}

void DrawSquare(HDC hdc, Point c, int l, COLORREF color)
{
    int xedge = c.x;
    int yedge = c.y;
    Line_Midpoint(hdc, Point(xedge, yedge), Point(xedge + l, yedge), RGB(0, 0, 0));
    Line_Midpoint(hdc, Point(xedge, yedge), Point(xedge, yedge + l), RGB(0, 0, 0));
    Line_Midpoint(hdc, Point(xedge, yedge + l), Point(xedge + l, yedge + l), RGB(0, 0, 0));
    Line_Midpoint(hdc, Point(xedge + l, yedge + l), Point(xedge + l, yedge), RGB(0, 0, 0));
}

void fillSquareHermiteCurves(HDC hdc, Point c, int l, COLORREF color)
{
    int xdown = c.x;
    int xup = xdown;
    int ydown = c.y;
    int yup = ydown + l;
    int constraint = (c.x) + l;
    while (xdown <= constraint)
    {
        drawHermiteCurveWithConstraints(hdc, Point(xdown, ydown), Point(20, 0), Point(xup, yup), Point(20, 0), color, c, Constraint(0, 2 * l, "S"));
        xdown += 3;
        xup += 3;
    }
}
void drawRectangle(HDC hdc, Point c, int w, int l, COLORREF color)
{
    int xedge = c.x;
    int yedge = c.y;
    Line_Midpoint(hdc, Point(xedge, yedge), Point(xedge + w, yedge), RGB(0, 0, 0));
    Line_Midpoint(hdc, Point(xedge, yedge), Point(xedge, yedge + l), RGB(0, 0, 0));
    Line_Midpoint(hdc, Point(xedge, yedge + l), Point(xedge + w, yedge + l), RGB(0, 0, 0));
    Line_Midpoint(hdc, Point(xedge + w, yedge + l), Point(xedge + w, yedge), RGB(0, 0, 0));
}

void drawBezierCurveWithConstraints(HDC hdc, Point P0, Point P1, Point P2, Point P3, COLORREF color, Point c, Constraint cons)
{
    Point T0(3 * (P1.x - P0.x), 3 * (P1.y - P0.y));
    Point T1(3 * (P3.x - P2.x), 3 * (P3.y - P2.y));
    drawHermiteCurveWithConstraints(hdc, P0, T0, P3, T1, color, c, cons);
}

void fillRectangleBezierCurve(HDC hdc, Point c, int w, int l, COLORREF color)
{
    int xedge = c.x;
    int yedge = c.y;
    int constraint = yedge + l;
    while (yedge <= constraint)
    {
        yedge += 3;
        drawBezierCurveWithConstraints(hdc, Point(xedge, yedge), Point(xedge - 10, yedge + 10), Point(xedge - 10, yedge - 10), Point(xedge + w, yedge), color, c, Constraint(2 * w, 2 * l, "R"));
    }
}
