#ifndef CONTROLS_H
#define CONTROLS_H

#include <string.h>
#include <pspctrl.h>

void controls_init(void);
void controls_read(void);
int controls_pressed(const unsigned int button);
int controls_held(const unsigned int button);
int controls_released(const unsigned int button);
int controls_AnalogX(void);
int controls_AnalogY(void);

#endif // CONTROLS_H