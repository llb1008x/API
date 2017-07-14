
sudo ./fastboot flash modem NON-HLOS.bin
sudo ./fastboot flash splash splash.img
sudo ./fastboot flash sbl1 sbl1.mbn
sudo ./fastboot flash sbl1bak sbl1.mbn
sudo ./fastboot flash rpm  rpm.mbn
sudo ./fastboot flash rpmbak rpm.mbn
sudo ./fastboot flash tz tz.mbn
sudo ./fastboot flash tzbak tz.mbn
sudo ./fastboot flash devcfg devcfg.mbn
sudo ./fastboot flash devcfgbak devcfg.mbn

sudo ./fastboot flash dsp adspso.bin
sudo ./fastboot flash aboot emmc_appsboot.mbn
sudo ./fastboot flash abootbak emmc_appsboot.mbn
sudo ./fastboot flash boot  boot.img
sudo ./fastboot flash recovery recovery.img
sudo ./fastboot flash system system.img
sudo ./fastboot flash cache cache.img
sudo ./fastboot flash persist persist.img
sudo ./fastboot flash mdtp mdtp.img


sudo ./fastboot flash cmnlib cmnlib.mbn
sudo ./fastboot flash cmnlibbak cmnlib.mbn

sudo ./fastboot flash cmnlib64 cmnlib64.mbn
sudo ./fastboot flash cmnlib64bak cmnlib64.mbn
sudo ./fastboot flash keymaster keymaster.mbn
sudo ./fastboot flash keymasterbak  keymaster.mbn

sudo ./fastboot flash userdata userdata.img

sudo ./fastboot reboot
