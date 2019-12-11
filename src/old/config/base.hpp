#ifndef BASE_CONFIG_H
#define BASE_CONFIG_H

#include <string>

#include <yaml-cpp/yaml.h>


class BaseConfig {
public:
  BaseConfig(std::string config_path) : config_path(config_path) {};
  
  std::string config_path;
  
  YAML::Node get_mapping_node(YAML::Node node) {
    YAML::Node mapping_node;
    
    // is there a distinct mapping file?
    if (node["mapping_file"]) {
      std::string mapping_file = get_parent_dir(config_path) + node["mapping_file"].as<std::string>();

      // load mapping file
      mapping_node = YAML::LoadFile(mapping_file);
    } else {
      mapping_node = node["mapping"];
    }

    return mapping_node;
  }
  
  std::string get_parent_dir(std::string file_path) {
    // get last "/" position
    std::size_t sep_pos = file_path.rfind("/");

    return file_path.substr(0, sep_pos + 1);
  };
};

#endif
