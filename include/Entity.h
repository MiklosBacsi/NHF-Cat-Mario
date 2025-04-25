#ifndef ENTITY_H
#define ENTITY_H

#ifndef CPORTA

#include <iostream>

#include "RigidBody.h"

class Entity {
protected:
    Vector2D transform;
    Vector2D dimension;
    RigidBody rigidBody;
public:
    Entity();
    virtual void update(float dt) = 0;
    virtual ~Entity();
};

class Player : public Entity {
private:
public:
    Player();
    void update(float dt);
    ~Player();
};

class Enemy : public Entity {
private:
public:
    Enemy();
    void update(float dt);
    ~Enemy();
};

#endif // CPORTA

#endif // ENTITY_H