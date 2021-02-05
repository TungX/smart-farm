const httpUtils = require('./httpUtils');
const configs = require('../config');
async function sendTelegram(groupId, text) {
    const url = `https://api.telegram.org/bot${configs.telegramConfig.TOKEN}/sendMessage`;
    const chatId = (groupId || configs.telegramConfig.GROUP_CUSTOMER).replace('g', '-');
    console.log('chat id: ', chatId);
    const msg = {
        chat_id: chatId,
        text,
    }
    httpUtils.postRequest(url, {data: msg});
}
module.exports = {
    sendTelegram,
};