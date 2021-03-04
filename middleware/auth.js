const contants = require('../utils/contants');
const responseUtils = require('../utils/responseUtils');
const authUtils = require('./authUtils');
const moment = require('moment');
async function authUser(req, res, next) {
    try {
        const { user, token } = await authUtils.checkWebClient(req.header('Authorization'));
        req.user = user;
        req.token = token;
        checkPermission(req, user);
        next();
    } catch (error) {
        console.error('error at authWithToken', error);
        return res.send(responseUtils.toErrorResponse(error));
    }
}
function checkPermission(req, user) {
}
module.exports = authUser;
