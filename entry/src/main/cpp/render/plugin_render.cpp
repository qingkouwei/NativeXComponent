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

#include <cstdint>
#include <cstring>
#include <hilog/log.h>
#include <js_native_api.h>
#include <js_native_api_types.h>
#include <string>

#include "../common/common.h"
#include "../manager/plugin_manager.h"
#include "plugin_render.h"

#define EXPECTED_FRAME_RATE 30

namespace NativeXComponentSample {
namespace {
void OnSurfaceCreatedCB(OH_NativeXComponent *component, void *window) {
  OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "Callback",
               "OnSurfaceCreatedCB");
  if ((component == nullptr) || (window == nullptr)) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "Callback",
                 "OnSurfaceCreatedCB: component or window is null");
    return;
  }

  char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
  uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
  if (OH_NativeXComponent_GetXComponentId(component, idStr, &idSize) !=
      OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "Callback",
                 "OnSurfaceCreatedCB: Unable to get XComponent id");
    return;
  }

  std::string id(idStr);
  auto render = PluginRender::GetInstance(id);
  uint64_t width;
  uint64_t height;
  int32_t xSize =
      OH_NativeXComponent_GetXComponentSize(component, window, &width, &height);
  if ((xSize == OH_NATIVEXCOMPONENT_RESULT_SUCCESS) && (render != nullptr)) {
    if (render->eglCore_->EglContextInit(window, width, height)) {
      render->eglCore_->Background();
    }
  }
}

void OnSurfaceChangedCB(OH_NativeXComponent *component, void *window) {
  OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "Callback",
               "OnSurfaceChangedCB");
  if ((component == nullptr) || (window == nullptr)) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "Callback",
                 "OnSurfaceChangedCB: component or window is null");
    return;
  }

  char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
  uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
  if (OH_NativeXComponent_GetXComponentId(component, idStr, &idSize) !=
      OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "Callback",
                 "OnSurfaceChangedCB: Unable to get XComponent id");
    return;
  }

  std::string id(idStr);
  auto render = PluginRender::GetInstance(id);
  if (render != nullptr) {
    render->OnSurfaceChanged(component, window);
    OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "Callback",
                 "surface changed");
  }
}

void OnSurfaceDestroyedCB(OH_NativeXComponent *component, void *window) {
  OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "Callback",
               "OnSurfaceDestroyedCB");
  if ((component == nullptr) || (window == nullptr)) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "Callback",
                 "OnSurfaceDestroyedCB: component or window is null");
    return;
  }

  char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
  uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
  if (OH_NativeXComponent_GetXComponentId(component, idStr, &idSize) !=
      OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "Callback",
                 "OnSurfaceDestroyedCB: Unable to get XComponent id");
    return;
  }

  std::string id(idStr);
  PluginRender::Release(id);
}

void DispatchTouchEventCB(OH_NativeXComponent *component, void *window) {
  OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "Callback",
               "DispatchTouchEventCB");
  if ((component == nullptr) || (window == nullptr)) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "Callback",
                 "DispatchTouchEventCB: component or window is null");
    return;
  }

  char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
  uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
  if (OH_NativeXComponent_GetXComponentId(component, idStr, &idSize) !=
      OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "Callback",
                 "DispatchTouchEventCB: Unable to get XComponent id");
    return;
  }

  std::string id(idStr);
  PluginRender *render = PluginRender::GetInstance(id);
  if (render != nullptr) {
    render->OnTouchEvent(component, window);
  }
}
} // namespace

std::unordered_map<std::string, PluginRender *> PluginRender::instance_;
int32_t PluginRender::hasDraw_ = 0;
int32_t PluginRender::hasChangeColor_ = 0;

PluginRender::PluginRender(std::string &id) {
  this->id_ = id;
  this->eglCore_ = new EGLCore();
}

PluginRender *PluginRender::GetInstance(std::string &id) {
  if (instance_.find(id) == instance_.end()) {
    PluginRender *instance = new PluginRender(id);
    instance_[id] = instance;
    return instance;
  } else {
    return instance_[id];
  }
}

void PluginRender::Export(napi_env env, napi_value exports) {
  if ((env == nullptr) || (exports == nullptr)) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginRender",
                 "Export: env or exports is null");
    return;
  }

  napi_property_descriptor desc[] = {
      {"drawPatternX", nullptr, PluginRender::NapiDrawPattern, nullptr, nullptr,
       nullptr, napi_default, nullptr},
      {"getStatusX", nullptr, PluginRender::TestGetXComponentStatus, nullptr,
       nullptr, nullptr, napi_default, nullptr}};
  if (napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]),
                             desc) != napi_ok) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginRender",
                 "Export: napi_define_properties failed");
  }
}

