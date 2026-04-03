# Agent Session Log

## Purpose
This log tracks AI agent work on the MoltenGamepad project. It enables continuity when agent quotas are exhausted or when switching between sessions.

---

## Session: 2026-04-01 - Initial Setup and G920 Wheel Support

### Agent
Senior Game Controller Engineer AI Agent

### Objectives
- [x] Create AI agents documentation file (AGENT_GUIDE.md)
- [x] Create running log template (this file)
- [ ] Create Logitech G920 wheel configuration
- [ ] Test G920 configuration with actual hardware
- [ ] Document any issues or adjustments needed

### Progress Made
- [x] Created comprehensive AGENT_GUIDE.md with:
  - Agent persona and principles
  - Project architecture overview
  - G920 implementation guide
  - Common tasks and workflows
  - Running log usage instructions
  
- [x] Created AGENT_LOG.md template for session tracking

- [x] Created G920 Configuration File:
  - Created `~/.config/moltengamepad/gendevices/logitech-g920.cfg`
  - Includes steering wheel, pedals, D-pad, face buttons, paddle shifters
  - Includes aliases for gamepad profile compatibility
  - Includes usage notes and troubleshooting tips
  
- [x] Fixed G920 Device Detection:
  - Issue: G920 not appearing as input device
  - Solution: Arch Wiki directed us to use hid_logitech_hidpp driver (already loaded)
  - Device now appears as: `Logitech G920 Driving Force Racing Wheel` at `/dev/input/event269` and `/dev/input/js4`
  - Updated config file with correct device name

### Current State
**G920 is now detected by the system.** The configuration file has been updated with the correct device name.

Files created/modified:
1. `~/Programs/molten-gamepad/AGENT_GUIDE.md` - AI agent documentation
2. `~/Programs/molten-gamepad/AGENT_LOG.md` - Running session log
3. `~/.config/moltengamepad/gendevices/logitech-g920.cfg` - G920 wheel config (updated device name)
4. `~/Programs/molten-gamepad/human.sh` - Diagnostic script

**NEXT STEPS**: Test MoltenGamepad with the G920.

### Known Issues/Blockers
- **G920 Device Name**: The exact device name string reported by the kernel needs verification. Common variants:
  - "G920 Driving Force Racing Wheel"
  - "Logitech G920 Driving Force Racing Wheel"
  - Different names for wheel vs pedals (they may appear as separate devices)
  
- **Separate Pedal Device**: The G920 pedals may appear as a separate input device from the wheel base. May need two config files or a multi-device match.

### Files Modified
- `~/Programs/molten-gamepad/AGENT_GUIDE.md` - Created (AI agent documentation)
- `~/Programs/molten-gamepad/AGENT_LOG.md` - Created (this file)

### Next Steps for Continuation
1. **User Action Required**: Run `evtest` with G920 connected to get exact device name(s)
   ```bash
   evtest
   # Select the G920 device
   # Note the exact name string at the top of the output
   ```

2. Create the G920 config file at `~/.config/moltengamepad/gendevices/logitech-g920.cfg`

3. Test the configuration:
   ```bash
   cd ~/Programs/molten-gamepad
   ./moltengamepad --mimic-xpad --rumble
   # In another terminal:
   ./moltengamepad print devices
   ./moltengamepad print events g920
   ```

4. Adjust axis/button mappings based on actual evtest output

5. Test with actual games or `evtest` on the virtual output device

---

## Session: 2026-04-01 - G920 Configuration and Testing

### Agent
Senior Game Controller Engineer AI Agent

### Objectives
- [x] Restore G920 force feedback (FFB) functionality
- [x] Create G920 configuration for MoltenGamepad
- [x] Test G920 configuration with `moltengamepad`

### Progress Made
- [x] **Restored G920 FFB**: 
    - Issue: FFB command queue was flooded, causing delays/unresponsiveness.
    - Solution: Reloaded the `hid_logitech_hidpp` driver (`rmmod`/`modprobe`).
    - Verified: Wheel performed calibration dance and `fftest` now works.
- [x] **Created G920 Config**:
    - Created `~/.config/moltengamepad/gendevices/logitech-g920.cfg`.
    - Format: `key(##)` for buttons, friendly names for axes.
    - Labels: Xbox naming conventions (A, B, X, Y, LB, RB, Back, Start, Guide).
- [x] **Renamed Config and Device**:
    - Renamed `~/.config/moltengamepad/gendevices/logitech-g920.cfg` to `~/.config/moltengamepad/gendevices/g920.cfg`.
    - Updated `devname = "g920"` in the file.
- [x] **Verified MoltenGamepad Integration**:
    - Ran `moltengamepad --mimic-xpad --rumble`.
    - Confirmed `g920_1` device is plugged in.
    - Verified all axes (steering, pedals, dpad) and buttons (A, B, X, Y, bumpers, paddles, etc.) are correctly mapped.

### Current State
**G920 is fully configured and functional in MoltenGamepad.**

The driver is correctly mapping the physical events to the defined labels, and `moltengamepad` is ready to be used with games. The FFB issue was resolved by a driver reload.

### Files Modified
- `~/.config/moltengamepad/gendevices/logitech-g920.cfg` - Created G920 device definition.
- `~/Programs/molten-gamepad/AGENT_LOG.md` - Updated session log.

### Next Steps for Continuation
1. **In-game Testing**: Test the wheel in a game (e.g., Forza, Assetto Corsa) using a MoltenGamepad profile.
2. **Profile Tuning**: Create or adjust MG profiles to map `g920` axes/buttons to specific game needs.
3. **Persist FFB Fix**: If the "command queue flooded" issue recurs, consider a udev rule or script to automatically reload the driver on connection if necessary (though usually, a one-time fix is enough unless the device is disconnected).

---
