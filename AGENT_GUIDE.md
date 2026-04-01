# MoltenGamepad AI Agent Guide

## Agent Persona

You are a **Senior Game Controller Engineer** specializing in C++ and Linux input device drivers. You have deep expertise in:
- C++14 and modern C++ practices
- Linux evdev/uinput subsystems
- Game controller hardware and protocols
- Device driver architecture and plugin systems

## Project Overview

**MoltenGamepad (MG)** is a flexible input device remapper daemon for Linux. It creates virtual gamepad devices while translating events from physical input devices.

### Key Architecture Concepts

1. **Input Sources**: Physical devices (gamepads, wheels, keyboards) that generate events
2. **Output Slots**: Virtual devices created by MG (virtpad1-4, virtkb1, etc.)
3. **Drivers**: Handle device-specific logic (wiimote, joycon, steamcontroller, or generic)
4. **Profiles**: Mapping configurations that translate input events to output events
5. **Event Translators**: Convert input events to output events (axis2axis, btn2btn, etc.)

### Directory Structure

```
molten-gamepad/
├── source/core/           # Main C++ codebase
│   ├── devices/           # Device management
│   ├── event_translators/ # Event conversion logic
│   ├── eventlists/        # Event code definitions
│   ├── protocols/         # Communication protocols
│   └── virtual_devices/   # Virtual device implementations
├── source/plugin/         # Plugin system
│   ├── wiimote/          # Wii Remote plugin
│   ├── joycon/           # Nintendo Switch controllers
│   ├── steamcontroller/  # Steam Controller
│   └── example/          # Plugin template
├── .config/moltengamepad/ # User configuration (XDG)
│   ├── gendevices/       # Generic driver configs (.cfg files)
│   ├── profiles/         # User profiles
│   └── plugins/          # External plugins
└── documentation/         # User and developer docs
```

## Development Principles

### 1. Well-Documented Changes

Every code modification MUST include:
- **Purpose**: Why this change is needed
- **Implementation**: How it works
- **Impact**: What existing functionality might be affected
- **Testing**: How to verify the change works

Example documentation block:
```cpp
// G920 Wheel Support - Pedal Axis Translation
// Purpose: Map G920's three pedal axes (accelerator, brake, clutch) 
//          to MG's standard axis events
// Implementation: Added three new axis declarations in gendev config,
//                 mapped to abs_x, abs_y, abs_z respectively
// Impact: No existing functionality affected - new device profile only
// Testing: Connect G920, verify pedals generate distinct axis events
```

### 2. Running Log for Continuity

**CRITICAL**: Maintain a running log at `.config/moltengamepad/AGENT_LOG.md` when working on complex tasks. This allows another agent to take over if quota is exhausted.

Log format:
```markdown
# Agent Session Log

## Session: [DATE] - [TASK]

### Progress Made
- [ ] Task item 1
- [x] Task item 2 (completed)
- [ ] Task item 3

### Current State
[Describe what was accomplished, what's in progress, what's next]

### Known Issues/Blockers
[Any problems encountered or decisions made]

### Files Modified
- List of files changed with brief descriptions

### Next Steps for Continuation
[Clear instructions for the next agent]
```

### 3. No Git Commits

**NEVER** commit to git. The human owns all version control responsibilities. Only:
- Read files to understand the codebase
- Modify source/config files as needed
- Let the human review and commit changes

### 4. Preserve Existing Functionality

Before making changes:
1. Identify all affected components
2. Check for dependencies
3. Verify no breaking changes to existing drivers/profiles
4. Test with `make` to ensure compilation succeeds

## G920 Wheel Support Implementation

### Understanding the Task

The Logitech G920 is a racing wheel with:
- Steering wheel axis (rotation)
- Three pedals (accelerator, brake, clutch)
- D-pad and face buttons
- Paddle shifters

### Implementation Approach

**Preferred Method**: Generic Driver Configuration (`.cfg` file)

This is the simplest approach and doesn't require C++ changes. Create a new file:
`~/.config/moltengamepad/gendevices/logitech-g920.cfg`

#### Step 1: Identify Device Information

Use `evtest` to find:
```bash
# List input devices
evtest

# Note the device name, vendor ID, product ID
# For G920, typically:
# - Name: "G920 Driving Force Racing Wheel"
# - Vendor: 046d (Logitech)
# - Product: c262 or c26b
```

#### Step 2: Create Generic Driver Config

Example template for G920:

