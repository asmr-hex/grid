#ifndef TEST_FIXTURES_CONFIGS_H
#define TEST_FIXTURES_CONFIGS_H

#include <string>


namespace fixture {
  namespace config {
    const std::string base_path    = "test/fixtures/configs/";
    const std::string simple       = base_path + "simple.yml";
    const std::string nested       = base_path + "nested.yml";
    const std::string default_conf = base_path + "default.yml";
  }
}

#endif
