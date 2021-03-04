const mongoose = require('mongoose');
const ObjectId = mongoose.Types.ObjectId;
const config = require('../config');
const bcrypt = require('bcryptjs');
const jwt = require('jsonwebtoken');
const constant = require('../utils/contants');

const userSchema = new mongoose.Schema(
    {
        _id: { type: ObjectId, auto: true },
        name: { type: String, required: true, trim: true },
        password: { type: String, required: true, trim: true },
        email: { type: String, index: true, lowercase: true, unique: true },
        telephone: { type: String, index: true, lowercase: true, unique: true },
        facebookId: { type: String, index: true, lowercase: true, unique: true },
        googleId: { type: String, index: true, lowercase: true, unique: true },
        avatar: { type: String },
        avatarPath: { type: String },
        tokens: [{ _id: { type: ObjectId, auto: true }, token: { type: String, required: true } }],
        farm: { type: ObjectId, ref: 'Farm' },
    },
    {
        timestamps: true,
        _id: false,
    },
);

userSchema.pre('save', async function (next) {
    // Hash the password before saving the user model
    const user = this;
    if (user.isModified('password')) {
        user.password = await bcrypt.hash(user.password, 8);
        user.tokens = [];
    }
    next();
});

userSchema.methods.generateAuthToken = async function () {
    // Generate an auth token for the user
    const user = this;
    const token = jwt.sign({ _id: user._id }, config.dbConfig.JWT_KEY, {
        expiresIn: config.dbConfig.JWT_TOKEN_LIFETIME
    });
    user.tokens = user.tokens.concat({ token });
    await user.save();
    return token;
};

userSchema.statics.findByCredentials = async (email, password) => {
    const user = await Admin.findOne({ email }).populate('farm');
    if (!user) {
        throw { ...constant.errors.NOT_FIND_OBJECT, message: `email.not_found` };
    }
    const isPasswordMatch = await bcrypt.compare(password, user.password);
    if (isPasswordMatch) {
        return user;
    }
    throw { ...constant.errors.PARAM_INVALID, message: `password.wrong` };
};
const Admin = mongoose.model('User', userSchema);

module.exports = mongoose.model('User', userSchema);
