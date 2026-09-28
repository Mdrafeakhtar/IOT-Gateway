#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WebServer.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);   

#define RXD2 16
#define TXD2 17

unsigned long previousMillis = 0;  
unsigned long lastSeen1 = 0;
unsigned long lastSeen2 = 0;
unsigned long lastSeen3 = 0;
unsigned long lastSeen4 = 0;

const unsigned long timeoutNode = 10000; // 10 sec offline timeout

const long interval = 1000; 
int screen_frame = 0;
String inString = "";

byte temp2 = 0;   
byte hum2 = 0;   
byte temp3 = 0;   
byte hum3 = 0;   
byte motion3 = 0;
byte temp4 = 0;   
byte hum4 = 0;   
byte temp1 = 0;   
byte hum1 = 0;  

/* ================= LOGIN ================= */
const char* loginUser = "admin";
const char* loginPass = "CHANGE_THIS_ADMIN_PASSWORD";

bool isLoggedIn = false;

/* ================= WIFI ================= */
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

/* ================= WEBPAGE ================= */
String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>IOT Gateway Pro Dashboard</title>
<meta name="viewport" content="width=device-width, initial-scale=1">

<style>

*{
margin:0;
padding:0;
box-sizing:border-box;
font-family:'Segoe UI',sans-serif;
}

