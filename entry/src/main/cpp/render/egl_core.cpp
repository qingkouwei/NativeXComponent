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

#include "egl_core.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>
#include <GLES3/gl3.h>
#include <cmath>
#include <cstdio>
#include <hilog/log.h>

#include "../common/common.h"
#include "EGLConst.h"
#include "plugin_render.h"

namespace NativeXComponentSample {
namespace {
/**
 * Get context parameter count.
 */
const size_t GET_CONTEXT_PARAM_CNT = 1;

/**
 * Egl red size default.
 */
const int EGL_RED_SIZE_DEFAULT = 8;

/**
 * Egl green size default.
 */
const int EGL_GREEN_SIZE_DEFAULT = 8;

/**
 * Egl blue size default.
 */
const int EGL_BLUE_SIZE_DEFAULT = 8;

/**
 * Egl alpha size default.
 */
const int EGL_ALPHA_SIZE_DEFAULT = 8;

} // namespace
// [Start native_create_context_init]
bool EGLCore::EglContextInit(void *window, int width, int height) {
  // [StartExclude native_create_context_init]
  OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "EGLCore",
               "EglContextInit execute");
  if ((window == nullptr) || (width <= 0) || (height <= 0)) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "EglContextInit: param error");
    return false;
  }
  // [EndExclude native_create_context_init]
  UpdateSize(width, height);
  eglWindow_ = reinterpret_cast<EGLNativeWindowType>(window);

  // 初始化display
  eglDisplay_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (eglDisplay_ == EGL_NO_DISPLAY) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "eglGetDisplay: unable to get EGL display");
    return false;
  }
  // 初始化EGL
  EGLint majorVersion;
  EGLint minorVersion;
  if (!eglInitialize(eglDisplay_, &majorVersion, &minorVersion)) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "eglInitialize: unable to get initialize EGL display");
    return false;
  }

  // 选择配置
  const EGLint maxConfigSize = 1;
  EGLint numConfigs;
  if (!eglChooseConfig(eglDisplay_, ATTRIB_LIST, &eglConfig_, maxConfigSize,
                       &numConfigs)) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "eglChooseConfig: unable to choose configs");
    return false;
  }
  // 创建环境
  return CreateEnvironment();
}
// [End native_create_context_init]
// [Start native_create_environment]
bool EGLCore::CreateEnvironment() {
  // 创建Surface
  if (eglWindow_ == 0) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "eglWindow_ is null");
    return false;
  }
  eglSurface_ =
      eglCreateWindowSurface(eglDisplay_, eglConfig_, eglWindow_, NULL);
  if (eglSurface_ == nullptr) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "eglCreateWindowSurface: unable to create surface");
    return false;
  }
  // 创建context
  eglContext_ = eglCreateContext(eglDisplay_, eglConfig_, EGL_NO_CONTEXT,
                                 CONTEXT_ATTRIBS);
  if (!eglMakeCurrent(eglDisplay_, eglSurface_, eglSurface_, eglContext_)) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "eglMakeCurrent failed");
    return false;
  }
  // 创建program
  program_ = CreateProgram(VERTEX_SHADER, FRAGMENT_SHADER);
  if (program_ == PROGRAM_ERROR) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "CreateProgram: unable to create program");
    return false;
  }
  return true;
}
// [End native_create_environment]
// [Start native_background]
// 绘制背景颜色
void EGLCore::Background() {
  GLint position = PrepareDraw();
  if (position == POSITION_ERROR) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "Background get position failed");
    return;
  }

  if (!ExecuteDraw(position, BACKGROUND_COLOR, BACKGROUND_RECTANGLE_VERTICES,
                   sizeof(BACKGROUND_RECTANGLE_VERTICES))) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "Background execute draw failed");
    return;
  }

  if (!FinishDraw()) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "Background FinishDraw failed");
    return;
  }
}
// [End native_background]
// [Start native_draw]
void EGLCore::Draw(int &hasDraw) {
  flag_ = false;
  OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "EGLCore", "Draw");
  GLint position = PrepareDraw();
  if (position == POSITION_ERROR) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "Draw get position failed");
    return;
  }

  // 绘制背景
  if (!ExecuteDraw(position, BACKGROUND_COLOR, BACKGROUND_RECTANGLE_VERTICES,
                   sizeof(BACKGROUND_RECTANGLE_VERTICES))) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "Draw execute draw background failed");
    return;
  }

  // 将五角星分为五个四边形，计算其中一个四边形的四个顶点
  GLfloat rotateX = 0;
  GLfloat rotateY = FIFTY_PERCENT * height_;
  GLfloat centerX = 0;
  // Convert DEG(54° & 18°) to RAD
  GLfloat centerY = -rotateY * (M_PI / 180 * 54) * (M_PI / 180 * 18);
  // Convert DEG(18°) to RAD
  GLfloat leftX = -rotateY * (M_PI / 180 * 18);
  GLfloat leftY = 0;
  // Convert DEG(18°) to RAD
  GLfloat rightX = rotateY * (M_PI / 180 * 18);
  GLfloat rightY = 0;

  // 确定绘制四边形的顶点，使用绘制区域的百分比表示
  const GLfloat shapeVertices[] = {
      centerX / width_, centerY / height_, leftX / width_,  leftY / height_,
      rotateX / width_, rotateY / height_, rightX / width_, rightY / height_};

  // 绘制图形 (第一个部分)
  if (!ExecuteDraw(position, DRAW_PALETTE[0], shapeVertices,
                   sizeof(shapeVertices))) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "Draw execute draw shape failed");
    return;
  }

  // Convert DEG(72°) to RAD
  GLfloat rad = M_PI / 180 * 72;
  // Rotate four times
  for (int i = 0; i < NUM_4; ++i) {
    // 旋转得其他四个四边形的顶点
    Rotate2d(centerX, centerY, &rotateX, &rotateY, rad);
    Rotate2d(centerX, centerY, &leftX, &leftY, rad);
    Rotate2d(centerX, centerY, &rightX, &rightY, rad);

    // 确定绘制四边形的顶点，使用绘制区域的百分比表示
    const GLfloat shapeVertices[] = {
        centerX / width_, centerY / height_, leftX / width_,  leftY / height_,
        rotateX / width_, rotateY / height_, rightX / width_, rightY / height_};

    // 绘制图形 (后续四个部分)
    if (!ExecuteDraw(position, DRAW_PALETTE[i + 1], shapeVertices,
                     sizeof(shapeVertices))) {
      OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                   "Draw execute draw shape failed");
      return;
    }
  }

  // 结束绘制
  if (!FinishDraw()) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "Draw FinishDraw failed");
    return;
  }
  hasDraw = 1;

  flag_ = true;
}
// [End native_draw]
// [Start native_change_color]
void EGLCore::ChangeColor(int &hasChangeColor) {
  if (!flag_) {
    return;
  }
  OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "EGLCore", "ChangeColor");
  GLint position = PrepareDraw();
  if (position == POSITION_ERROR) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "ChangeColor get position failed");
    return;
  }

  // 绘制背景
  if (!ExecuteDraw(position, BACKGROUND_COLOR, BACKGROUND_RECTANGLE_VERTICES,
                   sizeof(BACKGROUND_RECTANGLE_VERTICES))) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "ChangeColor execute draw background failed");
    return;
  }

  // 确定绘制四边形的顶点，使用绘制区域的百分比表示
  GLfloat rotateX = 0;
  GLfloat rotateY = FIFTY_PERCENT * height_;
  GLfloat centerX = 0;
  // Convert DEG(54° & 18°) to RAD
  GLfloat centerY = -rotateY * (M_PI / 180 * 54) * (M_PI / 180 * 18);
  // Convert DEG(18°) to RAD
  GLfloat leftX = -rotateY * (M_PI / 180 * 18);
  GLfloat leftY = 0;
  // Convert DEG(18°) to RAD
  GLfloat rightX = rotateY * (M_PI / 180 * 18);
  GLfloat rightY = 0;

  // 确定绘制四边形的顶点，使用绘制区域的百分比表示
  const GLfloat shapeVertices[] = {
      centerX / width_, centerY / height_, leftX / width_,  leftY / height_,
      rotateX / width_, rotateY / height_, rightX / width_, rightY / height_};

  // 使用新的调色盘绘制 (第一个部分)
  if (!ExecuteDraw(position, CHANGE_PALETTE[0], shapeVertices,
                   sizeof(shapeVertices))) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "Draw execute draw shape failed");
    return;
  }

  // Convert DEG(72°) to RAD
  GLfloat rad = M_PI / 180 * 72;
  // Rotate four times
  for (int i = 0; i < NUM_4; ++i) {
    // 旋转得其他四个四边形的顶点
    Rotate2d(centerX, centerY, &rotateX, &rotateY, rad);
    Rotate2d(centerX, centerY, &leftX, &leftY, rad);
    Rotate2d(centerX, centerY, &rightX, &rightY, rad);
    // 确定绘制四边形的顶点，使用绘制区域的百分比表示
    const GLfloat shapeVertices[] = {
        centerX / width_, centerY / height_, leftX / width_,  leftY / height_,
        rotateX / width_, rotateY / height_, rightX / width_, rightY / height_};

    // 使用新的调色盘绘制 (后续四个部分)
    if (!ExecuteDraw(position, CHANGE_PALETTE[i + 1], shapeVertices,
                     sizeof(shapeVertices))) {
      OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                   "Draw execute draw shape failed");
      return;
    }
  }

  // 结束绘制
  if (!FinishDraw()) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "ChangeColor FinishDraw failed");
  }
  hasChangeColor = 1;
}
// [StartExclude native_change_color]
// [Start native_prepare_draw]
// 绘前准备，获取position，创建成功时position值从0开始
GLint EGLCore::PrepareDraw() {
  if ((eglDisplay_ == nullptr) || (eglSurface_ == nullptr) ||
      (eglContext_ == nullptr) ||
      (!eglMakeCurrent(eglDisplay_, eglSurface_, eglSurface_, eglContext_))) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "PrepareDraw: param error");
    return POSITION_ERROR;
  }

  // 该gl函数没有返回值。
  glViewport(DEFAULT_X_POSITION, DEFAULT_Y_POSITION, width_, height_);
  glClearColor(GL_RED_DEFAULT, GL_GREEN_DEFAULT, GL_BLUE_DEFAULT,
               GL_ALPHA_DEFAULT);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(program_);

  return glGetAttribLocation(program_, POSITION_NAME);
}
// [End native_prepare_draw]
// [Start native_execute_draw]
// 依据传入参数在指定区域绘制指定颜色
bool EGLCore::ExecuteDraw(GLint position, const GLfloat *color,
                          const GLfloat shapeVertices[],
                          unsigned long vertSize) {
  if ((position > 0) || (color == nullptr) ||
      (vertSize / sizeof(shapeVertices[0])) != SHAPE_VERTICES_SIZE) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "ExecuteDraw: param error");
    return false;
  }

  // 该gl函数没有返回值。
  glVertexAttribPointer(position, POINTER_SIZE, GL_FLOAT, GL_FALSE, 0,
                        shapeVertices);
  glEnableVertexAttribArray(position);
  glVertexAttrib4fv(1, color);
  glDrawArrays(GL_TRIANGLE_FAN, 0, TRIANGLE_FAN_SIZE);
  glDisableVertexAttribArray(position);

  return true;
}

