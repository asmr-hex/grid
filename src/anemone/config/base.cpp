// #include "anemone/config/base.hpp"
// #include "anemone/util/filepath.hpp"


// Conf::Base::Base(std::string path) : path(path) {
//   yml = YAML::LoadFile(path);
// }

// Conf::Base::Base(YAML::Node yml, std::string path) : path(path), yml(yml) {}


// Conf::Base Conf::Base::operator[](std::string field) {
//   // check to see if the field with "_file" exists.
//   auto field_file = field + "_file";

//   if (yml[field_file]) {
//     auto file_path = filepath::get_parent_dir(path) + yml[field_file].as<std::string>();

//     return Conf::Base(file_path);
//   }

//   return Conf::Base(yml[field], path);
// }
