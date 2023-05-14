
/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 * Brief: INI 格式的配置文件解析器
 */

#include "../../include/ini_section.h"
#include "../../include/logger.h"
#include "../../include/util.h"

namespace csguide_webserver {

bool INISection::ParseKV(const std::string& line) {
  auto equal_pos = line.find('=');
  if (equal_pos == std::string::npos) {
    return false;
  }

  std::string key = line.substr(0, equal_pos);
  std::string value = line.substr(equal_pos + 1);

  // 去除首尾空白字符
  Trim(key);
  Trim(value);
  Logger::GetInstance().LogRun("get key value, section=%s, key=%s, value=%s", name_.c_str(), key.c_str(),
                               value.c_str());
  SetValue(key, value);
  return true;
}

bool INISection::Parse(const std::string& input_line) {
  std::string line = input_line;

  // 去除行首和行尾空格
  Trim(line);

  // 空行或者 # 注释行 跳过
  if (line.empty() || line[0] == '#') {
    Logger::GetInstance().LogRun("empty line: %s", line.c_str());
    return true;
  }

  // 首次读到起始行 类似：[Worker], 取出其中的 KEY
  if (line[0] == '[' && !section_start_) {
    // 如果不是 ] 结尾，则报错
    if (line[line.size() - 1] != ']') {
      Logger::GetInstance().LogErr("section not end with ], line: %s", line.c_str());
      // 继续读下一行
      return true;
    }

    // 提取这一节的 Section Name
    name_ = ExtractSubstring(line, '[', ']');
    section_start_ = true;
    return true;
  }

  // 非首次读到 [ 开头，说明是下一个 Section开始了，直接返回
  if (line[0] == '[') {
    Logger::GetInstance().LogRun("[%s] section end, this line is next section, line: %s", name_.c_str(), line.c_str());
    return false;
  }

  // 正常情况解析这一行的 key和 value
  ParseKV(line);
  return true;
}

std::istream& operator>>(std::istream& is, INISection& section) {
  std::string line;
  while (std::getline(is, line)) {
    // 去除行首和行尾空格
  }

  return is;
}

std::ostream& operator<<(std::ostream& os, const INISection& section) {
  os << '[' << section.GetName() << "]\n";
  for (const auto& kv : section.key_value_pairs_) {
    os << kv.first << '=' << kv.second << '\n';
  }
  return os;
}

const std::string& INISection::GetValue(const std::string& key) const {
  auto it = key_value_pairs_.find(key);
  if (it == key_value_pairs_.end()) {
    static const std::string empty;
    return empty;
  }

  return it->second;
}

void INISection::SetValue(const std::string& key, const std::string& value) { key_value_pairs_[key] = value; }
}  // namespace csguide_webserver