#include <iostream>
#include<sstream>
#include<string.h>
using namespace std;
class Expression {
 public:
  Expression() {}
  virtual string toString() const = 0;
  virtual long evaluate() const = 0;
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
  long evaluate() const{
    return number;
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
  long evaluate() const{
    return lhs->evaluate()+rhs->evaluate();
  }
  ~PlusExpression() {
    delete lhs;
    delete rhs;
  }
};

class MinusExpression:public Expression{
private:
    Expression* lhs;
    Expression* rhs;
public:
    MinusExpression(Expression* l, Expression* r): lhs(l),rhs(r){}

    string toString() const {
        stringstream result;
        result << "(" << lhs->toString() << " - " << rhs->toString() << ")";
        return result.str();
    }
    long evaluate() const{
        return lhs->evaluate()-rhs->evaluate();
    }
    ~MinusExpression(){
        delete lhs;
        delete rhs;
    }
};
class TimesExpression:public Expression{
private:
    Expression* lhs;
    Expression* rhs;
public:
    TimesExpression(Expression* l, Expression* r): lhs(l),rhs(r){}

    string toString() const {
        stringstream result;
        result << "(" << lhs->toString() << " * " << rhs->toString() << ")";
        return result.str();
    }
    long evaluate() const{
        return lhs->evaluate()*rhs->evaluate();
    }
    ~TimesExpression(){
        delete lhs;
        delete rhs;
    }
};

class DivExpression:public Expression{
private:
    Expression* lhs;
    Expression* rhs;
public:
    DivExpression(Expression* l, Expression* r): lhs(l),rhs(r){}

    string toString() const {
        stringstream result;
        result << "(" << lhs->toString() << " / " << rhs->toString() << ")";
        return result.str();
    
    }
    long evaluate() const{
        return lhs->evaluate()/rhs->evaluate();
    }
    ~DivExpression(){
        delete lhs;
        delete rhs;
    }
};
