#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
int gate = 4;
int panic = 5;

char ssid[50] = "Wifi"; // Enter your WiFi name
char password[50] =  "password"; // Enter WiFi password
char name[50] =  "YinX-ESP8266"; // Enter WiFi password
char webUsername[32] = "admin";
char webPassword[50] = "126457";
const char* currentWfSsid = "Yin-ESP8266";
const char* currentWfpw = "123456789";
char mqttServer[50] = "smartfarm.yangyinhouse.com";
int mqttPort = 8883;
char mqttUser[50] = "admin";
char mqttPassword[50] = "yinX1506";
char topicRemote[50] = "remote";
char topicBeatHeart[50] = "esp-smart-farm-beat-heart";
int currentStateDoor = 0;
IPAddress _cip = IPAddress(192, 168, 1, 156);
IPAddress _cgw = IPAddress(192, 168, 1, 1);
IPAddress _csn = IPAddress(255, 255, 255, 0);

IPAddress _ip = IPAddress(192, 168, 1, 156);
IPAddress _gw = IPAddress(192, 168, 1, 1);
IPAddress _sn = IPAddress(255, 255, 255, 0);
IPAddress _dns = IPAddress(8, 8, 8, 8);
long totalByte = 0;
long address = 0;
void writeLong(long value, int index)
{
  EEPROM.write(address++, 156);
  EEPROM.write(address++, index);
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address++, four);
  EEPROM.write(address++, three);
  EEPROM.write(address++, two);
  EEPROM.write(address++, one);
}

//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to adress + 3.
long readLong()
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address++);
  long three = EEPROM.read(address++);
  long two = EEPROM.read(address++);
  long one = EEPROM.read(address++);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

String loadString() {
  String buffer;
  int totalByte = EEPROM.read(address++);
  if (totalByte == 0) {
    return "";
  }
  for (int i = 0; i < totalByte; i++) {
    int c = EEPROM.read(address++);
    buffer += char(c);
  }
  return buffer;
}

void writeString(const char* s, int index) {
  int len = strlen(s);
  EEPROM.write(address++, 156);
  EEPROM.write(address++, index);
  EEPROM.write(address++, len);
  for (int i = 0; i < len; i++) {
    EEPROM.write(address++, s[i]);
  }
}

IPAddress readIP() {
  int first = EEPROM.read(address++);
  int two = EEPROM.read(address++);
  int three = EEPROM.read(address++);
  int four = EEPROM.read(address++);
  return IPAddress(first, two, three, four);
}

void writeIp(IPAddress ip, int index) {
  EEPROM.write(address++, 156);
  EEPROM.write(address++, index);
  for (int i = 0; i < 4; i++) {
    EEPROM.write(address++, ip[i]);
  }
}

String Ip2S(const IPAddress& address) {
  return String() + address[0] + "." + address[1] + "." + address[2] + "." + address[3];
}

String orders[] = {"ssid", "password", "name", "ip-address", "gateway", "subnet", "dns", "webUsername", "webPassword", "mqttServer", "mqttPort", "mqttUser", "mqttPassword", "topicRemote"};
void save() {
  //Write ssid
  address = 0;
  EEPROM.write(address++, 156);
  EEPROM.write(address++, 13);
  writeString(ssid, 0);
  writeString(password, 1);
  writeString(name, 2);

  writeIp(_ip, 3);
  writeIp(_gw, 4);
  writeIp(_sn, 5);
  writeIp(_dns, 6);

  writeString(webUsername, 7);
  writeString(webPassword, 8);
  writeString(mqttServer, 9);
  writeLong(mqttPort, 10);
  writeString(mqttUser, 11);
  writeString(mqttPassword, 12);
  writeString(topicRemote, 13);
  EEPROM.commit();
}

