#include <iostream>
#include <sstream>
#include <string>

class Polynomial;

class Term {
    friend class Polynomial;
    friend std::ostream& operator<<(std::ostream& os, const Term& term);
    friend std::istream& operator>>(std::istream& is, Term& term);
private:
    float coef;
    int exp;
public:
    Term(float c = 0.0, int e = 0) : coef(c), exp(e) {}
};

class Polynomial {
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly);
    friend std::istream& operator>>(std::istream& is, Polynomial& poly);
private:
    Term* termArray;
    int capacity;
    int terms;

    void expandCapacity() {
        capacity *= 2;
        Term* newArray = new Term[capacity];
        for (int i = 0; i < terms; ++i)
            newArray[i] = termArray[i];
        delete[] termArray;
        termArray = newArray;
    }

    void sortTerms() {
        for (int i = 0; i < terms - 1; ++i)
            for (int j = i + 1; j < terms; ++j)
                if (termArray[i].exp < termArray[j].exp) {
                    Term temp = termArray[i];
                    termArray[i] = termArray[j];
                    termArray[j] = temp;
                }
    }

public:
    Polynomial(int initialCapacity = 4) : capacity(initialCapacity), terms(0) {
        termArray = new Term[capacity];
    }

    ~Polynomial() {
        delete[] termArray;
    }

    void addTerm(float c, int e) {
        if (terms >= capacity) expandCapacity();
        termArray[terms++] = Term(c, e);
    }
};

std::istream& operator>>(std::istream& is, Term& term) {
    return is >> term.coef >> term.exp;
}

std::ostream& operator<<(std::ostream& os, const Term& term) {
    os << term.coef << "x^" << term.exp;
    return os;
}

std::istream& operator>>(std::istream& is, Polynomial& poly) {
    poly.terms = 0;
    std::string line;
    std::getline(is, line);
    std::istringstream iss(line);
    float coef; int exp;
    while (iss >> coef >> exp)
        poly.addTerm(coef, exp);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& poly) {
    if (poly.terms == 0) {
        os << "0";
        return os;
    }

    ((Polynomial&)poly).sortTerms();

    bool first = true;
    for (int i = 0; i < poly.terms; ++i) {
        float c = poly.termArray[i].coef;
        int e = poly.termArray[i].exp;
        if (c == 0) continue;

        if (!first) os << (c > 0 ? " + " : " - ");
        else if (c < 0) os << "-";

        float absC = c < 0 ? -c : c;

        if (absC != 1 || e == 0) os << absC;

        if (e > 0) {
            os << "x";
            if (e > 1) os << "^" << e;
        }

        first = false;
    }
    if (first) os << "0";
    return os;
}

int main() {
    Polynomial p;
    std::cout << "Enter polynomial (coef exp pairs): ";
    std::cin >> p;
    std::cout << "Polynomial: " << p << std::endl;
    return 0;
}
