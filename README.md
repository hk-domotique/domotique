# domotique

Smart home and Internet of Things become popular and buzz word in last couple of years. Domotique is small hackathon project
developed by couple of developers with basic microcontrollers knowledge.

The goal of the project was proof of concept for RaspeberryPi and Arduino communication and plug and play approach for
their communication.

RaspeberryPi was used ad server and central point of comunication while number of Arduino's were acting as clients.

##How it worsk

1. RasperryPi starts as server ready to accept clients for future handling
2. Arduino[s] as clients, uses Wi-Fi network to connect to the server telling server like: "Hey, I'm your client and I'm lamp" or
"Hey, I'm your client and I'm temperature sensor".
3. Using external UI it is possible to show clients and control them, like. if it is temp. sensor it will show up current temperature,
If it is lamp it will be possible to turn it on/off and so on.


