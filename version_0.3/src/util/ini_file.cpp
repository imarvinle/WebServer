
/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 * Brief: INI 格式的配置文件解析器
 */

#include <fstream>
#include <iostream>

#include "../../include/ini_file.h"

namespace csguide_webserver {

bool INIFile::Load(const std::string& file_path) {
  std::ifstream ifs(file_path);
  if (!ifs.is_open()) {
    return false;
  }

  INISection section;
  std::string line;
  while (std::getline(ifs, line)) {
    if (section.Parse(line)) {
      continue;
    } else {
      // 解析完一个 Section，新构造一个
      AddSection(section);
      section = INISection();
      section.Parse(line);
    }
  }
  // 最后这个 Section 要加入
  AddSection(section);
  return true;
}

bool INIFile::Save(const std::string& file_path) const {
  std::ofstream ofs(file_path);
  if (!ofs.is_open()) {
    return false;
  }

  for (const auto& section : sections_) {
    ofs << section << '\n';
  }

  return true;
}

const INISection* INIFile::GetSection(const std::string& name) const {
  for (const auto& section : sections_) {
    if (section.GetName() == name) {
      return &section;
    } else {
      std::cout << "sectionname: " << section.GetName() << std::endl;
    }
  }
  return nullptr;
}

void INIFile::AddSection(const INISection& section) { sections_.push_back(section); }
}  // namespace csguide_webserver