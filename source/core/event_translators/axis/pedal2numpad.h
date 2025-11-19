#pragma once
#include "../event_change.h"

#define PEDAL_2_NUMPAD "pedal2numpad"
#define STOP_KEY KEY_S
#define TICK_COUNTER_MAX 1
#define STOP_VAL_INDEX 10

class pedal2numpad : public event_translator {
public:
  virtual_device* virt_keyboard = nullptr;
  int val_caches[11] = {0};
  int tick_counter = 0;
  int prev_tick_index = STOP_VAL_INDEX;

  pedal2numpad(virtual_device* virt_keyboard) : virt_keyboard(virt_keyboard) {
  }

  virtual void process(struct mg_ev ev, virtual_device* out);

  virtual pedal2numpad* clone() {
    return new pedal2numpad(*this);
  }

  static const char* decl;
  pedal2numpad(std::vector<MGField>& fields);
  virtual void fill_def(MGTransDef& def);
};
