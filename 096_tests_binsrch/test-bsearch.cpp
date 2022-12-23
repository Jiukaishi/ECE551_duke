#include<cmath>
#include <iostream>

#include "function.h"

    int binarySearchForZero(Function<int, int> * f, int low, int high);

class CountedIntFn : public Function<int, int> {
 protected:
  unsigned remaining;
  Function<int, int> * f;
  const char * mesg;

 public:
  CountedIntFn(unsigned n, Function<int, int> * fn, const char * m) :
      remaining(n), f(fn), mesg(m) {}
  virtual int invoke(int arg) {
    if (remaining == 0) {
      fprintf(stderr, "Too many function invocations in %s\n", mesg);
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }
};
class SinFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 10000000 * (sin(arg / 100000.0) - 0.5); }
};
class NegFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return -10; }
};
class PosFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 3; }
};

class MonoIncrsFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 2 * arg + 2; }
};

class MonoDcrsFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return -arg; };
};
void check(Function<int, int> * f,
           int low,
           int high,
           int expected_ans,
           const char * mesg) {
  int num;

  if (high > low) {
    num = log(high - low) / log(2) + 1;
  }
  else {
    num = 1;
  }
  CountedIntFn * function = new CountedIntFn(num, f, mesg);
  int res = binarySearchForZero(function, low, high);
  if (res != expected_ans) {
    fprintf(stderr, "The result of funtion %s is wrong!\n", mesg);
    exit(EXIT_FAILURE);
  }
}
int main() {
  SinFunction sf;
  NegFunction nf;
  PosFunction pf;
  MonoIncrsFunction mif;
  MonoDcrsFunction mdf;
  check(&sf, 0, 150000, 52359, "Sin");
  check(&nf, -100, 100, 99, "Neg in range(-100,100)");
  check(&nf, 10, 1000, 999, "Neg in range(10,100)");
  check(&nf, -100, -10, -11, "Neg in range(-100,-10)");
  check(&nf, 0, 0, 0, "Neg in (0,0)");

  check(&pf, -100, 100, -100, "Pos in range(-100,100)");
  check(&pf, 10, 1000, 10, "Pos in range(10,100)");
  check(&pf, -100, -10, -100, "Pos in range(-100,-10)");
  check(&pf, 0, 0, 0, "Pos in (0,0)");

  check(&mif, -100, 100, -1, "MIF in range(-100,100)");
  check(&mif, 10, 1000, 10, "MIF in range(10,100)");
  check(&mif, -100, -10, -11, "MIF in range(-100,-10)");

  check(&mif, 0, 0, 0, "MIF in (0,0)");

  check(&mdf, -100, 100, 0, "MDF in range(-100,100)");
  check(&mdf, 10, 1000, 999, "MDF in range(10,100)");
  check(&mdf, -100, -10, -100, "MDF in range(-100,-10)");
  check(&mdf, 0, 0, 0, "MDF in (0,0)");
}
