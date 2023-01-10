#pragma once

#include <cmath>

class Complex {
private:
    double re;
    double im;
    friend Complex operator+(const Complex&, const Complex&);
    friend Complex operator-(const Complex&, const Complex&);
    friend Complex operator*(const Complex&, const Complex&);
public:
    Complex(double r = 0.0, double i = 0.0): re(r), im(i) {};
    Complex(const Complex& c): re(c.re), im(c.im)         {};

    Complex& operator=(const Complex& c) = default;
    Complex& operator+=(const Complex& c) {
        re += c.re;
        im += c.im;
        return *this;
    }
    Complex& operator-=(const Complex& c) {
        re -= c.re;
        im -= c.im;
        return *this;
    }
    Complex& operator*=(const Complex& c) {
        double re_t = re;
        double im_t = im;
        re = re_t * c.re - im_t * c.im;
        im = re_t * c.im + im_t * c.re;
        return *this;
    }


    Complex operator-() const {
        return Complex(-re, -im);
    }

    Complex operator+() const {
        return Complex(*this);
    }


    bool operator==(const Complex& c) const {
        return (im == c.im) && (re == c.re);
    }

    bool operator!=(const Complex& c) const {
        return !(*this == c);
    }

    double GetRe() const {return re;};
    double GetIm() const {return im;};
};

Complex operator+(const Complex& a, const Complex& b) {
    return Complex(a.re + b.re, a.im + b.im);
}

Complex operator-(const Complex& a, const Complex& b) {
    return Complex(a.re - b.re, a.im - b.im);
}

Complex operator*(const Complex& a, const Complex& b) {
    return Complex(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
}

template <typename U>
U Abs(U number) {
  return number > 0 ? number : -number;
}

double Abs(Complex number) {
    return sqrt(number.GetRe() * number.GetRe() + number.GetIm() * number.GetIm());
}
