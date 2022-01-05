//
// Created by bhiaibogf on 2021/12/31.
//

#ifndef Z_BUFFER_SCENE_H
#define Z_BUFFER_SCENE_H


#include <vector>

#include "object.h"

class Scene : public Object {
public:
    Scene() = default;

    ~Scene() = default;

    void AddObject() {}


private:
    std::vector<Object> objects_;

};


#endif //Z_BUFFER_SCENE_H
