#ifndef CONFIG_BASE_H
#define CONFIG_BASE_H

#include <string>

#include <yaml-cpp/yaml.h>


namespace Conf {
  
  class Base {
  public:
    std::string path;
    YAML::Node yml;
  
    Base(std::string config_path);
  
    YAML::Node get_layout(YAML::Node node);
    std::string get_parent_dir(std::string file_path);

    YAML::Node operator[](std::string);
  };
  
}

#endif
