/*
 * osd.c
 *
 *  Created on: 17 de out de 2022
 *      Author: Samuel
 */

#include "osd.h"
#include "usbh_hid.h"
#include "nes_input.h"
uint16_t palette565[256];


uint16_t * framebuffer = (uint16_t*)(0xC0000000);


//#define  INP_PAD_A         0x01
//#define  INP_PAD_B         0x02
//#define  INP_PAD_SELECT    0x04
//#define  INP_PAD_START     0x08
//#define  INP_PAD_UP        0x10
//#define  INP_PAD_DOWN      0x20
//#define  INP_PAD_LEFT      0x40
//#define  INP_PAD_RIGHT     0x80

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{


	uint8_t key = 0;

	if(USBH_HID_GetDeviceType(phost)==HID_KEYBOARD)
	{
		HID_KEYBD_Info_TypeDef *Keyboard_Info;

		Keyboard_Info = USBH_HID_GetKeybdInfo(phost);


		for(uint8_t i = 0; i < 4; i++)
		{
			switch (Keyboard_Info->keys[i])
			{
				case 14: key |= INP_PAD_A; 		break; // INP_PAD_A 	--> k
				case 18: key |= INP_PAD_B; 		break; // INP_PAD_B 	--> L
				case 44: key |= INP_PAD_SELECT; break; // INP_PAD_SELECT --> space
				case 41: key |= INP_PAD_START; 	break; // INP_PAD_START --> esc
				case 26: key |= INP_PAD_UP; 	break; // INP_PAD_UP 	--> w
				case 22: key |= INP_PAD_DOWN; 	break; // INP_PAD_DOWN  --> s
				case 4: key  |= INP_PAD_LEFT;  	break; // INP_PAD_LEFT  --> a
				case 7: key  |= INP_PAD_RIGHT;  break; // INP_PAD_RIGHT  --> d

				default: break;
			}
		}
		static uint8_t old_key;

		if(old_key != key)
		{
			old_key = key;
		}

		input_update(INP_JOYPAD0, key);
	}
}


/* video */
void osd_setpalette(rgb_t *pal)
{
    for (int i = 0; i < 64; i++)
    {
        uint16_t c = (pal[i].b>>3) | ((pal[i].g>>2)<<5) | ((pal[i].r>>3)<<11);

        // The upper bits are used to indicate background and transparency.
        // They need to be indexed as well.
        palette565[i]        = c;
        palette565[i | 0x40] = c;
        palette565[i | 0x80] = c;
    }
}
void osd_blitscreen(bitmap_t *bmp)
{
    const int w1 = bmp->width;
    const int h2 = 240;
    const int hpad = 800;

    for (int y = 0; y < h2; y++)
    {
        uint8_t  *src_row  = bmp->line[y];

        uint16_t *dest_row_1 = &framebuffer[(y * 2) * hpad];
        //uint16_t *dest_row_2 = &framebuffer[800 + (y * 2) * hpad];

        for (int x = 0; x < w1; x++)
        {
        	dest_row_1[(x * 2)] = palette565[src_row[x]];
        	//dest_row_2[(x * 2) + 1] = palette565[src_row[x]];
        }
    }
}

/* control */
int osd_init(void)
{
	return 0;
}
void osd_shutdown(void)
{

}
void osd_loadstate(void)
{

}
void osd_event(int event)
{

}
void osd_vsync(void)
{

}

/* input */
void osd_getinput(void)
{


}

/* get rom data */
unsigned int osd_getromcrc(void)
{
	return 0;
}
unsigned int osd_getromsize(void)
{
	return 0;
}
size_t osd_getromdata(unsigned char **data)
{

	return 0;
}

/* Log output */
void osd_logprint(int type, char *message)
{

}
