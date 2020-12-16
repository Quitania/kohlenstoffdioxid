# oco
Indoor climate control - Mobile app to display output from CO<sub>2</sub> sensor.

## Remote debug
Step 1:
Connect the device to the host computer with a USB cable.

Step 2:
Set the target device to listen for a TCP/IP connection on port 5555

    adb tcpip 5555

Step 3:
Disconnect the USB cable from the target device.

Step 4:
Find the IP address of the Android device. For example, on a Nexus device,
you can find the IP address at Settings > About tablet (or About phone) > Status > IP address. Or, on a Wear OS device, you can find the IP address at Settings > Wi-Fi Settings > Advanced > IP address.

Step 5:
Connect to the device by its IP address

    adb connect device_ip_address

Step 8:
Confirm that your host computer is connected to the target device:

    adb devices

See [https://stackoverflow.com/a/64384114](https://stackoverflow.com/a/64384114)