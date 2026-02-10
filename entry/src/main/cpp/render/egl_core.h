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
#ifndef NATIVE_XCOMPONENT_EGL_CORE_H
#define NATIVE_XCOMPONENT_EGL_CORE_H

#include "string"
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>

namespace NativeXComponentSample {
class EGLCore {
public:
  explicit EGLCore() {}
  ~EGLCore() {}
  bool EglContextInit(void *window, int width, int height);
  bool CreateEnvironment();
  void Draw(int &hasDraw);
  void Background();
  void ChangeColor(int &hasChangeColor);
  void Release();
  void UpdateSize(int width, int height);

private:
  GLuint LoadShader(GLenum type, const char *shaderSrc);
  GLuint CreateProgram(const char *vertexShader, const char *fragShader);
  GLint PrepareDraw();
  bool ExecuteDraw(GLint position, const GLfloat *color,
                   const GLfloat shapeVertices[], unsigned long vertSize);
  void Rotate2d(GLfloat centerX, GLfloat centerY, GLfloat *rotateX,
                GLfloat *rotateY, GLfloat theta);
  bool FinishDraw();

private:
  EGLNativeWindowType eglWindow_;
  EGLDisplay eglDisplay_ = EGL_NO_DISPLAY;
  EGLConfig eglConfig_ = EGL_NO_CONFIG_KHR;
  EGLSurface eglSurface_ = EGL_NO_SURFACE;
  EGLContext eglContext_ = EGL_NO_CONTEXT;
  GLuint program_;
  bool flag_ = false;
  int width_;
  int height_;
  GLfloat widthPercent_;
};
} // namespace NativeXComponentSample
#endif // NATIVE_XCOMPONENT_EGL_CORE_H
