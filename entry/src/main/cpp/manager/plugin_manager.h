/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
