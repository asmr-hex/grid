#!/usr/bin/env bash

docker build -t grid-dev -f dockerfiles/dev .
docker run -it --rm -v "$PWD":/grid --privileged -v /dev:/dev grid-dev /bin/bash
