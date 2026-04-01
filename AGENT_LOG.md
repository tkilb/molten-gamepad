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

- [ ] G920 Configuration: **IN PROGRESS**
  - Need to create `~/.config/moltengamepad/gendevices/logitech-g920.cfg`
  - Need user to run `evtest` to verify exact device name and event codes
  - Template config ready in AGENT_GUIDE.md

### Current State
The AI agents file has been created at `/home/tylerkilburn/Programs/molten-gamepad/AGENT_GUIDE.md`. 

The G920 configuration file needs to be created, but requires verification of the exact device name string from evtest output to ensure proper device matching.

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
