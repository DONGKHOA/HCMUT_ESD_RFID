#include "display_information.h"
#include "lcd.h"
#include "main.h"

<<<<<<< HEAD
#define Write_Pin_GrLed(x) 	\
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, x)
#define Write_Pin_RedLed(x) 	\
=======

void DISPLAY_Init_Door(LCD_Name *lcd_p);
void DISPLAY_Open_Door(LCD_Name *lcd_p);

#define Write_Pin_Led(x) 	\
>>>>>>> 8d63747f5b3ff9fb305971eb23f3fe18368256f5
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, x)
#define Write_Pin_Buzzer(x) \
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, x)

void DISPLAY_Init_Door(LCD_Name *lcd_p)
{
	lcd_set_cursor(lcd_p, 0, 0);
	lcd_write_string(lcd_p, "Scan your card");
}
void DISPLAY_Open_Door(LCD_Name *lcd_p)
{
	lcd_set_cursor(lcd_p, 5, 0);
	lcd_write_string(lcd_p, "Unlocked");
	lcd_set_cursor(lcd_p, 4, 1);
	lcd_write_string(lcd_p, "Successfully!");
	Write_Pin_GrLed(1);
	Write_Pin_Buzzer(1);
	delay_us(100000);
	Write_Pin_GrLed(0);
	Write_Pin_Buzzer(0);
}

<<<<<<< HEAD
void DISPLAY_ReEnter_Password(LCD_Name *lcd_p)
{
	lcd_set_cursor(lcd_p, 5, 0);
	lcd_write_string(lcd_p, "Wrong password!");
	lcd_set_cursor(lcd_p, 4, 1);
	lcd_write_string(lcd_p, "Re-enter your password!");
	Write_Pin_RedLed(1);
	Write_Pin_Buzzer(1);
	delay_us(100000);
	Write_Pin_RedLed(0);
	Write_Pin_Buzzer(0);
}

void DISPLAY_Block_User(LCD_Name *lcd_p)
{
	lcd_set_cursor(lcd_p, 5, 0);
	lcd_write_string(lcd_p, "You was blocked!");
}

void DISPLAY_Admin_Mode(LCD_Name *lcd_p)
{
	lcd_set_cursor(lcd_p, 5, 0);
	lcd_write_string(lcd_p, "Admin mode:");
	lcd_set_cursor(lcd_p, 5, 0);
	lcd_write_string(lcd_p, "1, Change admin password");
	lcd_set_cursor(lcd_p, 5, 0);
	lcd_write_string(lcd_p, "2, Unblock User mode");
}

void DISPLAY_Block_Admin(LCD_Name *lcd_p)
{
	lcd_set_cursor(lcd_p, 5, 0);
	lcd_write_string(lcd_p, "You was blocked!");
}

void DISPLAY_Change_Password_Admin(LCD_Name *lcd_p)
{
	lcd_set_cursor(lcd_p, 5, 0);
	lcd_write_string(lcd_p, "Enter your new Admin Password:");
}

void DISPLAY_Administrator(LCD_Name *lcd_p)
{
	
}

void DISPLAY_Unblock_User(LCD_Name *lcd_p)
{

}

void DISPLAY_Change_Password_User(LCD_Name *lcd_p)
{

}

void DISPLAY_Success_Notification(LCD_Name *lcd_p)
{

}

void DISPLAY_Swipe_Card(LCD_Name *lcd_p)
{

}

void DISPLAY_ReSwipe_Card(LCD_Name *lcd_p)
{

}

void DISPLAY_Card_Added(LCD_Name *lcd_p)
{
	
}

void DISPLAY_CardExists_DeleteCard(LCD_Name *lcd_p)
{
	lcd_set_cursor(lcd_p, 5, 0);
	lcd_write_string(lcd_p, "");
}
=======
void DISPLAY_ReEnter_Password(LCD_Name *lcd_p);
void DISPLAY_Block_User(LCD_Name *lcd_p);
void DISPLAY_Admin_Mode(LCD_Name *lcd_p);
void DISPLAY_Block_Admin(LCD_Name *lcd_p);
void DISPLAY_Change_Password_Admin(LCD_Name *lcd_p);
void DISPLAY_Administrator(LCD_Name *lcd_p);
void DISPLAY_Unblock_User(LCD_Name *lcd_p);
void DISPLAY_Change_Password_User(LCD_Name *lcd_p);
void DISPLAY_Success_Notification(LCD_Name *lcd_p);
void DISPLAY_Swipe_Card(LCD_Name *lcd_p);
void DISPLAY_ReSwipe_Card(LCD_Name *lcd_p);
void DISPLAY_Card_Added(LCD_Name *lcd_p);
void DISPLAY_CardExists_DeleteCard(LCD_Name *lcd_p);
>>>>>>> 8d63747f5b3ff9fb305971eb23f3fe18368256f5
