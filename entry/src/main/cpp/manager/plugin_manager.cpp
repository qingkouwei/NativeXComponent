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

#include "plugin_manager.h"
#include "../common/common.h"
#include "arkui/native_interface.h"
#include "arkui/native_node.h"
#include "arkui/native_node_napi.h"
#include <ace/xcomponent/native_interface_xcomponent.h>
#include <cstdint>
#include <cstdio>
#include <hilog/log.h>
#include <string>

#define COLUMN_MARGIN 10
#define XC_WIDTH 300
#define XC_HEIGHT 300
#define ARG_CNT 2

namespace NativeXComponentSample {
// [Start plugin_manager_cpp]
// plugin_manager.cpp
std::unordered_map<void *, OH_ArkUI_SurfaceCallback *>
    PluginManager::callbackMap_;
ArkUI_AccessibilityProvider *PluginManager::provider_ = nullptr;
ArkUI_NativeNodeAPI_1 *nodeAPI = reinterpret_cast<ArkUI_NativeNodeAPI_1 *>(
    OH_ArkUI_QueryModuleInterfaceByName(ARKUI_NATIVE_NODE,
                                        "ArkUI_NativeNodeAPI_1"));
// [StartExclude plugin_manager_cpp]
PluginManager PluginManager::pluginManager_;
// [Start plugin_manager_cpp]
// [StartExclude plugin_manager_cpp]
PluginManager::~PluginManager() {
  OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "Callback",
               "~PluginManager");
}

// [EndExclude plugin_manager_cpp]
static std::string value2String(napi_env env, napi_value value) {
  size_t stringSize = 0;
  napi_get_value_string_utf8(env, value, nullptr, 0, &stringSize);
  std::string valueString;
  valueString.resize(stringSize);
  napi_get_value_string_utf8(env, value, &valueString[0], stringSize + 1,
                             &stringSize);
  return valueString;
}

PluginManager::PluginManager() {}

void PluginManager::Export(napi_env env, napi_value exports) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginManager",
                 "Export start");
  if ((env == nullptr) || (exports == nullptr)) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginManager",
                 "Export: env or exports is null");
    return;
  }

  napi_value exportInstance = nullptr;
  if (napi_get_named_property(env, exports, OH_NATIVE_XCOMPONENT_OBJ,
                              &exportInstance) != napi_ok) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginManager",
                 "Export: napi_get_named_property fail");
    return;
  }

  OH_NativeXComponent *nativeXComponent = nullptr;
  if (napi_unwrap(env, exportInstance,
                  reinterpret_cast<void **>(&nativeXComponent)) != napi_ok) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginManager",
                 "Export: napi_unwrap fail");
    return;
  }

  char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
  uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
  if (OH_NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize) !=
      OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginManager",
                 "Export: OH_NativeXComponent_GetXComponentId fail");
    return;
  }

  std::string id(idStr);
  auto context = PluginManager::GetInstance();
  if ((context != nullptr) && (nativeXComponent != nullptr)) {
    auto render = context->GetRender(id);
    if (render != nullptr) {
      render->RegisterCallback(nativeXComponent);
      render->Export(env, exports);
    }
  }
}

PluginRender *PluginManager::GetRender(std::string &id) {
  if (pluginRenderMap_.find(id) == pluginRenderMap_.end()) {
    PluginRender *instance = PluginRender::GetInstance(id);
    pluginRenderMap_[id] = instance;
    return instance;
  }

  return pluginRenderMap_[id];
}

} // namespace NativeXComponentSample
