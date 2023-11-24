#ifndef _MENU_MODE
#define _MENU_MODE

#include "stdint.h"

typedef enum
{
    BLOCK_USER,
    BLOCK_ADMIN,
    OPEN_DOOR,
    CLOSE_DOOR,
    USER_ACCESS,
    ADMIN_ACCESS,
    ADMIN_ACCESS_SUCCESSFULLY,
    ADMIN_ACCESS_FAILED,
    STATE_FORWARD,
    STATE_END
}state_door_t;

void Init_Door(void);
state_door_t Password_identification(uint8_t *pass);
state_door_t USER_Mode_Keypad(uint8_t *pass);
state_door_t USER_Mode_Card(uint8_t *uid);
state_door_t ADMIN_Mode(uint8_t *pass);
uint8_t _Administrator(void);
state_door_t __Change_password_admin(void);
state_door_t __Unblock_User(uint8_t number_enter_password);
state_door_t _Change_password_user(uint8_t *password);
void _Add_New_Card(uint8_t *UID);

state_door_t OPEN_DOOR(void);


#endif