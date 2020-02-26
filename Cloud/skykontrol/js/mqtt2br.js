var hostname = "yourserver.com";
var port = 9883;
var clientId = "SkyKontrol_web_";
clientId += new Date().getUTCMilliseconds();
var username = "mqtt_server_username";
var password = "mqtt_server_password";

mqttClient = new Paho.MQTT.Client(hostname, port, clientId);
mqttClient.onMessageArrived =  MessageReceived;
mqttClient.onConnectionLost = ConnectionLost;

/*Initiates a connection to the MQTT broker*/
function Connect(){
	mqttClient.connect({
		onSuccess: Connected,
		onFailure: ConnectionFailed,
		keepAliveInterval: 10,
		userName: username,
		useSSL: true,
		password: password	
	});
}

/*Callback for successful MQTT connection */
function Connected() {
  console.log("Connection sucessful.");
  openMQTT();
}

/*Callback for failed connection*/
function ConnectionFailed(res) {
	console.log("Connection failed:" + res.errorMessage);
}

/*Callback for lost connection*/
function ConnectionLost(res) {
  if (res.errorCode != 0) {
	console.log("Connection lost:" + res.errorMessage);
	Connect();
  }
}

/*Callback for incoming message processing */
function Subscribe() {
        topic = "2br/skykontrol/toClient/" + mac;
        mqttClient.subscribe(topic);
        console.log("Subscribed to topic: " + topic);
}

/*Callback for incoming message processing */
function MessageReceived(message) {
	console.log("Received from "+ message.destinationName + ": " + message.payloadString);
	IncomingData(message.payloadString);
}

/*Function for sending message to remitent */
function MessageSend(message){
        message = new Paho.MQTT.Message(message);
	message.destinationName = "2br/skykontrol/toServer/" + mac;
	mqttClient.send(message);
        console.log("Sended to "+ message.destinationName + ": " + message.payloadString);	
}



