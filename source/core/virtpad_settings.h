#pragma once
#include "uinput.h"

enum virtpad_type { VIRTPAD_GAMEPAD, VIRTPAD_WHEEL };

struct virtpad_settings {
  uinput_ids u_ids;
  bool dpad_as_hat;
  bool analog_triggers;
  bool rumble;
  virtpad_type type;
};
