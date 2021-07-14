#!/usr/bin/env bash

DEV_NAME=ttyUSB0

#docker build -t ghcr.io/asmr-hex/grid/dev:latest -f dockerfiles/dev .
docker run \
       -it \
       --rm \
       --privileged \
       --pull always \
       -v "$PWD":/grid \
       -v /run/udev:/run/udev:ro \
       --device=/dev/$DEV_NAME:/dev/$DEV_NAME \
       ghcr.io/asmr-hex/grid/dev:latest \
       nodemon -L --watch src --exec "make run || exit 1" --ext hpp,cpp,h,c

# NOTES:
# the libmonome code (particularly for platform/linux_libudev) tries to resolve some
# ENV properties on the udev device-- specifically ID_SERIAL_SHORT. If run on the host
# machine, this property will be available, but inside the Docker container, this property
# (along with all other ID_* properties are stripped from the udev info
# (see with udevadm info /dev/ttyUSB0 for example)). So the work around is to mount a read-only
# volume of the udev data (at /run/udev) into the container so that it has access to all the data.
