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

        for (auto &active_edge: active_edge_list_) {
            if (active_edge.NeedUpdate()) {
                for (auto &edge: edge_list_[y]) {
                    if (edge.id() == active_edge.id()) {
                        active_edge.UpdateEdge(edge);
                    }
                }
            }
        }

        for (auto &polygon: polygon_list_[y]) {
            active_polygon_list_.push_back(polygon);
            Edge active_edges[2];
            int cnt = 0;
            for (auto &edge: edge_list_[y]) {
                if (edge.id() == polygon.id()) {
                    active_edges[cnt++] = edge;
                }
            }
            assert(cnt == 2);
            active_edge_list_.emplace_back(active_edges[0], active_edges[1], polygon.normal());
        }

        for (auto &active_edge: active_edge_list_) {
            float z = 1;
            for (int x = active_edge.l(); x <= active_edge.r(); x++) {
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

        for (int x = 0; x < width_; x++) {
            fragment_buffer_[GetIdx(x, y)] = depth_buffer_[x];
        }

        UpdateActiveEdgeList();
        UpdateActivePolygonList();
    }
}

void ScanLine::CreatTable() {
    for (auto &triangle: mesh_.Triangles()) {
        Polygon polygon(triangle);
        polygon_list_[polygon.min_y()].push_back(polygon);
        auto &vertices = triangle.vertices();
        for (int i = 0; i < vertices.size(); i++) {
            Edge edge(polygon.id(), vertices[i], vertices[(i + 1) % vertices.size()]);
            if (edge.NeedDraw()) {
                edge_list_[edge.min_y()].push_back(edge);
            }
        }
    }
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
