#ifndef MS20_CONFIG_H
#define MS20_CONFIG_H

#include <string>

#include <yaml-cpp/yaml.h>

#include "base.hpp"
#include "mappings/ms20.hpp"


class MS20Config : BaseConfig {
public:
  MS20Config(YAML::Node conf, std::string config_path) : BaseConfig(config_path) {
    parse(conf);
  };
private:
  void parse(YAML::Node conf) {
    // get mapping node
    YAML::Node mapping = get_mapping_node(conf);
  };
};

#endif
