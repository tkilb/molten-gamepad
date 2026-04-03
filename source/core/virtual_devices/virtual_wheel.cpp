#include "virtual_wheel.h"

virtual_wheel::virtual_wheel(std::string name, std::string descr, virtpad_settings settings, slot_manager* slot_man, uinput* ui) : virtual_device(name, descr, slot_man), padstyle(settings) {
  this->ui = ui;
  uinput_fd = -1;
}

virtual_wheel::~virtual_wheel() {
  destroy_uinput_devs();
}

void virtual_wheel::init() {
  uinput_ids ids = padstyle.u_ids;
  if (ids.phys.empty())
    ids.phys = "moltengamepad/" + name;
  uinput_fd = ui->make_wheel(ids, padstyle.rumble);
  if (uinput_fd >= 0 && padstyle.rumble) {
    auto self = shared_from_this();
    ui->watch_for_ff(uinput_fd, self);
  }
}

void virtual_wheel::destroy_uinput_devs() {
  if (uinput_fd >= 0) {
    ui->uinput_destroy(uinput_fd);
    uinput_fd = -1;
  }
}

void virtual_wheel::take_event(struct input_event in) {
  if (uinput_fd >= 0) {
    write(uinput_fd, &in, sizeof(in));
  }
}

void virtual_wheel::clear_outputs() {
  struct input_event out_ev;
  memset(&out_ev, 0, sizeof(out_ev));
  
  //Clear axes
  out_ev.type = EV_ABS;
  out_ev.value = 0;
  int axes[] = {ABS_X, ABS_Y, ABS_Z, ABS_RZ, ABS_HAT0X, ABS_HAT0Y};
  for (int axis : axes) {
    out_ev.code = axis;
    if (axis == ABS_X) out_ev.value = 32768; else out_ev.value = 0;
    take_event(out_ev);
  }

  //Clear buttons
  out_ev.type = EV_KEY;
  out_ev.value = 0;
  for (out_ev.code = 288; out_ev.code <= 313; out_ev.code++) take_event(out_ev);
  for (out_ev.code = 704; out_ev.code <= 711; out_ev.code++) take_event(out_ev);

  out_ev.type = EV_SYN;
  out_ev.code = SYN_REPORT;
  out_ev.value = 0;
  take_event(out_ev);
}
