#ifndef SP404_CONFIG_H
#define SP404_CONFIG_H

#include <string>

#include <yaml-cpp/yaml.h>

#include "base.hpp"
#include "mappings/sp404.hpp"


class SP404Config : BaseConfig {
public:
  SP404Config(YAML::Node conf, std::string config_path) : BaseConfig(config_path) {
    parse(conf);
  };

  static std::string name() {
    return "sp404";
  }
private:
  void parse(YAML::Node conf) {
    // get mapping node
    YAML::Node mapping = get_mapping_node(conf);
  };
};

#endif
