//
// Created by bhiaibogf on 2022/1/5.
//

#include "scan_line.h"

ScanLine::ScanLine(int width, int height) : Rasterizer(width, height) {
    polygon_table_.resize(height_);
    edge_table_.resize(height_);
    line_table_.resize(height_);

    depth_buffer_.resize(width_);
    fragment_buffer_.resize(width_ * height_);
}

void ScanLine::Clear() {
    for (auto &polygon_list: polygon_table_) {
        polygon_list.clear();
    }
    for (auto &edge_list: edge_table_) {
        edge_list.clear();
    }
    active_polygon_list_.clear();
    active_edge_list_.clear();

    Polygon::Reset();

    std::fill(depth_buffer_.begin(), depth_buffer_.end(), 1.f);
    std::fill(fragment_buffer_.begin(), fragment_buffer_.end(), 1.f);
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

        for (auto &polygon: polygon_table_[y]) {
            active_polygon_list_.push_back(polygon);
            Edge active_edges[3];
            int cnt = 0;
            for (auto &edge: edge_table_[y]) {
                if (edge.id() == polygon.id()) {
                    active_edges[cnt++] = edge;
                }
            }
            active_edge_list_.emplace_back(active_edges[0], active_edges[1], polygon.normal());
            assert(cnt == 2);
        }

        for (auto &active_edge: active_edge_list_) {
            float z = 1;
            // if (y == 511 - 253) {
            //     std::cout << active_edge.l() << ' ' << active_edge.r() << std::endl;
            // }
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
                }
            }
        }

        // if (y == 511 - 253) {
        //     std::cout << line_table_[y].size() << std::endl;
        // }
        for (auto &line: line_table_[y]) {
            float z = 1;
            for (int x = line.l(); x <= line.r(); x++) {
                if (x < 0 || x >= width_) {
                    continue;
                }
                if (z == 1) {
                    z = line.z();
                } else {
                    z += line.dz();
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
        } else if (polygon.InLine()) {
            // std::cout << "in line " << polygon.min_y() << std::endl;
            line_table_[polygon.min_y()].emplace_back(triangle, polygon.normal());
        } else {
            // std::cout << "??" << polygon.min_y() << std::endl;
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
