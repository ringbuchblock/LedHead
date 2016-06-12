# LedHead

A simple library for controlling a total of 3 LEDs (2 for the eyes and 1 for the status) via the Adafruit's NeoPixel library.

![head](head.jpg)

##### Example

See [simple example with ESP](https://github.com/ringbuchblock/LedHead/tree/master/examples/simple_with_esp) for whole example.

```arduino

WiFiManager wifiManager;
LedHead head;

std::vector<uint32_t> colors;
int curColor;

[...]

void rotateEyeColor() {
  head.updateEyeColor(colors[curColor]);

  curColor = curColor + 1;
  if (curColor >= colors.size()) {
    curColor = 0;
  }  
}

void initHead() {
  head = LedHead();
  colors = {head.yellow(), head.blue(), head.violet(), head.off()};
  curColor = 0;
}

void setup() {
  initHead();
  [...]
}

void loop() {
  if (noWifi()) {
    log("no wifi");
    head.updateStatusLed(head.red());
  } else {
    head.updateStatusLed(head.green());
  }

  rotateEyeColor();

  delay(SEC_1);
  // head.removeStatusColor();
  head.updateStatusLed(head.off());
  delay(DELAY_MILLIS);
}
```
