#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char *ssid = "MyESP8266_AP"; 
const char *password = "password123"; 
ESP8266WebServer server(80);
const char* htmlContent = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
 <title>8051 Timer Calculator</title>
 <style>
 body {
 font-family: Arial, sans-serif;
 background-color: #f4f4f9;
 margin:0;
 padding:0;
 }
 .container {
 max-width:800px;
 margin:20px auto;
 padding:20px;
 background-color: #fff;
 box-shadow:0 0 10px rgba(0,0,0,0.1);
 border-radius:8px;
 }
 h2, h3 {
 color: #333;
 }
 .calculator {
 margin:20px 0;
 padding:15px;
 border:1px solid #ddd;
 border-radius:5px;
 background-color: #fafafa;
 }
 .input-group {
 margin:15px 0;
 display: flex;
 flex-wrap: wrap;
 align-items: center;
 }
 label {
 display: inline-block;
 width:180px;
 color: #555;
 }
 input[type="number"], input[type="text"] {
 width:120px;
 padding:8px;
 border:1px solid #ccc;
 border-radius:4px;
 margin-right:10px;
 }
 select {
 padding:8px;
 border:1px solid #ccc;
 border-radius:4px;
 margin-right:10px;
 }
 button {
 padding:8px 15px;
 background-color: #007bff;
 color: #fff;
 border: none;
 border-radius:4px;
 cursor: pointer;
 margin-left:5px;
 }
 button:hover {
 background-color: #0056b3;
 }
 .nav-button {
 background-color: #e0e0e0;
 color: #333;
 margin:5px;
 padding:10px 15px;
 border: none;
 border-radius:4px;
 cursor: pointer;
 }
 .nav-button:hover {
 background-color: #d0d0d0;
 }
 .active {
 background-color: #007bff;
 color: #fff;
 }
 .error {
 color: red;
 margin-top:10px;
 }
 .results {
 margin-top:15px;
 padding:10px;
 background-color: #f5f5f5;
 border:1px solid #ddd;
 border-radius:5px;
 }
 @media (max-width:600px) {
 .input-group {
 flex-direction: column;
 align-items: flex-start;
 }
 label {
 width:100%;
 margin-bottom:5px;
 }
 input[type="number"], input[type="text"], select {
 width:100%;
 margin-right:0;
 margin-bottom:10px;
 }
 button {
 margin-left:0;
 margin-bottom:10px;
 }
 }
 </style>