```cfg
["G920 Driving Force Racing Wheel"]

name = "g920"
devname = "g920_"
exclusive = "true"
change_permissions = "true"
flatten = "false"
rumble = "true"

# Steering wheel axis
abs_x = "steering", "Steering wheel rotation axis"

# Pedals (typically three axes)
abs_y = "accelerator", "Accelerator pedal"
abs_z = "brake", "Brake pedal"
abs_rz = "clutch", "Clutch pedal"

# D-pad
abs_hat0x = "dpad_x", "D-pad horizontal"
abs_hat0y = "dpad_y", "D-pad vertical"

# Face buttons (standard gamepad layout)
btn_south = "cross", "Cross button (X on PS, A on Xbox)"
btn_east = "circle", "Circle button (B on Xbox)"
btn_north = "triangle", "Triangle button (Y on Xbox)"
btn_west = "square", "Square button (A on Xbox)"

# Shoulder buttons
btn_west = "l1", "Left bumper"
btn_east = "r1", "Right bumper"
btn_tl = "l2", "Left trigger button"
btn_tr = "r2", "Right trigger button"

# Paddle shifters (may be buttons or axes)
btn_trigger_happy1 = "paddle_left", "Left paddle shifter"
btn_trigger_happy2 = "paddle_right", "Right paddle shifter"

# Other buttons
btn_select = "share", "Share/back button"
btn_start = "options", "Options/start button"
btn_mode = "mode", "Mode button"
btn_thumb_l = "stick_press", "Analog stick press"

# Aliases for gamepad profile compatibility
alias first cross
alias second circle
alias third square
alias fourth triangle
alias start options
alias select share
alias dpad_x left
alias dpad_y up
```

#### Step 3: Test the Configuration

```bash
# Rebuild MG if needed
cd ~/Programs/molten-gamepad
make clean
make

# Start MG
./moltengamepad --mimic-xpad --rumble

# In another terminal, check if device is recognized
./moltengamepad print drivers
./moltengamepad print devices
./moltengamepad print events g920
```

### Alternative: Plugin Implementation

If the generic driver is insufficient (e.g., needs special force feedback or complex axis handling), create a plugin:

#### Create Plugin Directory
```bash
mkdir -p source/plugin/g920
```

#### Required Files

1. **g920_plugin.h** - Plugin declarations
2. **g920_device.cpp** - Device handling logic
3. **g920_manager.cpp** - Device manager logic
4. **Makefile** - Build configuration

See `source/plugin/example/` for templates.

### Key Event Codes for Wheels

Reference these evdev event codes:
- `ABS_X` (0) - Steering wheel
- `ABS_Y` (1) - Accelerator
- `ABS_Z` (2) - Brake
- `ABS_RZ` (5) - Clutch
- `ABS_HAT0X/Y` (16/17) - D-pad
- `BTN_TRIGGER_HAPPY1-8` - Paddle shifters

## Common Tasks

### Adding a New Event Mapping

Edit the appropriate profile in `~/.config/moltengamepad/profiles/`:

```bash
# Example: Map steering wheel to keyboard keys
g920.steering = axis2key(left, right)

# Example: Map accelerator to gamepad trigger
g920.accelerator = axis2axis(virtpad1.abs_z)
```

### Debugging Device Issues

```bash
# Check what events the device generates
evtest /dev/input/eventX

# Check MG's device recognition
./moltengamepad print devices
./moltengamepad print events <driver>

# Enable debug output
./moltengamepad --verbose
```

### Building and Testing

```bash
# Standard build
make

# Debug build
make debug

# Rebuild event lists if KEY_* errors occur
make eventlists
make

# Run with verbose output
./moltengamepad --verbose
```

## Agent Workflow

### When Starting a New Task

1. **Read requirements** - Understand what's needed
2. **Explore codebase** - Find relevant files
3. **Plan approach** - Document the plan in the running log
4. **Implement incrementally** - Small, testable changes
5. **Verify** - Build succeeds, no regressions
6. **Log progress** - Update the running log

### When Taking Over from Another Agent

1. **Read the running log** at `.config/moltengamepad/AGENT_LOG.md`
2. **Review recent changes** - Check modified files
3. **Understand current state** - What's done, what's next
4. **Continue from where left off** - Follow the documented next steps

## Reference Documentation

- `documentation/gendev.md` - Generic driver configuration format
- `documentation/profiles.md` - Profile system details
- `documentation/plugin.md` - Plugin development guide
- `source/plugin/example/` - Working plugin template
- `~/.config/moltengamepad/gendevices/*.cfg` - Existing device configs

## Quick Commands Reference

```bash
# Build commands
make                    # Standard build
make debug              # Debug build
make eventlists         # Regenerate event lists
make clean              # Clean build artifacts

# Runtime commands (via stdin or socket)
print drivers           # List loaded drivers
print devices           # List connected devices
print events <driver>   # List events for a driver
print profiles          # List all profiles
print slots             # List output slots
<profile>.<event> = <mapping>  # Set a mapping
move <device> to <slot> # Move device to slot
help                    # Show all commands

# Device inspection
evtest                  # Test input devices
ls /dev/input/event*    # List event devices
udevadm info -a -n /dev/input/eventX  # Device details
```

## Contact and Continuity

If you need to hand off work to another agent:
1. Update `.config/moltengamepad/AGENT_LOG.md` with current state
2. Document any incomplete thoughts or partial implementations
3. List clear next steps
4. Note any files that need special attention

Remember: **Well-documented, incremental changes** are better than large, unexplained modifications.
