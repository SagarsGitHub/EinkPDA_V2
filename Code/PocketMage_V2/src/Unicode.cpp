#include "globals.h"

void renderTextToBuffer(const char *text) {
  memset(u8g2Buffer, 0x00, bufferSize);  // Clear buffer

  u8g2Bitmap.setFont(u8g2_font_unifont_t_japanese1);

  u8g2Bitmap.drawUTF8(0, 16, text);
}

void drawBufferToEPD() {
  display.flush();
  display.fillScreen(GxEPD_WHITE);  // Clear the display
  display.drawBitmap(0, 0, u8g2Buffer, SCREEN_WIDTH, SCREEN_HEIGHT, GxEPD_BLACK);
}


uint8_t my_display_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr){
    if (msg == U8X8_MSG_DISPLAY_DRAW_TILE) {
        Serial.println("Drawing tile...");
        u8x8_tile_t *tile = (u8x8_tile_t *)arg_ptr;
        // Get the tile horizontal start position (in tiles)
        uint16_t x_tile = tile->x_pos;
        // Get the tile vertical start position (in tiles)
        uint16_t y_tile = tile->y_pos;
        // Number of tiles to draw in this chunk (horizontally)
        uint8_t cnt = tile->cnt;
        // Pointer to the tile data bytes
        uint8_t *ptr = tile->tile_ptr;
        Serial.println("tile counts: " + String(cnt));
        // Each tile is 8 pixels wide and 8 pixels tall
        // Calculate pixel start positions
        uint16_t x_pix = x_tile * 8;
        uint16_t y_pix = y_tile * 8;
        
        // For each tile in this line
        for (uint8_t tile_index = 0; tile_index < cnt; tile_index++) {
        // For each vertical byte in tile (8 bytes = 8 rows vertically)
        for (uint8_t row = 0; row < 8; row++) {
            // Byte of pixels for this row in the current tile
            uint8_t pixel_byte = ptr[row + tile_index * 8];

            uint16_t buffer_index = (y_pix + row) * (SCREEN_WIDTH / 8) + (x_pix / 8) + tile_index;
            u8g2Buffer[buffer_index] = pixel_byte;
        }
        }
        return 1;
    }

    return 0;
    /*
  uint8_t x, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8x8);
      u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1306_128x32_univision_init_seq);    
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      if ( arg_int == 0 )
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1306_128x32_univision_powersave0_seq);
      else
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1306_128x32_univision_powersave1_seq);
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      if ( arg_int == 0 )
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1306_128x32_univision_flip0_seq);
	u8x8->x_offset = u8x8->display_info->default_x_offset;
      }
      else
      {
	u8x8_cad_SendSequence(u8x8, u8x8_d_ssd1306_128x32_univision_flip1_seq);
	u8x8->x_offset = u8x8->display_info->flipmode_x_offset;
      }
      break;
#ifdef U8X8_WITH_SET_CONTRAST
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      u8x8_cad_StartTransfer(u8x8);
      u8x8_cad_SendCmd(u8x8, 0x081 );
      u8x8_cad_SendArg(u8x8, arg_int );	// ssd1306 has range from 0 to 255 
      u8x8_cad_EndTransfer(u8x8);
      break;
#endif
    case U8X8_MSG_DISPLAY_DRAW_TILE:
      u8x8_cad_StartTransfer(u8x8);
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;    
      x *= 8;
      x += u8x8->x_offset;
      u8x8_cad_SendCmd(u8x8, 0x010 | (x>>4) );
      u8x8_cad_SendCmd(u8x8, 0x000 | ((x&15)));
      u8x8_cad_SendCmd(u8x8, 0x0b0 | (((u8x8_tile_t *)arg_ptr)->y_pos));
      
      do
      {
	c = ((u8x8_tile_t *)arg_ptr)->cnt;
	ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
	u8x8_cad_SendData(u8x8, c*8, ptr); 	// note: SendData can not handle more than 255 bytes 
	arg_int--;
      } while( arg_int > 0 );
      
      u8x8_cad_EndTransfer(u8x8);
      break;
    default:
      return 0;
  }
  return 1;    
  */
}

uint8_t my_cad_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    return 0;
  /*
  uint8_t i;
  
  switch(msg)
  {
    case U8X8_MSG_CAD_SEND_CMD:
      u8x8_mcd_byte_SetDC(mcd->next, 1);
      u8x8_mcd_byte_Send(mcd->next, arg_int);
      break;
    case U8X8_MSG_CAD_SEND_ARG:
      u8x8_mcd_byte_SetDC(mcd->next, 1);
      u8x8_mcd_byte_Send(mcd->next, arg_int);
      break;
    case U8X8_MSG_CAD_SEND_DATA:
      u8x8_mcd_byte_SetDC(mcd->next, 0);
      for( i = 0; i < 8; i++ )
	u8x8_mcd_byte_Send(mcd->next, ((uint8_t *)arg_ptr)[i]);
      break;
    case U8X8_MSG_CAD_RESET:
      return mcd->next->cb(mcd->next, msg, arg_int, arg_ptr);
    case U8X8_MSG_CAD_START_TRANSFER:
      return mcd->next->cb(mcd->next, msg, arg_int, arg_ptr);
    case U8X8_MSG_CAD_END_TRANSFER:
      return mcd->next->cb(mcd->next, msg, arg_int, arg_ptr);
    default:
      break;
  }
  return 1;
  */
}