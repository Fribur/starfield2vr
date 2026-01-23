# starfield2vr

starfield2vr is a VR modding project for Starfield that aims to deliver immersive gameplay. This project is based on [PrayDog's REFramework](https://github.com/praydog/REFramework/).

## Prerequisites

- Set your VR headset as the default OpenXR engine
- Install ViGEmBus driver for motion controllers to work as joystick:
  [ViGEmBus v1.22.0](https://github.com/nefarius/ViGEmBus/releases/download/v1.22.0/ViGEmBus_1.22.0_x64_x86_arm64.exe)

## Installation

### Steam
Copy content from archive `starfield2vr-{openxr/openvr}-vX.X.X.zip` into:
```
C:\Program Files (x86)\Steam\steamapps\common\Starfield
```

### Gamepass
Copy content from archive `starfield2vr-{openxr/openvr}-vX.X.X.zip` into:
```
C:\XboxGames\Starfield\Content
```

## In-game Settings

- **Display:** Set to Windowed
- **Frame Generation:** Off
- **VSYNC:** Off
- **Motion Blur:** Off
- **Depth of Field:** Off
- **Dynamic Resolution:** Off (recommended)

## Notes

- In-game Overlay has F11 hotkey (works only on main display, not in HMD)
- Mod is based on [PrayDog's REFramework](https://github.com/praydog/REFramework/)

## Controller Layout

| Control | Action |
|---------|--------|
| Thumbrest | Left/Right buttons respectively |
| Left Grip + Left Stick Button | Turn screen to flat screen |
| Left Grip + Left Joystick | Acts as cross in joystick |
| Right Grip | Aim (joystick left trigger) |
| Left Grip + System Button | Xbox Start or system menu button (Switches between third-person/first-person views) |

## Additional Tips

- TAA, DLSS, and CAS will work; Frame Generation will not
- **Remember to never turn on Frame Generation**

## Known Limitations/Issues

- OpenVR missing bindings for controllers

## Video Tutorials

- [Starfield VR Setup Guide by Good Samaritan](https://youtu.be/UKt2utvotxA?si=2EJMFvhelMTlFuHG)
- [Starfield VR Tutorial by ParadiseDecay](https://youtu.be/cJ9ccj92xNM?t=475&si=15dW7ORzva_0O0GM)
