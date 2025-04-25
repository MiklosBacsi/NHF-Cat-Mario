#include "RigidBody.h"

#include <iostream>

/* ************************************************************************************ */

/***** Class Vector2D *****/
Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

Vector2D::~Vector2D() {
    #ifdef DTOR
    std::clog << "~Vector2D Dtor" << std::endl;
    #endif 
}

bool Vector2D::operator==(const Vector2D& rhs) const {
    if (x == rhs.x && y == rhs.y && y == rhs.y)
        return true;
    return false;
}

bool Vector2D::operator!=(const Vector2D& rhs) const {
    return !(*this == rhs);
}

Vector2D Vector2D::operator+(const Vector2D& rhs) const {
    return Vector2D(x + rhs.x, y + rhs.y);
}

Vector2D Vector2D::operator-(const Vector2D& rhs) const {
    return Vector2D(x - rhs.x, y - rhs.y);
}

Vector2D Vector2D::operator*(const Vector2D& rhs) const {
    return Vector2D(x * rhs.x, y * rhs.y);
}

Vector2D Vector2D::operator/(const Vector2D& rhs) const {
    return Vector2D(x / rhs.x, y / rhs.y);
}

Vector2D& Vector2D::operator+=(const Vector2D& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vector2D& Vector2D::operator*=(const Vector2D& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    return *this;
}

Vector2D& Vector2D::operator/=(const Vector2D& rhs) {
    x /= rhs.x;
    y /= rhs.y;
    return *this;
}

Vector2D Vector2D::operator+(float rhs) const {
    return Vector2D(x + rhs, y + rhs);
}

Vector2D Vector2D::operator-(float rhs) const {
    return Vector2D(x - rhs, y - rhs);
}

Vector2D Vector2D::operator*(float rhs) const {
    return Vector2D(x * rhs, y * rhs);
}

Vector2D Vector2D::operator/(float rhs) const {
    return Vector2D(x / rhs, y / rhs);
}

Vector2D operator+(float lhs, const Vector2D& rhs) {
    return Vector2D(rhs.x + lhs, rhs.y + lhs);
}

Vector2D operator-(float lhs, const Vector2D& rhs) {
    return Vector2D(rhs.x - lhs, rhs.y - lhs);
}

Vector2D operator*(float lhs, const Vector2D& rhs) {
    return Vector2D(rhs.x * lhs, rhs.y * lhs);
}

Vector2D operator/(float lhs, const Vector2D& rhs) {
    return Vector2D(rhs.x / lhs, rhs.y / lhs);
}

std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
    return os << '(' << vec.x << ',' << vec.y << ')';
}

void Vector2D::log() const {
    std::clog << *this << std::endl;
}
/* ************************************************************************************ */

/***** Class RigidBody *****/
RigidBody::RigidBody(float mass) : mass(mass) {}

void RigidBody::update(float dt) {
    acceleration.x = force.x / mass;
    acceleration.y = gravity + force.y / mass;
    velocity = acceleration * dt;
    position = velocity * dt;
}

Vector2D RigidBody::getPosition() const { return position; }

void RigidBody::applyForce(Vector2D F) { force = F;}

void RigidBody::applyForceX(float Fx) { force.x = Fx; }

void RigidBody::applyForceY(float Fy) { force.y = Fy; }

void RigidBody::removeForces() { force = Vector2D(0,0); }

RigidBody::~RigidBody() {
    #ifdef DTOR
    std::clog << "~RigidBody Dtor" << std::endl;
    #endif
}