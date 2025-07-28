# 51115142

作業二

## 解題說明

本作業要求實作一個 Polynomial 類別，根據提供的抽象資料型別（ADT）與私有資料成員（圖 1 和圖 2），並實作 >> 和 << 運算子來處理多項式的輸入與輸出。輸入格式為係數與指數對，輸出需符合數學多項式表示法（例如，3x^2 + 2x - 1），並按指數遞減排序。

### 解題策略

1. 類別設計：
    Term 類別：封裝單一多項式的項，包含係數 (coef) 和指數 (exp)，提供公開的 getter 方法。
    Polynomial 類別：使用動態陣列 (termArray) 儲存 Term 物件，管理項數 (terms) 和容量 (capacity)，並支援動態擴展。
   
3. 輸入處理：
    使用 getline 讀取整行輸入，透過 istringstream 解析係數與指數對，支援靈活的輸入格式。
   
4. 輸出處理：
    將項依指數遞減排序，使用 std::sort 實現。
    處理特殊情況：忽略零係數項、係數 ±1 時省略數值（非常數項）、指數為 0 不顯示 x、指數為 1 省略 ^1。
   
5. 動態記憶體管理：
    當 termArray 容量不足時，自動倍增容量並重新分配記憶體，確保靈活性與效率。

## 程式實作

以下為主要程式碼：

```cpp
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
    float getCoef() const { return coef; }
    int getExp() const { return exp; }
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
        float c = poly.termArray[i].getCoef();
        int e = poly.termArray[i].getExp();
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

```

## 效能分析

1. 時間複雜度：
    輸入：逐項讀取並儲存，O(n)，其中 n 為項數。
    輸出：排序項使用 std::sort，為 O(n log n)；遍歷輸出為 O(n)。總體為 O(n log n)。
    記憶體管理：擴展容量時，複製陣列為 O(n)。
2. 空間複雜度：
    動態陣列 termArray 使用 O(n) 空間。
    排序時使用臨時 vector，額外 O(n) 空間。
    總空間複雜度為 O(n)。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $n = 0$      | 0        | 0        |
| 測試二   | $n = 1$      | 1        | 1        |
| 測試三   | $n = 3$      | 6        | 6        |
| 測試四   | $n = 5$      | 15       | 15       |
| 測試五   | $n = -1$     | 異常拋出 | 異常拋出 |

### 結論

1. 程式能正確計算 $n$ 到 $1$ 的連加總和。  
2. 在 $n < 0$ 的情況下，程式會成功拋出異常，符合設計預期。  
3. 測試案例涵蓋了多種邊界情況（$n = 0$、$n = 1$、$n > 1$、$n < 0$），驗證程式的正確性。

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. **程式邏輯簡單直觀**  
   遞迴的寫法能夠清楚表達「將問題拆解為更小的子問題」的核心概念。  
   例如，計算 $\Sigma(n)$ 的過程可分解為：  

   $$
   \Sigma(n) = n + \Sigma(n-1)
   $$

   當 $n$ 等於 1 或 0 時，直接返回結果，結束遞迴。

2. **易於理解與實現**  
   遞迴的程式碼更接近數學公式的表示方式，特別適合新手學習遞迴的基本概念。  
   以本程式為例：  

   ```cpp
   int sigma(int n) {
       if (n < 0)
           throw "n < 0";
       else if (n <= 1)
           return n;
       return n + sigma(n - 1);
   }
   ```

3. **遞迴的語意清楚**  
   在程式中，每次遞迴呼叫都代表一個「子問題的解」，而最終遞迴的返回結果會逐層相加，完成整體問題的求解。  
   這種設計簡化了邏輯，不需要額外變數來維護中間狀態。

透過遞迴實作 Sigma 計算，程式邏輯簡單且易於理解，特別適合展示遞迴的核心思想。然而，遞迴會因堆疊深度受到限制，當 $n$ 值過大時，應考慮使用迭代版本來避免 Stack Overflow 問題。
