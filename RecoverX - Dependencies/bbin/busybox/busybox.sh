#!/data/local/tmp/sh
# RecoverX Installation Script

mount -o remount rw /system

mkdir /system/xbin

cat /data/local/tmp/busybox > /system/xbin/busybox
chmod 755 /system/xbin/busybox

/system/xbin/busybox --install /system/xbin

mount -o remount ro /system