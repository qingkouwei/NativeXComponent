# HarmonyOS NativeXComponent OpenGL 渲染示例

这是一个基于 HarmonyOS Next 的 NDK 开发示例项目，演示了如何利用 `NativeXComponent` 结合 OpenGL ES 实现高性能的图形渲染。

## 项目简介

本项目通过 C++ 层的 NDK 开发，展示了在 HarmonyOS 应用中集成 OpenGL ES 的完整流程。核心功能是展示一个采用“筝形分解法”绘制的多彩五角星，并支持点击交互切换配色方案。

### 核心特性

- **NativeXComponent 集成**：演示 ArkTS 层组件与 C++ 底层渲染引擎的关联。
- **OpenGL ES 渲染**：包含着色器程序创建、顶点数据传输、多种图元绘制模式的应用。
- **EGL 环境管理**：完整的 EGL Display、Surface 和 Context 的生命周期管理。
- **高级几何算法**：采用筝形（Kite）分解法实现五角星绘制，简化了顶点计算通过数学旋转矩阵生成图形。
- **动态交互**：支持通过 NAPI 触发的点击变色功能，演示前后端通信与实时渲染。

## 目录结构

```text
├── entry
│   └── src
│       └── main
│           ├── cpp              # C++ 源代码
│           │   ├── manager      # 渲染管理类
│           │   ├── render       # OpenGL/EGL 核心实现 (egl_core.cpp, EGLConst.h)
│           │   └── napi_init.cpp # NAPI 接口桥接
│           └── ets              # ArkTS 源代码
│               └── pages        # UI 页面
├── docs
│   ├── images                   # 算法逻辑示意图
│   └── HarmonyOS_6_OpenGL_Drawing.md # 详细技术教学文档
└── README.md                    # 项目说明
```

## 技术文档

本项目配有详尽的系列开发文档，涵盖了从基础环境搭建到核心渲染算法的各个方面：

- **核心教程**：[HarmonyOS 6：基于 OpenGL 的 Native 绘图详解](docs/HarmonyOS_6_OpenGL_Drawing.md)
  - 涵盖 OpenGL/EGL API 深潜
  - 筝形分解法数学原理
  - 渲染管线逐行代码拆解

## 快速开始

1. **环境准备**：使用 DevEco Studio (Next 版本) 导入项目。
2. **构建运行**：连接真机或模拟器，点击运行。
3. **交互体验**：在屏幕上点击，观察五角星颜色的动态变换。

## 许可证

本项目遵循 Apache License 2.0。
