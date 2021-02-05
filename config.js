const mqttConfig = {
    MQTT_HOST: process.env.MQTT_HOST || 'mqtt://smartfarm.yangyinhouse.com:8883',
    MQTT_POST: process.env.MQTT_POST || 8883,
    MQTT_USERNAME: process.env.MQTT_USERNAME || 'admin',
    MQTT_PASSWORD: process.env.MQTT_PASSWORD || '123456789',
    MQTT_TOPIC_MONITOR: process.env.MQTT_TOPIC_MONITOR || "esp-smart-farm-beat-heart"
};
const dbConfig = {
    CONFIG_MONGO_URI: process.env.MONGODB_URI || 'mongodb://localhost:27017/smart-farm',
}
const telegramConfig = {
    TOKEN: process.env.TELEGRAM_TOKEN || '',
    GROUP_SYSTEM: process.env.GROUP_SYSTEM || 'g427922300',
    GROUP_CUSTOMER: process.env.GROUP_CUSTOMER || 'g520248511',
}
module.exports = {
    mqttConfig,
    dbConfig,
    telegramConfig,
}