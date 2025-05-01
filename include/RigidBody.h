#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <iostream>

struct Vector2D {
    float x;
    float y;

    Vector2D(float x = 0.0f, float y = 0.0f);
    ~Vector2D();
    
    bool operator==(const Vector2D& rhs) const;
    bool operator!=(const Vector2D& rhs) const;
    
    Vector2D operator+(const Vector2D& rhs) const;
    Vector2D operator-(const Vector2D& rhs) const;
    Vector2D operator*(const Vector2D& rhs) const;
    Vector2D operator/(const Vector2D& rhs) const;
    
    Vector2D& operator+=(const Vector2D& rhs);
    Vector2D& operator-=(const Vector2D& rhs);
    Vector2D& operator*=(const Vector2D& rhs);
    Vector2D& operator/=(const Vector2D& rhs);

    Vector2D operator+(float rhs) const;
    Vector2D operator-(float rhs) const;
    Vector2D operator*(float rhs) const;
    Vector2D operator/(float rhs) const;

    Vector2D& operator+=(float rhs);
    Vector2D& operator-=(float rhs);
    Vector2D& operator*=(float rhs);
    Vector2D& operator/=(float rhs);

    friend Vector2D operator+(float lhs, const Vector2D& rhs);
    friend Vector2D operator-(float lhs, const Vector2D& rhs);
    friend Vector2D operator*(float lhs, const Vector2D& rhs);
    friend Vector2D operator/(float lhs, const Vector2D& rhs);

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& vec);
    void log() const;
};

class RigidBody {
public:
    static float gravity;
private:
    float mass;
    Vector2D force;
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;

public:
    RigidBody(float mass = 1.0f);
    void Update(float dt);
    Vector2D GetPosition() const;
    void ApplyForce(Vector2D F);
    void ApplyForceX(float Fx);
    void ApplyForceY(float Fy);
    void RemoveForces();
    ~RigidBody();
};

#endif // RIGID_BODY_H