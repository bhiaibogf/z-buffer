//
// Created by bhiaibogf on 2022/1/5.
//

#ifndef Z_BUFFER_ACTIVE_EDGE_H
#define Z_BUFFER_ACTIVE_EDGE_H


class ActiveEdge {
public:
    void Update() {
        d_y_l_--;
        d_y_r_--;
        x_l_ += d_x_l_;
        x_r_ += d_x_r_;
        z_l_ += d_z_x_ * d_x_l_ + d_z_y_; // seems wrong
    }

    bool NeedDeleted() const {
        return d_y_l_ < 0 || d_y_r_ < 0;
    }

    bool IsActive(int y) const {
        return true; // TODO
    }

    float x_l_, d_x_l_, d_y_l_;
    float x_r_, d_x_r_, d_y_r_;
    float z_l_, d_z_x_, d_z_y_;
    int id_;

};


#endif //Z_BUFFER_ACTIVE_EDGE_H
