/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef NATIVEXCOMPONENT_EGLCONST_H
#define NATIVEXCOMPONENT_EGLCONST_H
// [Start egl_const_h]
// EGLConst.h
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

const unsigned int LOG_PRINT_DOMAIN = 0xFF00;

/**
 * Program 错误
 */
const GLuint PROGRAM_ERROR = 0;

/**
 * 位置错误。
 */
const GLint POSITION_ERROR = -1;

/**
 * 默认x坐标。
 */
const int DEFAULT_X_POSITION = 0;

/**
 * 默认y坐标。
 */
const int DEFAULT_Y_POSITION = 0;

/**
 * Gl 红色默认值。
 */
const GLfloat GL_RED_DEFAULT = 0.0;

/**
 * Gl 绿色默认值。
 */
const GLfloat GL_GREEN_DEFAULT = 0.0;

/**
 * Gl 蓝色默认值。
 */
const GLfloat GL_BLUE_DEFAULT = 0.0;

/**
 * Gl 透明度。
 */
const GLfloat GL_ALPHA_DEFAULT = 1.0;

/**
 * Pointer 数量。
 */
const GLint POINTER_SIZE = 2;

/**
 * Triangle fan 尺寸。
 */
const GLsizei TRIANGLE_FAN_SIZE = 4;

/**
 * Shape vertices 尺寸。
 */
const int SHAPE_VERTICES_SIZE = 8;

/**
 * 50%。
 */
const float FIFTY_PERCENT = 0.5;

/**
 * 位置句柄名字。
 */
const char POSITION_NAME[] = "a_position";

// [StartExclude egl_const_h]
/**
 * numeric value 0.
 */
const int NUM_0 = 0;

/**
 * numeric value 4.
 */
const int NUM_4 = 4;
// [EndExclude egl_const_h]

/**
 * Background color #f4f4f4.
 */
const GLfloat BACKGROUND_COLOR[] = {244.0f / 255, 244.0f / 255, 244.0f / 255,
                                    1.0f};

/**
 * Draw 颜色调色盘 (与示意图一致)
 */
const GLfloat DRAW_PALETTE[5][4] = {
    {218.0f / 255, 165.0f / 255, 32.0f / 255, 1.0f},  // 金色 (顶)
    {188.0f / 255, 94.0f / 255, 71.0f / 255, 1.0f},   // 棕色 (右侧顶)
    {95.0f / 255, 158.0f / 255, 160.0f / 255, 1.0f},  // 青色 (右侧底)
    {102.0f / 255, 126.0f / 255, 153.0f / 255, 1.0f}, // 蓝灰色 (左侧底)
    {133.0f / 255, 153.0f / 255, 94.0f / 255, 1.0f}   // 橄榄色 (左侧顶)
};

/**
 * Click/Change 颜色调色盘 (切换效果)
 */
const GLfloat CHANGE_PALETTE[5][4] = {
    {133.0f / 255, 153.0f / 255, 94.0f / 255, 1.0f}, // 切换为橄榄色
    {218.0f / 255, 165.0f / 255, 32.0f / 255, 1.0f}, // ...以此类推
    {188.0f / 255, 94.0f / 255, 71.0f / 255, 1.0f},
    {95.0f / 255, 158.0f / 255, 160.0f / 255, 1.0f},
    {102.0f / 255, 126.0f / 255, 153.0f / 255, 1.0f}};

/**
 * 背景区域。
 */
const GLfloat BACKGROUND_RECTANGLE_VERTICES[] = {-1.0f, 1.0f,  1.0f,  1.0f,
                                                 1.0f,  -1.0f, -1.0f, -1.0f};

const EGLint ATTRIB_LIST[] = {
    // 键，值。
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_RENDERABLE_TYPE,
    EGL_OPENGL_ES2_BIT,
    // 结束。
    EGL_NONE};

const EGLint CONTEXT_ATTRIBS[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

/**
 * 顶点着色器
 */
const char VERTEX_SHADER[] = "#version 300 es\n"
                             "layout(location = 0) in vec4 a_position;\n"
                             "layout(location = 1) in vec4 a_color;   \n"
                             "out vec4 v_color;                       \n"
                             "void main()                             \n"
                             "{                                       \n"
                             "   gl_Position = a_position;            \n"
                             "   v_color = a_color;                   \n"
                             "}                                       \n";

/**
 * 片元着色器。
 */
const char FRAGMENT_SHADER[] = "#version 300 es\n"
                               "precision mediump float;                  \n"
                               "in vec4 v_color;                          \n"
                               "out vec4 fragColor;                       \n"
                               "void main()                               \n"
                               "{                                         \n"
                               "   fragColor = v_color;                   \n"
                               "}                                         \n";
// [End egl_const_h]
#endif // NATIVEXCOMPONENT_EGLCONST_H