body{
background: linear-gradient(135deg,#0f2027,#203a43,#2c5364);
color:white;
min-height:100vh;
}

/* ===== HEADER CENTER ===== */
.main-header{
width:100%;
text-align:center;
padding:20px;
font-size:28px;
font-weight:600;
letter-spacing:1px;
background:rgba(255,255,255,0.08);
backdrop-filter:blur(12px);
border-bottom:1px solid rgba(255,255,255,0.1);
}

/* ===== CARD CONTAINER ===== */
.container{
display:flex;
flex-wrap:wrap;
justify-content:center;
gap:25px;
padding:40px 20px;
max-width:1100px;
margin:auto;
}

/* ===== NODE CARD ===== */
.card{
width:240px;
padding:25px;
border-radius:18px;
background:rgba(255,255,255,0.08);
backdrop-filter: blur(15px);
box-shadow:0 10px 30px rgba(0,0,0,0.4);
transition:0.3s;
}

.card:hover{
transform:translateY(-8px);
box-shadow:0 15px 40px rgba(0,0,0,0.6);
}

.card h2{
margin-bottom:15px;
font-size:20px;
color:#00e5ff;
}

.value{
font-size:32px;
font-weight:bold;
margin:10px 0;
}

.label{
opacity:0.7;
font-size:14px;
}

.motion-on{
color:#00ff9c;
}

.motion-off{
color:#ff4d4d;
}

/* ===== FLOATING LOGOUT BUTTON ===== */
.floating-logout{
position:fixed;
bottom:25px;
right:25px;
padding:15px 25px;
border:none;
border-radius:50px;
background:linear-gradient(45deg,#ff416c,#ff4b2b);
color:white;
font-size:16px;
cursor:pointer;
box-shadow:0 0 15px rgba(255,75,43,0.7);
transition:0.3s;
}

.floating-logout:hover{
transform:scale(1.08);
box-shadow:0 0 25px rgba(255,75,43,1),
           0 0 50px rgba(255,75,43,0.7);
}

footer{
text-align:center;
padding:20px;
opacity:0.5;
}

</style>
</head>

<body>

<header class="main-header">
IOT GATEWAY CONTROL CENTER
</header>

<div class="container">

<div class="card">
<h2>Node 1</h2>

<div class="label">Temperature</div>
<div class="value" id="t1">-- °C</div>
<div class="label">Humidity</div>
<div class="value" id="h1">-- %</div>
<div class="label">Status</div>
<div class="value" id="s1">--</div>
</div>

<div class="card">
<h2>Node 2</h2>

<div class="label">Temperature</div>
<div class="value" id="t2">-- °C</div>
<div class="label">Humidity</div>
<div class="value" id="h2">-- %</div>
<div class="label">Status</div>
<div class="value" id="s2">--</div>
</div>

<div class="card">
<h2>Node 3</h2>

<div class="label">Motion Status</div>
<div class="value" id="m3">--</div>
<div class="label">Status</div>
<div class="value" id="s3">--</div>
</div>

<div class="card">
<h2>Node 4</h2>

<div class="label">Temperature</div>
<div class="value" id="t4">-- °C</div>
<div class="label">Humidity</div>
<div class="value" id="h4">-- %</div>
<div class="label">Status</div>
<div class="value" id="s4">--</div>
</div>

</div>

<footer>Live Data Refresh Every 2 Seconds</footer>

<!-- FLOATING LOGOUT BUTTON -->
<button class="floating-logout" onclick="confirmLogout()">
 Logout
</button>

<script>

/* ===== AUTO DATA UPDATE ===== */
setInterval(()=>{
fetch("/data")
.then(res=>res.json())
.then(data=>{

/* ===== TEMPERATURE + HUMIDITY ===== */
document.getElementById("t1").innerHTML=data.t1+" &deg;C";
document.getElementById("h1").innerHTML=data.h1+" %";

document.getElementById("t2").innerHTML=data.t2+" &deg;C";
document.getElementById("h2").innerHTML=data.h2+" %";

document.getElementById("t4").innerHTML=data.t4+" &deg;C";
document.getElementById("h4").innerHTML=data.h4+" %";

/* ===== MOTION ===== */
let motionText=document.getElementById("m3");
if(data.m3==1){
motionText.innerHTML="Motion Detected";
}
else{
motionText.innerHTML="No Motion";
}

/* ===== NODE STATUS (ADD HERE) ===== */
function showStatus(id,val){
 let el=document.getElementById(id);

 if(val==1){
   el.innerHTML="ONLINE";
   el.style.color="#00ff9c";
 }
 else{
   el.innerHTML="OFFLINE";
   el.style.color="#ff4d4d";
 }
}

showStatus("s1",data.s1);
showStatus("s2",data.s2);
showStatus("s3",data.s3);
showStatus("s4",data.s4);

});
},2000);


/* ===== LOGOUT CONFIRM ===== */
function confirmLogout(){
if(confirm("Are you sure you want to logout?")){
window.location.href="/logout";
}
}

</script>

</body>
</html>
)rawliteral";


/*============Login Page==============*/
String loginPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>IOT Gateway Secure Login</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta charset="UTF-8">

<style>

*{
margin:0;
padding:0;
box-sizing:border-box;
font-family:'Segoe UI',sans-serif;
}

body{
height:100vh;
display:flex;
justify-content:center;
align-items:center;
background: linear-gradient(135deg,#0f2027,#203a43,#2c5364);
overflow:hidden;
}

/* animated background glow */
body::before{
content:"";
position:absolute;
width:600px;
height:600px;
background:radial-gradient(circle,#00e5ff55,transparent);
top:-200px;
left:-200px;
animation:float 8s infinite linear;
}

body::after{
content:"";
position:absolute;
width:500px;
height:500px;
background:radial-gradient(circle,#ff4b2b55,transparent);
bottom:-200px;
right:-200px;
animation:float 10s infinite linear reverse;
}

@keyframes float{
0%{transform:translateY(0px)}
50%{transform:translateY(40px)}
100%{transform:translateY(0px)}
}

/* glass login box */
.login-box{
position:relative;
background:rgba(255,255,255,0.08);
backdrop-filter:blur(18px);
padding:45px 35px;
border-radius:18px;
width:320px;
text-align:center;
box-shadow:0 20px 50px rgba(0,0,0,0.5);
z-index:1;
}

/* title */
.login-title{
font-size:26px;
font-weight:600;
margin-bottom:25px;
color:white;
letter-spacing:1px;
}

/* input container */
.input-group{
position:relative;
margin-bottom:18px;
}

.input-group input{
width:100%;
padding:14px 12px 14px 42px;
border:none;
border-radius:10px;
background:rgba(255,255,255,0.12);
color:white;
font-size:15px;
outline:none;
}

/* input icons */
.input-group span{
position:absolute;
left:12px;
top:50%;
transform:translateY(-50%);
font-size:18px;
opacity:0.7;
}

/* glowing login button */
.login-btn{
width:100%;
padding:14px;
margin-top:10px;
border:none;
border-radius:12px;
background:linear-gradient(45deg,#00e5ff,#00ff9c);
font-size:17px;
font-weight:600;
cursor:pointer;
color:black;
transition:0.3s;
box-shadow:0 0 15px rgba(0,255,156,0.5);
}

.login-btn:hover{
transform:scale(1.05);
box-shadow:0 0 25px rgba(0,255,156,1),
           0 0 50px rgba(0,255,156,0.7);
}

/* footer text */
.secure-text{
margin-top:15px;
font-size:13px;
opacity:0.6;
}

</style>
</head>

<body>

<div class="login-box">

<div class="login-title">🔐 Secure Login</div>

<form action="/login" method="POST">

<div class="input-group">
<span>👤</span>
<input name="username" placeholder="Username" required>
</div>

<div class="input-group">
<span>🔑</span>
<input type="password" name="password" placeholder="Password" required>
</div>

<button class="login-btn">Login to Dashboard</button>

</form>

<div class="secure-text">IoT Gateway Protected Access</div>

</div>

</body>
</html>
)rawliteral";


/* ================= SERVER HANDLERS ================= */
void handleRoot() {
  if(!isLoggedIn){
    server.sendHeader("Location","/login");
    server.send(302,"text/plain","");
    return;
  }
  server.send(200, "text/html", webpage);
}

void handleData() {

  String json = "{";

  json += "\"t1\":" + String(temp1) + ",";
  json += "\"h1\":" + String(hum1) + ",";

  json += "\"t2\":" + String(temp2) + ",";
  json += "\"h2\":" + String(hum2) + ",";

  json += "\"t4\":" + String(temp4) + ",";
  json += "\"h4\":" + String(hum4) + ",";

  /* ===== NODE STATUS ===== */
  json += "\"s1\":" + String(millis()-lastSeen1 < timeoutNode ? 1:0) + ",";
  json += "\"s2\":" + String(millis()-lastSeen2 < timeoutNode ? 1:0) + ",";
  json += "\"s3\":" + String(millis()-lastSeen3 < timeoutNode ? 1:0) + ",";
  json += "\"s4\":" + String(millis()-lastSeen4 < timeoutNode ? 1:0) + ",";

  /* ===== MOTION ===== */
  json += "\"m3\":" + String(motion3);

  json += "}";

  server.send(200, "application/json", json);
}



void handleLoginPage(){
  server.send(200,"text/html",loginPage);
}

void handleLogin(){
  if(server.hasArg("username") && server.hasArg("password")){
    
    if(server.arg("username")==loginUser && server.arg("password")==loginPass){
      isLoggedIn = true;
      server.sendHeader("Location","/");
      server.send(302,"text/plain","");
      return;
    }
  }
  server.send(401,"text/plain","Login Failed");
}

void handleLogout(){
  isLoggedIn = false;
  server.sendHeader("Location","/login");
  server.send(302,"text/plain","");
}

/* ================= SETUP ================= */
void setup() {

  Serial.begin(9600);   
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);  

  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("IOT GATEWAY");

  delay(2000);
  lcd.clear();

  /* WIFI CONNECT */
  WiFi.begin(ssid, password);
  lcd.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  /* ===== SERVER ROUTES ===== */
server.on("/", handleRoot);
server.on("/login", HTTP_GET, handleLoginPage);
server.on("/login", HTTP_POST, handleLogin);
server.on("/logout", handleLogout);
server.on("/data", handleData);

  lcd.clear();
  lcd.print("WiFi Connected");
  delay(2000);

  //lcd.clear();
  //lcd.print(WiFi.localIP());
  //delay(2000);
  //lcd.clear();

lcd.clear();
lcd.setCursor(0,0);
lcd.print("ESP IP:");

lcd.setCursor(0,1);
lcd.print(WiFi.localIP());

delay(5000);   // show IP for 5 seconds
lcd.clear();


  /* SERVER START */
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

/* ================= LOOP ================= */
void loop() {

  server.handleClient();   // WEB SERVER RUNNING
  
  unsigned long currentMillis = millis();
  delay(10);

  if (currentMillis - previousMillis >= interval) {

    previousMillis = millis();
    screen_frame++;

    if(screen_frame==1){
      slave_read(0x10);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("NODE1_TP:");
      lcd.print(temp1);
      lcd.setCursor(11, 0);
      lcd.print((char)223);
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("RH:");
      lcd.print(hum1);
      lcd.print(" %");
    }

    if(screen_frame==2){
      slave_read(0x20);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("NODE2_TP:");
      lcd.print(temp2);
      lcd.setCursor(11, 0);
      lcd.print((char)223);
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("RH:");
      lcd.print(hum2);
      lcd.print(" %");
    }
  
    if(screen_frame==3){
      slave_read(0x30);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("NODE3_MOTION");
      lcd.setCursor(0, 1);
      if(motion3 == 1){
        lcd.print("Motion Detected");
      } else {
        lcd.print("No Motion");
      }
    }

    if(screen_frame==4){
      slave_read(0x40);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("NODE4_TP:");
      lcd.print(temp4);
      lcd.setCursor(11, 0);
      lcd.print((char)223);
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("RH:");
      lcd.print(hum4);
      lcd.print(" %");
    }

    if(screen_frame==4){
      screen_frame=0;
    }
  }

  while (Serial2.available() > 0) {
    char inChar = Serial2.read();
    inString += inChar;

    if (inString == "s") {

      Serial2.print("&t1=");
      Serial2.print(temp1);

      Serial2.print("&h1=");
      Serial2.print(hum1);
      
      Serial2.print("&t2=");
      Serial2.print(temp2);

      Serial2.print("&h2=");
      Serial2.print(hum2);

      Serial2.print("&m3=");
      if(motion3 == 1) Serial2.print("1");
      else Serial2.print("0");
      
      Serial2.print("&t4=");
      Serial2.print(temp4);

      Serial2.print("&h4=");
      Serial2.print(hum4);
    }

    inString = "";
  }

  delay(2000);
}

/* ================= SLAVE READ ================= */
void slave_read(byte id){
  byte TEMP = 0; 
  byte HUM = 0; 
  byte node = 0;

  Serial.write(id);
  delay(1);

  if (Serial.available() >= 2) {
    node  = Serial.read();
    TEMP  = Serial.read();
    HUM   = Serial.read();
  }
if(node==0x10){
  temp1=TEMP;
  hum1 = HUM;
  lastSeen1 = millis();
}

else if(node==0x20){
  temp2=TEMP;
  hum2 = HUM;
  lastSeen2 = millis();
}

else if(node==0x30){
  motion3 = TEMP;
  lastSeen3 = millis();
}

else if(node==0x40){
  temp4 = TEMP;
  hum4 = HUM;
  lastSeen4 = millis();
}
}



