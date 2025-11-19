
#include "pedal2numpad.h"
#include "../event_translator_macros.h"
#include <linux/input-event-codes.h>


const int NUMERIC_KEYS[10] = {KEY_KP1, KEY_KP2, KEY_KP3, KEY_KP4, KEY_KP5, KEY_KP6, KEY_KP7, KEY_KP8, KEY_KP9, KEY_KP0 };

void pedal2numpad::process(struct mg_ev ev, virtual_device* out) {
  bool isKeyPubCycle = tick_counter < TICK_COUNTER_MAX;
  int pressedIndex = STOP_VAL_INDEX;
  
  if(ev.value > MIN_PEDAL_VALUE) {
    // Determin pressed index based on signed range
    int unsignedValue = ev.value + ABS_RANGE;
    int lastPossibleIndex = (sizeof(NUMERIC_KEYS) / sizeof(NUMERIC_KEYS[0])) - 1;

    pressedIndex = ((unsignedValue * 10 + 2500) / UNSIGNED_ABS_RANGE);
    pressedIndex = pressedIndex > lastPossibleIndex ? lastPossibleIndex : pressedIndex;
  }

  //std::cout << "-------------------------------" << std::endl;
  int numericKeysSize = sizeof(NUMERIC_KEYS) / sizeof(NUMERIC_KEYS[0]); // Calculate array size
  for (int i = 0; i <= numericKeysSize; i++) {
      struct input_event out_ev;
      memset(&out_ev, 0, sizeof(out_ev));
      out_ev.type = EV_KEY;
      out_ev.code = i < numericKeysSize ? NUMERIC_KEYS[i] : STOP_KEY;
      out_ev.value = isKeyPubCycle && (pressedIndex == i);

      //std::cout << "code" << "|" << i << "|" << out_ev.code << "|" << out_ev.value << "|" << std::endl;

      if (out_ev.value != val_caches[i]) {
        write_out(out_ev, virt_keyboard);
        flush(virt_keyboard);

        val_caches[i] = out_ev.value;
      }
  }
  //std::cout << "-------------------------------" << std::endl;

  if (prev_tick_index != pressedIndex) {
    tick_counter = 0;
  } else if (tick_counter < TICK_COUNTER_MAX) {
    tick_counter ++;
  }

  prev_tick_index = pressedIndex;
}

const char* pedal2numpad::decl = "axis = pedal2numpad(trans)";
pedal2numpad::pedal2numpad(std::vector<MGField>& fields) {
  BEGIN_READ_DEF;
}
void pedal2numpad::fill_def(MGTransDef& def) {
  BEGIN_FILL_DEF(PEDAL_2_NUMPAD);
}
