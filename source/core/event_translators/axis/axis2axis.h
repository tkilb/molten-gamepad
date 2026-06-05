#pragma once
#include "../event_change.h"

class axis2axis : public event_translator {
public:
  int out_axis;
  int direction;
  virtual_device *virt_keyboard = nullptr;
  int a_pressed = 0;
  int d_pressed = 0;
  axis2axis(int axis, int dir, virtual_device *virt_keyboard = nullptr) : out_axis(axis), direction(dir), virt_keyboard(virt_keyboard) {}
  virtual void process(struct mg_ev ev, virtual_device *out);

  virtual axis2axis *clone() { return new axis2axis(*this); }

  static const char *decl;
  axis2axis(std::vector<MGField> &fields);
  virtual void fill_def(MGTransDef &def);
};
