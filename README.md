# ESP8266HVSG
A High voltage signal generator with ESP8266

# How does it work?
First, the voltage from the lithium battery(3.0-4.2v) is stepped up to ~150v by a boost converter with a MAX1771 IC.
<img width="1157" height="569" alt="螢幕擷取畫面 2026-01-15 202716" src="https://github.com/user-attachments/assets/a809f3d0-2f7a-45ec-83a1-71cd8ad54d85" />

After that, the high-voltage DC is current limited by a PNP bipolar transistor.
<img width="704" height="711" alt="螢幕擷取畫面 2026-01-15 203012" src="https://github.com/user-attachments/assets/33a183ee-8d88-424c-accb-c1dcc62d5c83" />

After the current is limited to a safe level , the high-voltage DC is converted to pulses with a H-bridge.
<img width="1343" height="483" alt="螢幕擷取畫面 2026-01-15 203209" src="https://github.com/user-attachments/assets/efafc5b6-e050-4ab7-a5a3-826e7c872c31" />

The H-bridge can be controlled by a MCU(ESP8266 here) to output different frequency and different current pulses.
<img width="647" height="502" alt="螢幕擷取畫面 2026-01-15 203333" src="https://github.com/user-attachments/assets/e332d30b-f0bb-4081-b984-897ac5c60bc5" />
<img width="576" height="513" alt="螢幕擷取畫面 2026-01-15 203338" src="https://github.com/user-attachments/assets/f3694898-1c07-462b-9f2d-ea070fc8becb" />

After all these steps are completed, the pulse signal will be output through the 3.5 mm audio jack.
<img width="753" height="299" alt="螢幕擷取畫面 2026-01-15 203403" src="https://github.com/user-attachments/assets/36285751-91d9-4b52-bbf7-f78190bf39fb" />

# How to use it?
The device's firmware is written using Arduino IDE and currently (January 15, 2026) only supports three pulse modes.

It can be controlled via Wi-Fi and a web browser.

<img width="347" height="138" alt="螢幕擷取畫面 2026-01-15 203859" src="https://github.com/user-attachments/assets/33e984cb-5e61-4bb0-9cb6-baf3ff798cd2" />
<img width="606" height="653" alt="螢幕擷取畫面 2026-01-15 203830" src="https://github.com/user-attachments/assets/ff96220f-396e-414c-952c-65eb47542845" />

To access the user interface, you need to connect to the device's access point (AP).

SSID:8266HVSG PASSWORD:8266HVSG

And then open http://192.168.6.1 in your web browser
<img width="338" height="86" alt="螢幕擷取畫面 2026-01-15 204353" src="https://github.com/user-attachments/assets/5032b1ea-84fc-41b1-aee9-42a3c9fc547b" />
And there you have your user interface to control the device

# What to do if I don't want to use a web browser or I want to use a smartphone?
I also created a app with App Inventor , which is technically a customized web browser

<img width="278" height="311" alt="Screenshot_20260115-204938" src="https://github.com/user-attachments/assets/63664c9b-ad43-4398-89be-25bd41435a09" />

<img width="1080" height="1920" alt="Screenshot_20260115-204920" src="https://github.com/user-attachments/assets/c7dd6d1c-fa06-42c7-8482-3468e7f2bbb1" />

<img width="1080" height="1920" alt="Screenshot_20260115-204931" src="https://github.com/user-attachments/assets/56f65c8e-d242-46e5-bff2-ce0593df9709" />

# How to use the UI?

<img width="1080" height="1920" alt="EScreenshot_20260115-204920" src="https://github.com/user-attachments/assets/0e89aa4a-2c8f-47f4-815b-dedf007fe66f" />

<img width="1080" height="1920" alt="EScreenshot_20260115-204931" src="https://github.com/user-attachments/assets/81fcc878-04ea-44fd-aa0c-4dd9645b4365" />

<h1>Warinig : As you can see there is a button for unlocking the current limit from 487 to 1023 but doing this might be dangerous if you accidentally touch the output when it's current is set too high.</h1>
