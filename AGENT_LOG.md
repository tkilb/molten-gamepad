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
1. `/home/tylerkilburn/Programs/molten-gamepad/AGENT_GUIDE.md` - AI agent documentation
2. `/home/tylerkilburn/Programs/molten-gamepad/AGENT_LOG.md` - Running session log
3. `/home/tylerkilburn/.config/moltengamepad/gendevices/logitech-g920.cfg` - G920 wheel config (updated device name)
4. `/home/tylerkilburn/Programs/molten-gamepad/human.sh` - Diagnostic script

**NEXT STEPS**: Test MoltenGamepad with the G920.

### Known Issues/Blockers
- **G920 Device Name**: The exact device name string reported by the kernel needs verification. Common variants:
  - "G920 Driving Force Racing Wheel"
  - "Logitech G920 Driving Force Racing Wheel"
  - Different names for wheel vs pedals (they may appear as separate devices)
  
- **Separate Pedal Device**: The G920 pedals may appear as a separate input device from the wheel base. May need two config files or a multi-device match.

### Files Modified
- `/home/tylerkilburn/Programs/molten-gamepad/AGENT_GUIDE.md` - Created (AI agent documentation)
- `/home/tylerkilburn/Programs/molten-gamepad/AGENT_LOG.md` - Created (this file)

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

## Session Notes Template

### Session: [DATE] - [TASK]

### Objectives
- [ ] Task item 1
- [ ] Task item 2

### Progress Made
- Detailed bullet points of what was accomplished

### Current State
[Paragraph describing current progress and immediate next actions]

### Known Issues/Blockers
[Any problems encountered or decisions pending]

### Files Modified
- List of files changed with brief descriptions

### Next Steps for Continuation
[Clear, actionable instructions for the next agent]

---
