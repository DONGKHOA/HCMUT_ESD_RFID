#ifndef _DISPLAY_INFORMATION
#define _DISPLAY_INFORMATION

#include "stdint.h"
#include "lcd.h"


void DISPLAY_Open_Door(LCD_Name *lcd_p);
void DISPLAY_ReEnter_Password(LCD_Name *lcd_p);
void DISPLAY_Block_User(LCD_Name *lcd_p);
void DISPLAY_Admin_Mode(LCD_Name *lcd_p);
void DISPLAY_Block_Admin(LCD_Name *lcd_p);
void DISPLAY_Administrator(LCD_Name *lcd_p);
void DISPLAY_Change_Password_Admin(LCD_Name *lcd_p);
void DISPLAY_Unblock_User(LCD_Name *lcd_p);
void DISPLAY_Change_Password_User(LCD_Name *lcd_p);
void DISPLAY_Success_Notification(LCD_Name *lcd_p);
void DISPLAY_Swipe_Card(LCD_Name *lcd_p);
void DISPLAY_ReSwipe_Card(LCD_Name *lcd_p);
void DISPLAY_Card_Added(LCD_Name *lcd_p);
void DISPLAY_CardExists_DeleteCard(LCD_Name *lcd_p);


#endif