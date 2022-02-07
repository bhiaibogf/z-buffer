//
// Created by bhiaibogf on 2022/1/5.
//

#include "scan_line.h"

ScanLine::ScanLine(int width, int height) : Rasterizer(width, height) {
    z_buffer_.resize(width_);

    polygon_table_.resize(height_);
    edge_table_.resize(height_);
    line_table_.resize(height_);
}

void ScanLine::Clear() {
    this->Rasterizer::Clear();

    for (auto &polygon_list: polygon_table_) {
        polygon_list.clear();
    }
    for (auto &edge_list: edge_table_) {
        edge_list.clear();
    }
    for (auto &line_list: line_table_) {
        line_list.clear();
    }
    active_polygon_list_.clear();
    active_edge_list_.clear();

    Polygon::Reset();
}

void ScanLine::FragmentShader(bool draw_line) {
    CreatTable();

    for (int y = 0; y < height_; y++) {
        std::fill(z_buffer_.begin(), z_buffer_.end(), 1.f);

        std::unordered_map<int, ActiveEdge *> active_edge_need_update;
        for (auto &active_edge: active_edge_list_) {
            if (active_edge.NeedUpdate()) {
                active_edge_need_update[active_edge.id()] = &active_edge;
            }
        }
        for (auto &edge: edge_table_[y]) {
            if (active_edge_need_update.count(edge.id())) {
                active_edge_need_update[edge.id()]->UpdateEdge(edge);
            }
        }

        std::unordered_map<int, std::pair<Eigen::Vector3f, Eigen::Vector3f>> new_active_polygon;
        for (auto &polygon: polygon_table_[y]) {
            active_polygon_list_.push_back(polygon);
            new_active_polygon[polygon.id()] = {polygon.normal(), polygon.color()};
        }

        Edge active_edges[2];
        int cnt = 0;
        for (auto &edge: edge_table_[y]) {
            if (new_active_polygon.count(edge.id())) {
                active_edges[cnt++] = edge;
                if (cnt == 2) {
                    auto &[normal, color] = new_active_polygon[edge.id()];
                    active_edge_list_.emplace_back(active_edges[0], active_edges[1], normal, color);
                    cnt = 0; // TODO
                }
            }
        }

        for (auto &active_edge: active_edge_list_) {
            float z = 1;
            // if (y == 511 - 253) {
            //     std::cout << active_edge.l() << ' ' << active_edge.r() << std::endl;
            // }
            int l = std::max(0, active_edge.l()), r = std::min(width_ - 1, active_edge.r());
            for (int x = l; x <= r; x++) {
                if (z == 1) {
                    z = active_edge.z();
                } else {
                    z += active_edge.dz();
                }
                if (z < z_buffer_[x] && z >= -1) {
                    z_buffer_[x] = z;
                    if (draw_line) {
                        if (l <= r) {
                            fragment_buffer_[GetIdx(l, y)] = active_edge.color();
                            fragment_buffer_[GetIdx(r, y)] = active_edge.color();
                        }
                    } else {
                        fragment_buffer_[GetIdx(x, y)] = active_edge.color();
                    }
                }
            }
        }

        // if (y == 511 - 253) {
        //     std::cout << line_table_[y].size() << std::endl;
        // }
        for (auto &line: line_table_[y]) {
            float z = 1;
            for (int x = std::max(0, line.l()); x <= std::min(width_ - 1, line.r()); x++) {
                if (z == 1) {
                    z = line.z();
                } else {
                    z += line.dz();
                }
                if (z < z_buffer_[x] && z >= -1) {
                    z_buffer_[x] = z;
                    fragment_buffer_[GetIdx(x, y)] = line.color();
                }
            }
        }

        for (int x = 0; x < width_; x++) {
            depth_buffer_[GetIdx(x, y)] = z_buffer_[x];
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
        } else if (polygon.InLine(height_)) {
            // std::cout << "in line " << polygon.min_y() << std::endl;
            line_table_[polygon.min_y()].emplace_back(triangle, polygon.normal(), polygon.color());
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
