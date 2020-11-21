# WeatherStation


## IoT led weather ticker 

### We used electronic components:
<ol>
  <li>ESP-01 <i>(sketch Weather)</i></li>
  <li>ESP-01 Adapter</i></li>
  <li>Arduino Nano <i>(sketch Matrix)</i></li>
  <li>Led Matrix 8 pieces 8x8</li>
  <li>RTC DS3231</li>
  <li>Cables, box, AC/DC...</li>
</ol>

### We used software:
<ol>
  <li>MD_Parola</li>
  <li>ArduinoJson</li>
  <li>ESP8266HTTPClient</li>
  <li>ESPAsyncWebServer</li>
  <li>AltSoftSeial</li>
  <li>Alex Gyver's helper libs...</li>
</ol>

### Algorithm:
<ol>
  <li>ESP sends GET request to OpenWeather,</li>
  <li>then parses Json response,</li>
  <li>then builds and sends the string to Nano. </li>
  <li>Nano reacts and runs a ticker on the lower row of the LED Matrix,</li>
  <li>in the background Nano displays date-time in the upper row...</li>
  <li>ESP also provides a web interface in order you could choose your place (for weather request)</li>
</ol>

### To clone this project:
<ol>
  <li>Buy all parts</li>
  <li>Create OpenWeather account and get you API key <i>(Weather scetch)</i></li>
  <li>Install Arduino IDE and all Libs mentioned above</li>
  <li>Fill in your WIFI passwords, IP settings, API key<i>(Weather scetch)</i></li>
  <li>Upload scetches to Nano and ESP-01</li>
  <li>Assemble the circuit</li>
  <li>Enjoy!</li>
</ol>


### Electrical circuit:
<p align="left">
  <img src="assets/my.png" width="400" title="Screen">
</p>

### What we'v got:
<p align="left">  
  <img src="assets/my1.gif" width="400" title="Screen">
  <img src="assets/pic3.jpg" width="400" title="Screen">
</p>

