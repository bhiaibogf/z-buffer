//
// Created by bhiaibogf on 2022/1/5.
//

#include "scan_line.h"

ScanLine::ScanLine(int width, int height) : Rasterizer(width, height) {
    depth_buffer_.resize(width_);

    polygon_table_.resize(height_);
    edge_table_.resize(height_);
}

void ScanLine::Clear() {
    this->Rasterizer::Clear();

    for (auto &polygon_list: polygon_table_) {
        polygon_list.clear();
    }
    for (auto &edge_list: edge_table_) {
        edge_list.clear();
    }
    active_polygon_list_.clear();
    active_edge_list_.clear();

    Polygon::Reset();
}

void ScanLine::FragmentShader() {
    CreatTable();

    for (int y = 0; y < height_; y++) {
        std::fill(depth_buffer_.begin(), depth_buffer_.end(), 1.f);

        for (auto &active_edge: active_edge_list_) {
            if (active_edge.NeedUpdate()) {
                for (auto &edge: edge_table_[y]) {
                    if (edge.id() == active_edge.id()) {
                        active_edge.UpdateEdge(edge);
                    }
                }
            }
        }

        std::map<int, std::pair<Eigen::Vector3f, Eigen::Vector3f>> active_polygon_id;
        for (auto &polygon: polygon_table_[y]) {
            active_polygon_list_.push_back(polygon);
            active_polygon_id[polygon.id()] = {polygon.normal(), polygon.color()};
        }

        Edge active_edges[2];
        int cnt = 0;
        for (auto &edge: edge_table_[y]) {
            if (active_polygon_id.count(edge.id())) {
                active_edges[cnt++] = edge;
                if (cnt == 2) {
                    auto &[normal, color] = active_polygon_id[edge.id()];
                    active_edge_list_.emplace_back(active_edges[0], active_edges[1], normal, color);
                    cnt = 0; // TODO
                }
            }
        }

        for (auto &active_edge: active_edge_list_) {
            // debug cube (edge rank wrong when creating active edge)
            // if (y == 175) {
            //     if (active_edge.id() == 4) {
            //         std::cout << active_edge.l() << ' ' << active_edge.r() << std::endl;
            //     }
            // }
            // std::cout << active_edge.id() << std::endl;
            // std::cout << active_edge.l() << ' ' << active_edge.r() << std::endl;
            float z = 1;
            for (int x = active_edge.l(); x <= active_edge.r(); x++) {
                if (x < 0 || x >= width_) {
                    continue;
                }
                if (z == 1) {
                    z = active_edge.z();
                } else {
                    z += active_edge.dz();
                }
                if (z < depth_buffer_[x]) {
                    depth_buffer_[x] = z;
                    fragment_buffer_[GetIdx(x, y)] = active_edge.color();
                }
            }
        }

        for (int x = 0; x < width_; x++) {
            depth_map_[GetIdx(x, y)] = depth_buffer_[x];
        }

        UpdateActiveEdgeList();
        UpdateActivePolygonList();
    }
}

void ScanLine::CreatTable() {
    // int polygon_cnt = 0, edge_cnt = 0;
    for (auto &triangle: mesh_.Triangles()) {
        Polygon polygon(triangle);
        if (polygon.NeedDraw(height_)) {
            polygon_table_[polygon.min_y()].push_back(polygon);
            // polygon_cnt++;
            auto &vertices = triangle.vertices();
            for (int i = 0; i < vertices.size(); i++) {
                Edge edge(polygon.id(), vertices[i], vertices[(i + 1) % vertices.size()]);
                if (edge.NeedDraw(height_)) {
                    edge_table_[edge.min_y()].push_back(edge);
                    // edge_cnt++;
                }
            }
        }
    }
    // std::cout << polygon_cnt << std::endl;
    // std::cout << edge_cnt << std::endl;
}

void ScanLine::UpdateActiveEdgeList() {
    for (auto iter = active_edge_list_.begin(); iter != active_edge_list_.end();) {
        iter->Update();
        if (iter->NeedDeleted()) {
            iter = active_edge_list_.erase(iter);
        } else {
            iter++;
        }
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