void load() {
  address = 0;
  int r = EEPROM.read(address++);
  if (r != 156) {
    return;
  }
  Serial.println(r);
  int len = EEPROM.read(address++);
  Serial.println(len);
  int index = -1;
  String text;
  IPAddress ip;
  for (int i = 0; i < len; i++) {
    r = EEPROM.read(address++);
    if (r != 156) {
      return;
    }
    index = EEPROM.read(address++);
    switch (index) {
      case 0:
        text = loadString();
        text.toCharArray(ssid, 50);
        break;
      case 1:
        text = loadString();
        text.toCharArray(password, 50);
        break;
      case 2:
        text = loadString();
        text.toCharArray(name, 50);
        break;
      case 3:
        ip = readIP();
        _ip = ip;
        break;
      case 4:
        ip = readIP();
        _gw = ip;
        break;
      case 5:
        ip = readIP();
        _sn = ip;
        break;
      case 6:
        ip = readIP();
        _dns = ip;
        break;
      case 7:
        text = loadString();
        text.toCharArray(webUsername, 32);
        break;
      case 8:
        text = loadString();
        text.toCharArray(webPassword, 50);
        break;
      case 9:
        text = loadString();
        text.toCharArray(mqttServer, 50);
        break;
      case 10:
        mqttPort = readLong();
        break;
      case 11:
        text = loadString();
        text.toCharArray(mqttUser, 50);
        break;
      case 12:
        text = loadString();
        text.toCharArray(mqttPassword, 50);
        break;
      case 13:
        text = loadString();
        text.toCharArray(topicRemote, 50);
        break;
      default:
        return;
    }
  }
}
void reset() {
  String("wifi").toCharArray(ssid, 50);
  String("password").toCharArray(password, 50);
  _ip = IPAddress(192, 168, 1, 156);
  _gw = IPAddress(192, 168, 1, 1);
  _sn = IPAddress(255, 255, 255, 0);
  _dns = IPAddress(8, 8, 8, 8);
  String("YinX-ESP8266").toCharArray(name, 50);
  String("admin").toCharArray(webUsername, 32);
  String("126457").toCharArray(webPassword, 50);
  String("smartfarm.yangyinhouse.com").toCharArray(mqttServer, 50);
  mqttPort = 8883;
  String("admin").toCharArray(mqttUser, 50);
  String("").toCharArray(mqttPassword, 50);
  String("remote").toCharArray(topicRemote, 50);
}
ESP8266WebServer server(80);
String header;
bool needResetWifi = false;
bool needResetMqtt = false;
bool connectedWifi = false;
WiFiClient espClient;
PubSubClient client(espClient);
int numberRetryConnectMax = 100;
void setupWifi() {
  Serial.println("Begin connect with wifi at " + String(ssid) + ":" + String(password) );
//  WiFi.config(_ip, _dns, _gw, _sn);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  int countRetryConnect = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    countRetryConnect++;
    if (countRetryConnect > numberRetryConnectMax) {
      WiFi.disconnect();
      // Start Access Point
      Serial.println("Start Access Point");
      
      WiFi.softAPConfig(_cip, _cgw, _csn);
      WiFi.softAP(currentWfSsid, currentWfpw, 1, false);
      IPAddress myIP = WiFi.softAPIP();
      Serial.print("AP IP address: ");
      Serial.println(myIP);
      connectedWifi = false;
      //      WiFi.softAPdisconnect (false);
      return;
    }
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.softAPdisconnect (true);
  connectedWifi = true;
}
unsigned long lastMsg = 0;

