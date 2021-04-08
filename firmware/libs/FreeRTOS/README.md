* FreeRTOS port steps
```
$ wget https://github.com/FreeRTOS/FreeRTOS-Kernel/archive/refs/tags/V10.4.3.tar.gz -O downloads/FreeRTOS-v10.4.3.tar.gz
$ tar xfz downloads/FreeRTOS-v10.4.3.tar.gz -C firmware/libs/
$ mv firmware/libs/FreeRTOS-Kernel-10.4.3 firmware/libs/FreeRTOS
$ find firmware/libs/FreeRTOS -type f -not  -name "*.c" -maxdepth 1 -exec rm -f {} \+
$ find firmware/libs/FreeRTOS/portable -not \( -name portable -o -name GCC -o -name Common -o -name MemMang \) -maxdepth 1 -exec rm -rf {} \+
$ find firmware/libs/FreeRTOS/portable/GCC -type d -not \( -name GCC -o -name ARM_CM4F -o -name ARM_CM7 \) -maxdepth 1 -exec rm -rf {} \+
$ rm -rf firmware/libs/FreeRTOS/.github
```

* Download a `FreeRTOSConfig.h` for building purpose. This will be fine-tuned more later. Borrowed one from [this repo](https://github.com/jakub-m/nucleo).
```
$ mkdir -p firmware/libs/FreeRTOS/config
$ wget -L https://raw.githubusercontent.com/jakub-m/nucleo/master/inc/FreeRTOSConfig.h -O firmware/libs/FreeRTOS/config/FreeRTOSConfig.h
```
