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
#include "key_pad_basic.h"

/*********************
 *      DEFINES
 *********************/
// tạm thời sửa sau
#define ADDRESS_STORE_PASSWORD_ADMIN 0X0000
#define ADDRESS_STORE_PASSWORD_USER 0X0000
#define ADDRESS_START_STORE_UID 0X0000

#define INIT_PASSWORD 1
#define OPEN_CLOSE_DOOR(state) HAL_GPIO_WritePin(GPI0B, GPIO_PIN_11, state)
#define

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  EXTERN FUNCTION
 **********************/
extern uint8_t ENTER_Password(uint8_t *data_keypad);

/**********************
 *  GLOBAL VARIABLES
 **********************/
extern LCD_Name lcd_0;

/**********************
 *  STATIC VARIABLES
 **********************/
static uint8_t password_user[10];
static uint8_t password_admin[15];
static uint8_t incorrect_enter_pass_user_u8;
static uint8_t incorrect_enter_pass_admin_u8;
static uint8_t uid_card[5][4];

#if INIT_PASSWORD
static uint8_t password_use_init[10] = "1234567890";
static uint8_t password_admin_init[15] = "*#000#12345678";
#endif

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void Init_Door(void)
{
#if INIT_PASSWORD
    // ghi mật khẩu user vao eeprom
    // ghi mật khẩu admin vao eeprom
    // ghi uid vào eeprom
#endif
    // đọc số lần sai mật khẩu user từ eeprom
    // đọc số lần sai mật khẩu admin từ eeprom
    // đọc uid rfid từ eeprom
    DISPLAY_Init_Door(&lcd_0);
}
state_door_t Password_identification(uint8_t *pass)
{
    for (uint8_t i = 0; i < 6; i++)
    {
        if (*(pass + i) != password_admin[i])
        {
            return USER_ACCESS;
        }
    }
    return ADMIN_ACCESS;
}

state_door_t USER_Mode_Keypad(uint8_t *pass)
{
    if (incorrect_enter_pass_user_u8 <= 5)
    {
        // đọc password user từ eeprom
        if (memcmp(pass, password_user, sizeof(password_user)) == 0)
        {
            DISPLAY_Open_Door(&lcd_0);
            incorrect_enter_pass_user_u8 = 0;
            /* kiểm tra số lần nhập sai trog eeprom nếu khác số lần
            nhập sai hiện tại thì cập nhật*/
            return OPEN_DOOR;
        }
        else
        {
            DISPLAY_ReEnter_Password(&lcd_0);
            incorrect_enter_pass_user_u8++;
            return CLOSE_DOOR;
        }
    }
    DISPLAY_Block_User(&lcd_0);
    return BLOCK_USER;
}

state_door_t USER_Mode_Card(uint8_t *uid)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t j;
        for (uint8_t j = 0; j < 4; j++)
        {
            if (uid_card[i][j] != *(uid + j))
            {
                break;
            }
        }
        if (j == 4)
        {
            DISPLAY_Open_Door(&lcd_0);
            incorrect_enter_pass_user_u8 = 0;
            return OPEN_DOOR;
        }
    }
    DISPLAY_ReSwipe_Card(&lcd_0);
    return CLOSE_DOOR;
}

state_door_t ADMIN_Mode(uint8_t *pass)
{
    if (incorrect_enter_pass_admin_u8 <= 5)
    {
        // đọc pasword_admin từ eeprom
        if (memcmp(pass, password_user, sizeof(password_admin)) == 0)
        {
            DISPLAY_Admin_Mode(&lcd_0);
            incorrect_enter_pass_admin_u8 = 0;
            /* kiểm tra số lần nhập sai trog eeprom nếu khác số lần
            nhập sai hiện tại thì cập nhật*/
            return ADMIN_ACCESS_SUCCESSFULLY;
        }
        else
        {
            DISPLAY_ReEnter_Password(&lcd_0);
            incorrect_enter_pass_admin_u8++;
            return ADMIN_ACCESS_FAILED;
        }
    }
    DISPLAY_Block_Admin(&lcd_0);
    return BLOCK_ADMIN;
}

uint8_t _Administrator(void)
{
    DISPLAY_Administrator(&lcd_0);
    uint8_t state_keypad_temp = (uint8_t)NONE_PRESSING_STATE;
    while (state_keypad_temp == NONE_PRESSING_STATE)
    {
        state_keypad_temp = keypad_handle();
    }
    return state_keypad_temp;
}

state_door_t __Change_password_admin(uint8_t *password, uint8_t change_state)
{
    DISPLAY_Change_Password_Admin(&lcd_0);
    if (change_state == '#')
    {
        // nhấn # để xác nhận và lưu password
        DISPLAY_Success_Notification(&lcd_0);
        return STATE_END;
    }

    if (change_state == '*')
    {
        return STATE_FORWARD;
    }
}

state_door_t __Unblock_User(uint8_t number_enter_password)
{
    DISPLAY_Unblock_User(&lcd_0);

    // nhấn # để xác nhận và lưu
    uint8_t data = 0;
    while (1)
    {
        data = keypad_handle();
        if (data == '#')
        {
            incorrect_enter_pass_user_u8 = 0;
            // store incorrect in eeprom
            DISPLAY_Success_Notification(&lcd_0);
            return STATE_END;
        }
        if (data == '*')
        {
            return STATE_FORWARD;
        }
    }
}

state_door_t _Change_password_user(uint8_t *password, uint8_t change_state)
{
    DISPLAY_Change_Password_User(&lcd_0);
    if (change_state == '#')
    {
        // nhấn # để xác nhận và lưu password
        DISPLAY_Success_Notification(&lcd_0);
        return STATE_END;
    }

    if (change_state == '*')
    {
        return STATE_FORWARD;
    }
}

void _Add_New_Card(uint8_t *UID)
{
}

void OPEN_DOOR(void)
{
    DISPLAY_Open_Door(&lcd_0);
    OPEN_CLOSE_DOOR(1);
    HAL_Delay(2000);
    OPEN_CLOSE_DOOR(0);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/