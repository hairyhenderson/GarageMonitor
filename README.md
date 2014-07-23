# GarageMonitor

An Arduino sketch for monitoring the state of a garage door (or, really, the state of any switch).

When the garage door is first opened, an LED will be lit.
After ~5 minutes, the LED will continuously blink _"The door is ajar"_ in morse code.

I've written this to fit on an ATTiny85 chip, but it should run on any Arduino-compatible controller.

## License

[The MIT License](http://opensource.org/licenses/MIT)

Copyright (c) 2014 Dave Henderson
