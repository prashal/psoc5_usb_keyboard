/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

#define DEVICE 0u
#define USB_KEYBOARD_ENDPOINT 1u



// USB keyboard codes
#define USB_HID_KEY_A                0x04
#define USB_HID_KEY_RETURN           0x28
#define USB_HID_KEY_SPACE            0x2C
#define USB_HID_KEY_COMMA_LEFT_ARROW 0x36
#define USB_HID_KEY_DOT_RIGHT_ARROW  0x37

#define USB_HID_MODIFIER_LEFT_CTRL   0x01
#define USB_HID_MODIFIER_LEFT_SHIFT  0x02
#define USB_HID_MODIFIER_LEFT_ALT    0x04
#define USB_HID_MODIFIER_LEFT_GUI    0x08
#define USB_HID_MODIFIER_RIGHT_CTRL  0x10
#define USB_HID_MODIFIER_RIGHT_SHIFT 0x20
#define USB_HID_MODIFIER_RIGHT_ALT   0x40
#define USB_HID_MODIFIER_RIGHT_GUI   0x80
#define USB_HID_KEY_L                0x0F


uint8 bufferHID[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void SendKey()
{    
    /* Loads EP1 for a IN transfer to PC */
    USB_LoadInEP(USB_KEYBOARD_ENDPOINT, (uint8 *)&bufferHID, 8);
    /* Waits for ACK from the host */
    //while(!USB_bGetEPAckState(USB_KEYBOARD_ENDPOINT));
    CyDelay(10);
    
    
    // Send 'null' report
    bufferHID[0] = 0;
    bufferHID[1] = 0;
    bufferHID[2] = 0;
    bufferHID[3] = 0;
    bufferHID[4] = 0;
    bufferHID[5] = 0;
    bufferHID[6] = 0;
    bufferHID[7] = 0;    
    
    /* Loads EP1 for a IN transfer to PC */
    USB_LoadInEP(USB_KEYBOARD_ENDPOINT, (uint8 *)&bufferHID, 8);
    /* Waits for ACK from the host */
    CyDelay(10);
}

void SendAlphaString(char* str)
{
    uint8 ch = *str;
    while(ch != '\0') {
        if(isupper(ch))
            bufferHID[0] = USB_HID_MODIFIER_LEFT_SHIFT;         //Modifier 'Left Shift'
        if(isalpha(ch))
            bufferHID[2] = USB_HID_KEY_A + toupper(ch) - 'A';     //Key Code converted from the ascii value
        else if (ch == '\n')
            bufferHID[2] = USB_HID_KEY_RETURN;
        else if (ch == ' ')
            bufferHID[2] = USB_HID_KEY_SPACE;     
        else if (ch == '.')
            bufferHID[2] = USB_HID_KEY_DOT_RIGHT_ARROW;
        else if (ch == ',')
            bufferHID[2] = USB_HID_KEY_COMMA_LEFT_ARROW;
            
        SendKey();
        
        ch = *(++str);
    }
}


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    USB_Start(DEVICE, USB_5V_OPERATION); 
    
    for(;;)
    {
        /* Place your application code here. */
        /* Place your application code here. */
        if(0u != USB_IsConfigurationChanged())
        {
            /* Initialize IN endpoints when device configured */
            if(0u != USB_GetConfiguration())   
            {                
                /* Begins initial communication with PC */
                //USB_LoadInEP(USB_KEYBOARD_ENDPOINT, (uint8 *)&csButtStates, 1);
                
            } 
        }        
        
        /* Service USB MIDI when device is configured */
        if(0u != USB_GetConfiguration())    
        {    
            SendAlphaString("Hello World, This is from basic keyboard.\n");
            CyDelay(1000);
            
        }
    }
}

/* [] END OF FILE */
