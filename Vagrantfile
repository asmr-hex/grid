Vagrant.configure("2") do |config|
  config.vm.box = "geerlingguy/ubuntu1604"
  config.vm.hostname = "grid-dev.local"
  config.vm.provision "docker", images: ["ghcr.io/asmr-hex/grid/dev:latest"]

  # share directories
  config.vm.synced_folder ".", "/grid"

  # use VirtualBox as the VM provider. TODO (coco|2021.07.14) use Docker provider (falls back to boot2docker on osx?)
  config.vm.provider "virtualbox" do |v|
    v.gui = false

    # set usb device capturing
    # use `VBoxManage list usbhost` to list host usb devices
    v.customize ["modifyvm", :id, "--usb", "on"]
    v.customize ["modifyvm", :id, "--usbehci", "on"]
    v.customize ["modifyvm", :id, "--audio", "coreaudio"]

    # add usbfilters to add usb devices
    # execute `VBoxManage list usbhost` on host for usb device info
    # NOTE: these devices will NOT be accessible from host.
    v.customize ["usbfilter", "add", "0",
                 "--target", :id,
                 "--name", "Monome",
                 "--manufacturer", "monome",
                 "--product", "monome"]
    v.customize ["usbfilter", "add", "1",
                 "--target", :id,
                 "--name", "AudioBox Interface",
                 "--manufacturer", "PreSonus",
                 "--product", "AudioBox USB 96"]
  end
end
