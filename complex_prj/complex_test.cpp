#include "complex/complex.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <cmath>

bool areEqual(const Complex& lhs, const Complex& rhs) {
    return std::abs(lhs.getRe() - rhs.getRe()) < Complex::EPSILON &&
           std::abs(lhs.getIm() - rhs.getIm()) < Complex::EPSILON;
}

class ComplexTester {
private:
    int testsCount = 0;
    int passedTests = 0;

    void printTestResult(const std::string& testName, bool result) {
        testsCount++;
        if (result) passedTests++;
        std::cout << std::setw(30) << std::left << testName << ": " 
                  << (result ? "ПРОЙДЕН" : "ПРОВАЛЕН") << std::endl;
    }

public:
    void runAllTests() {
        testConstructors();
        testBasicOperations();
        testCompoundOperators();
        testDoubleOperations();
        testComparisonOperations();
        testIOOperations();
        
        printSummary();
    }

    void testConstructors() {
        Complex z1;
        Complex z2(5.0);
        Complex z3(3.0, 4.0);
        Complex z4(z3);

        printTestResult("Конструктор по умолчанию", 
            std::abs(z1.getRe()) < Complex::EPSILON && 
            std::abs(z1.getIm()) < Complex::EPSILON);
        
        printTestResult("Конструктор действ. числа", 
            std::abs(z2.getRe() - 5.0) < Complex::EPSILON && 
            std::abs(z2.getIm()) < Complex::EPSILON);
        
        printTestResult("Конструктор компл. числа", 
            std::abs(z3.getRe() - 3.0) < Complex::EPSILON && 
            std::abs(z3.getIm() - 4.0) < Complex::EPSILON);
        
        printTestResult("Конструктор копирования", areEqual(z3, z4));
    }

    void testBasicOperations() {
        Complex z1(3.0, 4.0), z2(1.0, 2.0);

        printTestResult("Сложение", areEqual(z1 + z2, Complex(4.0, 6.0)));
        printTestResult("Вычитание", areEqual(z1 - z2, Complex(2.0, 2.0)));
        printTestResult("Умножение", areEqual(z1 * z2, Complex(-5.0, 10.0)));
        printTestResult("Деление", areEqual(Complex(4.0, 8.0) / Complex(2.0, 0.0), Complex(2.0, 4.0)));
    }

    void testCompoundOperators() {
        Complex z1(3.0, 4.0);
        Complex z2(1.0, 2.0);
        
        Complex temp = z1;
        temp += z2;
        printTestResult("Составное сложение", areEqual(temp, Complex(4.0, 6.0)));

        temp = z1;
        temp -= z2;
        printTestResult("Составное вычитание", areEqual(temp, Complex(2.0, 2.0)));

        temp = z1;
        temp *= z2;
        printTestResult("Составное умножение", areEqual(temp, Complex(-5.0, 10.0)));

        temp = Complex(4.0, 8.0);
        temp /= Complex(2.0, 0.0);
        printTestResult("Составное деление", areEqual(temp, Complex(2.0, 4.0)));
    }

    void testDoubleOperations() {
        Complex z(2.0, 3.0);
        double scalar = 2.0;

        printTestResult("Сложение с действ. числом", areEqual(z + scalar, Complex(4.0, 3.0)));
        printTestResult("Вычитание действ. числа", areEqual(z - scalar, Complex(0.0, 3.0)));
        printTestResult("Умножение на действ. число", areEqual(z * scalar, Complex(4.0, 6.0)));
        printTestResult("Деление на действ. число", areEqual(z / scalar, Complex(1.0, 1.5)));

        Complex temp = z;
        temp += scalar;
        printTestResult("Составное сложение с действ.", areEqual(temp, Complex(4.0, 3.0)));
    }

    void testComparisonOperations() {
        Complex z1(4.0, 2.0);
        Complex z2(4.0, 2.0);
        Complex z3(4.1, 2.1);

        printTestResult("Проверка равенства", z1 == z2);
        printTestResult("Проверка неравенства", z1 != z3);
    }

    void testIOOperations() {
        Complex z(3.5, 4.5);
        std::ostringstream out;
        out << z;
        
        Complex z_in;
        std::istringstream in(out.str());
        in >> z_in;

        printTestResult("Ввод/Вывод", areEqual(z, z_in));
        printTestResult("Формат вывода", out.str() == "{3.5,4.5}");
    }

    void printSummary() {
        std::cout << "\nРезультаты тестирования:\n";
        std::cout << "Всего тестов: " << testsCount << "\n";
        std::cout << "Пройдено: " << passedTests << "\n";
        std::cout << "Провалено: " << (testsCount - passedTests) << "\n";
        std::cout << "Успешность: " << (passedTests * 100.0 / testsCount) << "%\n";
    }
};

int main() {
    try {
        ComplexTester tester;
        tester.runAllTests();
    } catch (const std::exception& e) {
        std::cerr << "Произошла ошибка: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Произошла неизвестная ошибка" << std::endl;
        return 1;
    }
    return 0;
}