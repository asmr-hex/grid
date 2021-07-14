## Notes
dependencies are provisioned and installed within a docker image. on linux workstations, you can just run the dev environment within docker. however on OSX workstations, you need to run a linux virtual machine which, in turn, runs the docker dev container. this is because docker cannot share devices with mac os (at time of writing this is not possible). so, we need to run a virtual machine with usb devices forwarded.

this uses vagrant to spin up the docker host vm on osx. so if you are on osx, you need to install virtualbox and vagrant (and optionally vagrant manager).
``` shell
brew install virtualbox
brew install virtualbox-extension-pack # for USB 2.0 support
brew install vagrant
brew install vagrant-manager # optional
vagrant up
vagrant ssh
cd /grid
./run.sh
```

### Installation
##### Requirements
**UPDATE: all these package requirements are now installed automatically for linux (particularly raspbian) within the `Makefile`**
* [liblo](http://liblo.sourceforge.net/) - lightweight OSC implementation
   ``` shell
   $ brew install liblo  # on OSX
   $ ????                # on raspbian
   ```
* [libmonome](https://github.com/monome/libmonome) - monome c library
   ```shell
   $ git clone https://github.com/monome/libmonome.git
   $ cd libmonome
   $ ./waf configure
   $ ./waf
   $ sudo ./waf install

   $ # may require updating cython with:
   $ pip install --upgrade cython  # i guess for python bindings?
   $ # on OSX, you may need to symlink the dynamic gcc if xcode was updated -__-
   $ cd /usr/local/lib && sudo ln -s ../../lib/libSystem.B.dylib libgcc_s.10.5.dylib
   ```
* [Boost](https://www.boost.org/doc/libs/1_71_0/more/getting_started/unix-variants.html) - using Boost threads for multi-platform compatibility
  ```shell
  $ brew install boost  # on osx
  $ ????                # on raspbian
  ```
* [RtMidi](https://www.music.mcgill.ca/~gary/rtmidi/) - portable, cross-platform real time midi
  ```shell
  $ brew install rtmidi  # on osx
  $ ????                 # on raspbian
  ```
* [yaml-cpp](https://github.com/jbeder/yaml-cpp) - a yaml serialization library
  ```shell
  $ brew install yaml-cpp  # on osx
  $ ????                   # on raspbian
  ```
* [spdlog](https://github.com/gabime/spdlog) - a fast logging library
  ``` shell
  $ brew install spdlog  #on osx
  $ ???                  # on raspbian
  ```

### Troubleshooting
##### Running on a RaspberryPi
since we will be directly communicating with the monome via `usb-serial` we need to provide the path to the `usb-serial` device when connecting to it. this is typically in `/dev/[ttyusb...]` or something. when initially plugging in the monome via usb to the rpi, i noticed that no `usb-serial` device was available. however, `lsusb` output definitely confirmed that it was a connected and recognized usb device.

``` shell
$ lsusb
Bus 001 Device 006: ID 0403:6001 Future Technology Devices International, Ltd FT232 Serial (UART) IC # this is the monome
Bus 001 Device 003: ID 0424:ec00 Standard Microsystems Corp. SMSC9512/9514 Fast Ethernet Adapter
Bus 001 Device 002: ID 0424:9514 Standard Microsystems Corp. SMC9514 Hub
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
```
in order to get a `usb-serial` device to show up in `/dev` i had to add the monome's device id as an [FTDI](https://en.m.wikipedia.org/wiki/FTDI) serial driver. to do this, you get the device id (in this case `0403:6001`) and add it as a new id:

``` shell
$ sudo sh -c "echo 0403 6001 > /sys/bus/usb-serial/drivers/ftdi_sio/new_id"
```
then the `usb-serial` device should show up in `/dev`!

### Resources
* [Molisam](https://github.com/AskBre/Molisam) - sampler using libmonome, ([demo](https://llllllll.co/t/introducing-molisam-simple-monome-live-sampler-written-i-c/4249))
* [libmonome public header](https://github.com/monome/libmonome/blob/master/public/monome.h)
* [monome osc protocol](https://monome.org/docs/osc/)
* [ALSA](https://www.alsa-project.org/alsa-doc/alsa-lib/seq.html) - use this for midi sequencing.
* [Example Sequencer with pyRtMidi](https://github.com/SpotlightKid/python-rtmidi/blob/master/examples/sequencer/sequencer.py)
* [Real Time Clocks for Audio Synthesis SO](https://stackoverflow.com/questions/48448491/c-precise-44100hz-clock-for-real-time-audio-synthesis) - describes how to use c++ high resolution clocks for high precision timing events.
* [Midi Table](http://fmslogo.sourceforge.net/manual/midi-table.html) - midi spec
