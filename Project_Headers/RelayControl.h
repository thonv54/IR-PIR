/*
 * RelayControl.h
 *
 *  Created on: Nov 13, 2015
 *      Author: hp8460p
 */

#ifndef RELAYCONTROL_H_
#define RELAYCONTROL_H_



typedef enum{
	Have_PW = 1,
	No_PW = 0,
}PwStatusEnum;

#define Switch_on		PORT_PTCD_PTCD5 = 1
#define Switch_off		PORT_PTCD_PTCD5 = 0

#define ZERO_DT_PW		PORT_PTBD_PTBD0
#define ADC_ZERO_DT_PW			4


void RL_on(void);
void RL_off(void);
void GetPwSource(void);

#endif /* RELAYCONTROL_H_ */
