# Project source
Downloaded software from page [2D LIDAR using multiple VL53L1X Time-of-Flight long distance ranging sensors](https://www.st.com/en/embedded-software/stsw-img017.html).

# Import and Discard non-source bits
```
$ mkdir -p firmware/nucleo/
$ cd firmware/nucleo/
$ unzip ../../downloads/en.2D_LiDAR.zip
$ cd Lidar\ Delivery/
$ mv Lidar\ Delivery/ Lidar_Delivery
$ find . -type f -not \( -name "*.[c|h|s]" -o -name "*.ld" \) -exec rm -f {} \+
$ find . -type d -empty -exec rmdir {} \+
```