</head>
<body>
 <div class="container">
 <h2>8051 Timer Calculator</h2>
 <button class="nav-button" onclick="showPage('page1')" id="btnPage1">Frequency To TH/TL</button>
 <button class="nav-button" onclick="showPage('page2')" id="btnPage2">TH/TL To Frequency</button>
 <!-- Frequency to TH/TL Calculator -->
 <div id="page1" class="calculator">
 <h3>Frequency to Timer Values</h3>
 <div class="input-group">
 <label>Crystal Frequency (MHz):</label>
 <input type="number" id="freq1" step="0.1" value="11.0592" min="1" max="24">
 </div>
 <div class="input-group">
 <label>Timer Selection:</label>
 <select id="timer1">
 <option value="0">Timer0</option>
 <option value="1">Timer1</option>
 </select>
 </div>
 <div class="input-group">
 <label>Mode:</label>
 <select id="mode1">
 <option value="1">Mode1 (16-bit)</option>
 <option value="2">Mode2 (8-bit auto-reload)</option>
 </select>
 </div>
 <div class="input-group">
 <label>Desired Frequency (Hz):</label>
 <input type="number" id="desiredFreq" step="1" min="1">
 <button onclick="calculateFromFrequency()">Calculate</button>
 <button onclick="resetPage1()">Reset</button>
 </div>
 <div id="results1" class="results"></div>
 </div>
 <!-- TH/TL to Frequency Calculator -->
 <div id="page2" class="calculator" style="display:none">
 <h3>Timer Values to Frequency</h3>
 <div class="input-group">
 <label>Crystal Frequency (MHz):</label>
 <input type="number" id="freq2" step="0.1" value="11.0592" min="1" max="24">
 </div>
 <div class="input-group">
 <label>Timer Selection:</label>
 <select id="timer2">
 <option value="0">Timer0</option>
 <option value="1">Timer1</option>
 </select>
 </div>
 <div class="input-group">
 <label>Mode:</label>
 <select id="mode2">
 <option value="1">Mode1 (16-bit)</option>
 <option value="2">Mode2 (8-bit auto-reload)</option>
 </select>
 </div>
 <div class="input-group">
 <label>TH (Hex):</label>
 <input type="text" id="thInput" maxlength="2" placeholder="00">
 <label>TL (Hex):</label>
 <input type="text" id="tlInput" maxlength="2" placeholder="00">
 <button onclick="calculateFromValues()">Calculate</button>
 <button onclick="resetPage2()">Reset</button>
 </div>
 <div id="results2" class="results"></div>
 </div>
 </div>
 <script>
 function showPage(pageId) {
 document.getElementById('page1').style.display = 'none';
 document.getElementById('page2').style.display = 'none';
 document.getElementById(pageId).style.display = 'block';
 document.getElementById('btnPage1').classList.remove('active');
 document.getElementById('btnPage2').classList.remove('active');
 document.getElementById(`btn${pageId.charAt(0).toUpperCase() + pageId.slice(1)}`).classList.add('active');
 }
 function calculateFromFrequency() {
 const freqInput = document.getElementById('freq1').value.trim();
 const desiredFreqInput = document.getElementById('desiredFreq').value.trim();
 const f_osc = parseFloat(freqInput) *1e6;
 const desiredFreq = parseFloat(desiredFreqInput);
 const mode = parseInt(document.getElementById('mode1').value);
 const timer = parseInt(document.getElementById('timer1').value);
 const results = document.getElementById('results1');
 if (!validateFrequencyInputs(freqInput, desiredFreqInput, f_osc, desiredFreq, results)) return;
 const T =12 / f_osc; 
 const is16Bit = mode ===1;
 const maxCount = is16Bit ?65536 :256;
 const minFreq =1 / (2 * maxCount * T);
 const maxFreq =1 / (2 * T);
 if (desiredFreq < minFreq || desiredFreq > maxFreq) {
 showError(results, `Desired frequency out of range. Valid range: ${formatFreq(minFreq)} - ${formatFreq(maxFreq)}`);
 return;
 }
 const delay =1 / (2 * desiredFreq);
 let count, th, tl;
 count = Math.round(delay / T);
 if (count <=0 || count >= maxCount) {
 showError(results, `Frequency out of range after calculation. Valid range: ${formatFreq(minFreq)} - ${formatFreq(maxFreq)}`);
 return;
 }
 if (is16Bit) {
 count =65536 - count;
 th = (count >>8) &0xFF;
 tl = count &0xFF;
 } else {
 th = tl =256 - count;
 }
 const tmod = calculateTMOD(timer, mode);
 displayResults(results, th, tl, tmod);
}
 function calculateFromValues() {
 const freqInput = document.getElementById('freq2').value.trim();
 const f_osc = parseFloat(freqInput) *1e6;
 const mode = parseInt(document.getElementById('mode2').value);
 const timer = parseInt(document.getElementById('timer2').value);
 const results = document.getElementById('results2');
 const thStr = document.getElementById('thInput').value.trim();
 const tlStr = document.getElementById('tlInput').value.trim();
 const th = convertHexInput(thStr);
 const tl = convertHexInput(tlStr);
 if (!validateHexInputs(thStr, tlStr, th, tl, mode, results)) return;
 const T =12 / f_osc;
 let count, frequency;
 if (mode ===1) {
 count =65536 - ((th <<8) | tl);
 if (count <=0 || count >65535) {
 showError(results, "Computed count is out of valid range for16-bit mode.");
 return;
 }
 } else {
 if (th !== tl) {
 showError(results, "In Mode2, TH and TL must be identical.");
 return;
 }
 count =256 - th;
 }
 frequency =1 / (2 * count * T);
 const tmod = calculateTMOD(timer, mode);
 displayResults(results, th, tl, tmod, frequency);
 }
 function calculateTMOD(timer, mode) {
 return timer ===0 ? (mode <<0) : (mode <<4);
 }
 function validateFrequencyInputs(freqStr, desiredFreqStr, f_osc, desiredFreq, container) {
 if (freqStr === "" || isNaN(f_osc) || f_osc <=0) {
 showError(container, "Invalid crystal frequency. Please enter a positive number.");
 return false;
 }
 if (desiredFreqStr === "" || isNaN(desiredFreq) || desiredFreq <=0) {
 showError(container, "Invalid desired frequency. Please enter a positive number.");
 return false;
 }
 return true;
 }
 function validateHexInputs(thStr, tlStr, th, tl, mode, container) {
 const hexRegex = /^0x[a-fA-F0-9]{1,2}$|^[a-fA-F0-9]{1,2}$/;
 if (thStr === "" || !hexRegex.test(thStr)) {
 showError(container, "Invalid TH value. Enter a valid hex number (00 to FF).");
 return false;
 }
 if (tlStr === "" || !hexRegex.test(tlStr)) {
 showError(container, "Invalid TL value. Enter a valid hex number (00 to FF).");
 return false;
 }
 if (th <0 || th >0xFF) {
 showError(container, "TH value out of range. Must be between00 and FF.");
 return false;
 }
 if (tl <0 || tl >0xFF) {
 showError(container, "TL value out of range. Must be between00 and FF.");
 return false;
 }
 if (mode ===2 && th !== tl) {
 showError(container, "In Mode2, TH and TL must match.");
 return false;
 }
 return true;
 }
 function convertHexInput(hexStr) {
 hexStr = hexStr.trim();
 if (hexStr.toLowerCase().startsWith("0x")) {
 return parseInt(hexStr,16);
 }
 return parseInt(hexStr,16);
 }
 function displayResults(container, th, tl, tmod, frequency) {
 let output = `
 <p>TH =0x${th.toString(16).padStart(2, '0').toUpperCase()}</p>
 <p>TL =0x${tl.toString(16).padStart(2, '0').toUpperCase()}</p>
 <p>TMOD =0x${tmod.toString(16).padStart(2, '0').toUpperCase()}</p>`;
 if (frequency !== undefined) {
 output += `<p>Calculated Frequency: ${formatFreq(frequency)}</p>`;
 }
 container.innerHTML = output;
 }
 function formatFreq(freq) {
 return freq >=1000 ? `${(freq /1000).toFixed(2)} kHz` : `${freq.toFixed(2)} Hz`;
 }
 function showError(container, message) {
 container.innerHTML = `<div class="error">${message}</div>`;
 }
 function resetPage1() {
 document.getElementById('freq1').value = '11.0592';
 document.getElementById('desiredFreq').value = '';
 document.getElementById('results1').innerHTML = '';
 }
 function resetPage2() {
 document.getElementById('thInput').value = '';
 document.getElementById('tlInput').value = '';
 document.getElementById('results2').innerHTML = '';
 }
 showPage('page1');
 </script>
