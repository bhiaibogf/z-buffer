[TOC]

## 运行环境

### 第三方依赖

`OpenCV`：用于显示与存储渲染结果

`Eigen`：矩阵库，用于坐标的储存与计算

### Linux

#### 测试环境

```txt
OS: Ubuntu 20.04 focal(on the Windows Subsystem for Linux)
Kernel: x86_64 Linux 5.10.60.1-microsoft-standard-WSL2
GTK Theme: Adwaita [GTK3]

CPU: Intel Core i5-9400F @ 6x 2.904GHz
RAM: 7928MiB

g++ (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
cmake version 3.16.3
```

#### 获取代码

```bash
git clone https://github.com/bhiaibogf/z-buffer.git
```

#### 安装依赖

```bash
sudo apt install libeigen3-dev libopencv-dev
```

#### 编译

```bash
cd z-buffer && mkdir build && cd build && cmake .. && make
```

#### 运行

```bash
./z_buffer
```

### Windows

#### 测试环境

```txt
版本	Windows 11 专业版
版本	21H2
操作系统版本	22000.434

处理器	Intel(R) Core(TM) i5-9400F CPU @ 2.90GHz   2.90 GHz
机带 RAM	16.0 GB

Microsoft Visual Studio Community 2019 Version 16.11.6
```

#### 安装依赖

```powershell
vcpkg install eigen3:x64-windows opencv:x64-windows
```

#### 获取代码/编译运行代码

使用 Visual Studio 图形界面从本仓库拉取代码并打开为 CMake 项目

## 使用说明

### 场景

请将 obj 文件放置在与可执行文件同一位置下的 model 目录中，并更改 main 函数，正确设置相机位置，合理调整物体大小，以获取更好的显示效果

#### 运行界面

终端中会显示当前使用的 z-buffer 算法以及实时帧率，同时图形界面展示实时的渲染结果

#### 操作

使用键盘控制图形界面

| 按键 | 功能                               |
| ---- | ---------------------------------- |
| `w`  | 相机前进                           |
| `s`  | 相机后退                           |
| `a`  | 相机沿 y 轴顺时针旋转              |
| `d`  | 相机沿 y 轴逆时针旋转              |
| `c`  | 切换深度、法线画面                 |
| `v`  | 保存当前画面                       |
| `l`  | 切换扫描线算法与基本 z-buffer 算法 |

## 数据结构

主要介绍与扫描线 z-buffer 相关的数据结构

### object 类（源码 object 目录）

基类 Object，继承产生三角形类 Triangle，三角形面片类 Mesh，用于储存顶点、颜色等信息

### 工具类（源码 utils 目录）

Camera 类，用于生成 view, projection 矩阵

ObjLoader 类，用于将 obj 文件读取到 Mesh 中

transform 命名空间提供仿射变换的矩阵

### 光栅化类（源码 rasterizer 目录）

基类 Rasterizer，光栅化框架

```c++
class Rasterizer {
protected:
    int width_, height_; // 场景长宽
    float near_, far_; // 相机近、远平面

    Mesh mesh_; // 场景面片表示

    std::vector<float> z_buffer_; // z-buffer 缓存
    std::vector<float> depth_buffer_; // 深度缓存
    std::vector<Eigen::Vector3f> fragment_buffer_; // 着色信息缓存
};
```

子类 Vanilla，传统 z-buffer 算法，通过遍历每一个片元完成深度检测

子类 ScanLIne，使用扫描线的 z-buffer 算法

```c++
class ScanLine : public Rasterizer {
private:
    std::vector<std::list<Polygon>> polygon_table_; // 多边形表
    std::vector<std::list<Edge>> edge_table_; // 边表
    std::list<Polygon> active_polygon_list_; // 活化多边形表（实际没有使用，可以删除）
    std::list<ActiveEdge> active_edge_list_; // 活化边表
};
```

#### 扫描线相关数据结构（源码 rasterizer/struct 目录）

Polygon 类，储存三角形片元相关数据

```c++
class Polygon {
private:
    static int id_cnt_; // 片元计数
    int id_; // 片元的编号
    Eigen::Vector3f normal_; // 片元所在平面的法向
    Eigen::Vector3f color_; // 片元的颜色
    float min_y_; // 片元在 y 轴上投影的最小值
    int line_; // 片元跨越的扫描线数量或片元剩余部分跨越的扫描线数量
};
```

Edge 类，储存边有关的信息

```c++
class Edge {
private:
    int id_; // 边所在的片元的编号
    Eigen::Vector4f vertex_; // 边 y 轴最低的点的坐标
    float dx_; // 扫描线移动时 x 坐标的增量
    int line_; // 边跨越的扫描线数量
};
```

ActiveEdge 类，储存活化边

```c++
class ActiveEdge {
private:
    int id_; // 边所在的片元的编号
    int line_l_ = -1, line_r_ = -1; // 左、右边跨越的扫描线数量
    float x_l_, dx_l_; // 左边 x 坐标及 x 坐标增量
    float x_r_, dx_r_; // 右边 x 坐标及 x 坐标增量
    float z_l_, dz_x_, dz_y_; // 左边起点 z 坐标及 z 坐标随 x, y 增加的增量
    Eigen::Vector3f color_; // 边对内应填充的颜色
};
```

Line 类，储存一条需要被光栅化的边

## 加速

无

