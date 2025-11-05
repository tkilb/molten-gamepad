#include "pedal2btn.h"
#include "../event_translator_macros.h"
#include <linux/input-event-codes.h>

void pedal2btn::process(struct mg_ev ev, virtual_device* out) {
  struct input_event out_ev;
  memset(&out_ev, 0, sizeof(out_ev));

  out_ev.type = EV_KEY;
  out_ev.code = btn;
  out_ev.value = ev.value > MIN_PEDAL_VALUE;
  if (out_ev.value != pos_cache) {
    virtual_device* out_dev = btn < MAX_KEYBOARD_ASCII ? virt_keyboard : out;
    write_out(out_ev, out_dev);

    flush(out_dev);

    pos_cache = out_ev.value;
  }
}

const char* pedal2btn::decl = "axis = pedal2btn(key_code)";
pedal2btn::pedal2btn(std::vector<MGField>& fields) {
  BEGIN_READ_DEF;
  READ_KEY(btn);
}
void pedal2btn::fill_def(MGTransDef& def) {
  BEGIN_FILL_DEF("pedal2btn");
  FILL_DEF_KEY(btn);
}
