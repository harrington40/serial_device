#include <linux/module.h>
#include <linux/usb.h>


#define USB_TELE_VENDOR_ID    0xffff  /* Manufacturer's Vendor ID */
#define USB_TELE_PRODUCT_ID   0x0035  /* Device's Product ID */

/* USB ID Table specifying the devices that this driver supports */
static struct usb_device_id uhf_rfid_table[] = {
  { USB_DEVICE(USB_TELE_VENDOR_ID, USB_TELE_PRODUCT_ID) },
  { } /* Terminate */
};

MODULE_DEVICE_TABLE (usb, uhf_rfid_table);

static int uhf_rfid_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_device *dev = interface_to_usbdev(interface);
    unsigned char buf[64];
    int retval;

    printk(KERN_INFO "USB device (%04X:%04X) plugged in\n", id->idVendor, id->idProduct);

    /* Read data from the device */
    retval = usb_control_msg(dev, usb_rcvctrlpipe(dev, 0), 0x01, 0xC0, 0, 0, buf, sizeof(buf), 1000);
    if (retval < 0) {
        printk(KERN_ERR "Error reading data from USB device\n");
        return retval;
    }

    /* Print the data received from the device */
    printk(KERN_INFO "Received data: %02X %02X %02X %02X %02X %02X %02X %02X\n",
           buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);

    return 0;
}
static void uhf_rfid_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "uhf_rfid drive removed\n");
}

static struct usb_driver uhf_rfid_driver = {
    .name = "uhf_rfid_driver",
    .id_table = uhf_rfid_table,
    .probe = uhf_rfid_probe,
    .disconnect = uhf_rfid_disconnect,
};


static int __init uhf_rfid_init(void)
{
    return usb_register(&uhf_rfid_driver);
}


static void __exit uhf_rfid_exit(void)
{
    usb_deregister(&uhf_rfid_driver);
}


module_init(uhf_rfid_init);
module_exit(uhf_rfid_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Harrington Ricks");
MODULE_DESCRIPTION("USB uhf_rfid Drive Driver");
