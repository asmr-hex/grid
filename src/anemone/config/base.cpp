#include "anemone/config/base.hpp"


Conf::Base::Base(std::string path) : path(path) {
  yml = YAML::LoadFile(path);
}

YAML::Node Conf::Base::get_layout(YAML::Node node) {
  YAML::Node layout_node;
    
  // is there a distinct layout file?
  if (node["layout_file"]) {
    std::string layout_file = get_parent_dir(path) + node["layout_file"].as<std::string>();

    // load mapping file
    layout_node = YAML::LoadFile(layout_file);
  } else {
    layout_node = node["layout"];
  }

  return layout_node;
}


std::string Conf::Base::get_parent_dir(std::string file_path) {
  // get last "/" position
  std::size_t sep_pos = file_path.rfind("/");

  return file_path.substr(0, sep_pos + 1);
}


YAML::Node Conf::Base::operator[](std::string field) {
  return yml[field];
}
