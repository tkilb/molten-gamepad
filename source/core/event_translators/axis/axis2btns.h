#pragma once
#include "../event_change.h"

class axis2btns : public event_translator {
public:
  virtual_device* virt_keyboard = nullptr;
  int neg_btn;
  int pos_btn;

  int neg_cache = 0;
  int pos_cache = 0;

  axis2btns(int neg_btn, int pos_btn, virtual_device* virt_keyboard) : neg_btn(neg_btn), pos_btn(pos_btn), virt_keyboard(virt_keyboard) {
  }

  virtual void process(struct mg_ev ev, virtual_device* out);

  virtual axis2btns* clone() {
    return new axis2btns(*this);
  }

  static const char* decl;
  axis2btns(std::vector<MGField>& fields);
  virtual void fill_def(MGTransDef& def);

};
