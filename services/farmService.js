const BaseService = require('./baseService');
const Farm = require('../models/farm');
const contants = require('../utils/contants');

class FarmService extends BaseService {
    constructor() {
        super(Farm);
    }
}
module.exports = new FarmService();
