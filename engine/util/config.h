/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef incl_config_H_
#define incl_config_H_

#include <optional>
#include <stack>
#include "json.hpp"
using json = nlohmann::json;
#include "activity-emitter.h"
using ae = MonkeyCAM::ActivityEmitter;

namespace MonkeyCAM {

class Config {
 public:
  Config(const std::string configName, const std::string filename);

  static void validateIsArray(const json& j);
  static void validateIsObject(const json& j);

  const json& getRootJson() { return m_rootJson; }

  void push(const char* name, const json& j) {
    auto e = m_stack.top();
    auto p = e.m_path + (e.m_path.empty() ? "" : ".") + name;
    m_stack.push({p, j});
  }

  void pop() { m_stack.pop(); }

  template <typename T>
  T getCommon(const char* key,
              std::function<void(const json&)> validator = NULL) {
    try {
      auto e = m_stack.top();
      auto j = e.m_json.at(key);
      if (validator) validator(j);
      return j.get<T>();
    } catch (json::type_error& err) {
      std::string what = err.what();
      std::string msg = what.substr(what.find(']') + 2, std::string::npos);
      auto e = m_stack.top();
      printf("Type error in %s (%s): '%s%s%s', %s\n", m_configName.c_str(),
             m_fileName.c_str(), e.m_path.c_str(), e.m_path.empty() ? "" : ".",
             key, msg.c_str());
      ae::emitter().write({{"error",
                            {{"kind", "Type Error"},
                             {"configName", m_configName},
                             {"path", e.m_path},
                             {"key", key},
                             {"message", msg}}}});
      exit(1);
    }
  }

  template <typename T>
  std::optional<T> getOptional(
      const char* key, std::function<void(const json&)> validator = NULL) {
    try {
      return std::optional<T>(getCommon<T>(key, validator));
    } catch (json::out_of_range& e) {
      return std::nullopt;
    }
  }

  template <typename T>
  T get(const char* key, T defaultValue,
        std::function<void(const json&)> validator = NULL) {
    try {
      return getCommon<T>(key);
    } catch (json::out_of_range& err) {
      return defaultValue;
    }
  }

  template <typename T>
  T get(const char* key, std::function<void(const json&)> validator = NULL) {
    try {
      return getCommon<T>(key, validator);
    } catch (json::out_of_range& err) {
      auto e = m_stack.top();
      printf("Missing parameter in %s (%s): '%s%s%s'\n", m_configName.c_str(),
             m_fileName.c_str(), e.m_path.c_str(), e.m_path.empty() ? "" : ".",
             key);
      ae::emitter().write({{"error",
                            {{"kind", "Missing Parameter"},
                             {"configName", m_configName},
                             {"path", e.m_path},
                             {"key", key}}}});
      exit(1);
    }
  }

  class ObjectHolder {
   public:
    ObjectHolder(Config& config, const char* key)
        : m_config(config), m_pop(true) {
      auto v = config.get<json>(key, Config::validateIsObject);
      config.push(key, v);
    }

    ObjectHolder(Config& config, const char* key,
                 std::function<void()> optionalWork)
        : m_config(config), m_pop(false) {
      auto v = config.getOptional<json>(key, Config::validateIsObject);
      if (v) {
        m_pop = true;
        config.push(key, *v);
        optionalWork();
      }
    }

    ~ObjectHolder() {
      if (m_pop) m_config.pop();
    }

   private:
    Config& m_config;
    bool m_pop;
  };

  class ArrayHolder {
   public:
    ArrayHolder(Config& config, const char* key,
                std::function<void(json& array)> optionalWork)
        : m_config(config), m_pop(false) {
      auto v = config.getOptional<json>(key, Config::validateIsArray);
      if (v) {
        m_pop = true;
        config.push(key, *v);
        optionalWork(*v);
      }
    }

    ~ArrayHolder() {
      if (m_pop) m_config.pop();
    }

   private:
    Config& m_config;
    bool m_pop;
  };

 private:
  const std::string m_configName;
  const std::string m_fileName;
  json m_rootJson;

  struct Entry {
    const std::string m_path;
    json m_json;
  };
  std::stack<Entry> m_stack;
};

}  // namespace MonkeyCAM

#endif  // incl_config_H_
