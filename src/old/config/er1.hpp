#ifndef ER1_CONFIG_H
#define ER1_CONFIG_H

#include <string>

#include <yaml-cpp/yaml.h>

#include "base.hpp"
#include "mappings/er1.hpp"


class ER1Config : BaseConfig {
public:
  ER1Config(YAML::Node conf, std::string config_path) : BaseConfig(config_path) {
    parse(conf);
  };

  static std::string name() {
    return "er1";
  }
private:
  void parse(YAML::Node conf) {
    // get mapping node
    YAML::Node mapping = get_mapping_node(conf);
  };
};

#endif
