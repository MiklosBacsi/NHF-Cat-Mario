# include <iostream>

#include "gtest_lite.h"
#include "RigidBody.h"

void  TEST_RigidBody() {    
    /***** Checking struct Vector2D *****/

    // Checks for throwing error for non-existing file
    TEST(Vector2D, Constructor) {
        EXPECT_NO_THROW(Vector2D());
        EXPECT_NO_THROW(Vector2D(-40, 80));
        
        Vector2D vec1;
        EXPECT_FLOAT_EQ(0.0f, vec1.x);
        EXPECT_FLOAT_EQ(0.0f, vec1.y);
        
        Vector2D vec2(5.8f, -4.2f);
        EXPECT_FLOAT_EQ(5.8f, vec2.x);
        EXPECT_FLOAT_EQ(-4.2f, vec2.y);
    } END

    TEST(Vector2D, oparator==) {
        Vector2D vec1, vec2;
        EXPECT_NO_THROW(vec1 == vec2);
        EXPECT_TRUE(vec1 == vec2);
        vec1.x = 10.0f;
        EXPECT_FALSE(vec1 == vec2);
        EXPECT_TRUE(Vector2D() == Vector2D());
        EXPECT_TRUE(Vector2D(-10.3f, 4.7f) == Vector2D(-10.3f, 4.7f));
        EXPECT_FALSE(Vector2D(4.5f, 2.3f) == Vector2D(4.5f, 1.0f));
        EXPECT_FALSE(Vector2D(3.2f, 5.5f) == Vector2D(2.4f, 5.5f));
    } END

    TEST(Vector2D, operator!=) {
        Vector2D vec1, vec2;
        EXPECT_NO_THROW(vec1 != vec2);
        EXPECT_FALSE(vec1 != vec2);
        vec1.x = 10.0f;
        EXPECT_TRUE(vec1 != vec2);
        EXPECT_FALSE(Vector2D() != Vector2D());
        EXPECT_FALSE(Vector2D(-10.3f, 4.7f) != Vector2D(-10.3f, 4.7f));
        EXPECT_TRUE(Vector2D(4.5f, 2.3f) != Vector2D(4.5f, 1.0f));
        EXPECT_TRUE(Vector2D(3.2f, 5.5f) != Vector2D(2.4f, 5.5f));
    } END

    TEST(Vector2D, 'Vector2D + Vector2D') {
        Vector2D vec1(5.0f, 15.0f), vec2(5.0f, 12.5f);
        EXPECT_FLOAT_EQ(5.0f, vec1.x);
        EXPECT_FLOAT_EQ(vec1.x, vec2.x);
        Vector2D vec3 = vec1 + vec2;
        EXPECT_FLOAT_EQ(10.0f, vec3.x);
        EXPECT_FLOAT_EQ(5.0f, vec1.x);
        EXPECT_FLOAT_EQ(27.5f, vec3.y);
        EXPECT_FLOAT_EQ(15.0f, vec1.y);
    } END

    TEST(Vector2D, 'Vector2D - Vector2D') {
        Vector2D vec1(5.0f, 15.0f), vec2(5.0f, 12.5f);
        EXPECT_FLOAT_EQ(5.0f, vec1.x);
        EXPECT_FLOAT_EQ(vec1.x, vec2.x);
        Vector2D vec3 = vec1 - vec2;
        EXPECT_FLOAT_EQ(0.0f, vec3.x);
        EXPECT_FLOAT_EQ(5.0f, vec1.x);
        EXPECT_FLOAT_EQ(2.5f, vec3.y);
        EXPECT_FLOAT_EQ(15.0f, vec1.y);
    } END

    TEST(Vector2D, 'Vector2D * Vector2D') {
        Vector2D vec1(2.0f, 3.0f), vec2(10.0f, -5.0f);
        Vector2D vec3 = vec1 * vec2;
        
        EXPECT_FLOAT_EQ(20.0f, vec3.x);
        EXPECT_FLOAT_EQ(-15.0f, vec3.y);

        EXPECT_FLOAT_EQ(2.0f, vec1.x);
        EXPECT_FLOAT_EQ(3.0f, vec1.y);
    } END

    TEST(Vector2D, 'Vector2D / Vector2D') {
        Vector2D vec1(2.0f, 3.0f), vec2(10.0f, -5.0f);
        Vector2D vec3 = vec1 / vec2;
        
        EXPECT_FLOAT_EQ(0.2f, vec3.x);
        EXPECT_FLOAT_EQ(-0.6f, vec3.y);

        EXPECT_FLOAT_EQ(2.0f, vec1.x);
        EXPECT_FLOAT_EQ(3.0f, vec1.y);
    } END

    TEST(Vector2D, 'Vector2D += Vector2D') {
        Vector2D vec1(2.0f, 3.0f), vec2(10.0f, -5.0f);
        vec1 += vec2;
        
        EXPECT_FLOAT_EQ(12.0f, vec1.x);
        EXPECT_FLOAT_EQ(-2.0f, vec1.y);

        EXPECT_FLOAT_EQ(10.0f, vec2.x);
        EXPECT_FLOAT_EQ(-5.0f, vec2.y);
    } END

    TEST(Vector2D, 'Vector2D -= Vector2D') {
        Vector2D vec1(2.0f, 3.0f), vec2(10.0f, -5.0f);
        vec1 -= vec2;
        
        EXPECT_FLOAT_EQ(-8.0f, vec1.x);
        EXPECT_FLOAT_EQ(8.0f, vec1.y);

        EXPECT_FLOAT_EQ(10.0f, vec2.x);
        EXPECT_FLOAT_EQ(-5.0f, vec2.y);
    } END

    TEST(Vector2D, 'Vector2D *= Vector2D') {
        Vector2D vec1(2.0f, 3.0f), vec2(10.0f, -5.0f);
        vec1 *= vec2;
        
        EXPECT_FLOAT_EQ(20.0f, vec1.x);
        EXPECT_FLOAT_EQ(-15.0f, vec1.y);

        EXPECT_FLOAT_EQ(10.0f, vec2.x);
        EXPECT_FLOAT_EQ(-5.0f, vec2.y);
    } END

    TEST(Vector2D, 'Vector2D /= Vector2D') {
        Vector2D vec1(2.0f, 3.0f), vec2(10.0f, -5.0f);
        vec1 /= vec2;
        
        EXPECT_FLOAT_EQ(0.2f, vec1.x);
        EXPECT_FLOAT_EQ(-0.6f, vec1.y);

        EXPECT_FLOAT_EQ(10.0f, vec2.x);
        EXPECT_FLOAT_EQ(-5.0f, vec2.y);
    } END

    TEST(Vector2D, 'Vector2D + float') {
        Vector2D vec1(2.0f, 3.0f);
        float f = 10.0f;
        Vector2D vec2 = vec1 + f;

        EXPECT_FLOAT_EQ(12.0f, vec2.x);
        EXPECT_FLOAT_EQ(13.0f, vec2.y);

        EXPECT_FLOAT_EQ(2.0f, vec1.x);
        EXPECT_FLOAT_EQ(3.0f, vec1.y);
    } END

    TEST(Vector2D, 'Vector2D - float') {
        Vector2D vec1(2.0f, 3.0f);
        float f = 10.0f;
        Vector2D vec2 = vec1 - f;

        EXPECT_FLOAT_EQ(-8.0f, vec2.x);
        EXPECT_FLOAT_EQ(-7.0f, vec2.y);

        EXPECT_FLOAT_EQ(2.0f, vec1.x);
        EXPECT_FLOAT_EQ(3.0f, vec1.y);
    } END

    TEST(Vector2D, 'Vector2D * float') {
        Vector2D vec1(2.0f, 3.0f);
        float f = 10.0f;
        Vector2D vec2 = vec1 * f;

        EXPECT_FLOAT_EQ(20.0f, vec2.x);
        EXPECT_FLOAT_EQ(30.0f, vec2.y);

        EXPECT_FLOAT_EQ(2.0f, vec1.x);
        EXPECT_FLOAT_EQ(3.0f, vec1.y);
    } END

    TEST(Vector2D, 'Vector2D / float') {
        Vector2D vec1(2.0f, 3.0f);
        float f = 10.0;
        Vector2D vec2 = vec1 / f;
        
        EXPECT_FLOAT_EQ(0.2f, vec2.x);
        EXPECT_FLOAT_EQ(0.3f, vec2.y);

        EXPECT_FLOAT_EQ(2.0f, vec1.x);
        EXPECT_FLOAT_EQ(3.0f, vec1.y);
    } END

    TEST(Vector2D, 'Vector2D += float') {
        Vector2D vec1(2.0f, 3.0f);
        float f = 10.0f;
        vec1 += f;
        
        EXPECT_FLOAT_EQ(12.0f, vec1.x);
        EXPECT_FLOAT_EQ(13.0f, vec1.y);
    } END

    TEST(Vector2D, 'Vector2D -= float') {
        Vector2D vec1(2.0f, 3.0f);
        float f = 10.0f;
        vec1 -= f;
        
        EXPECT_FLOAT_EQ(-8.0f, vec1.x);
        EXPECT_FLOAT_EQ(-7.0f, vec1.y);
    } END

    TEST(Vector2D, 'Vector2D *= float') {
        Vector2D vec1(2.0f, 3.0f);
        float f = 10.0f;
        vec1 *= f;
        
        EXPECT_FLOAT_EQ(20.0f, vec1.x);
        EXPECT_FLOAT_EQ(30.0f, vec1.y);
    } END

    TEST(Vector2D, 'Vector2D /= float') {
        Vector2D vec1(2.0f, 3.0f);
        float f = 10.0f;
        vec1 /= f;
        
        EXPECT_FLOAT_EQ(0.2f, vec1.x);
        EXPECT_FLOAT_EQ(0.3f, vec1.y);
    } END

    TEST(Vector2D, 'float + Vector2D') {
        Vector2D vec1(2.0f, 3.0f);
        float f = 10.0f;
        Vector2D vec2 = f + vec1;

        EXPECT_FLOAT_EQ(12.0f, vec2.x);
        EXPECT_FLOAT_EQ(13.0f, vec2.y);

        EXPECT_FLOAT_EQ(2.0f, vec1.x);
        EXPECT_FLOAT_EQ(3.0f, vec1.y);
    } END

    TEST(Vector2D, 'float - Vector2D') {
        Vector2D vec1(2.0f, 3.0f);
        float f = 10.0f;
        Vector2D vec2 = f - vec1;

        EXPECT_FLOAT_EQ(8.0f, vec2.x);
        EXPECT_FLOAT_EQ(7.0f, vec2.y);

        EXPECT_FLOAT_EQ(2.0f, vec1.x);
        EXPECT_FLOAT_EQ(3.0f, vec1.y);
    } END

    TEST(Vector2D, 'float * Vector2D') {
        Vector2D vec1(2.0f, 3.0f);
        float f = 10.0f;
        Vector2D vec2 = f * vec1;

        EXPECT_FLOAT_EQ(20.0f, vec2.x);
        EXPECT_FLOAT_EQ(30.0f, vec2.y);

        EXPECT_FLOAT_EQ(2.0f, vec1.x);
        EXPECT_FLOAT_EQ(3.0f, vec1.y);
    } END

    TEST(Vector2D, 'float / Vector2D') {
        Vector2D vec1(2.0f, 4.0f);
        float f = 10.0;
        Vector2D vec2 = f / vec1;
        
        EXPECT_FLOAT_EQ(5.0f, vec2.x);
        EXPECT_FLOAT_EQ(2.5f, vec2.y);

        EXPECT_FLOAT_EQ(2.0f, vec1.x);
        EXPECT_FLOAT_EQ(4.0f, vec1.y);
    } END

    /***** Checking class RigidBody *****/
    TEST(RigidBody, 'Constructor & GetPosition()') {
        EXPECT_NO_THROW(RigidBody());
        EXPECT_NO_THROW(RigidBody(2.0f));
        RigidBody body;
        EXPECT_NO_THROW(body.GetPosition());
        EXPECT_FLOAT_EQ(0.0f, body.GetPosition().x);
        EXPECT_FLOAT_EQ(0.0f, body.GetPosition().y);
    } END

    TEST(RigidBody, 'Update') {
        RigidBody body(0.1f);
        EXPECT_NO_THROW(body.Update(0.5f));
        EXPECT_NO_THROW(body.ApplyForce({10,10}));
        EXPECT_NO_THROW(body.RemoveForces());
        body.Update(0.5f);
        EXPECT_FLOAT_EQ(0.0f, body.GetPosition().x);
        EXPECT_FLOAT_EQ(2.5f, body.GetPosition().y);

        body.ApplyForce({-10,-1.0});
        body.Update(0.5f);
        EXPECT_FLOAT_EQ(-25.0f, body.GetPosition().x);
        EXPECT_FLOAT_EQ(0.0f, body.GetPosition().y);
    } END

    TEST(RigidBody, 'Apply & Remove Force') {
        RigidBody body;
        body.ApplyForceY(-10.0f);
        body.Update(0.1f);
        EXPECT_FLOAT_EQ(0.0f, body.GetPosition().x);
        EXPECT_FLOAT_EQ(0.0f, body.GetPosition().y);

        body.ApplyForceX(50.0f);
        body.Update(0.1f);
        EXPECT_FLOAT_EQ(0.5f, body.GetPosition().x);
        EXPECT_FLOAT_EQ(0.0f, body.GetPosition().y);

        body.RemoveForces();
        body.Update(0.5f);
        EXPECT_FLOAT_EQ(0.0f, body.GetPosition().x);
        EXPECT_FLOAT_EQ(2.5f, body.GetPosition().y);

        RigidBody body2(2.0f);
        body2.ApplyForce({-10.0f,5.0f});
        body2.Update(2.0f);
        EXPECT_FLOAT_EQ(-20.0f, body2.GetPosition().x);
        EXPECT_FLOAT_EQ(50.0f, body2.GetPosition().y);

        body2.RemoveForces();
        body2.Update(2.0f);
        EXPECT_FLOAT_EQ(0.0f, body2.GetPosition().x);
        EXPECT_FLOAT_EQ(40.0f, body2.GetPosition().y);
    } END
}