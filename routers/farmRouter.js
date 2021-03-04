const router = require('express').Router();
const controller = require('../controllers/farmController');
const auth = require('../middleware/auth');
router.get('/', auth, (req, res) => { return controller.getAll(req, res) });

module.exports = router;
