const router = require('express').Router();
const controller = require('../controllers/userController');
const auth = require('../middleware/auth');
router.post('/login', (req, res) => {
    return controller.login(req, res)
});
// router.post('/logout', auth, (req, res) => {
//     return controller.logout(req, res)
// });
// router.post('/logout-all', auth, (req, res) => {
//     return controller.logoutAll(req, res)
// });
// router.post('/change-password', auth, (req, res) => {
//     return controller.changePassword(req, res)
// });
// router.post('/auth', auth, (req, res) => {
//     return controller.auth(req, res)
// });
// // forgot password
// router.post('/forgot-password', (req, res) => {
//     return controller.forgotPassword(req, res)
// });
// // reset password
// router.post('/reset-password', (req, res) => {
//     return controller.resetPassword(req, res)
// });
// router.get('/', auth, (req, res) => {
//     return controller.getAll(req, res)
// });
// router.get('/check-user', (req, res) => {
//     return controller.checkUser(req, res)
// });
// router.get('/:id', auth, (req, res) => {
//     return controller.get(req, res)
// });
// router.post('/', auth, (req, res) => {
//     return controller.create(req, res)
// });
// router.put('/:id', auth, (req, res) => {
//     return controller.update(req, res)
// });
// router.delete('/:id', auth, (req, res) => {
//     return controller.remove(req, res)
// });
module.exports = router;
