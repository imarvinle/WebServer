/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 * Brief: INI 格式的配置文件解析器
 */

#pragma once

#include <vector>

#include "ini_section.h"

namespace csguide_webserver {

class INIFile {
public:
  bool Load(const std::string& file_path);
  bool Save(const std::string& file_path) const;

  const INISection* GetSection(const std::string& name) const;
  void AddSection(const INISection& section);

private:
  std::vector<INISection> sections_;
};
}  // namespace csguide_webserver