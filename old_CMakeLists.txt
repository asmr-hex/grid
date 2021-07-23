cmake_minimum_required(VERSION 3.7...3.21)

project(
  Anemone
  VERSION 2.0
  DESCRIPTION "multimodal, tentacled, sonic controller"
  LANGUAGES CXX)

target_compile_features(anemone PUBLIC cxx_std_17)

add_subdirectory(src)


# Idea:
# What if I built all the modules of anemone into libraries
# then added them as dependencies to the main executable target?
