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

  if (value < -ABS_RANGE)
    value = -ABS_RANGE;
  if (value > ABS_RANGE)
    value = ABS_RANGE;

  if (direction == 4) {
    value = SHORT_ABS_RANGE - (SHORT_ABS_CONVERSION * ev.value);
  }

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
