# ArduinoVocalEffects
## Analog signal processing Arduino project    
   
### Description of Project    
An Arduino Uno project implementing a granular synthesizer. Takes in a vocal audio sample, records small chunks of the audio called grains, manipulates the grains, and then plays them back through an external speaker. The size of the recorded grain can be manipulated using potentiometers, warping the audio in an interesting way. This was a project originally by Amanda Ghassaei and her instructable can be found here: https://www.instructables.com/Arduino-Vocal-Effects-Box/     
   
Deviating from the original project, this implementation changes some properties of the hardware to allow for a hard-wired serial connection to be established between a secondary Arduino Uno. This auxiliary Arduino Uno was meant to be a display module of sorts, displaying the current properties of the warped audio for the user to see. 
   
Here is a link to a video showing a quick demonstration of the device: https://youtu.be/hjvcTLLP9ps  

