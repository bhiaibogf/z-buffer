//
// Created by bhiaibogf on 2022/1/5.
//

#ifndef Z_BUFFER_SCAN_LINE_H
#define Z_BUFFER_SCAN_LINE_H


#include "rasterizer.h"
#include "struct/polygon.h"
#include "struct/edge.h"

class ScanLine : public Rasterizer {
public:
    ScanLine(int width, int height);

    ~ScanLine() = default;

    void Clear() override;

    void FragmentShader() override;

    void Show() override;

private:
    std::vector<std::vector<Polygon>> polygon_list_;
    std::vector<std::vector<Edge>> edge_list_;

    std::vector<Polygon> active_polygon_list_;
    std::vector<Edge> active_edge_list_;

};


#endif //Z_BUFFER_SCAN_LINE_H
