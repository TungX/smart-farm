const utils = require('../utils/utils');
const jwt = require('jsonwebtoken');
const User = require('../models/user');
const { dbConfig } = require('../config');
const contants = require('../utils/contants');

async function checkWebClient(authorization) {
    if (!authorization) {
        throw ({
            ...contants.authenErrors.PARAM_REQUIRED,
            message: 'authorization.missing',
        });
    }
    const token = authorization.replace('Bearer ', '');
    if (!token) {
        throw ({
            ...contants.authenErrors.PARAM_INVALID,
            message: 'token.invalid',
            desc: `The param toke = ${token} is invalid`,
        });
    }
    let data;
    try {
        data = jwt.verify(token, dbConfig.JWT_KEY);
    } catch (error) {
        console.log("Error at jwt verify", error);
        throw ({
            ...contants.authenErrors.PARAM_INVALID,
            message: 'token.invalid',
            desc: `The param token = ${token} is invalid`,
        });
    }

    const user = await User.findOne({ _id: data._id, 'tokens.token': token }, '-password -tokens');
    if (!user) {
        throw ({
            ...contants.authenErrors.NOT_FIND_OBJECT,
            message: 'user.not_found',
            desc: `cannot find user id = ${data._id} and token = ${token}`,
        });
    }
    return { user, token };
}

module.exports = {
    checkWebClient,
};
