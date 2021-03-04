const BaseController = require('./baseController');
const userService = require('../services/userService');
const responseUtils = require('../utils/responseUtils');
const { messageResponse, errors } = require('../utils/contants');
const Utils = require('../utils/utils');

class UserController extends BaseController {
    constructor() {
        super(userService);
    }

    async login(req, res) {
        const { email, password } = req.body;
        try {
            const { token, name, _id, avatar } = await userService.login(email.trim(), password);
            return res.send(responseUtils.toResponse({ token, name, _id, avatar }, messageResponse.LOGIN_SUCCESS));
        } catch (error) {
            console.error(error);
            return res.send(responseUtils.toErrorResponse(error));
        }
    }
}
module.exports = new UserController();
