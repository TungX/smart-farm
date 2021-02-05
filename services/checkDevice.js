const Device = require('../models/device');
const noticeUtils = require('../utils/noticeUtils');
function checkDevice() {
    console.log("Begin check device");
    const d = new Date();
    d.setSeconds(d.getSeconds() - 60);
    Device.find({monitor: true, updatedAt: {$lt: d}}, function(error, devices){
        for (const device of devices) {
            noticeUtils.sendTelegram(device.telegramGroupId, `Thiết bị ${device.name} mất kết nối. Bạn cần kiểm tra`);
        }
    })
}
setInterval(() => {
    checkDevice()
}, 30000);
