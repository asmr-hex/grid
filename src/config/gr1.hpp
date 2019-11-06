#ifndef GR1_CONFIG_H
#define GR1_CONFIG_H

#include <string>

#include <yaml-cpp/yaml.h>

#include "base.hpp"
#include "mappings/gr1.hpp"


class GR1Config : BaseConfig {
public:
  GR1Config(YAML::Node conf, std::string config_path) : BaseConfig(config_path) {
    parse(conf);
  };

  static std::string name() {
    return "gr1";
  }
private:
  void parse(YAML::Node conf) {
    // get mapping node
    YAML::Node mapping = get_mapping_node(conf);
  };
};

#endif
