#include "complex.hpp"
#include <stdexcept>
#include <cmath>

Complex::Complex(const double re)
    : re(re), im(0.0)
{}

Complex::Complex(const double re, const double im)
    : re(re), im(im)
{}

double Complex::getRe() const {
    return re;
}

double Complex::getIm() const {
    return im;
}

Complex& Complex::operator+=(const Complex& rhs) {
    re += rhs.re;
    im += rhs.im;
    return *this;
}

Complex& Complex::operator-=(const Complex& rhs) {
    re -= rhs.re;
    im -= rhs.im;
    return *this;
}

Complex& Complex::operator*=(const Complex& rhs) {
    const double new_re = re * rhs.re - im * rhs.im;
    const double new_im = im * rhs.re + re * rhs.im;
    re = new_re;
    im = new_im;
    return *this;
}

Complex& Complex::operator/=(const Complex& rhs) {
    if (std::abs(rhs.re) < Complex::EPSILON && std::abs(rhs.im) < Complex::EPSILON) {
        throw std::invalid_argument("Division by zero complex number is undefined!");
    }
    const double denominator = rhs.re * rhs.re + rhs.im * rhs.im;
    const double new_re = (re * rhs.re + im * rhs.im) / denominator;
    const double new_im = (im * rhs.re - re * rhs.im) / denominator;
    re = new_re;
    im = new_im;
    return *this;
}

Complex operator+(const Complex& lhs, const Complex& rhs) {
    Complex result(lhs);
    result += rhs;
    return result;
}

Complex operator-(const Complex& lhs, const Complex& rhs) {
    Complex result(lhs);
    result -= rhs;
    return result;
}

Complex operator*(const Complex& lhs, const Complex& rhs) {
    Complex result(lhs);
    result *= rhs;
    return result;
}

Complex operator/(const Complex& lhs, const Complex& rhs) {
    Complex result(lhs);
    result /= rhs;
    return result;
}

Complex& Complex::operator+=(double rhs){
    this->re += rhs;
    return *this;
}

Complex& Complex::operator-=(double rhs){
    this->re -= rhs;
    return *this;
}

Complex& Complex::operator*=(double rhs){
    this->re *= rhs;
    this->im *= rhs;
    return *this;
}

Complex& Complex::operator/=(double rhs){
    if (std::abs(rhs) < Complex::EPSILON){
        throw std::invalid_argument("Division by zero is undefined!");
    }
    this->re /= rhs;
    this->im /= rhs;
    return *this;
}


bool operator==(const Complex& lhs, const Complex& rhs) {
    return (std::abs(lhs.getRe() - rhs.getRe()) < Complex::EPSILON) && 
           (std::abs(lhs.getIm() - rhs.getIm()) < Complex::EPSILON);
}

bool operator!=(const Complex& lhs, const Complex& rhs) {
    return !(lhs == rhs);
}

std::ostream& Complex::writeTo(std::ostream& ostrm) const {
    ostrm << leftBrace << re << separator << im << rightBrace;
    return ostrm;
}

std::istream& Complex::readFrom(std::istream& istrm) {
    char left_brace, comma, right_brace;
    istrm >> left_brace >> re >> comma >> im >> right_brace;
    
    if (istrm.good()) {
        if (left_brace != leftBrace || comma != separator || right_brace != rightBrace) {
            istrm.setstate(std::ios_base::failbit);
        }
    }
    
    return istrm;
}

std::ostream& operator<<(std::ostream& ostrm, const Complex& c) {
    return c.writeTo(ostrm);
}

std::istream& operator>>(std::istream& istrm, Complex& c) {
    if (!c.readFrom(istrm)) {
        istrm.setstate(std::ios_base::failbit);
    }
    return istrm;
}

Complex operator+(const Complex& lhs, double rhs) {
    Complex result(lhs);
    result += rhs;
    return result;
}

Complex operator-(const Complex& lhs, double rhs) {
    Complex result(lhs);
    result -= rhs;
    return result;
}

Complex operator*(const Complex& lhs, double rhs) {
    Complex result(lhs);
    result *= rhs;
    return result;
}

Complex operator/(const Complex& lhs, double rhs) {
    Complex result(lhs);
    result /= rhs;
    return result;
}

Complex operator+(double lhs, const Complex& rhs) {
    return Complex(lhs) + rhs;
}

Complex operator-(double lhs, const Complex& rhs) {
    return Complex(lhs) - rhs;
}

Complex operator*(double lhs, const Complex& rhs) {
    return Complex(lhs) * rhs;
}

Complex operator/(double lhs, const Complex& rhs) {
    return Complex(lhs) / rhs;
}
