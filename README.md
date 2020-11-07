# WeatherStation


## IoT led weather ticker 

### We used electronic components:
<ol>
  <li>ESP-01 <i>(sketch Weather)</i></li>
  <li>Arduino Nano <i>(sketch Matrix)</i></li>
  <li>Led Matrix 4 pieces 8x8</li>
  <li>Cables, box, AC/DC...</li>
</ol>

### We used software:
<ol>
  <li>MD_Parola</li>
  <li>ArduinoJson</li>
  <li>ESP8266HTTPClient</li>
  <li>AltSoftSeial</li>
  <li>Alex Gyver's helper libs...</li>
</ol>

### Algorithm:
<ol>
  <li>ESP sends GET request to OpenWeather,</li>
  <li>then parses Json response,</li>
  <li>then builds and sends the string to Nano. </li>
  <li>Nano reacts and runs a ticker on the LED Matrix</li>
</ol>


### What we'v got:
<p align="left">
  <img src="assets/image4.jpg" width="200" title="Screen">

