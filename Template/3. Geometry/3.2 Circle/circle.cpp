#include "point_and_line.cpp"
#include <cmath>
#include <vector>

struct circle {
    point o;
    num r;

    explicit circle(point o = point(), num r = 0) : o(o), r(r) {}
};

// 1: in    0: on    -1: out of
int inCircle(const point &p, const circle &c) {
    return cmp(sqr(c.r), dis2(p, c.o));
}

// 1: in    0: intersect    -1: out of
int segInCircle(const line &l, const circle &c) {
    int in1 = inCircle(l.a, c);
    int in2 = inCircle(l.b, c);
    if (in1 == 0 || in2 == 0)
        return 0;
    if (in1 != in2)
        return 0;
    if (in1 == 1 && in2 == 1)
        return 1;
    if (cmp(dis(c.o, l), c.r) == 1)
        return -1;
    if ((cmp(dot(l.a, l.b, c.o)) >= 0) && (cmp(dot(l.b, l.a, c.o)) >= 0))
        return 0;
    return -1;
}

bool intersect(const circle &a, const line &l, point &p1, point &p2) {
    num x = ((l.a - a.o) ^ (l.b - l.a));
    num y = (l.b - l.a).len2();
    num d = sqr(x) - y * ((l.a - a.o).len2() - sqr(a.r));
    if (cmp(d) < 0)
        return false;
    point p = l.a - ((l.b - l.a) * (x / y));
    d = std::max(d, static_cast<num>(0));
    point delta = (l.b - l.a) * (std::sqrt(d) / y);
    p1 = p + delta;
    p2 = p - delta;
    return true;
}

// connot handle coincident circle
bool intersect(const circle &a, const circle &b, point &p1, point &p2) {
    num s1 = (a.o - b.o).len();
    if (cmp(s1 - a.r - b.r) > 0 || cmp(s1 - std::abs(a.r - b.r)) < 0)
        return false;
    num s2 = (sqr(a.r) - sqr(b.r)) / s1;
    num aa = (s1 + s2) * 0.5;
    num bb = (s1 - s2) * 0.5;
    num d = sqr(a.r) - sqr(aa);
    point o = (b.o - a.o) * (aa / (aa + bb)) + a.o;
    d = std::max(d, static_cast<num>(0));
    point delta = (b.o - a.o).norm().turn90() * std::sqrt(d);
    p1 = o + delta;
    p2 = o - delta;
    return true;
}

bool tangentPoint(const point &p0, const circle &c, point &p1, point &p2) {
    num x = (p0 - c.o).len2();
    num d = x - sqr(c.r);
    if (cmp(d) <= 0)
        return false;
    point p = (p0 - c.o) * (sqr(c.r) / x);
    point delta = ((p0 - c.o) * (-c.r * std::sqrt(d) / x)).turn90();
    p1 = c.o + p + delta;
    p2 = c.o + p - delta;
    return true;
}

std::vector<line> exTangentLine(const circle &c1, const circle &c2) {
    std::vector<line> ans = std::vector<line>();
    if (cmp(c1.r - c2.r) == 0) {
        point dir = c2.o - c1.o;
        dir = (dir * (c1.r / dir.len())).turn90();
        ans.push_back(line(c1.o + dir, c2.o + dir));
        ans.push_back(line(c1.o - dir, c2.o - dir));
    } else {
        point p = (c2.o * c1.r - c1.o * c2.r) / (c1.r - c2.r);
        point p1, p2, q1, q2;
        if (tangentPoint(p, c1, p1, p1) && tangentPoint(p, c2, q1, q2)) {
            ans.push_back(line(p1, q1));
            ans.push_back(line(p2, q2));
        }
    }
    return ans;
}

std::vector<line> inTangentLine(const circle &c1, const circle &c2) {
    std::vector<line> ans = std::vector<line>();
    point p = (c2.o * c1.r + c1.o * c2.r) / (c1.r + c2.r);
    point p1, p2, q1, q2;
    if (tangentPoint(p, c1, p1, p2) && tangentPoint(p, c2, q1, q2)) {
        ans.push_back(line(p1, q1));
        ans.push_back(line(p2, q2));
    }
    return ans;
}

// unchecked