String toHtml(String styles, String body) {
  String head = "<!DOCTYPE html><html>";
  head += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  head += "<link rel=\"icon\" href=\"data:,\">";
  head += styles + "</head>";
  return head + body + "</html>";
}
String showLoginForm(String error) {
  String styles = "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; }";
  styles += ".reset-config { margin-left: auto; margin-right: auto; }";
  styles += ".login-form table { margin-left: auto; margin-right: auto; }";
  styles += ".login-form .error { color: red; }";
  styles += ".login-form .submit { background-color: #4CAF50; border: none; color: white; padding: 0 5px; text-decoration: none; font-size: 30px; cursor: pointer; width: 100%;}";
  styles += "</style>";

  String body = "<body><h1>Yin ESP8266 Web Server</h1>";
  String form = "<form class=\"login-form\" method=\"post\" action=\"/info\"><p class=\"error\">" + error + "</p><table><tbody>";
  form += "<tr><th>User name:</th><td><input type=\"text\" name=\"username\" required></td></tr>";
  form += "<tr><th>Password:</th><td><input type=\"password\" name=\"password\" required></td></tr>";
  form += "<tr><td colspan=\"2\"><button class=\"submit\">Login</button></td></tr>";
  form += "</tbody></table></form>";
  String p = "<p class='reset-config'><a href='/reset-config'>Reset Config</a></p>";
  body += form + p + "</body>";
  return toHtml(styles, body);
}
void handleRoot() {
  server.send(200, "text/html", showLoginForm(""));
}
boolean isLogin() {
  if ( ! server.hasArg("username") || ! server.hasArg("password")
       || server.arg("username") == NULL || server.arg("password") == NULL) { // If the POST request doesn\"t have username and password data
    return false;
  }

  return strcmp(webUsername, server.arg("username").c_str()) == 0 && strcmp(server.arg("password").c_str(), webPassword) == 0;
}
String showInfoPage() {
  String styles = "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; }";
  styles += ".reconnect { margin-left: auto; margin-right: auto; text-align: left; width: 100%; max-width: 600px;}";
  styles += "table { margin-left: auto; margin-right: auto; text-align: left; border: 2px solid black; width: 100%; max-width: 600px;}";
  styles += "table .title { text-align: center; width: 100%; max-width: 600px; border-top: 1px solid black; border-bottom: 1px solid black;}";
  styles += "button { margin-top: 10px; width: 100%; max-width: 600px; padding: 10px; font-size: 18px; font-weight: bold; color: white; background-color: #4CAF50; }";
  styles += "</style>";
  String body = "<body><h1>Yin ESP8266 Web Server</h1>";
  String table = "<table>";
  table += "<tr><th colspan=\"2\" class=\"title\" style=\"border-top: none;\">Wifi Config</th></tr>";
  table += "<tr><th>SSID</th><td>" + String(ssid) + "</td></tr>";
  table += "<tr><th>Password</th><td>" + String(password) + "</td></tr>";
//  table += "<tr><th>IPv4 Address</th><td>" + Ip2S(_ip) + "</td></tr>";
//  table += "<tr><th>Default Gateway</th><td>" + Ip2S(_gw) + "</td></tr>";
//  table += "<tr><th>Subnet Mask</th><td>" + Ip2S(_sn) + "</td></tr>";
//  table += "<tr><th>DNS</th><td>" + Ip2S(_dns) + "</td></tr>";
  table += "<tr><th colspan=\"2\" class=\"title\">Device</th></tr>";
  table += "<tr><th>Name</th><td>" + String(name) + "</td></tr>";
  table += "<tr><th>Mac Address</th><td>" + WiFi.macAddress() + "</td></tr>";
  table += "<tr><th colspan=\"2\" class=\"title\">User info</th></tr>";
  table += "<tr><th>User name</th><td>" + String(webUsername) + "</td></tr>";
  table += "<tr><th>Password</th><td>" + String(webPassword) + "</td></tr>";
  table += "<tr><th colspan=\"2\" class=\"title\">MQTT Info</th></tr>";
  table += "<tr><th>Host</th><td>" + String(mqttServer) + "</td></tr>";
  table += "<tr><th>Post</th><td>" + String(mqttPort) + "</td></tr>";
  table += "<tr><th>User name</th><td>" + String(mqttUser) + "</td></tr>";
  table += "<tr><th>Password</th><td>" + String(mqttPassword) + "</td></tr>";
  table += "<tr><th>Topic remote</th><td>" + String(topicRemote) + "</td></tr>";
  table += "</table>";
  String form = "<form method=\"post\" action=\"/edit-info\"><input type=\"hidden\" name=\"username\" value=\"" + String(webUsername) + "\"><input type=\"hidden\" name=\"password\" value=\"" + String(webPassword) + "\"><button type=\"submit\">Edit</button></form>";
  String p = "<p class='reconnect'><a href='/reconnect-wifi'>Reconnect wifi</a></p>";
  body += table + form + p + "</body>";
  return toHtml(styles, body);
}
void handleReconnectWifi() {
  server.send(200, "text/html", "<h1>The system is reconnecting</h1><p>The system will reset, you can visit manager system at <strong><a href=\"http://" + Ip2S(_ip) + "\">" + Ip2S(_ip) + "</a></strong></p><p><a href=\"/\">Home Page</a></p>");
  WiFi.disconnect();
  setupWifi();
}

