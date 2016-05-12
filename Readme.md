# Speed Controller
This repository contains the source code for our custom speed controllers. More details will follow when this project exceeds the proof-of-concept state.


## TODO
- [x] Output PWM to FET's / H-Bridge
- [x] Read receiver signal
- [x] Proof of concept
- [x] Verify RC Input signal, eg verify there is a 10-40ms time the pin is low
- [x] Slow down motor response, to prevent magic smoke from exiting the FETs
- [x] Make min/max pulse length user-configurable.
- [x] Stabilize RC input (sampling + median filter)
- [ ] Add neutral signal (stick center) to 'programming' sequence.
- [ ] Read motor temperature and automatically slow down when the motor is too hot.
- [ ] Add LiPo battery protection
