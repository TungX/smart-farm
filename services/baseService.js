const contants = require('../utils/contants');
const Utils = require('../utils/utils');

class BaseService {
    constructor(model) {
        this.model = model;
    }


    async getAll({ query = {}, fiels = "", page = 0, size = 20, sorts = undefined, populate = [] } = {}) {
        const options = { sort: { '_id': -1 } };
        if (sorts && typeof (sorts) === 'object') {
            options['sort'] = sorts;
        }
        const rs = Utils.normalizeParams(query, fiels, page, size, options);
        const data = await this.model.find(rs.query, rs.fields, options).populate(populate);
        const count = await this.model.count(rs.query);
        const numberPage = size > 0 ? Math.ceil((count / size)) : 0;
        const meta = { count, size, totalPage: numberPage, page }
        return { meta, data };
    }

    async create(object) {
        return await this.model.create(object);
    }

    async update(query, object) {
        try {
            for (const att of this.model.getIgnoreUpdateAttr() || []) {
                delete object[att];
            }
        } catch (error) {
        }
        const rs = await this.model.findOneAndUpdate(query, object, { new: true });
        if (!rs) {
            throw { ...contants.errors.NOT_FIND_OBJECT, desc: 'Cannot update' };
        }
        return rs;
    }

    async get(query, populate = []) {
        const rs = await this.model.findOne(query).populate(populate);
        if (!rs) {
            throw { ...contants.errors.NOT_FIND_OBJECT, desc: 'Cannot find' };
        }
        return rs;
    }

    async remove(query) {
        const rs = await this.model.findOneAndRemove(query);
        if (!rs) {
            throw { ...contants.errors.NOT_FIND_OBJECT, desc: 'Cannot remove' };
        }
        return rs;
    }
}

module.exports = BaseService;