// [End native_change_color]
void EGLCore::Rotate2d(GLfloat centerX, GLfloat centerY, GLfloat *rotateX,
                       GLfloat *rotateY, GLfloat theta) {
  GLfloat tempX =
      cos(theta) * (*rotateX - centerX) - sin(theta) * (*rotateY - centerY);
  GLfloat tempY =
      sin(theta) * (*rotateX - centerX) + cos(theta) * (*rotateY - centerY);
  *rotateX = tempX + centerX;
  *rotateY = tempY + centerY;
}
// [Start native_finish_draw]
// 结束绘制操作
bool EGLCore::FinishDraw() {
  // 强制刷新缓冲
  glFlush();
  glFinish();
  return eglSwapBuffers(eglDisplay_, eglSurface_);
}
// [End native_finish_draw]
// [Start native_load_shader]
GLuint EGLCore::LoadShader(GLenum type, const char *shaderSrc) {
  if ((type <= 0) || (shaderSrc == nullptr)) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "glCreateShader type or shaderSrc error");
    return PROGRAM_ERROR;
  }

  GLuint shader = glCreateShader(type);
  if (shader == 0) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "glCreateShader unable to load shader");
    return PROGRAM_ERROR;
  }

  // 该gl函数没有返回值。
  glShaderSource(shader, 1, &shaderSrc, nullptr);
  glCompileShader(shader);

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (compiled != 0) {
    return shader;
  }

  GLint infoLen = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
  if (infoLen <= 1) {
    glDeleteShader(shader);
    return PROGRAM_ERROR;
  }

  char *infoLog = (char *)malloc(sizeof(char) * (infoLen + 1));
  if (infoLog != nullptr) {
    memset(infoLog, 0, infoLen + 1);
    glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "glCompileShader error = %s", infoLog);
    free(infoLog);
    infoLog = nullptr;
  }
  glDeleteShader(shader);
  return PROGRAM_ERROR;
}
// [End native_load_shader]
// [Start native_create_program]
GLuint EGLCore::CreateProgram(const char *vertexShader,
                              const char *fragShader) {
  if ((vertexShader == nullptr) || (fragShader == nullptr)) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "createProgram: vertexShader or fragShader is null");
    return PROGRAM_ERROR;
  }

  GLuint vertex = LoadShader(GL_VERTEX_SHADER, vertexShader);
  if (vertex == PROGRAM_ERROR) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "createProgram vertex error");
    return PROGRAM_ERROR;
  }

  GLuint fragment = LoadShader(GL_FRAGMENT_SHADER, fragShader);
  if (fragment == PROGRAM_ERROR) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "createProgram fragment error");
    return PROGRAM_ERROR;
  }

  GLuint program = glCreateProgram();
  if (program == PROGRAM_ERROR) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "createProgram program error");
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return PROGRAM_ERROR;
  }

  // 该gl函数没有返回值。
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);

  GLint linked;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (linked != 0) {
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
  }

  OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
               "createProgram linked error");
  GLint infoLen = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
  if (infoLen > 1) {
    char *infoLog = (char *)malloc(sizeof(char) * (infoLen + 1));
    memset(infoLog, 0, infoLen + 1);
    glGetProgramInfoLog(program, infoLen, nullptr, infoLog);
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "glLinkProgram error = %s", infoLog);
    free(infoLog);
    infoLog = nullptr;
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  glDeleteProgram(program);
  return PROGRAM_ERROR;
}
// [End native_create_program]
// [Start native_update_size]
void EGLCore::UpdateSize(int width, int height) {
  // width_和height_在头文件中定义
  width_ = width;
  height_ = height;
  if (width_ > 0) {
    widthPercent_ = FIFTY_PERCENT * height_ / width_;
  }
}
// [End native_update_size]
// [Start native_release]
void EGLCore::Release() {
  // 释放Surface
  if ((eglDisplay_ == nullptr) || (eglSurface_ == nullptr) ||
      (!eglDestroySurface(eglDisplay_, eglSurface_))) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "Release eglDestroySurface failed");
  }
  // 释放context
  if ((eglDisplay_ == nullptr) || (eglContext_ == nullptr) ||
      (!eglDestroyContext(eglDisplay_, eglContext_))) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "Release eglDestroyContext failed");
  }
  // 释放display
  if ((eglDisplay_ == nullptr) || (!eglTerminate(eglDisplay_))) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "EGLCore",
                 "Release eglTerminate failed");
  }
}
// [End native_release]
} // namespace NativeXComponentSample
