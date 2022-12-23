#include <cstdlib>
#include <iostream>
#include <exception>
class special_exception : public std::exception {
public:
    virtual const char * what() { return "special exception\n"; }
};
class X {
public:
    int x;
    X(int in) : x(in) { std::cout << "X(" << x << ")\n"; }
    ~X() { std::cout << "~X(" << x << ")\n"; }
};
void f(X myX) {
    if (myX.x > 1) {
        throw special_exception();
    }
    else {
        std::cout << "X: " << myX.x << "\n";
    }
}
int main(void) {
    try {
        X ans(0);
        for (int i = 0; i < 4; i++) {
            ans.x++;
            f(ans);
        }
    }
    catch (special_exception & e) {
        std::cout << e.what();
    }
    catch (std::exception & e) {
        std::cout << "standard exception\n";
    }
    return EXIT_SUCCESS;
}

