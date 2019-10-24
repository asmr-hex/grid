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


### Resources
* [Molisam](https://github.com/AskBre/Molisam) - sampler using libmonome, ([demo](https://llllllll.co/t/introducing-molisam-simple-monome-live-sampler-written-i-c/4249))
* [libmonome public header](https://github.com/monome/libmonome/blob/master/public/monome.h)
* [monome osc protocol](https://monome.org/docs/osc/)
