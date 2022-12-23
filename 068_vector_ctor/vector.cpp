#include "vector.hpp"

#include <cmath>
#include <cstdio>

void Vector2D::initVector(double x, double y){
    this->x = x;
    this->y = y;
    return;
}
double Vector2D::getMagnitude() const{
    return    std::sqrt(this->x * this->x + this->y*this->y);\
}

Vector2D Vector2D::operator+(const Vector2D & rhs) const{
    Vector2D new_vector;
    new_vector.initVector(this->x+rhs.x, this->y+rhs.y);
    return new_vector;
}

Vector2D & Vector2D::operator+=(const Vector2D & rhs){
    x +=rhs.x;
    y += rhs.y;
    return *this;
}
double Vector2D::dot(const Vector2D & rhs) const{
    return x*rhs.x+y*rhs.y;
}
void Vector2D::print() const{
    printf("<%.2f,%.2f>",x,y);
    return ;
}


