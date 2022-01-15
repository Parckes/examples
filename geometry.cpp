#include <cstdio>
#include <cmath>
#include <cinttypes>
#include <algorithm>

class Line;

class Point
{
private:
    int64_t x;
    int64_t y;
public:
    Point (double x, double y) : x(x), y(y) {};
    Point() = default;
    void print() const
    {
        printf ("%ld %ld\n", x, y);
    };
    bool operator==(const Point& a)
    {
        if (x == a.x && y == a.y)
            return 1;
        return 0;
    }
    friend double distance_dots (const Point&, const Point&);
    friend bool is_cross_nondeg_segments(const Line&, const Line&, const Point&, const Point&,
                                                             const Point&, const Point&);
    friend bool is_cross_segments(const Point&, const Point&, const Point&, const Point&);
    friend int64_t oriented_area (const Point&, const Point&, const Point&);
    friend bool is_dot_inside(const Point&, int64_t);
    friend long double area (int64_t);
    friend long double area (Point*, int64_t);
    friend Point* build_convex_hull(int64_t);
    friend bool point_comparator(const Point&, const Point&);
    friend bool are_points_clockwise(const Point&, const Point&, const Point&);
    friend bool are_points_counterclockwise(const Point&, const Point&, const Point&);
    friend class Line;
    friend class Vector;
};
double distance_dots(const Point& a, const Point& b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
bool point_comparator(const Point& a, const Point& b)
{
    return a.x < b.x || a.x == b.x && a.y < b.y;
}
bool are_points_counterclockwise(const Point& a, const Point& b, const Point& c)
{
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) > 0;
}
bool are_points_clockwise(const Point& a, const Point& b, const Point& c)
{
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) < 0;
}

//////////////////////////////////////////////////
/////////////////////////////////////////////////
class Vector
{
private:
    double x;
    double y;
public:
    Vector(double a_x, double a_y, double b_x, double b_y) : x(b_x - a_x), y(b_y - a_y){};
    Vector(const Vector& a) : Vector(0, 0, a.x, a.y) {};
    Vector (const Point& a, const Point& b) : x(b.x - a.x), y(b.y - a.y){};
    void print() const
    {
        printf ("%f %f\n", static_cast<double>(x), static_cast<double>(y));
    };
    double length() const
    {
        return sqrt(static_cast<double>(x * x + y * y));
    }
    friend Vector operator+(const Vector&, const Vector&);
    friend double scal_prod(const Vector&, const Vector&);
    friend double vector_prod(const Vector&, const Vector&);
    friend double area_tri(const Vector&, const Vector&);
    friend int is_proport (const Vector&, const Vector&);

    friend class Line;
};

