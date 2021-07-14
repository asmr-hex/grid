#!/usr/bin/env bash

DEV_NAME=ttyUSB0

docker build -t grid-dev -f dockerfiles/dev .
docker run -it --rm -v "$PWD":/grid --privileged -v /run/udev:/run/udev:ro --device=/dev/$DEV_NAME:/dev/$DEV_NAME grid-dev /bin/bash

# NOTES:
# the libmonome code (particularly for platform/linux_libudev) tries to resolve some
# ENV properties on the udev device-- specifically ID_SERIAL_SHORT. If run on the host
# machine, this property will be available, but inside the Docker container, this property
# (along with all other ID_* properties are stripped from the udev info
# (see with udevadm info /dev/ttyUSB0 for example)). So the work around is to mount a read-only
# volume of the udev data (at /run/udev) into the container so that it has access to all the data.
