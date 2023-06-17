# Neuropathy-feedback-device
This project attempts to make a wearable haptic feedback device which assists people with neuropathic symptoms in gaining back feedback in the limbs they have less neural feedback from. This project is not an attempt to cure Neuropathy. Merely an attempt to make a haptic feedback wearable device which is user friendly and easy to make. 
## The system components
- 2x Seeed studio xiao nRF52840
- Flexible Force sensing resistors (FSR)
- vibration motor modules
- 10 kOhm resistors 
- 2 lithium ion batteries 1250 mAh (this can change in the future when the code is optimized and power usage is known)
- 2 switches
- 4x JST-XH connectors
## Schematic and PCB design Peripheral
Instead of a schematic with the components this is the actual schematic I used to create the first PCB for this device. Replacing wired jumper wires it made more sense to add the connectors to a circuit board and solder the microcontroller onto the PCB itself. I don't know yet if this is the best approach. Since eventually I probably want to recycle the microcontroller. However, it decreases the footprint of the device dramatically. And I hope to make it such an integrated device already that I can already start user testing in real life situations. Recycling the microcontroller might be possible eventually with a heatgun. This would be great. 
![Schematic_Peripheral_V3](https://github.com/max-1200/Neuropathy-feedback-device/assets/71038234/29c1c603-bfb9-4915-ac42-e398dd00685d)
![PCB_Peripheral_V1](https://github.com/max-1200/Neuropathy-feedback-device/assets/71038234/0964df9b-1de6-42af-8b50-029e1727d1d1)

## Schematic Central
For Now I really trying to finalize the peripheral side of this project. When this is done I will get back to integration into fabric or a wristband and this is where Central will be further developed. 
![Schematic_Central_V1](https://github.com/max-1200/Neuropathy-feedback-device/assets/71038234/48620f34-6957-480d-835c-b1bfacd19995)

