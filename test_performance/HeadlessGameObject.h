#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../Vector3.h"
#include "HeadlessRigidBody.h"
#include "HeadlessModel.h"

class HeadlessGameObject {
public:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    std::unique_ptr<HeadlessRigidBody> rigidBody;
    std::unique_ptr<HeadlessModel> model;

    HeadlessGameObject() 
        : position(0,0,0), rotation(0,0,0), scale(1,1,1) {}

    void AddRigidBody(HeadlessRigidBody* rb) {
        rigidBody.reset(rb);
    }
    
    void AddModel(HeadlessModel* m) {
        model.reset(m);
    }
};
