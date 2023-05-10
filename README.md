# WiiChuck
An Arduino library for talking to every extension controller made for the Wii including the Nunchuk and Classic Controller over I²C.

## Supported Controllers:

* Nunchuk
* Classic Controller
* Guitar Hero Guitar
* Guitar Hero Drums
* DJ Hero
* Drawesome Tablet
* Taiko Drums


# Mapping

All controllers have been mapped across a single readable array so that code written for one Wii accessort can be made generic for all of Wii accessory devices. The values that come from the controller are scaled to a 0-255 range for all analog and for all digital values. Each value is stored in a single byte in the 'values[]' array, a public member of the accessory class. 

Initialize the controller first:

```
nunchuck1.begin();
```

In loop, when reading the controller values, first call:

```
nunchuck1.readData();    // Read inputs and update maps
```

Then read the controller values out of the 'values[]' array:

```
uint8_t joystickValueX = nunchuck1.values[1];
uint8_t joystickValueY = nunchuck1.values[2];

...

uint8_t lastValue = nunchuck1.values[19];

```

## Nunchuck mapping

```
	values[1]=map(getJoyX(),0,255,0,255);
	values[2]=map(getJoyY(),0,255,0,255);
	values[3]=map(getRollAngle(),0,1024,0,256);
	values[4]=map(getPitchAngle(),0,1024,0,256);
	values[5]=map(getAccelX(),0,1024,0,256);
	values[6]=map(getAccelY(),0,1024,0,256);

	values[7]=map(getAccelZ(),0,1024,0,256);
	values[8]=0;
	values[9]=0;
	values[10]=0;
	values[11]=getButtonZ()?255:0;
	values[12]=getButtonC()?255:0;
	values[13]=0;
	values[14]=0;

	values[15]=0;
	values[16]=0;
	values[17]=0;

	values[18]=0;
	values[19]=0;
```

## Classic Controller Mapping

```
	values[1]=map(getJoyXLeft(),0,64,0,256);
	values[2]=map(getJoyYLeft(),0,64,0,256);
	values[3]=map(getJoyXRight(),0,32,0,256);
	values[4]=map(getJoyYRight(),0,32,0,256);
	values[5]=0;
	values[6]=0;

	values[7]=getPadRight()?255:(getPadLeft()?0:128);
	values[8]=getPadDown()?0:(getPadUp()?255:128);
	values[9]=getButtonX()?255:0;
	values[10]=getButtonY()?255:0;
	values[11]=getButtonZLeft()?255:0;
	values[12]=map(getTriggerLeft(),0,32,0,256);
	values[13]=getButtonA()?255:0;
	values[14]=getButtonB()?255:0;

	values[15]=getButtonMinus()?0:(getButtonPlus()?255:128);
	values[16]=getButtonHome()?255:0;
	values[17]=0;

	values[18]=map(getTriggerRight(),0,32,0,256);
	values[19]=getButtonZRight()?255:0;
```

## DJ Table mapping

```
	values[1]=map(getCrossfadeSlider(),0,255,0,256);
	values[2]=map(getEffectDial(),0,255,0,256);
	values[3]=map(getStickX(),0,255,0,256);
	values[4]=map(getStickY(),0,255,0,256);
	values[5]=map(getRightDJTable(),0,255,0,256);
	values[6]=map(getLeftDJTable(),0,255,0,256);

	values[7]=getPlusButton()?255:(getMinusButton()?0:128);
	values[8]=getEuphoriaButton()?255:0;
	values[9]=0;
	values[10]=0;
	values[11]=getRightBlueButton()?255:0;
	values[12]=getRightRedButton()?255:0;
	values[13]=getRightGreenButton()?255:0;
	values[14]=getLeftBlueButton()?255:0;

	values[15]=getLeftRedButton()?255:0;
	values[16]=getLeftGreenButton()?255:0;
	values[17]=getButtonPlus()?255:0;

	values[18]=getButtonZLeft()?255:0;
	values[19]=getButtonZRight()?255:0;
```

## Drawesome Mapping

```
	values[1] = map(getPenXPosition(), 0, 64, 0, 256);
	values[2] = map(getPenYPosition(), 0, 64, 0, 256);
	values[3] = map(getPenPressure(), 0, 32, 0, 256);
	values[4] = 0;
	values[5] = 0;
	values[6] = 0;

	values[7] = 0;
	values[8] = 0;
	values[9] = 0;
	values[10] = 0;
	values[11] = getPenContact() ? 255 : 0;
	values[12] = 0;
	values[13] = 0;
	values[14] = 0;

	values[15] = 0;
	values[16] = 0;
	values[17] = 0;

	values[18] = 0;
	values[19] = 0;
```

## Drums Mapping

```
	values[1]=map(getCrossfadeSlider(),0,255,0,256);
	values[2]=map(getEffectDial(),0,255,0,256);
	values[3]=map(getStickXGuitar(),0,255,0,256);
	values[4]=map(getStickYGuitar(),0,255,0,256);
	values[5]=map(getRightDJTable(),0,255,0,256);
	values[6]=map(getLeftDJTable(),0,255,0,256);

	values[7]=getPlusButtonGuitar()?255:(getMinusButtonGuitar()?0:128);
	values[8]=getEuphoriaButton()?255:0;
	values[9]=0;
	values[10]=0;
	values[11]=getRightBlueButton()?255:0;
	values[12]=getRightRedButton()?255:0;
	values[13]=getRightGreenButton()?255:0;
	values[14]=getLeftBlueButton()?255:0;

	values[15]=getLeftRedButton()?255:0;
	values[16]=getLeftGreenButton()?255:0;
	values[17]=getButtonPlus()?255:0;

	values[18]=getButtonZLeft()?255:0;
	values[19]=getButtonZRight()?255:0;
```

## Guitar Mapping

```
	values[1]=map(getWhammyBar(),0,255,0,256);
	values[2]=0;
	values[3]=0;
	values[4]=0;
	values[5]=0;
	values[6]=0;

	values[7]=getPlusButtonGuitar()?255:(getMinusButtonGuitar()?0:128);
	values[8]=getStrumUp()?255:(getStrumDown()?0:128);
	values[9]=getPedalButton()?255:0;
	values[10]=0;
	values[11]=getGreenButton()?255:0;
	values[12]=getRedButton()?255:0;
	values[13]=getYellowButton()?255:0;
	values[14]=getBlueButton()?255:0;

	values[15]=getOrangeButton()?255:0;
	values[16]=getLeftGreenButton()?255:0;
	values[17]=getButtonPlus()?255:0;

	values[18]=0;
	values[19]=0;
```



# Repository Structure 
This repository is forked from a curated set of old Arduino Libraries. I kept the old commits and the fork linking to keep attribution to the work done before I picked up the torch. In my mind we all see farther by standing on the shoulders of giants, so it is only proper to give credit where credit is due.


