#include <iostream>
#include<sstream>
#include<string.h>
using namespace std;
class Expression {
 public:
  Expression() {}
  virtual string toString() const = 0;
  virtual ~Expression() {}
};
class NumExpression : public Expression {
 private:
  long number;

 public:
  NumExpression(long n) : number(n) {}
  string toString() const {
    stringstream result;
    result << number;
    return result.str();
  }
  ~NumExpression() {}
};
class PlusExpression : public Expression {
 private:
  Expression * lhs;
  Expression * rhs;

 public:
  PlusExpression(Expression * l, Expression * r):lhs(l), rhs(r) {
  }
  string toString() const {
    stringstream result;
    result << "(" << lhs->toString() << " + " << rhs->toString() << ")";
    return result.str();
  }
  ~PlusExpression() {
    delete lhs;
    delete rhs;
  }
};
