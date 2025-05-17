/** @file RigidBody.h
  * @brief Contains declarations of Vector2D and RigidBody.
  * @author Bácsi Miklós
  * @date 2025-05-18
*/

#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <iostream>

/**
 * @brief Vector2D encapsulates x and y coordinates, and is used for mathematical calculations.
 */
struct Vector2D {
    float x;    ///< x coordinate
    float y;    ///< y coordinate

    /****************************************************************************************/

    /**
     * @brief Creates a new Vector2D with given (or default) values.
     * @param x x coordinate
     * @param y y coordinate
     */
    Vector2D(float x = 0.0f, float y = 0.0f);

    /**
     * @brief Destructor.
     */
    ~Vector2D();

    /****************************************************************************************/
    
    /**
     * @param rhs The other vector that we are comparing to.
     * @return Returns whether all the coordinates match.
     */
    bool operator==(const Vector2D& rhs) const;

    /**
     * @param rhs The other vector that we are comparing to.
     * @return Return whether the coordinates do not match.
     */
    bool operator!=(const Vector2D& rhs) const;

    /****************************************************************************************/
    
    /**
     * @brief Operator+ overload between two Vector2D(s).
     * @param rhs The other operand on the right-hand side.
     * @return Returns a new Vector2D with the value of lhs + rhs.
     */
    Vector2D operator+(const Vector2D& rhs) const;

    /**
     * @brief Operator- overload between two Vector2D(s).
     * @param rhs The other operand on the right-hand side.
     * @return Returns a new Vector2D with the value of lhs - rhs.
     */
    Vector2D operator-(const Vector2D& rhs) const;

    /**
     * @brief Operator* overload between two Vector2D(s).
     * @param rhs The other operand on the right-hand side.
     * @return Returns a new Vector2D with the value of lhs * rhs.
     */
    Vector2D operator*(const Vector2D& rhs) const;

    /**
     * @brief Operator/ overload between two Vector2D(s).
     * @param rhs The other operand on the right-hand side.
     * @return Returns a new Vector2D with the value of lhs / rhs.
     */
    Vector2D operator/(const Vector2D& rhs) const;

    /****************************************************************************************/
    
    /**
     * @brief Operator+= overload between two Vector2D(s).
     * @param rhs The other operand on the right-hand side.
     * @return Returns a reference of this changed vector with the value of lhs + rhs.
     */
    Vector2D& operator+=(const Vector2D& rhs);

    /**
     * @brief Operator-= overload between two Vector2D(s).
     * @param rhs The other operand on the right-hand side.
     * @return Returns a reference of this changed vector with the value of lhs - rhs.
     */
    Vector2D& operator-=(const Vector2D& rhs);

    /**
     * @brief Operator*= overload between two Vector2D(s).
     * @param rhs The other operand on the right-hand side.
     * @return Returns a reference of this changed vector with the value of lhs * rhs.
     */
    Vector2D& operator*=(const Vector2D& rhs);

    /**
     * @brief Operator/= overload between two Vector2D(s).
     * @param rhs The other operand on the right-hand side.
     * @return Returns a reference of this changed vector with the value of lhs / rhs.
     */
    Vector2D& operator/=(const Vector2D& rhs);

    /****************************************************************************************/

    /**
     * @brief Operator+ overload between a Vector2D and a float.
     * @param rhs The other operand on the right-hand side.
     * @return Returns a new Vector2D with the value of lhs + rhs.
     */
    Vector2D operator+(float rhs) const;

    /**
     * @brief Operator- overload between a Vector2D and a float.
     * @param rhs The other operand on the right-hand side.
     * @return Returns a new Vector2D with the value of lhs - rhs.
     */
    Vector2D operator-(float rhs) const;

    /**
     * @brief Operator* overload between a Vector2D and a float.
     * @param rhs The other operand on the right-hand side.
     * @return Returns a new Vector2D with the value of lhs * rhs.
     */
    Vector2D operator*(float rhs) const;

    /**
     * @brief Operator/ overload between a Vector2D and a float.
     * @param rhs The other operand on the right-hand side.
     * @return Returns a new Vector2D with the value of lhs / rhs.
     */
    Vector2D operator/(float rhs) const;

    /****************************************************************************************/

    /**
     * @brief Operator+= overload between a Vector2D and a float.
     * @param rhs The other operand on the right-hand side.
     * @return Returns a reference of this changed vector with the value of lhs + rhs.
     */
    Vector2D& operator+=(float rhs);

    /**
     * @brief Operator-= overload between a Vector2D and a float.
     * @param rhs The other operand on the right-hand side.
     * @return Returns a reference of this changed vector with the value of lhs - rhs.
     */
    Vector2D& operator-=(float rhs);
    
