const BaseService = require('./baseService');
const User = require('../models/user');
const contants = require('../utils/contants');

class UserService extends BaseService {
    constructor() {
        super(User);
    }

    async login(email, password) {
        const user = await User.findByCredentials(email, password);
        if (!user) {
            throw {
                ...contants.authenErrors.NOT_FIND_OBJECT,
                message: 'user.not_found',
                desc: `Cannot find email = ${email}`
            };
        }
        console.log(user);
        const token = await user.generateAuthToken();
        return {token, name: user.name, _id: user._id, avatar: user.avatar};
    }
}
module.exports = new UserService();
