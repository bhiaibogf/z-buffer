//
// Created by bhiaibogf on 2021/12/31.
//

#ifndef Z_BUFFER_OBJECT_H
#define Z_BUFFER_OBJECT_H


#include <iostream>
#include <array>

#include <eigen3/Eigen/Eigen>

class Object {
public:
    Object() { model_ = Eigen::Matrix4f::Identity(); }

    ~Object() = default;

    void set_model(const Eigen::Matrix4f &model) { model_ = model; }

    auto &model() const { return model_; }

    virtual void Transform(const Eigen::Matrix4f &mvp) = 0;

private:
    Eigen::Matrix4f model_;

};


#endif //Z_BUFFER_OBJECT_H