Vector operator+(const Vector& a, const Vector& b)
{
    return Vector(0, 0, a.x + b.x, a.y + b.y);
}
double scal_prod(const Vector& a, const Vector& b)
{
    return (a.x * b.x + a.y * b.y);
}
double vector_prod(const Vector& a, const Vector& b)
{
    return a.x * b.y - b.x * a.y;
}
double area_tri(const Vector& a, const Vector& b)
{
    return abs(vector_prod(a, b)) / 2.0;
}
int is_proport (const Vector& a, const Vector& b)
{
    if (a.x == 0 || b.x == 0 || b.y == 0 || a.y == 0)
    {
        if (a.x == 0 && b.x == 0 || a.y == 0 && b.y == 0)
            return 1;
        if (!a.x && b.x || a.x && !b.x || a.y && !b.y || !a.y && b.y)
            return 0;
    }
    if (a.x / b.x == a.y / b.y)
        return 1;
    return 0;
};
//////////////////////////////////////////////////
//////////////////////////////////////////////////
class Line
{
private:
    double A;
    double B;
    double C;
    //Point first;
    // Point last;
public:
    Line(double A, double B, double C) : A(A), B(B), C(C){};
    Line(Point a, Point b) : A(a.y - b.y), B(b.x - a.x), C(a.x * b.y - a.y * b.x){};
    Line(Vector a, Point x) : A(a.y), B(-a.x), C(a.x * x.y - a.y * x.x){};
    void print() const
    {
        printf ("%f %f %f\n", A, B, C);
    }
    Vector guide_vect() const
    {
        return Vector(0, 0, B, -A);
    }
    bool is_belongs_line(const Point& a) const
    {
        if (std::abs(A * a.x + B * a.y + C) < 0 + 0.000001)
            return 1;
        return 0;
    }
    bool is_belongs_ray(Point first, Point last, Point a) const
    {
        if (is_belongs_line(a))
        {
            if (first.x == last.x)
            {
                if (first.y < last.y)
                {
                    if (a.y >= first.y)
                        return 1;
                    else
                        return 0;
                }
                else if(first.y > last.y)
                {
                    if (a.y <= first.y)
                        return 1;
                    else
                        return 0;
                }
                else
                {
                    printf ("WTF ):<\n");
                }
            }
            else if (first.x < last.x)
            {
                if (a.x >= first.x)
                    return 1;
                else
                    return 0;
            }
            else
            {
                if (a.x <= first.x)
                    return 1;
                else
                    return 0;
            }
        }
        return 0;
    }
    bool is_belongs_segment(const Point& first, const Point& last, const Point& a) const
    {
        if (is_belongs_ray(first, last, a) && is_belongs_ray(last, first, a))
            return 1;
        return 0;
    }
    /*Point proection (const Point& a)
    {
        Point pr = find_cross(Line(A, B, C), Line(Vector(0, 0, A, B), a));
        //printf ("!%f %f!!\n", pr.x, pr.y);
            return pr;
    }*/
    double distance_line (const Point& first, const Point& last, const Point& a)
    {
        return 2 * area_tri(Vector(first, last), Vector(first, a)) / Vector(first, last).length();
    }
    double distance_ray(const Point& first, const Point& last, const Point& a)
    {
        if (scal_prod(Vector(first, last), Vector(first, a)) >= 0)
            return (distance_line(first, last, a));
        return distance_dots(a, first);
    }
    double distance_segment (const Point& first, const Point& last, const Point& a)
    {
        if (scal_prod(Vector(first, last), Vector(first, a)) >= 0 &&
                          scal_prod(Vector(last, first), Vector(last, a)) >= 0)
            return (distance_line(first, last, a));
        return std::min(distance_dots(a, first), distance_dots(a, last));
    }
    friend Point find_cross(const Line&, const Line&);
    friend double distance_lines (const Line&, const Line&);
    friend int is_cross(const Line&, const Line&);
    friend bool is_cross_segments(const Point&, const Point&, const Point&, const Point&);
    friend bool is_cross_nondeg_segments(const Line&, const Line&, const Point&, const Point&,
                                                             const Point&, const Point&);
    friend bool is_match(const Line&, const Line&);

};
int is_cross(const Line& l1, const Line& l2)
{
    if (l1.A * l2.B - l1.B * l2.A)
        return 1;
    return 0;
}
Point find_cross(const Line& l1, const Line& l2)
{
    double x;
    double y;
    double opr = (l1.A * l2.B - l1.B * l2.A);
    y = (l1.A * (-l2.C) - (-l1.C) * l2.A) / opr;
    x = ((-l1.C) * l2.B - l1.B * (-l2.C)) / opr;
    return Point (x, y);
}
double distance_lines (const Line& l1, const Line& l2)
{
    double dist;
    double A, B, C;
    A = l1.A;
    B = l1.B;
    C = l2.C * l1.A / l2.A;
    if (!sqrt(A * A + B * B))
        return 0;
    dist = std::abs(C - l1.C) / sqrt(A * A + B * B);
    return dist;
}
int64_t oriented_area (const Point& a, const Point& b, const Point& c)
{
    return (a.y - b.y) * (c.x - a.x) - (a.x - b.x) * (c.y - a.y);
}

