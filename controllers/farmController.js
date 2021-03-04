const BaseController = require('./baseController');
const farmService = require('../services/farmService');
const responseUtils = require('../utils/responseUtils');
const { messageResponse, errors } = require('../utils/contants');
const Utils = require('../utils/utils');

class FarmController extends BaseController {
    constructor() {
        super(farmService);
    }
}
module.exports = new FarmController();
