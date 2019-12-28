#ifndef CONFIG_BASE_H
#define CONFIG_BASE_H

#include <string>

#include <yaml-cpp/yaml.h>

#include "anemone/util/filepath.hpp"


namespace Conf {
  
  class Base {
  public:
    std::string path;
    YAML::Node yml;

    Base(std::string path) : path(path) {
      yml = YAML::LoadFile(path);
    }

    Base(YAML::Node yml, std::string path) : path(path), yml(yml) {}


    Base operator[](std::string field) {
      // check to see if the field with "_file" exists.
      auto field_file = field + "_file";

      if (yml[field_file]) {
        auto file_path = filepath::get_parent_dir(path) + yml[field_file].as<std::string>();

        return Base(file_path);
      }

      return Base(yml[field], path);
    };

    
    template<typename T>
    T as() {
      return yml.as<T>();
    };
  };
  
}

#endif
