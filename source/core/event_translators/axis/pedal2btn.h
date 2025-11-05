#pragma once
#include "../event_change.h"

class pedal2btn : public event_translator {
public:
  virtual_device* virt_keyboard = nullptr;
  int btn;
  int pos_cache = 0;

  pedal2btn(int btn, virtual_device* virt_keyboard) : btn(btn), virt_keyboard(virt_keyboard) {
  }

  virtual void process(struct mg_ev ev, virtual_device* out);

  virtual pedal2btn* clone() {
    return new pedal2btn(*this);
  }

  static const char* decl;
  pedal2btn(std::vector<MGField>& fields);
  virtual void fill_def(MGTransDef& def);
};
