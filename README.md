# IRTadiran

This is an implementation for IRremote for Tadirn AC device common in israel.

The i implementation was made oriiginally for [IRremoteESP8266](https://github.com/markszabo/IRremoteESP8266)  which is an addaptation for ESP8266 library [Sming](https://github.com/SmingHub/Sming) but it should work for IRremote for arduino

The base set of features include
<ul>
<li> On Off feature
<li> temp manipulation
<li> fan speed manipulation
<li> swing manipulation
</ul>

Mode (pretty sure about cool and heat less sure about the other) :
<ul>
  <li> 1: Cool
  <li> 2: Heat
  <li> 3: circulate
  <li> 4: Dry
</ul>

Usage:
```cpp
#include "IRremote.h"
#include "IRTadiran.h"
  
void loop()
{
  IRsend irsend(D3);
  IRTadiran ir(&irsend);
  e.send(true ,     //On
    1,              // Mode - Cool
    1,              // fan spead 1-4
    25,             // temp
    false);         // swing 
}
```
