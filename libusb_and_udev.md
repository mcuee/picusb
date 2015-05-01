# libusb and udev #

One issue about libusb under Linux and BSDs is how to run the program without using the root privilege.

Under Linux, the standard solution is to use udev rules.

Here are some good references about udev.
1. udev homepage
http://www.kernel.org/pub/linux/utils/kernel/hotplug/udev.html

2. Writing udev rules
http://www.reactivated.net/udevrules.php

3. Proper place to ask questions about udev rules
http://vger.kernel.org/vger-lists.html#linux-hotplug

4. The following pages are all a bit old and may not be correct in some cases. Still they may be of historical value. In many cases, simple udev rules will work.

http://sourceforge.net/apps/mediawiki/piklab/index.php?title=USB_Port_Problems

http://piklab.sf.net/files/libusb_udev_hotplug.txt (old, outdated)

http://www.gphoto.org/doc/manual/permissions-usb.html (touches Linux and FreeBSD)

5. More for FreeBSD

http://people.freebsd.org/~thierry/nut_FreeBSD_HowTo.txt