const mongoose = require('mongoose');
const ObjectId = mongoose.Types.ObjectId;
const farmSchema = new mongoose.Schema(
    {
        _id: { type: ObjectId, auto: true },
        // customer: { type: ObjectId, ref: 'Customer', index: true, required: true },
        name: { type: String, required: true },
    },
    {
        timestamps: true,
        _id: false,
    },
);
module.exports = mongoose.model('Farm', farmSchema);
