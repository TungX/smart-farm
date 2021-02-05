const express = require("express");
const bodyParser = require("body-parser");
const cors = require("cors");

const path = require("path");
require("dotenv").config();
const config = require('./config');
const dbUtils = require('./utils/dbUtils');
dbUtils.connectDB();
const app = express();
const corsOptions = {
  origin: "*",
  optionSuccessStatus: 200,
};
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json({ limit: "100mb" }));
app.use(cors(corsOptions));
app.set('trust proxy', true);
app.use(express.static(path.join(__dirname, "public")));
global.__rootPath = path.resolve(__dirname);
const apiVersion1 = '/api/v1';

const port = process.env.PORT || 11506;
app.listen(port, () => {
  console.log("The app listening at %s", port);
});

app.disable("x-powered-by");

require('./modules/mqttService').connect();
require('./services/checkDevice');
