/**
 * @file   config.hpp
 * @brief  Anemone Configuration
 * @author coco
 * @date   2020-02-19
 *************************************************/


#ifndef ANEMONE_CONFIG_H
#define ANEMONE_CONFIG_H

#include <string>

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

// we only need to import grid device coordinates so we can parse grid regions
// from the configuration file. since classes within the `types` directory need
// to include the Config, we will not include the entire `types.hpp` in order to
// avoid circular dependencies.
#include "anemone/types/io/grid/device/coordinates.hpp"


/// @brief configuration for anemone.
///
/// @details TODO write up examples.
///
class Config {
public:
  /// @brief path to yaml configuration file
  std::string path;

  /// @brief parsed YAML node.
  YAML::Node yml;

  /// @brief constructs a Config given a filepath to a YAML config file.
  ///
  /// @param file_path   a string of the file path to the configuration file.
  ///
  Config(std::string path)
    : path(path),
      yml(YAML::LoadFile(path)) {};

  /// @brief constructs a Config from a previously parsed YAML node and the path
  /// to the configuration file it is contained within. This is usually used to
  /// extract a sub-tree from a larger config.
  ///
  /// @param yml_node    a YAML::Node representation of a config.
  /// @param file_path   a string of the file path this config object is contained within.
  ///
  Config(YAML::Node yml, std::string path)
    : path(path),
      yml(yml) {};

    // when accessing a Base config using the [] operator, we directly
    // access the YAML root node of this object. Additionally, we will
    // check to see whether the accessed field has its contents within
    // another yaml file and if so read in that file as a new Base config
    // object.

  /// @brief gets a sub-tree of a parsed yaml config and returns a new Config object.
  ///
  /// @param field   a string representation of the field we wish to select.
  Config operator[](std::string field) {
      // check to see if the field with "_file" exists.
      auto field_file = field + "_file";

      // if the field has it's own dedicated file, read the file and return the
      // parsed YAML node from that file.
      if (yml[field_file]) {
        auto file_path = get_parent_dir(path) + yml[field_file].as<std::string>();

        return Config(file_path);
      }

      return Config(yml[field], path);
  };
  
  /// @brief similar to the [] operator, but works with pointers to configs.
  ///
  /// @param field   a string representation of a field.
  ///
  Config at(std::string field) {
    return (*this)[field];
  };
    // wrap the YAML::Node::as method so that we are able to perform the
    // same type conversions.

  /// @brief allows a config to perform type conversions on fields.
  ///
  /// @remark since we aren't inheriting from a YAML::Node, but we still want to
  /// expose its type conversion capabilities, we are just implementing a wrapper
  /// over the YAML::Node::as method. there is probably a better way to do this.
  ///
  template<typename T>
  T as() {
    return yml.as<T>();
  };

  /// @brief given a name of a layout section within the config, returns a parsed
  /// grid_region_t struct.
  ///
  /// @remark the YAML block specified by the provided section name must be a well
  /// formed layout section configuration. for example, the following section configurations
  /// are valid:
  ///
  /// sample_region:
  ///   x:
  ///     min: 0
  ///     max: 2
  ///   y:
  ///     min: 3
  ///     max: 8
  ///
  /// another_sample_region:
  ///   x: 3
  ///   y: 8
  ///
  /// yet_another_sample_region:
  ///   x: 3
  ///   y:
  ///     min: 3
  ///     max: 8
  ///
  /// @param section   the name of the layout section we wish to parse as a grid region.
  ///
  /// @return grid_region_t
  ///
  grid_region_t parse_grid_region(std::string section) {
  
    if ( (*this)[section].yml.IsNull() ) throw("grid region '" + section + "' undefined!");
  
    YAML::Node x = (*this)[section]["x"].yml;
    YAML::Node y = (*this)[section]["y"].yml;

    unsigned int x_min, x_max, y_min, y_max;

    try {
      if ( x.IsScalar() ) {
        x_min = x.as<unsigned int>();
        x_max = x.as<unsigned int>();
      } else {
        x_min = x["min"].as<unsigned int>();
        x_max = x["max"].as<unsigned int>();    
      }

      if ( y.IsScalar() ) {
        y_min = y.as<unsigned int>();
        y_max = y.as<unsigned int>();
      } else {
        y_min = y["min"].as<unsigned int>();
        y_max = y["max"].as<unsigned int>();    
      }      
    } catch (YAML::TypedBadConversion<unsigned int> & e) {
      spdlog::error("Error parsing '{}' section from YAML!", section);
      exit( EXIT_FAILURE );
    }

    return {
            .min = { .x = x_min, .y = y_min},
            .max = { .x = x_max, .y = y_max},
    };
  }
  
  /// @brief helper function for navigation up directories.
  ///
  /// @param file_path   the string of a file path to get the parent directory of.
  ///
  std::string get_parent_dir(std::string file_path) {
    // get last "/" position
    std::size_t sep_pos = file_path.rfind("/");

    return file_path.substr(0, sep_pos + 1);
  };
};

#endif
