const mongoose = require('mongoose');
const ObjectId = mongoose.Types.ObjectId;
const deviceSchema = new mongoose.Schema(
    {
        _id: { type: ObjectId, auto: true },
        // customer: { type: ObjectId, ref: 'Customer', index: true, required: true },
        name: { type: String, required: true },
        macAddress: { type: String, index: true },
        ipAddress: { type: String, index: true },
        status: { type: String, enum: ["on", "off", "initial"], default: "initial" },
        telegramGroupId: { type: String },
        monitor: { type: Boolean, default: true },
    },
    {
        timestamps: true,
        _id: false,
    },
);
module.exports = mongoose.model('Device', deviceSchema);
