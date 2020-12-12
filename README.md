# Kohlenstoffdioxid
This Arduino project sets up a portable CO<sub>2</sub> monitoring device.

## Hardware
### First prototype
Built inspired by [Umwelt-Campus](https://www.umwelt-campus.de/forschung/projekte/iot-werkstatt/ideen-zur-corona-krise-1):
- Adafruit Huzzah Feather ESP8266
- SGP30
- SHT35
- NeoPixel Ring

Instead of the SCD30, I choose a cheaper sensor. The SGP30 allows for humidity compensation so the SHT35 is used for that. 

![first_prototype_front](/docs/first_prototype_front.jpg) ![first_prototype_front](/docs/first_prototype_back.jpg) 
### Second prototype (planned)
Using lessons learned from first prototype:
- Adafruit Feather M0 Express
- SGP30
- SHT31-D
- NeoPixel FeatherWing

### Third prototype (planned)
Super mobile build:
- Adafruit Trinket M0 or Serpente R2
- SCD30
- BMP280
- Custom Android App

Not build yet. I choose a different controller, because I don't need WiFi on the go and this one has a little prototyping area. I'm trying to keep the size small enough, so it will fit into [this](https://learn.adafruit.com/3d-printed-case-for-adafruit-feather) case.

## Software
- InfluxDB

## See also
[Using the Grove Shield FeatherWing with the Huzzah Feather](https://github.com/Quitania/kohlenstoffdioxid/wiki/Using-the-Grove-Shield-FeatherWing-with-the-Huzzah-Feather)
