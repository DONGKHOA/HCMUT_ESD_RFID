#ifndef _MENU_MODE
#define _MENU_MODE

#include "stdint.h"

typedef enum
{
    BLOCK_USER,
    BLOCK_ADMIN,
    OPEN_DOOR,
    CLOSE_DOOR
}state_door_t;

void Init_Door(void);
state_door_t USER_Mode(uint8_t *pass);
void ADMIN_Mode(uint8_t *pass);
void _Administrator(void);
void __Change_password_admin(uint8_t *password);
void __Unblock_User(uint8_t number_enter_password);
void _Change_password_user(uint8_t *password);
void _Add_New_Card(uint8_t *UID);

void OPEN_DOOR(void);


#endif