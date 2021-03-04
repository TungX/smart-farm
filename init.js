const dbUtils = require('./utils/dbUtils');
const userService = require('./services/userService');
async function init() {
    dbUtils.connectDB();
    const user = await userService.create({
        name: 'Admin',
        email: 'admin@smartfarm.com',
        password: '12345678',
    });
    console.log("Init complete");
}

init();
