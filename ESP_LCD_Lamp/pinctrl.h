#ifndef PINCTRL_H
#define PINCTRL_H

extern uint8_t PINCTRL_toggle(void);
extern void PINCTRL_init(void);
extern uint8_t PINCTRL_getCurrent(void);
extern uint8_t PINCTRL_btnPressed(void);

#endif