    /**
     * @brief Operator*= overload between a Vector2D and a float.
     * @param rhs The other operand on the right-hand side.
     * @return Returns a reference of this changed vector with the value of lhs * rhs.
     */
    Vector2D& operator*=(float rhs);
    
    /**
     * @brief Operator/= overload between a Vector2D and a float.
     * @param rhs The other operand on the right-hand side.
     * @return Returns a reference of this changed vector with the value of lhs / rhs.
     */
    Vector2D& operator/=(float rhs);

    /****************************************************************************************/

    /**
     * @brief Operator+ overload between a float and a Vector2D.
     * @param lhs The operand on the left-hand side.
     * @param rhs The operand on the right-hand side.
     * @return Returns a new Vector2D with the value of lhs + rhs.
     */
    friend Vector2D operator+(float lhs, const Vector2D& rhs);

    /**
     * @brief Operator- overload between a float and a Vector2D.
     * @param lhs The operand on the left-hand side.
     * @param rhs The operand on the right-hand side.
     * @return Returns a new Vector2D with the value of lhs - rhs.
     */
    friend Vector2D operator-(float lhs, const Vector2D& rhs);

    /**
     * @brief Operator* overload between a float and a Vector2D.
     * @param lhs The operand on the left-hand side.
     * @param rhs The operand on the right-hand side.
     * @return Returns a new Vector2D with the value of lhs * rhs.
     */
    friend Vector2D operator*(float lhs, const Vector2D& rhs);

    /**
     * @brief Operator/ overload between a float and a Vector2D.
     * @param lhs The operand on the left-hand side.
     * @param rhs The operand on the right-hand side.
     * @return Returns a new Vector2D with the value of lhs / rhs.
     */
    friend Vector2D operator/(float lhs, const Vector2D& rhs);

    /****************************************************************************************/

    /**
     * @brief Operator<< overload. Prints vector's coordinates to the given output stream.
     * @param os Output stream reference that the vector's coordinates are printed to.
     * @param vec Its coordinates are printed in the output stream.
     * @return Returns the given output stream as reference.
     */
    friend std::ostream& operator<<(std::ostream& os, const Vector2D& vec);

    /**
     * @brief Logs the Vector2D's coordinates to stream std::clog.
     */
    void Log() const;
};

/********************************************************************************************************************************************/

/**
 * @brief Models an object on which we can apply basic physics operations.
 */
class RigidBody {
    friend class GameEngine;
public:
    static float gravity;   ///< Static member: universal gravity.
    static float scale;     ///< Static member: universal scale.

private:
    float mass; ///< mass of the object.
    Vector2D force;     ///< Resultant force (without gravity).
    Vector2D position;  ///< Position coordinate of the object.
    Vector2D velocity;  ///< Velocity of the object.
    Vector2D acceleration;  ///< Acceleration of the object.

public:
    /**
     * @brief Creates a new rigid body with given (or default) mass.
     * @param mass Mass of the object.
     */
    RigidBody(float mass = 1.0f);

    /**
     * @brief Updates the rigid body by delta time.
     * @param dt Delta time.
     */
    void Update(float dt);

    /**
     * @return Returns the position of the object.
     */
    const Vector2D& GetPosition() const;

    /**
     * @brief Applys the given force to the rigid body (overriding previous one).
     * @param F Force that is applied to the object.
     */
    void ApplyForce(Vector2D F);

    /**
     * @brief Applys the given force in the x direction to the rigid body (overriding previous one).
     * @param Fx Force that is applied to the object.
     */
    void ApplyForceX(float Fx);

    /**
     * @brief Applys the given force in the y direction to the rigid body (overriding previous one).
     * @param Fy Force that is applied to the object.
     */
    void ApplyForceY(float Fy);

    /**
     * @brief Applys the given velocity in the x direction to the rigid body (overriding previous one).
     * @param vx Velocity that is applied to the object.
     */
    void ApplyVelocityX(float vx);

    /**
     * @brief Applys the given velocity in the y direction to the rigid body (overriding previous one).
     * @param vy Velocity that is applied to the object.
     */
    void ApplyVelocityY(float vy);

    /**
     * @brief Resets the rigid body to the default state.
     */
    void Reset();

    /**
     * @return Returns reference to the mass of the object.
     */
    float& Mass();

    /**
     * @return Returns reference to the force applied to the object.
     */
    Vector2D& Force();

    /**
     * @return Returns reference to the position of the object.
     */
    Vector2D& Position();

    /**
     * @return Returns reference to the velocity of the object.
     */
    Vector2D& Velocity();

    /**
     * @return Returns reference to the acceleration of the object.
     */
    Vector2D& Acceleration();

    /**
     * @brief Destructor.
     */
    ~RigidBody();
};

#endif // RIGID_BODY_H