void handleResetConfig() {
  reset();
  save();
  server.send(200, "text/html", "<h1>The system is reset config</h1><p>The system will reset, you can visit manager system at <strong><a href=\"http://" + Ip2S(_ip) + "\">" + Ip2S(_ip) + "</a></strong></p><p><a href=\"/\">Home Page</a></p>");
  WiFi.disconnect();
  setupWifi();
}
void handleInfo() {
  if ( !isLogin()) {
    server.send(200, "text/html", showLoginForm("Username or password is wrong!"));
    return;
  }
  server.send(200, "text/html", showInfoPage());
}
String normalizeToHtml(String text) {
  text.replace("\"", "&quot;");
  return text;
}
String showEditInfoPage(String errors[], int size) {
  String styles = "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; }";
  styles += ".error { margin-left: auto; margin-right: auto; text-align: left; width: 100%; max-width: 600px; color: red; }";
  styles += "table { margin-left: auto; margin-right: auto; text-align: left; border: 2px solid black; width: 100%; max-width: 600px;}";
  styles += "table .title { text-align: center; width: 100%; max-width: 600px; border-top: 1px solid black; border-bottom: 1px solid black;}";
  styles += "button { margin-top: 10px; width: 100%; max-width: 600px; padding: 10px; font-size: 18px; font-weight: bold; color: white; background-color: #4CAF50; }";
  styles += "</style>";
  String body = "<body><h1>Yin ESP8266 Web Server</h1>";
  String error = "";
  for (int i = 0; i < size; i++) {
    error += "<p class=\"error\">* " + errors[i] + "</p>";
  }
  String table = "<table>";
  table += "<tr><th colspan=\"2\" class=\"title\" style=\"border-top: none;\">Wifi Config</th></tr>";
  table += "<tr><th>SSID</th><td><input type=\"text\" name=\"ssid\" value=\"" + normalizeToHtml(String(ssid)) + "\" required /></td></tr>";
  table += "<tr><th>Password</th><td><input type=\"text\" name=\"wfpw\" value=\"" + normalizeToHtml(String(password)) + "\" /></td></tr>";
//  table += "<tr><th>IPv4 Address</th><td><input type=\"text\" name=\"ip\" value=\"" + Ip2S(_ip) + "\" /></td></tr>";
//  table += "<tr><th>Default Gateway</th><td><input type=\"text\" name=\"gw\" value=\"" + Ip2S(_gw) + "\" /></td></tr>";
//  table += "<tr><th>Subnet Mask</th><td><input type=\"text\" name=\"sn\" value=\"" + Ip2S(_sn) + "\" /></td></tr>";
//  table += "<tr><th>DNS</th><td><input type=\"text\" name=\"dns\" value=\"" + Ip2S(_dns) + "\" /></td></tr>";
  table += "<tr><th colspan=\"2\" class=\"title\">Device</th></tr>";
  table += "<tr><th>Name</th><td><input type=\"text\" name=\"name\" value=\"" + String(name) + "\" required /></td></tr>";
  table += "<tr><th>Mac Address</th><td>" + WiFi.macAddress() + "</td></tr>";
  table += "<tr><th colspan=\"2\" class=\"title\">User info</th></tr>";
  table += "<tr><th>User name</th><td><input type=\"text\" name=\"wusername\" value=\"" + String(webUsername) + "\" required /></td></tr>";
  table += "<tr><th>Current Password</th><td><input type=\"password\" name=\"cpassword\" /></td></tr>";
  table += "<tr><th>New Password</th><td><input type=\"password\" name=\"npassword\" /></td></tr>";
  table += "<tr><th>Confirm Password</th><td><input type=\"password\" name=\"ncpassword\" /></td></tr>";
  table += "<tr><th colspan=\"2\" class=\"title\">MQTT Info</th></tr>";
  table += "<tr><th>Host</th><td><input type=\"text\" name=\"mqtt-server\" value=\"" + String(mqttServer) + "\" required /></td></tr>";
  table += "<tr><th>Post</th><td><input type=\"number\" name=\"mqtt-port\" value=\"" + String(mqttPort) + "\" required /></td></tr>";
  table += "<tr><th>User name</th><td><input type=\"text\" name=\"mqtt-user\" value=\"" + String(mqttUser) + "\" /></td></tr>";
  table += "<tr><th>Password</th><td><input type=\"text\" name=\"mqtt-password\" value=\"" + String(mqttPassword) + "\" /></td></tr>";
  table += "<tr><th>Topic remote</th><td><input type=\"text\" name=\"mqtt-topic-remote\" value=\"" + String(topicRemote) + "\" /></td></tr>";
  table += "</table>";
  String form = "<form method=\"post\" action=\"/update-info\"><input type=\"hidden\" name=\"username\" value=\"" + String(webUsername) + "\"/><input type=\"hidden\" name=\"password\" value=\"" + String(webPassword) + "\"/>" + table + "<button type=\"submit\">Save</button></form>";
  body += error + form + "</body>";
  return toHtml(styles, body);
}
void handleEditInfo() {
  if ( !isLogin()) {
    server.send(200, "text/html", showLoginForm("Username or password is wrong!"));
    return;
  }
  String errors[] = {};
  server.send(200, "text/html", showEditInfoPage(errors, 0));
}

