//
// Created by bhiaibogf on 2022/1/5.
//

#include "scan_line.h"

ScanLine::ScanLine(int width, int height) : Rasterizer(width, height) {
    polygon_list_.resize(height);
    edge_list_.resize(height);
}

void ScanLine::Clear() {
    for (auto &polygonList: polygon_list_) {
        polygonList.clear();
    }
    for (auto &polygonList: edge_list_) {
        polygonList.clear();
    }
}

void ScanLine::FragmentShader() {

}

void ScanLine::Show() {

}
