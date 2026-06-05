#pragma once
#include "virtual_device.h"
#include "../virtpad_settings.h"

class virtual_joystick : public virtual_device {
public:
  virtual_joystick(std::string name, std::string descr, virtpad_settings settings, slot_manager* slot_man, uinput* ui);
  virtual ~virtual_joystick();
  virtual void take_event(struct input_event in);
  virtual void clear_outputs();
  virtual void init();
protected:
  virtpad_settings padstyle;
  virtual void destroy_uinput_devs();
  int uinput_fd;
};
