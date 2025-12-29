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

#include "config.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "activity-emitter.h"
using ae = MonkeyCAM::ActivityEmitter;
#include "json.hpp"
using json = nlohmann::json;

using std::string;

namespace MonkeyCAM {
namespace {

string trimWhitespace(const string& value) {
  auto start = value.begin();
  auto end = value.end();
  while (start != end && std::isspace(static_cast<unsigned char>(*start))) {
    ++start;
  }
  while (end != start && std::isspace(static_cast<unsigned char>(*(end - 1)))) {
    --end;
  }
  return string(start, end);
}

string toLower(string value) {
  std::transform(value.begin(), value.end(), value.begin(),
                 [](unsigned char c) {
                   return static_cast<char>(std::tolower(c));
                 });
  return value;
}

double lengthInCentimeters(double value, LengthUnit unit) {
  switch (unit) {
    case LengthUnit::Centimeters:
      return value;
    case LengthUnit::Millimeters:
      return value / 10.0;
    case LengthUnit::Inches:
      return value * 2.54;
  }
  return 0.0;
}

double parseLengthValue(const json& j, LengthUnit defaultUnit) {
  if (j.is_number()) {
    return lengthInCentimeters(j.get<double>(), defaultUnit);
  }
  if (j.is_string()) {
    auto raw = trimWhitespace(j.get<string>());
    if (raw.empty()) {
      throw json::type_error::create(302, "length must not be empty", &j);
    }
    size_t idx = 0;
    double value = 0.0;
    try {
      value = std::stod(raw, &idx);
    } catch (const std::exception&) {
      throw json::type_error::create(
          302, "length must be a number with optional units (cm, mm, in)", &j);
    }
    auto suffix = trimWhitespace(raw.substr(idx));
    LengthUnit unit = defaultUnit;
    if (!suffix.empty()) {
      auto unitText = toLower(suffix);
      if (unitText == "cm") {
        unit = LengthUnit::Centimeters;
      } else if (unitText == "mm") {
        unit = LengthUnit::Millimeters;
      } else if (unitText == "in") {
        unit = LengthUnit::Inches;
      } else {
        throw json::type_error::create(
            302, "length unit must be cm, mm, or in", &j);
      }
    }
    return lengthInCentimeters(value, unit);
  }
  throw json::type_error::create(
      302, "length must be number or string", &j);
}

}  // namespace

Config::Config(const string configName, const string fileName)
    : m_configName(configName), m_fileName(fileName) {
  if (m_fileName.empty()) {
    m_stack.push({"", m_rootJson});
    return;
  }
  std::ifstream f(m_fileName.c_str());
  try {
    f >> m_rootJson;
    f.close();
    m_stack.push({"", m_rootJson});
  } catch (json::parse_error& e) {
    string what = e.what();
    string msg = what.substr(what.find(':') + 2, string::npos);
    f.seekg(0);
    auto offset = e.byte;
    auto lineNumber = 1;
    auto remainingOffset = e.byte;
    char c;
    while (f.get(c) && --offset) {
      if (c == '\n') {
        lineNumber++;
        remainingOffset = offset;
      }
    }
    printf("Parse error in %s on line %d character %lu: %s\n", fileName.c_str(),
           lineNumber, remainingOffset, msg.c_str());
    ae::emitter().write({{"error",
                          {{"kind", "Parse Error"},
                           {"configName", configName},
                           {"lineNumber", lineNumber},
                           {"offset", remainingOffset},
                           {"message", msg}}}});
    exit(1);
  }
}

double Config::getLengthCommon(const char* key, LengthUnit defaultUnit) {
  try {
    auto e = m_stack.top();
    auto j = e.m_json.at(key);
    return parseLengthValue(j, defaultUnit);
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

double Config::getLength(const char* key, LengthUnit defaultUnit) {
  try {
    return getLengthCommon(key, defaultUnit);
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

double Config::getLength(const char* key, double defaultValue,
                          LengthUnit defaultUnit) {
  try {
    return getLengthCommon(key, defaultUnit);
  } catch (json::out_of_range& err) {
    return defaultValue;
  }
}

std::optional<double> Config::getOptionalLength(const char* key,
                                                LengthUnit defaultUnit) {
  try {
    return std::optional<double>(getLengthCommon(key, defaultUnit));
  } catch (json::out_of_range& err) {
    return std::nullopt;
  }
}

/* static */ void Config::validateIsArray(const json& j) {
  if (!j.is_array()) {
    throw json::type_error::create(
      302, "type must be array, but is " + string(j.type_name()), &j);
  }
}

/* static */ void Config::validateIsObject(const json& j) {
  if (!j.is_object()) {
    throw json::type_error::create(
      302, "type must be object, but is " + string(j.type_name()), &j);
  }
}

}  // namespace MonkeyCAM
