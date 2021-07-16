#! /usr/bin/env bash

INSTALL=false
CONTAINER=false

function usage {
  echo "'build' generates build system files and installs builds."
  echo ""
  echo "Usage: build [<command>]"
  echo ""
  echo "  install          installs built src"
  echo "  container        builds and pushes the dev envireonment Docker container (must run on host)"
}

function build {
  cmake -S . -B build     # generate build files
  cmake --build build -v  # build src from build files
}

function install {
  cmake --install build  # install built src (CMake 3.15+ only)
}

function build_and_push_container {
  # build container image
  docker build -t ghcr.io/asmr-hex/grid/dev:latest -f dockerfiles/dev .

  # login to github container registry
  pass show github/registry | docker login ghcr.io --username asmr-hex --password-stdin

  # push to github container registry
  docker push ghcr.io/asmr-hex/grid/dev:latest
}

function run {
  if [ "$CONTAINER" = true ]
  then
    build_and_push_container
    exit 0
  fi

  # run build
  build

  # run install if specified
  if [ "$INSTALL" = true ]
  then
    install
  fi
}

function parse_args {
  while [[ $# -gt 0 ]]
  do
    local next_arg="$1"
    case $next_arg in
      install)
        INSTALL=true
        shift
        shift
        ;;
      container)
        CONTAINER=true
        shift
        shift
        ;;
    esac
  done
}

parse_args $@
run
