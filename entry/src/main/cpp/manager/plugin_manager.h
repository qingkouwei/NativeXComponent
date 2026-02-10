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
#ifndef NATIVE_XCOMPONENT_PLUGIN_MANAGER_H
#define NATIVE_XCOMPONENT_PLUGIN_MANAGER_H

#include "../render/egl_core.h"
#include "arkui/native_node_napi.h"
#include <ace/xcomponent/native_interface_xcomponent.h>
#include <cstdint>
#include <js_native_api.h>
#include <js_native_api_types.h>
#include <napi/native_api.h>
#include <string>
#include <unordered_map>

#include "../render/plugin_render.h"
// [Start plugin_manager_h_part]
// plugin_manager.h
namespace NativeXComponentSample {
// [StartExclude plugin_manager_h_part]
constexpr const int FIRST_ARG = 1;
constexpr const int SECOND_ARG = 2;
constexpr const int THIRD_ARG = 3;
constexpr const int FRAME_COUNT = 50;
// [Start plugin_manager_h]
// 在头文件中定义PluginManager类
// [EndExclude plugin_manager_h_part]
class PluginManager {
public:
  PluginManager();
  ~PluginManager();

  static PluginManager *GetInstance() { return &PluginManager::pluginManager_; }

  PluginRender *GetRender(std::string &id);
  void Export(napi_env env, napi_value exports);

private:
  static PluginManager pluginManager_;
  std::unordered_map<std::string, PluginRender *> pluginRenderMap_;

public:
  static std::unordered_map<void *, OH_ArkUI_SurfaceCallback *> callbackMap_;
  static ArkUI_AccessibilityProvider *provider_;
};
// [End plugin_manager_h]
} // namespace NativeXComponentSample
// [End plugin_manager_h_part]
#endif // NATIVE_XCOMPONENT_PLUGIN_MANAGER_H
