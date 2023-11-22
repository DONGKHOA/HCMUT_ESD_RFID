/**
 * @file menu_mode.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "menu_mode.h"
#include "memory.h"
#include "flash_memory.h"
#include "display_information.h"

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
 *  GLOBAL VARIABLES
 **********************/
extern LCD_Name lcd_0;

/**********************
 *  STATIC VARIABLES
 **********************/
static uint8_t password_user[10] = "1234567890";
static uint8_t password_admin[15] = "*#000#12345678";
static uint8_t incorrect_enter_pass_user_u8 = 0;
static uint8_t incorrect_enter_pass_admin_u8 = 0;
static uint8_t uid_card[5][4];

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
    //đọc uid rfid từ eeprom
}

state_door_t USER_Mode(uint8_t *pass)
{
    // enter password from keypad
    if(memcmp(pass, password_user, sizeof(password_user)) == 0)
    {
        DISPLAY_Open_Door(&lcd_0);
        incorrect_enter_pass_user_u8 = 0;
        return OPEN_DOOR;
    }
    else
    {
        DISPLAY_ReEnter_Password(&lcd_0);
        return CLOSE_DOOR;
    }
    
    //user use card
    for(uint8_t i = 0; i < 4; i++)
    {
        uint8_t j;
        for(uint8_t j = 0; j < 4; j++)
        {
            if(uid_card[i][j] != *(pass +j))
            {
                break;
            }
        }
        if(j == 4)
        {
            DISPLAY_Open_Door(&lcd_0);
            incorrect_enter_pass_user_u8 = 0;
            return OPEN_DOOR;
        }
    }
    DISPLAY_ReSwipe_Card(&lcd_0);
    incorrect_enter_pass_user_u8++;
    return CLOSE_DOOR;
}

void ADMIN_Mode(uint8_t *pass)
{
    if(memcmp(pass, password_user, sizeof(password_admin)) == 0)
    {
        DISPLAY_Admin_Mode(&lcd_0);
    }
}

void _Administrator(void)
{
    DISPLAY_Administrator(&lcd_0);
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