# Ninebot-BLE-PROTOCOL

The files and documents in this repository are published under the terms of the 
GNU Lesser General Public License as published by the Free Software Foundation; 
either version 2.1 of the License, or (at your option) any later version.

Los archivos y documentos publicados aquí estan publicados bajo la licencia 
GNU Lesser General Public License.

Si usan los datos de este repositorio, por favor indiquen la fuente y darme los creditos correspondientes.

Reverse engineering of UART protocol used to hack communication between BLE and ESC part of the scooter.
Used with ARM based Tiva (Stellaris) Launchpad by Texas Instruments. Can be ported to any uC.

Protocol dump:
https://pastebin.com/7Hn2hWcz


test code:

while(1)
{

  send_controls(desired_throttle,desired_brakes,!(question_counter));           // drive ESC
  
    if(question_counter>0x03)   question_counter=0x00; else question_counter++; // every 4th ask current state (battery%, velocity, light, beep, etc)
    
  ROM_ADCProcessorTrigger(ADC0_BASE, 3);                                        // state ADC conversion
  
  ROM_SysCtlDelay(ROM_SysCtlClockGet()/20);                                     // WAIT...
  
}

73deUb4raf
