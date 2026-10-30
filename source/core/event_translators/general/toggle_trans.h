#pragma once
#include "../event_change.h"
#include <atomic>

// Toggle Event Translator
// Purpose: Enables latching/autorun toggle behavior for buttons and keys.
//          Pressing once latches the output event in the active/down state;
//          pressing again unlatches and releases it. In addition, any external
//          input writing the same key to the target virtual device (e.g. manual 'W')
//          automatically cancels and clears the toggle.
// Implementation: Inherits from event_translator and virt_dev_listener.
//                 Subscribes to target virtual_device (e.g. virtual keyboard)
//                 to observe events on the destination key and clear toggle state.
// Impact: New event translator; existing translators and profiles are unaffected.
// Testing: Map a button to toggle(key(key_w)) or toggle(key_w), verify autorun
//          toggles ON/OFF, and verify stick/manual forward clears the toggle.

class toggle_trans : public event_translator, public virt_dev_listener {
public:
  event_translator* trans = nullptr;
  virtual_device* virt_keyboard = nullptr;

  std::atomic<bool> toggled{false};
  std::atomic<bool> is_emitting_self{false};
  std::atomic<virtual_device*> listening_dev{nullptr};
  int target_code = -1;

  toggle_trans(event_translator* trans = nullptr, virtual_device* virt_kb = nullptr);
  virtual ~toggle_trans();

  virtual void process(struct mg_ev ev, virtual_device* out) override;
  virtual void process_recurring(virtual_device* out) const override;
  virtual void attach(input_source* source) override;
  virtual bool wants_recurring_events() override;

  virtual int get_out_button() const override;
  virtual virtual_device* get_target_device(virtual_device* default_out) const override;

  virtual void on_virt_dev_event(const struct input_event& in) override;

  virtual toggle_trans* clone() override;

  static const char* decl;
  toggle_trans(std::vector<MGField>& fields);
  virtual void fill_def(MGTransDef& def) override;

private:
  void ensure_listener(virtual_device* target);
};
