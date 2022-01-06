//
// Created by bhiaibogf on 2022/1/5.
//

#ifndef Z_BUFFER_EDGE_H
#define Z_BUFFER_EDGE_H


class Edge {
public:
    Edge();

    ~Edge() = default;

    bool operator<(const Edge &edge) const { return x_ < edge.x_; }

    int id() const { return id_; }

    int min_y() const { return int(min_y_); }

    void CopyToActiveEdge(float &x, float &dx, int &line) const;

private:
    int id_;

    float x_, dx_, min_y_;

    int line_;

};


#endif //Z_BUFFER_EDGE_H
