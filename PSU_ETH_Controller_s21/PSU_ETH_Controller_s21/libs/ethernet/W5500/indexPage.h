
/*
 * indexPage.h
 *
 * Created: 06.07.2024 15:50:56
 *  Author: Vanya
 */ 
#ifndef  _INDEX_PAGE_H_
#define  _INDEX_PAGE_H_



const char psu_page[] = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html\r\n\r\n"
"<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>Power Supply Control</title>"
"<style>body{font-family:Arial,sans-serif;display:flex;flex-direction:column;align-items:center;padding:20px;background:#f0f0f0;}"
".container{max-width:400px;width:100%;background:white;padding:20px;border-radius:10px;box-shadow:0 0 10px rgba(0,0,0,0.1);}"
".title{text-align:center;margin-bottom:20px;}"
".row{display:flex;justify-content:space-between;margin-bottom:10px;}"
".row label{width:40%;}"
".row input{width:55%;text-align:right;}"
".button-row{display:flex;justify-content:space-between;margin-top:20px;}"
".button-row button{width:48%;padding:10px;background:#007bff;color:white;border:none;border-radius:5px;cursor:pointer;}"
".button-row button:hover{background:#0056b3;}"
".preset-row{display:flex;justify-content:space-between;margin-top:20px;}"
".preset-row button{width:32%;padding:10px;background:#28a745;color:white;border:none;border-radius:5px;cursor:pointer;}"
".preset-row button:hover{background:#218838;}</style></head>"
"<body><div class=\"container\"><h1 class=\"title\">Power Supply Control</h1>"
"<div class=\"row\"><label for=\"amp-display\">Amp:</label><input type=\"text\" id=\"amp-display\" readonly></div>"
"<div class=\"row\"><label for=\"volt-display\">Volt:</label><input type=\"text\" id=\"volt-display\" readonly></div>"
"<div class=\"row\"><label for=\"watt-display\">Watt:</label><input type=\"text\" id=\"watt-display\" readonly></div>"
"<div class=\"row\"><label for=\"amp-input\">Set Amp:</label><input type=\"text\" id=\"amp-input\"></div>"
"<div class=\"row\"><label for=\"volt-input\">Set Volt:</label><input type=\"text\" id=\"volt-input\"></div>"
"<div class=\"button-row\"><button onclick=\"setValues()\">Set Values</button>"
"<button onclick=\"controlDevice('psu','on')\">Turn PSU On</button></div>"
"<div class=\"button-row\"><button onclick=\"controlDevice('psu','off')\">Turn PSU Off</button>"
"<button onclick=\"controlDevice('fan','on')\">Turn Fan On</button></div>"
"<div class=\"button-row\"><button onclick=\"controlDevice('fan','off')\">Turn Fan Off</button></div>"
"<div class=\"preset-row\"><button onclick=\"setPreset(1)\">5V 1A</button>"
"<button onclick=\"setPreset(2)\">12V 2A</button>"
"<button onclick=\"setPreset(3)\">24V 2A</button></div></div>"
"<script>function fetchValues(){fetch('/get_vals')"
	".then(response=>response.json()).then(data=>{document.getElementById('amp-display').value=data.amp.toFixed(2);"
	"document.getElementById('volt-display').value=data.volt.toFixed(2);document.getElementById('watt-display').value=data.watt.toFixed(2);})"
".catch(error=>console.error('Error fetching values:',error));}"
"function setValues(){const amp=parseFloat(document.getElementById('amp-input').value);"
	"const volt=parseFloat(document.getElementById('volt-input').value);"
	"fetch(`/set_vals?amp=${amp}&volt=${volt}`)"
	".then(response=>response.json()).then(data=>{if(data.success){fetchValues();}else{alert('Error setting values');}})"
".catch(error=>console.error('Error setting values:',error));}"
"function controlDevice(device,action){fetch(`/control?device=${device}&action=${action}`)"
".then(response=>response.json()).then(data=>{if(data.success){alert(`${device.toUpperCase()} ${action.toUpperCase()} Successful`);}"
"else{alert(`Error ${action}ing ${device}`);}}).catch(error=>console.error(`Error ${action}ing ${device}:`,error));}"
"function setPreset(preset){let amp,volt;if(preset==1){amp=1.25;volt=5.3;}else if(preset==2){amp=2.0;volt=12.0;}else if(preset==3){amp=2.0;volt=24.0;}"
"document.getElementById('amp-input').value=amp;document.getElementById('volt-input').value=volt;setValues();}"
"setInterval(fetchValues,1000);</script></body></html>";



#endif