void handleSaveInfo() {
  if ( !isLogin()) {
    server.send(200, "text/html", showLoginForm("Username or password is wrong!"));
    return;
  }
  int size = 0;
  String errors[5];

  if (strcmp(server.arg("ssid").c_str(), ssid) != 0) {
    needResetWifi = true;
  }
  server.arg("ssid").toCharArray(ssid, 50);
  if (strcmp(server.arg("wfpw").c_str(), password) != 0) {
    needResetWifi = true;
  }
  server.arg("wfpw").toCharArray(password, 50);
  IPAddress ip;
  String text;
//  text = server.arg("ip");
//  text.trim();
//  if (text != "" && ip.fromString(text)) {
//    if (ip != _ip) {
//      needResetWifi = true;
//    }
//    _ip = ip;
//  } else {
//    errors[size] = "IPv4 Address is wrong";
//    size++;
//  }

//  text = server.arg("gw");
//  text.trim();
//  if (text != "" && ip.fromString(text)) {
//    if (ip != _gw) {
//      needResetWifi = true;
//    }
//    _gw = ip;
//  } else {
//    errors[size] = "Default Gateway is wrong";
//    size++;
//  }
//  text = server.arg("sn");
//  text.trim();
//  if (text != "" && ip.fromString(text)) {
//    if (ip != _sn) {
//      needResetWifi = true;
//    }
//    _sn = ip;
//  } else {
//    errors[size] = "Subnet Mask is wrong";
//    size++;
//  }
//  text = server.arg("dns");
//  text.trim();
//  if (text != "" && ip.fromString(text)) {
//    if (ip != _dns) {
//      needResetWifi = true;
//    }
//    _dns = ip;
//  } else {
//    errors[size] = "DNS is wrong";
//    size++;
//  }

  text = server.arg("name");
  text.trim();
  text.toCharArray(name, 50);

  text = server.arg("wusername");
  text.trim();
  text.toCharArray(webUsername, 32);

  text = server.arg("npassword");
  if (text != "") {
    boolean canChange = true;
    if (strcmp(server.arg("cpassword").c_str(), webPassword) != 0) {
      errors[size] = "Current password does not match";
      size++;
      canChange = false;
    }
    if (server.arg("ncpassword") != text) {
      errors[size] = "Confirm password does not match";
      size++;
      canChange = false;
    }
    if (canChange) {
      Serial.print("Change password to : ");
      text.toCharArray(webPassword, 50);
      Serial.println(webPassword);
    }
  }
  text = server.arg("mqtt-server");
  text.trim();
  if (strcmp(text.c_str(), mqttServer) != 0) {
    needResetMqtt = true;
  }
  text.toCharArray(mqttServer, 50);
  text = server.arg("mqtt-port");
  text.trim();
  if (text.toInt() != mqttPort) {
    needResetMqtt = true;
  }
  mqttPort = text.toInt();
  text = server.arg("mqtt-user");
  text.trim();
  if (strcmp(text.c_str(), mqttUser) != 0) {
    needResetMqtt = true;
  }
  text.toCharArray(mqttUser, 50);
  text = server.arg("mqtt-password");
  text.trim();
  if (strcmp(text.c_str(), mqttPassword) != 0) {
    needResetMqtt = true;
  }
  text.toCharArray(mqttPassword, 50);
  text = server.arg("mqtt-topic-remote");
  text.trim();
  if (strcmp(text.c_str(), topicRemote) != 0) {
    needResetMqtt = true;
  }
  text.toCharArray(topicRemote, 50);
  if (size > 0) {
    server.send(200, "text/html", showEditInfoPage(errors, size));
    return;
  }
  save();
  if (needResetMqtt) {
    Serial.println("Need reset mqtt");
    client.disconnect();
    Serial.println("Connect with: " + String(mqttServer) + ':' + mqttPort);
    client.setServer(mqttServer, mqttPort);
  }
  if (needResetWifi) {
    Serial.println("Need reset wifi");
    server.send(200, "text/html", "<h1>Your info updated</h1><p>The system will reset, you can visit manager system at <strong><a href=\"" + Ip2S(_ip) + "\">" + Ip2S(_ip) + "</a></strong></p><p><a href=\"/\">Home Page</a></p>");
    WiFi.disconnect();
    setupWifi();
    return;
  }

  server.send(200, "text/html", "<h1>Your info updated</h1><p><a href=\"/\">Home Page</a></p>");

}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there\"s no handler for the URI in the request
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  char sign = (char)payload[0];
  String mac = "";
  for (int i = 2; i <= 18; i++) {
    mac += (char)payload[i];
  }

  mac.toLowerCase();
  String currentMac = WiFi.macAddress();
  currentMac.toLowerCase();
  if (currentMac != mac) {
    Serial.println("mac address not match: " + mac + " != " + currentMac);
    return;
  }
  int remotePort = gate;
  Serial.print("length: ");
  Serial.println(length);
  if (length > 20 && (char)payload[20] == '1') {
    remotePort = panic;  
  }
  Serial.print("remotePort ");
  Serial.println(remotePort);
  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '0') {
    currentStateDoor = 0;
    Serial.println("Turn off");
    digitalWrite(remotePort, LOW);   // Turn the LED on (Note that LOW is the voltage level
  } else {
    currentStateDoor = 1;
    Serial.println("Turn on");
    digitalWrite(remotePort, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}
void setup() {
  // put your setup code here, to run once:
  pinMode(gate, OUTPUT);
  pinMode(panic, OUTPUT);
  Serial.begin(9600);
  //  delay(1000);
  EEPROM.begin(1024);
  load();

  setupWifi();
  server.begin();
  server.on("/", HTTP_GET, handleRoot);
  server.on("/info", HTTP_POST, handleInfo);
  server.on("/edit-info", HTTP_POST, handleEditInfo);
  server.on("/update-info", HTTP_POST, handleSaveInfo);
  server.on("/reconnect-wifi", HTTP_GET, handleReconnectWifi);
  server.on("/reset-config", HTTP_GET, handleResetConfig);

  server.onNotFound(handleNotFound);
  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqttCallback);
}
unsigned long lastConnect = 0;
int numberReconnect = 0;
void loop() {
  server.handleClient();
  if(!connectedWifi){
    client.loop();
    return;
  }
  unsigned long now = millis();
  if (!client.connected()) {
    if (now - lastConnect > 5000) {
      Serial.println("Connect mqtt with: " + String(mqttServer) + ':' + mqttPort);
      lastConnect = now;
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);
      bool connected = false;
      if (strcmp(mqttUser, "") == 0) {
        connected = client.connect(clientId.c_str());
      } else {
        connected = client.connect(clientId.c_str(), mqttUser, mqttPassword);
        Serial.println("Connect mqtt with user name and password: " + String(mqttUser) + ':' + mqttPassword);
      }
      if (connected) {
        Serial.println("connected");
        // Once connected, publish an announcement...
        String msg = String(name) + "~" + WiFi.macAddress()+ "~" + Ip2S(WiFi.localIP()) + "~" + String(topicRemote) + "~" + currentStateDoor;
        client.publish(topicBeatHeart, msg.c_str());
        // ... and resubscribe
        client.subscribe(topicRemote);
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.print(" at ");
        Serial.print(numberReconnect);
        Serial.println(" try again in 2 seconds");
        // Wait 2 seconds before retrying
        if(connectedWifi){
          numberReconnect++;
          if(numberReconnect > 20){
            WiFi.disconnect();
            // Start Access Point
            WiFi.softAPdisconnect (false);
            Serial.println("Start Access Point at loop");
            WiFi.softAPConfig(_cip, _cgw, _csn);
            WiFi.softAP(currentWfSsid, currentWfpw, 1, false);
            IPAddress myIP = WiFi.softAPIP();
            Serial.print("AP IP address: ");
            Serial.println(myIP);
            connectedWifi = false;
          }
        }
        
      }
    }
  } else {
    if (now - lastMsg > 5000) {
      lastMsg = now;
      String msg = String(name) + "~" + WiFi.macAddress()+ "~" + Ip2S(WiFi.localIP()) + "~" + String(topicRemote) + "~" + currentStateDoor;
      client.publish(topicBeatHeart, msg.c_str());
    }
  }
  client.loop();
}
