### Installation
##### Requirements
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
  $ ????                   on raspbian
  ```

### Sequencing Code
Use RtMidi lib adn build scheduler.... -__-

### Resources
* [Molisam](https://github.com/AskBre/Molisam) - sampler using libmonome, ([demo](https://llllllll.co/t/introducing-molisam-simple-monome-live-sampler-written-i-c/4249))
* [libmonome public header](https://github.com/monome/libmonome/blob/master/public/monome.h)
* [monome osc protocol](https://monome.org/docs/osc/)
* [ALSA](https://www.alsa-project.org/alsa-doc/alsa-lib/seq.html) - use this for midi sequencing.
* [Example Sequencer with pyRtMidi](https://github.com/SpotlightKid/python-rtmidi/blob/master/examples/sequencer/sequencer.py)
* [Real Time Clocks for Audio Synthesis SO](https://stackoverflow.com/questions/48448491/c-precise-44100hz-clock-for-real-time-audio-synthesis) - describes how to use c++ high resolution clocks for high precision timing events.
* [Midi Table](http://fmslogo.sourceforge.net/manual/midi-table.html) - midi spec