napi_value PluginRender::NapiDrawPattern(napi_env env,
                                         napi_callback_info info) {
  OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "PluginRender",
               "NapiDrawPattern");
  if ((env == nullptr) || (info == nullptr)) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginRender",
                 "NapiDrawPattern: env or info is null");
    return nullptr;
  }

  napi_value thisArg;
  if (napi_get_cb_info(env, info, nullptr, nullptr, &thisArg, nullptr) !=
      napi_ok) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginRender",
                 "NapiDrawPattern: napi_get_cb_info fail");
    return nullptr;
  }

  napi_value exportInstance;
  if (napi_get_named_property(env, thisArg, OH_NATIVE_XCOMPONENT_OBJ,
                              &exportInstance) != napi_ok) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginRender",
                 "NapiDrawPattern: napi_get_named_property fail");
    return nullptr;
  }

  OH_NativeXComponent *nativeXComponent = nullptr;
  if (napi_unwrap(env, exportInstance,
                  reinterpret_cast<void **>(&nativeXComponent)) != napi_ok) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginRender",
                 "NapiDrawPattern: napi_unwrap fail");
    return nullptr;
  }

  char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
  uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
  if (OH_NativeXComponent_GetXComponentId(nativeXComponent, idStr, &idSize) !=
      OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "PluginRender",
                 "NapiDrawPattern: Unable to get XComponent id");
    return nullptr;
  }

  std::string id(idStr);
  PluginRender *render = PluginRender::GetInstance(id);
  if (render != nullptr) {
    render->eglCore_->Draw(hasDraw_);
    OH_LOG_Print(LOG_APP, LOG_INFO, LOG_PRINT_DOMAIN, "PluginRender",
                 "render->eglCore_->Draw() executed");
  }
  return nullptr;
}

void PluginRender::Release(std::string &id) {
  PluginRender *render = PluginRender::GetInstance(id);
  if (render != nullptr) {
    render->eglCore_->Release();
    delete render->eglCore_;
    render->eglCore_ = nullptr;
    instance_.erase(instance_.find(id));
  }
}

void PluginRender::OnSurfaceChanged(OH_NativeXComponent *component,
                                    void *window) {
  char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
  uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
  if (OH_NativeXComponent_GetXComponentId(component, idStr, &idSize) !=
      OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN, "Callback",
                 "OnSurfaceChanged: Unable to get XComponent id");
    return;
  }

  std::string id(idStr);
  PluginRender *render = PluginRender::GetInstance(id);
  uint64_t width;
  uint64_t height;
  OH_NativeXComponent_GetXComponentSize(component, window, &width, &height);
  if (render != nullptr) {
    render->eglCore_->UpdateSize(width, height);
  }
}

void PluginRender::OnTouchEvent(OH_NativeXComponent *component, void *window) {
  char idStr[OH_XCOMPONENT_ID_LEN_MAX + 1] = {'\0'};
  uint64_t idSize = OH_XCOMPONENT_ID_LEN_MAX + 1;
  if (OH_NativeXComponent_GetXComponentId(component, idStr, &idSize) !=
      OH_NATIVEXCOMPONENT_RESULT_SUCCESS) {
    return;
  }
  OH_NativeXComponent_TouchEvent touchEvent;
  OH_NativeXComponent_GetTouchEvent(component, window, &touchEvent);
  std::string id(idStr);
  PluginRender *render = PluginRender::GetInstance(id);
  if (render != nullptr &&
      touchEvent.type ==
          OH_NativeXComponent_TouchEventType::OH_NATIVEXCOMPONENT_UP) {
    render->eglCore_->ChangeColor(hasChangeColor_);
  }
}

void PluginRender::RegisterCallback(OH_NativeXComponent *nativeXComponent) {
  memset(&renderCallback_, 0, sizeof(OH_NativeXComponent_Callback));
  renderCallback_.OnSurfaceCreated = OnSurfaceCreatedCB;
  renderCallback_.OnSurfaceChanged = OnSurfaceChangedCB;
  renderCallback_.OnSurfaceDestroyed = OnSurfaceDestroyedCB;
  renderCallback_.DispatchTouchEvent = DispatchTouchEventCB;
  OH_NativeXComponent_RegisterCallback(nativeXComponent, &renderCallback_);
}

napi_value PluginRender::TestGetXComponentStatus(napi_env env,
                                                 napi_callback_info info) {
  napi_value hasDraw;
  napi_value hasChangeColor;

  napi_status ret = napi_create_int32(env, hasDraw_, &(hasDraw));
  if (ret != napi_ok) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN,
                 "TestGetXComponentStatus", "napi_create_int32 hasDraw_ error");
    return nullptr;
  }
  ret = napi_create_int32(env, hasChangeColor_, &(hasChangeColor));
  if (ret != napi_ok) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN,
                 "TestGetXComponentStatus",
                 "napi_create_int32 hasChangeColor_ error");
    return nullptr;
  }

  napi_value obj;
  ret = napi_create_object(env, &obj);
  if (ret != napi_ok) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN,
                 "TestGetXComponentStatus", "napi_create_object error");
    return nullptr;
  }
  ret = napi_set_named_property(env, obj, "hasDraw", hasDraw);
  if (ret != napi_ok) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN,
                 "TestGetXComponentStatus",
                 "napi_set_named_property hasDraw error");
    return nullptr;
  }
  ret = napi_set_named_property(env, obj, "hasChangeColor", hasChangeColor);
  if (ret != napi_ok) {
    OH_LOG_Print(LOG_APP, LOG_ERROR, LOG_PRINT_DOMAIN,
                 "TestGetXComponentStatus",
                 "napi_set_named_property hasChangeColor error");
    return nullptr;
  }
  return obj;
}
} // namespace NativeXComponentSample
