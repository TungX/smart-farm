const mqtt = require('mqtt');
let mqttClient;
require("dotenv").config();
const configs = require('../config');

const Topic = {
    companyTopic: 'companyTopic',
    webClientTopic: 'webClientTopic',
    deviceTopic: 'deviceTopic'
};


function connect() {
    console.log("Connect with MQTT: "+configs.MqttConfig.MQTT_HOST);
    mqttClient = mqtt.connect(configs.MqttConfig.MQTT_HOST, {
        username: configs.MqttConfig.MQTT_USERNAME,
        password: configs.MqttConfig.MQTT_PASSWORD
    });
    mqttClient.on('connect', function () {
        console.log('Client connected to Mqtt broker');
        mqttClient.subscribe('esp-smart-farm-beat-heart', function (err) {
            if (err) {
               
            }
          })
        
    });
}

function sendPubMQTT(obj = '', company, message = {}, topic = '') {
    if (topic === Topic.companyTopic) {
        if (['device', 'member', 'accessHistory'].includes(obj)) {
            message.company = company;
            message.typeName = obj;
            mqttClient.publish(Topic.companyTopic, JSON.stringify(message));
        }
    } else if (topic === Topic.webClientTopic) {
        if (['dashboard', 'device', 'uploads'].includes(obj)) {
            message.company = company;
            message.typeName = obj;
            mqttClient.publish(Topic.webClientTopic, JSON.stringify(message));
        }
    } else if (topic === Topic.deviceTopic) {
        message.company = company;
        mqttClient.publish(Topic.deviceTopic, JSON.stringify(message));
    }
}

module.exports = {
    sendPubMQTT,
    connect,
};
