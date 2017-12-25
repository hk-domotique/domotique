# domotique

[Smart home](https://en.wikipedia.org/wiki/Home_automation) and [Internet of Things](https://en.wikipedia.org/wiki/Internet_of_things) become popular and buzz word in last couple of years. Domotique is small hackathon project
developed by couple of developers with basic microcontrollers knowledge.

The goal of the project was proof of concept for [RaspeberryPi](https://www.raspberrypi.org/) and [Arduino](https://www.arduino.cc/) communication and plug and play approach for
their communication.

RaspeberryPi was used as server and central point of communication while Arduino's were acting as clients.

## How it works

1. RasperryPi starts as server ready to accept clients for future handling
2. Arduino[s] as clients, uses Wi-Fi network (and WebSockets) to connect to the server telling server like: "Hey, I'm your client and I'm lamp" or
"Hey, I'm your client and I'm temperature sensor".
3. By using external UI it is possible to show clients and control them, like: if it is temp. sensor it will show up current temperature,
If it is lamp it will be possible to turn it on/off and so on.


