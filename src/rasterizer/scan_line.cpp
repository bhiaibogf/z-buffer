//
// Created by bhiaibogf on 2022/1/5.
//

#include "scan_line.h"

ScanLine::ScanLine(int width, int height) : Rasterizer(width, height) {
    polygon_list_.resize(height_);
    edge_list_.resize(height_);

    depth_buffer_.resize(width_);
    fragment_buffer_.resize(width_ * height_);
}

void ScanLine::Clear() {
    for (auto &polygon_list: polygon_list_) {
        polygon_list.clear();
    }
    for (auto &edge_list: edge_list_) {
        edge_list.clear();
    }

    std::fill(depth_buffer_.begin(), depth_buffer_.end(), 1.f);
    std::fill(fragment_buffer_.begin(), fragment_buffer_.end(), 1.f);
}

void ScanLine::FragmentShader() {
    CreatTable();

    for (int y = 0; y < height_; y++) {
        std::fill(depth_buffer_.begin(), depth_buffer_.end(), 1.f);

        std::set<int> ids;
        for (auto iter = active_edge_list_.begin(); iter != active_edge_list_.end();) {
            if (iter->NeedDeleted()) {
                ids.insert(iter->id_);
                iter = active_edge_list_.erase(iter);
            } else {
                iter++;
            }
        }

        for (auto &item: polygon_list_[y]) {
            active_polygon_list_.push_back(item);// TODO
            ids.insert(item.id_);
        }

        for (auto &item: active_polygon_list_) {
            if (ids.find(item.id_) != ids.end()) {
                for (auto &edge: item.Edge()) {
                    if (edge.IsActive(y)) {
                        active_edge_list_.push_back(edge);// TODO
                    }
                }
            }
        }

        for (auto &edge: active_edge_list_) {
            float z = -2;
            for (int x = edge.x_l_; x <= edge.x_r_; x++) {
                if (z == -2) {
                    z = edge.z_l_;
                } else {
                    z += edge.d_z_x_;
                }
                if (z < depth_buffer_[x]) {
                    depth_buffer_[x] = z;
                }
            }
        }

        for (int x = 0; x < width_; x++) {
            fragment_buffer_[GetIdx(x, y)] = depth_buffer_[x];
        }

        UpdateActiveEdgeList();
        UpdateActivePolygonList();
    }
}

void ScanLine::CreatTable() {
    // TODO
}

void ScanLine::UpdateActiveEdgeList() {
    for (auto &edge: active_edge_list_) {
        edge.Update();
    }
}

void ScanLine::UpdateActivePolygonList() {
    for (auto iter = active_polygon_list_.begin(); iter != active_polygon_list_.end();) {
        iter->Update();
        if (iter->NeedDeleted()) {
            iter = active_polygon_list_.erase(iter);
        } else {
            iter++;
        }
    }
}
