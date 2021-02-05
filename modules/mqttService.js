const mqtt = require('mqtt');
const noticeUtils = require('../utils/noticeUtils');
let mqttClient;
require("dotenv").config();
const configs = require('../config');
const Device = require('../models/device');

function connect() {
    console.log("Connect with MQTT: " + configs.mqttConfig.MQTT_HOST);
    mqttClient = mqtt.connect(configs.mqttConfig.MQTT_HOST, {
        username: configs.mqttConfig.MQTT_USERNAME,
        password: configs.mqttConfig.MQTT_PASSWORD
    });
    mqttClient.on('connect', function () {
        console.log('Client connected to Mqtt broker');
        mqttClient.subscribe('esp-smart-farm-beat-heart', function (err) {
            if (err) {
                noticeUtils.sendTelegram(configs.telegramConfig.GROUP_SYSTEM, `Has error connect mqtt: ${err}`)
            }
        });
    });
    mqttClient.on('message', async function (topic, message) {
        // message is Buffer
        const elements = message.toString().split('~');
        const device = {name: elements[0], macAddress: elements[1], ipAddress: elements[2], status: 'on'};
        const rs = await Device.findOneAndUpdate({macAddress: elements[1]}, device, {upsert: true, new: true, setDefaultsOnInsert: true});
        console.log(rs);
    });
}

module.exports = {
    connect,
};