bool is_cross_segments(const Point& a, const Point& b, const Point& c, const Point& d)
{
    double ax = a.x; double bx = b.x; double cx = c.x; double dx = d.x;
    double ay = a.y; double by = b.y; double cy = c.y; double dy = d.y;
    if (ax > bx)
        std::swap(ax, bx);
    if (cx > dx)
        std::swap(cx, dx);
    if (ay > by)
        std::swap(ay, by);
    if (cy > dy)
        std::swap (cy, dy);
    if (std::max(ax, cx) <= std::min(bx, dx) &&
        std::max(ay, cy) <= std::min(by, dy) &&
        (oriented_area(a, b, c) * oriented_area(a, b, d) <= 0) &&
        (oriented_area(c, d, a) * oriented_area(c, d, b) <= 0))
        return 1;
    return 0;
}

//cross or match fully or partially
bool is_cross_nondeg_segments(const Line& l_1, const Line& l_2, const Point& a, const Point& b,
                                                         const Point& c, const Point& d)
{
    if (is_match(l_1, l_2))
    {
        //printf ("INSIDE\n");
        if (a.x != b.x)
            if (a.x < b.x)
            {
                if ((c.x <= b.x && c.x >= a.x) || (d.x <= b.x && d.x >= a.x))
                    return 1;
            }
            else
            {
                if ((c.x <= a.x && c.x >= b.x) || (d.x <= a.x && d.x >= b.x))
                    return 1;
            }
        else
            if (a.y < b.y)
            {
                if ((c.y <= b.y && c.y >= a.y) || (d.y <= b.y && d.y >= a.y))
                    return 1;
            }
            else
            {
                if ((c.y <= a.y && c.y >= b.y) || (d.y <= a.y && d.y >= b.y))
                    return 1;
            }
    }
    Point x = find_cross(l_1, l_2);
    if (is_cross(l_1, l_2) && l_1.is_belongs_segment(a, b, x)
                           && l_2.is_belongs_segment(c, d, x))
        return 1;
    return 0;
}
double distance_segments (const Point& a, const Point& b, const Point& c, const Point& d)
{
    Line l1(a, b);
    Line l2(c, d);
    return std::min(std::min(l1.distance_segment(a, b, c), l1.distance_segment(a, b, d)),
                    std::min(l2.distance_segment(c, d, a), l2.distance_segment(c, d, b)));
}
bool is_match(const Line& l1, const Line& l2)
{
    /*if (is_cross(l1, l2) || (!l1.A && l2.A) || (l1.A && !l2.A) || (l1.B && !l2.B)
                         || (l1.B && !l2.B) || (l1.C && !l2.C) || (l1.C && !l2.C)
                         || (l2.A && l2.B && (l1.A / l2.A != l1.B / l2.B))
                         || (l2.A && l2.C && (l1.A / l2.A != l1.C / l2.C))
                         || (l2.C && l2.B && (l1.C / l2.C != l1.B / l2.B)))
        return 0;
    return 1;*/
    double ta;
    double tb;
    if (l1.A == 0)
    {
        ta = 1;
        tb = -l1.C / l1.B;
    }
    else if (l1.B == 0)
    {
        ta = -l1.C / l1.A;
        tb = 1;
    }
    else
    {
        ta = -l1.C / (2 * l1.A);
        tb = -l1.C / (2 * l1.B);
    }
    if (!is_cross(l1, l2) && (l2.A * ta + l2.B * tb + l2.C == 0))
        return 1;
    return 0;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool is_convex(int64_t N)
{
    Point* a = new Point[N];
    for (int i = 0; i < N; ++i)
    {
        int64_t x, y;
        scanf ("%ld %ld", &x, &y);
        a[i] = Point(x, y);
    }
    int sign = 0;
    for (int i = 0; i < N; ++i)
    {
        double prod = vector_prod(Vector(a[i], a[(i + 1) % N]),
                                  Vector(a[(i + 1) % N], a[(i + 2) % N]));
        if (prod > 0)
            sign = sign | 1;
        if (prod < 0)
            sign = sign | 2;
        if (sign == 3)
        {
            delete[] a;
            return 0;
        }
    }
    delete[] a;
    return 1;
}

bool is_dot_inside(const Point& p, int64_t N)
{
    int res = 0;
    Point* arr = new Point[N];
    for (int i = 0; i < N; ++i)
    {
        int64_t x, y;
        scanf ("%ld %ld", &x, &y);
        arr[i] = Point(x, y);
    }
    Point a = arr[0];
    Point b = arr[1];
    Point first = p;
    Point last(p.x + 10, p.y);
    for (int64_t i = 1; i <= N; ++i)
    {
        if (Line(a, b).is_belongs_segment(a, b, p))
        {
            delete[] arr;
            return 1;
        }
        int fl = 1;
        if (is_cross_segments(a, b, first, last))
        {
            if (Line(first, last).is_belongs_ray(first, last, a) && Line(first, last).is_belongs_ray(first, last, b))
            {
                fl = 0;
            }
            else if (Line(first, last).is_belongs_ray(first, last, a) && a.y < b.y)
            {
                fl = 0;
            }
            else if (Line(first, last).is_belongs_ray(first, last, b) && b.y < a.y)
            {
                fl = 0;
            }
            if (fl)
            {
                res = !res;
            }
        }
        a = arr[i % N];
        b = arr[(i + 1) % N];
    }
    delete[] arr;
    return res;
    }

long double area (int64_t N)
{
    long double res = 0;
    Point* a = new Point[N];
    for (int i = 0; i < N; ++i)
    {
        int64_t x, y;
        scanf ("%ld %ld", &x, &y);
        a[i] = Point(x, y);
    }
    double ar = 0;
    for (int i = 0; i < N; ++i)
    {
        Point A;
        Point B;
        if (i)
            A = a[i - 1];
        else
            A = a[N - 1];
        B = a[i];
        res += (A.x - B.x) * (A.y + B.y);
        //printf("%Lf\n", res);
    }
    res = std::abs(res) / 2;
    delete[] a;
    return res;
}

long double area (Point* a, int64_t N)
{
    long double res = 0;
    double ar = 0;
    for (int i = 0; i < N; ++i)
    {
        Point A;
        Point B;
        if (i)
            A = a[i - 1];
        else
            A = a[N - 1];
        B = a[i];
        res += (A.x - B.x) * (A.y + B.y);
        //printf("%Lf\n", res);
    }
    res = std::abs(res) / 2;
    return res;
}

Point* build_convex_hull(int64_t N)
{
    Point* a = new Point[N];
    for (int i = 0; i < N; ++i)
    {
        int64_t x, y;
        scanf ("%ld %ld", &x, &y);
        a[i] = Point(x, y);
    }
  	std::sort (a, a + N - 1, point_comparator);
  	Point p_1 = a[0];
    Point p_2 = a[N - 1];
  	Point* top = new Point[N];
    int64_t top_s = 0;
    Point* down = new Point[N];
    int64_t down_s = 0;
  	top[top_s++] = p_1;
  	down[down_s++] = p_1;
  	for (int64_t i = 1; i < N; ++i)
    {
    		if (i == N - 1 || are_points_clockwise (p_1, a[i], p_2))
        {
      			while (top_s >= 2 && !are_points_clockwise (top[top_s - 2], top[top_s - 1], a[i]))
      				  --top_s;
      			top[top_s++] = a[i];
  		  }
  		  if (i == N - 1 || are_points_counterclockwise (p_1, a[i], p_2))
        {
  			     while (down_s >= 2 && !are_points_counterclockwise (down[down_s - 2], down[down_s - 1], a[i]))
  				       --down_s;
  			     down[down_s++] = a[i];
  		  }
  	}
    Point* res = new Point[N];
  	int64_t res_s = 0;
  	for (int64_t i = 0; i < top_s; ++i)
  		  res[res_s++] = top[i];
  	for (int64_t i = down_s - 2; i > 0; --i)
  		  res[res_s++] = down[i];
    printf("%ld\n", res_s);
    for (int i = 0; i < res_s; ++i)
    {
        printf("%ld %ld\n", res[i].x, res[i].y);
    }
    printf("%.1Lf\n", area(res, res_s));
    delete[] top;
    delete[] down;
    delete[] a;
    return res;
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
