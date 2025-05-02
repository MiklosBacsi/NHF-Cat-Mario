#include "RigidBody.h"

#include <iostream>

float RigidBody::gravity = 2.0f;
float RigidBody::scale = 0.001f;

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

Vector2D& Vector2D::operator+=(float rhs) {
    x += rhs;
    y += rhs;
    return *this;
}

Vector2D& Vector2D::operator-=(float rhs) {
    x -= rhs;
    y -= rhs;
    return *this;
}

Vector2D& Vector2D::operator*=(float rhs) {
    x *= rhs;
    y *= rhs;
    return *this;
}

Vector2D& Vector2D::operator/=(float rhs) {
    x /= rhs;
    y /= rhs;
    return *this;
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
    return Vector2D(lhs / rhs.x, lhs / rhs.y);
}

std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
    return os << '(' << vec.x << ',' << vec.y << ')';
}

void Vector2D::Log() const {
    std::clog << *this << std::endl;
}
/* ************************************************************************************ */

/***** Class RigidBody *****/
RigidBody::RigidBody(float mass) : mass(mass) {}

void RigidBody::Update(float dt) {
    acceleration.x = force.x / mass;
    acceleration.y = gravity + force.y / mass;

    acceleration *= scale;

    velocity += acceleration * dt;

    const float crop = 1.5f;
    if (velocity.x > crop) velocity.x = crop;
    if (velocity.x < -crop) velocity.x = -crop;
    if (velocity.y > crop) velocity.y = crop;
    if (velocity.y < -crop) velocity.y = -crop;
    
    position = velocity * dt;
    std::clog << "*****\n" << "dt: " << dt << "\nForce: " << force << "\nAcceleration: " <<
    acceleration << "\nVelocity: " << velocity << "\nPosition: " << position << std::endl;
}

const Vector2D& RigidBody::GetPosition() const { return position; }

void RigidBody::ApplyForce(Vector2D F) { force = F;}

void RigidBody::ApplyForceX(float Fx) { force.x = Fx; }

void RigidBody::ApplyForceY(float Fy) { force.y = Fy; }

void RigidBody::ApplyVelocityX(float vx) { velocity.x = vx; }

void RigidBody::ApplyVelocityY(float vy) { velocity.y = vy; }

void RigidBody::Reset() {
    force = Vector2D(0,0);
    acceleration = Vector2D(0,0);
    velocity = Vector2D(0,0);
    position = Vector2D(0,0);
}

float& RigidBody::Mass() { return mass; }

Vector2D& RigidBody::Force() { return force; };

Vector2D& RigidBody::Position() { return position; };

Vector2D& RigidBody::Velocity() { return velocity; };

Vector2D& RigidBody::Acceleratoin() { return acceleration; };

RigidBody::~RigidBody() {
    #ifdef DTOR
    std::clog << "~RigidBody Dtor" << std::endl;
    #endif
}