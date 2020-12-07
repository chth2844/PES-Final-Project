# PES-Final-Project

<br/>#**Project Topic**: Interrupt based motion and freefall detection on KL25Z-FRDM using MMA8451

<br/>**Introduction**: The final project makes use of the onboard MMA8451 accelerometer to detect freefall, motion and tap events. The TPM module on the KL25Z is used to control LED colours for various indications. A popular application for motion detection is auto-sleep and auto-wake in portable devices like cell phones and tablets. Freefall Detection is used in Active Hard Drive Protection technology.

<br/>**Functionality**: Event driven code developed will handle the following interrupt events

<br/>1) **Freefall Detection**: The Freefall function of the MMA8451 detects linear freefall when the object falls from a height of greater that 20cm. Upon detecting a free fall the onboard LED turns magenta and blinks for 10 seconds.

<br/>2) **Motion Detection**: The accelerometer tracks motion and the board adjusts the intensity of the onboard R, G, B LED’s in proportion to the acceleration values read by the board in the X, Y, Z direction.

<br/>3) **Tap Detection**: Tapping the board will toggle between freefall and motion detection. Upon entering freefall/motion detection mode the onboard LED will turn magenta/white and stay on for a period of 2s.

<br/>**Testing**: Testing is automated without requiring any human intervention. The MMA8451 sensor has a self-test mode which will be activated during testing in the debug build configuration. In self-test mode a small electrostatic force is applied to the sensor, simulating acceleration. If the acceleration recorded in the X, Y, Z direction is within the prescribed range in the datasheet, the test is passed (indicated by green LED) otherwise the test fails (indicated by red LED).