</body>
</html>
)rawliteral";
void handleRoot() {
 String pageContent = htmlContent;
 server.send(200, "text/html", pageContent); 
}
void handleNotFound() {
 String message = "File Not Found";
 message += "URI: ";
 message += server.uri(); 
 message += " Method: ";
 message += (server.method() == HTTP_GET) ? "GET" : "POST"; 
 message += "Arguments: ";
 message += server.args(); 
 message += " ";
 for (uint8_t i =0; i < server.args(); i++) {
 message += " " + server.argName(i) + ": " + server.arg(i) + "";
 }
 server.send(404, "text/plain", message); 
}
void setup() {
 Serial.begin(115200);
 Serial.println();
 Serial.println("Configuring Access Point...");
 if (strlen(password) ==0) {
 WiFi.softAP(ssid);
 } else if (strlen(password) >=8) {
 WiFi.softAP(ssid, password);
 } else {
 Serial.println("ERROR: Password must be at least8 characters long for WPA2. Creating OPEN AP instead.");
 WiFi.softAP(ssid);
 }
 IPAddress myIP = WiFi.softAPIP();
 Serial.print("AP IP address: ");
 Serial.println(myIP); 
 server.on("/", HTTP_GET, handleRoot); 
 server.onNotFound(handleNotFound); 
 server.begin();
 Serial.println("HTTP server started. Ready for connections.");
}
void loop() {
 server.handleClient();
}
