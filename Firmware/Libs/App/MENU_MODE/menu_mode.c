/**
 * @file menu_mode.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "menu_mode.h"
#include "memory.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static uint8_t incorrect_enter_pass_user_u8 = 0;
static uint8_t incorrect_enter_pass_admin_u8 = 0;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void Init_Door(void)
{
    //đọc số lần sai mật khẩu user từ eeprom
    //đọc số lần sai mật khẩu admin từ eeprom
}

state_door_t USER_Mode(uint8_t *pass)
{
    if(memcmp(pass, password_user, sizeof(password_user)) == 0)
    {

        return OPEN_DOOR;
    }
}

void ADMIN_Mode(uint8_t *pass)
{

}

void _Administrator(void)
{

}

void __Change_password_admin(uint8_t *password)
{

}

void __Unblock_User(uint8_t number_enter_password)
{

}

void _Change_password_user(uint8_t *password)
{

}

void _Add_New_Card(uint8_t *UID);
void OPEN_DOOR(void);

/**********************
 *   STATIC FUNCTIONS
 **********************/

