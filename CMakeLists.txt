cmake_minimum_required(VERSION 3.7...3.21)

project(
  Anemone
  VERSION 2.0
  DESCRIPTION "multimodal, tentacled, sonic controller"
  LANGUAGES CXX)

# add main executable...what aboutu large src trees?
add_executable(anemone src/main.cpp)
target_compile_features(anemone PUBLIC cxx_std_17)
