#ifndef PINCTRL_H
#define PINCTRL_H

extern uint8_t PINCTRL_toggle(void);
extern void PINCTRL_init(void);
extern uint8_t PINCTRL_getCurrent(void);
extern void PINCTRL_btnPressed(void);
extern void PINCTRL_setIntesity(int i);
extern int PINCTRL_getIntesity(void);

#endif
