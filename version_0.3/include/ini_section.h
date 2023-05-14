/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 * Brief: INI 格式的配置文件解析器
 */

#pragma once

#include <istream>
#include <map>
#include <ostream>
#include <string>

namespace csguide_webserver {

class INISection {
public:
  INISection(const std::string& name = "") : name_(name) {}

  bool Parse(const std::string& input_line);

  bool ParseKV(const std::string& line);

  friend std::ostream& operator<<(std::ostream& os, const INISection& section);

  const std::string& GetName() const { return name_; }
  const std::string& GetValue(const std::string& key) const;
  void SetValue(const std::string& key, const std::string& value);

private:
  std::string name_;
  bool section_start_ = false;  // 已经读到这一节开始
  std::map<std::string, std::string> key_value_pairs_;
};
}  // namespace csguide_webserver