#include "axis2axis.h"
#include "../event_translator_macros.h"
#include <linux/input-event-codes.h>

void axis2axis::process(struct mg_ev ev, virtual_device *out) {
  int value = ev.value * direction;

  if (direction == 2) {
    value = (ev.value + ABS_RANGE) / 2;
  }
  if (direction == -2) {
    value = -(ev.value + ABS_RANGE) / 2;
  }
  if (direction == 3) {
    value = ev.value - ABS_RANGE;
  }
  if (direction == -3) {
    value = -(ev.value - ABS_RANGE);
  }
  if (direction == 4) {
    value = ABS_RANGE - ((ev.value + ABS_RANGE) / 2);

    if (virt_keyboard) {
      struct input_event key_ev;
      memset(&key_ev, 0, sizeof(key_ev));
      key_ev.type = EV_KEY;
      bool changed = false;

      // // if value is at 95% of abs, send keypress 'D'
      // int d_val = (value >= 0.95 * ABS_RANGE);
      // if (d_val != d_pressed) {
      //   key_ev.code = KEY_D;
      //   key_ev.value = d_val;
      //   write_out(key_ev, virt_keyboard);
      //   d_pressed = d_val;
      //   changed = true;
      // }
      //
      // // if value is at 5% of abs, send keypress 'A'
      // int a_val = (value <= 0.05 * ABS_RANGE);
      // if (a_val != a_pressed) {
      //   key_ev.code = KEY_A;
      //   key_ev.value = a_val;
      //   write_out(key_ev, virt_keyboard);
      //   a_pressed = a_val;
      //   changed = true;
      // }
      //
      // if (changed) {
      //   flush(virt_keyboard);
      // }
    }
  }


  if (value < -ABS_RANGE)
    value = -ABS_RANGE;
  if (value > ABS_RANGE)
    value = ABS_RANGE;

  struct input_event out_ev;
  memset(&out_ev, 0, sizeof(out_ev));
  out_ev.type = EV_ABS;
  out_ev.code = out_axis;
  out_ev.value = value;
  write_out(out_ev, out);
}

const char *axis2axis::decl = "axis = axis2axis(axis_code, int direction=1)";
axis2axis::axis2axis(std::vector<MGField> &fields) {
  BEGIN_READ_DEF;
  READ_AXIS(out_axis);
  READ_INT(direction);
  if (HAS_NEXT) {
    READ_KEYBOARD(virt_keyboard);
  }
}
void axis2axis::fill_def(MGTransDef &def) {
  BEGIN_FILL_DEF("axis2axis");
  FILL_DEF_AXIS(out_axis);
  FILL_DEF_INT(direction);
  FILL_DEF_KEYBOARD(virt_keyboard);
}
