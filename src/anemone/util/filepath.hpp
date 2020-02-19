#ifndef UTIL_FILEPATH_H
#define UTIL_FILEPATH_H

#include <string>


namespace filepath {

  inline std::string get_parent_dir(std::string file_path) {
    // get last "/" position
    std::size_t sep_pos = file_path.rfind("/");

    return file_path.substr(0, sep_pos + 1);
  };

}

#endif
