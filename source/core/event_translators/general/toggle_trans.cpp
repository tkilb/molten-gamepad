#include "toggle_trans.h"
#include "../event_translator_macros.h"

// Toggle Event Translator Implementation
// Purpose: Implement toggle latching logic and cross-input cancellation.
// Implementation: On button down (ev.value > 0), toggles state between 0 and 1.
//                 Button up is suppressed to keep the key held down.
//                 Listens on target virtual device; when an external source emits
//                 the target key, toggled state resets to false.
// Impact: None on existing codebase.
// Testing: Test with virtual keyboard via key(key_w) and observe cancellation.

const char* toggle_trans::decl = "event = toggle(trans)";

toggle_trans::toggle_trans(event_translator* trans, virtual_device* virt_kb)
    : trans(trans ? trans->clone() : nullptr), virt_keyboard(virt_kb) {
}

toggle_trans::~toggle_trans() {
  virtual_device* dev = listening_dev.load();
  if (dev) {
    dev->unregister_listener(this);
    listening_dev.store(nullptr);
  }
  if (trans) {
    delete trans;
    trans = nullptr;
  }
}

void toggle_trans::ensure_listener(virtual_device* target) {
  if (target_code < 0) {
    target_code = get_out_button();
  }
  virtual_device* current = listening_dev.load();
  if (target && current != target) {
    virtual_device* old_dev = listening_dev.exchange(target);
    if (old_dev) {
      old_dev->unregister_listener(this);
    }
    target->register_listener(this);
  }
}

void toggle_trans::on_virt_dev_event(const struct input_event& in) {
  if (is_emitting_self.load()) {
    return;
  }
  if (in.type == EV_KEY && in.code == target_code) {
    if (toggled.load()) {
      // External event sent our target key: clear autorun toggle
      toggled.store(false);
    }
  }
}

int toggle_trans::get_out_button() const {
  if (target_code >= 0) {
    return target_code;
  }
  return trans ? trans->get_out_button() : -1;
}

virtual_device* toggle_trans::get_target_device(virtual_device* default_out) const {
  if (trans) {
    virtual_device* dev = trans->get_target_device(default_out);
    if (dev && dev != default_out) {
      return dev;
    }
  }
  int code = get_out_button();
  if (code >= 0 && is_keyboard_key(code) && virt_keyboard) {
    return virt_keyboard;
  }
  return default_out;
}

void toggle_trans::process(struct mg_ev ev, virtual_device* out) {
  virtual_device* target_dev = get_target_device(out);
  ensure_listener(target_dev);

  if (ev.value > 0) {
    if (!toggled.load()) {
      toggled.store(true);
      is_emitting_self.store(true);
      if (trans) {
        trans->process({1, ev.override_type, ev.override_code, ev.override_value}, target_dev ? target_dev : out);
      }
      is_emitting_self.store(false);
      flush(target_dev ? target_dev : out);
    } else {
      toggled.store(false);
      is_emitting_self.store(true);
      if (trans) {
        trans->process({0, ev.override_type, ev.override_code, ev.override_value}, target_dev ? target_dev : out);
      }
      is_emitting_self.store(false);
      flush(target_dev ? target_dev : out);
    }
  }
  // ev.value == 0 (button release) is ignored so the toggled key stays latched
}

void toggle_trans::process_recurring(virtual_device* out) const {
  if (trans) {
    virtual_device* target_dev = get_target_device(out);
    trans->process_recurring(target_dev ? target_dev : out);
  }
}

void toggle_trans::attach(input_source* source) {
  virtual_device* target_dev = get_target_device(nullptr);
  if (target_dev) {
    ensure_listener(target_dev);
  }
  if (trans) {
    trans->attach(source);
  }
}

bool toggle_trans::wants_recurring_events() {
  return trans ? trans->wants_recurring_events() : false;
}

toggle_trans* toggle_trans::clone() {
  auto cl = new toggle_trans(this->trans, this->virt_keyboard);
  cl->target_code = this->target_code;
  return cl;
}

toggle_trans::toggle_trans(std::vector<MGField>& fields) {
  BEGIN_READ_DEF;
  READ_TRANS(trans, MG_TRANS);
}

void toggle_trans::fill_def(MGTransDef& def) {
  BEGIN_FILL_DEF("toggle");
  FILL_DEF_TRANS(trans, MG_TRANS);
}
