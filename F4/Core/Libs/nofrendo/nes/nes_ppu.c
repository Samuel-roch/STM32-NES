/*
** Nofrendo (c) 1998-2000 Matthew Conte (matt@conte.com)
**
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of version 2 of the GNU Library General
** Public License as published by the Free Software Foundation.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.  To obtain a
** copy of the GNU Library General Public License, write to the Free
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
**
**
** nes_ppu.c
**
** NES PPU emulation
** $Id: nes_ppu.c,v 1.2 2001/04/27 14:37:11 neil Exp $
*/

// TODO: Figure out why these warnings occur
#pragma GCC diagnostic ignored "-Warray-bounds"

#include <string.h>
#include <nofrendo.h>
#include <nes6502.h>
#include <bitmap.h>
#include "nes_input.h"
#include "nes_mmc.h"
#include "nes_ppu.h"
#include "nes.h"

/* static const palette_t nes_palettes[PPU_PAL_COUNT] */
#include "palettes.h"

/* PPU access */
#define PPU_MEM_READ(x)      (ppu.page[(x) >> 10][(x)])
#define PPU_MEM_WRITE(x,v)   (ppu.page[(x) >> 10][(x)] = (v))

/* Background (color 0) and solid sprite pixel flags */
#define BG_TRANS             (0x80)
#define SP_PIXEL             (0x40)
#define BG_CLEAR(V)          ((V) & BG_TRANS)
#define BG_SOLID(V)          (0 == BG_CLEAR(V))
#define SP_CLEAR(V)          (0 == ((V) & SP_PIXEL))

/* Full BG color */
#define FULLBG               (ppu.palette[0] | BG_TRANS)

/* Runtime settings */
#define OPT(n)                (ppu.options[(n)])

/* the NES PPU */
static ppu_t ppu;

static rgb_t gui_pal[] =
{
   { 0x00, 0x00, 0x00 }, /* black      */
   { 0x3F, 0x3F, 0x3F }, /* dark gray  */
   { 0x7F, 0x7F, 0x7F }, /* gray       */
   { 0xBF, 0xBF, 0xBF }, /* light gray */
   { 0xFF, 0xFF, 0xFF }, /* white      */
   { 0xFF, 0x00, 0x00 }, /* red        */
   { 0x00, 0xFF, 0x00 }, /* green      */
   { 0x00, 0x00, 0xFF }, /* blue       */
};


#ifndef PPU_MEM_READ
INLINE uint8 PPU_MEM_READ(uint32 x)
{
   if (ppu.vreadfunc)
      return ppu.vreadfunc(x, ppu.page[x >> 10][x]);
   else
      return ppu.page[x >> 10][x];
}
#endif

void ppu_setcontext(ppu_t *src_ppu)
{
   ASSERT(src_ppu);
   ppu = *src_ppu;
   ppu_setnametables(ppu.nt1, ppu.nt2, ppu.nt3, ppu.nt4);
}

void ppu_getcontext(ppu_t *dest_ppu)
{
   ASSERT(dest_ppu);
   *dest_ppu = ppu;
}

void ppu_setpage(int size, int page_num, uint8 *location)
{
   while (size--)
   {
      ppu.page[page_num++] = location;
   }
}

uint8 *ppu_getpage(int page)
{
   return ppu.page[page];
}

uint8 *ppu_getnametable(int table)
{
   return ppu.nametab + (0x400 * (table & 3));
}

void ppu_setnametables(int nt1, int nt2, int nt3, int nt4)
{
   ppu.nt1 = nt1 & 0x3; ppu.nt2 = nt2 & 0x3;
   ppu.nt3 = nt3 & 0x3; ppu.nt4 = nt4 & 0x3;

   ppu.page[8]  = ppu.nametab + (ppu.nt1 * 0x400) - 0x2000;
   ppu.page[9]  = ppu.nametab + (ppu.nt2 * 0x400) - 0x2400;
   ppu.page[10] = ppu.nametab + (ppu.nt3 * 0x400) - 0x2800;
   ppu.page[11] = ppu.nametab + (ppu.nt4 * 0x400) - 0x2C00;

   /* make sure $3000-$3F00 mirrors $2000-$2F00 */
   ppu.page[12] = ppu.page[8] - 0x1000;
   ppu.page[13] = ppu.page[9] - 0x1000;
   ppu.page[14] = ppu.page[10] - 0x1000;
   ppu.page[15] = ppu.page[11] - 0x1000;
}

void ppu_setmirroring(ppu_mirror_t type)
{
   switch (type)
   {
      case PPU_MIRROR_SCR0: ppu_setnametables(0, 0, 0, 0); break;
      case PPU_MIRROR_SCR1: ppu_setnametables(1, 1, 1, 1); break;
      case PPU_MIRROR_FOUR: ppu_setnametables(0, 1, 2, 3); break;
      case PPU_MIRROR_VERT: ppu_setnametables(0, 1, 0, 1); break;
      case PPU_MIRROR_HORI: ppu_setnametables(0, 0, 1, 1); break;
   }
}

INLINE void ppu_oamdma(uint8 value)
{
   uint32 cpu_address;
   uint8 oam_loc;

   cpu_address = (uint32) (value << 8);

   /* Sprite DMA starts at the current SPRRAM address */
   oam_loc = ppu.oam_addr;
   do
   {
      ppu.oam[oam_loc++] = mem_getbyte(cpu_address++);
   }
   while (oam_loc != ppu.oam_addr);

   /* TODO: enough with houdini */
   cpu_address -= 256;
   /* Odd address in $2003 */
   if ((ppu.oam_addr >> 2) & 1)
   {
      for (oam_loc = 4; oam_loc < 8; oam_loc++)
         ppu.oam[oam_loc] = mem_getbyte(cpu_address++);
      cpu_address += 248;
      for (oam_loc = 0; oam_loc < 4; oam_loc++)
         ppu.oam[oam_loc] = mem_getbyte(cpu_address++);
   }
   /* Even address in $2003 */
   else
   {
      for (oam_loc = 0; oam_loc < 8; oam_loc++)
         ppu.oam[oam_loc] = mem_getbyte(cpu_address++);
   }

   /* make the CPU spin for DMA cycles */
   nes6502_burn(513);
   nes6502_release();
}

/* Read from $2000-$2007 */
IRAM_ATTR uint8 ppu_read(uint32 address)
{
   uint8 value;

   /* handle mirrored reads up to $3FFF */
   switch (address & 0x2007)
   {
   case PPU_STAT:
      value = (ppu.stat & 0xE0) | (ppu.latch & 0x1F);

      if (ppu.strikeflag)
      {
         if (nes6502_getcycles() >= ppu.strike_cycle)
            value |= PPU_STATF_STRIKE;
      }

      /* clear both vblank flag and vram address flipflop */
      ppu.stat &= ~PPU_STATF_VBLANK;
      ppu.flipflop = 0;
      break;

   case PPU_VDATA:
      /* buffered VRAM reads */
      value = ppu.latch = ppu.vdata_latch;

      /* VRAM only accessible during VBL */
      if ((ppu.bg_on || ppu.obj_on) && !ppu.vram_accessible)
      {
         ppu.vdata_latch = 0xFF;
         MESSAGE_DEBUG("VRAM read at $%04X, scanline %d\n",
                        ppu.vaddr, nes_getptr()->scanline);
      }
      else
      {
         uint32 addr = ppu.vaddr;
         if (addr >= 0x3000)
            addr -= 0x1000;
         ppu.vdata_latch = PPU_MEM_READ(addr);
      }

      ppu.vaddr += ppu.vaddr_inc;
      ppu.vaddr &= 0x3FFF;
      break;

   case PPU_OAMDATA:
   case PPU_CTRL0:
   case PPU_CTRL1:
   case PPU_OAMADDR:
   case PPU_SCROLL:
   case PPU_VADDR:
   default:
      value = ppu.latch;
      break;
   }

   return value;
}

/* Write to $2000-$2007 and $4014 */
IRAM_ATTR void ppu_write(uint32 address, uint8 value)
{
   // Special case for the one $4000 address...
   if (address == PPU_OAMDMA) {
      ppu_oamdma(value);
      return;
   }

   /* write goes into ppu latch... */
   ppu.latch = value;

   switch (address & 0x2007)
   {
   case PPU_CTRL0:
      ppu.ctrl0 = value;

      ppu.obj_height = (value & PPU_CTRL0F_OBJ16) ? 16 : 8;
      ppu.bg_base = (value & PPU_CTRL0F_BGADDR) ? 0x1000 : 0;
      ppu.obj_base = (value & PPU_CTRL0F_OBJADDR) ? 0x1000 : 0;
      ppu.vaddr_inc = (value & PPU_CTRL0F_ADDRINC) ? 32 : 1;
      ppu.nametab_base = (value & PPU_CTRL0F_NAMETAB);

      /* Mask out bits 10 & 11 in the ppu latch */
      ppu.vaddr_latch &= ~0x0C00;
      ppu.vaddr_latch |= ((value & 3) << 10);
      break;

   case PPU_CTRL1:
      ppu.ctrl1 = value;

      ppu.obj_on = (value & PPU_CTRL1F_OBJON) ? true : false;
      ppu.bg_on = (value & PPU_CTRL1F_BGON) ? true : false;
      ppu.left_obj_on = (value & PPU_CTRL1F_OBJMASK) ? true : false;
      ppu.left_bg_on = (value & PPU_CTRL1F_BGMASK) ? true : false;
      break;

   case PPU_OAMADDR:
      ppu.oam_addr = value;
      break;

   case PPU_OAMDATA:
      ppu.oam[ppu.oam_addr++] = value;
      break;

   case PPU_SCROLL:
      if (0 == ppu.flipflop)
      {
         /* Mask out bits 4 - 0 in the ppu latch */
         ppu.vaddr_latch &= ~0x001F;
         ppu.vaddr_latch |= (value >> 3);    /* Tile number */
         ppu.tile_xofs = (value & 7);  /* Tile offset (0-7 pix) */
      }
      else
      {
         /* Mask out bits 14-12 and 9-5 in the ppu latch */
         ppu.vaddr_latch &= ~0x73E0;
         ppu.vaddr_latch |= ((value & 0xF8) << 2);   /* Tile number */
         ppu.vaddr_latch |= ((value & 7) << 12);     /* Tile offset (0-7 pix) */
      }

      ppu.flipflop ^= 1;
      break;

   case PPU_VADDR:
      if (0 == ppu.flipflop)
      {
         /* Mask out bits 15-8 in ppu latch */
         ppu.vaddr_latch &= ~0xFF00;
         ppu.vaddr_latch |= ((value & 0x3F) << 8);
      }
      else
      {
         /* Mask out bits 7-0 in ppu latch */
         ppu.vaddr_latch &= ~0x00FF;
         ppu.vaddr_latch |= value;
         ppu.vaddr = ppu.vaddr_latch;
      }

      ppu.flipflop ^= 1;
      break;

   case PPU_VDATA:
      if (ppu.vaddr < 0x3F00)
      {
         /* VRAM only accessible during scanlines 241-260 */
         if ((ppu.bg_on || ppu.obj_on) && !ppu.vram_accessible)
         {
            MESSAGE_DEBUG("VRAM write to $%04X, scanline %d\n",
                           ppu.vaddr, nes_getptr()->scanline);
            PPU_MEM_WRITE(ppu.vaddr, 0xFF); /* corrupt */
         }
         else
         {
            uint32 addr = ppu.vaddr;

            if (false == ppu.vram_present && addr >= 0x3000)
               ppu.vaddr -= 0x1000;

            PPU_MEM_WRITE(addr, value);
         }
      }
      else
      {
         if (0 == (ppu.vaddr & 0x0F))
         {
            for (int i = 0; i < 8; i ++)
               ppu.palette[i << 2] = (value & 0x3F) | BG_TRANS;
         }
         else if (ppu.vaddr & 3)
         {
            ppu.palette[ppu.vaddr & 0x1F] = value & 0x3F;
         }
      }

      ppu.vaddr += ppu.vaddr_inc;
      ppu.vaddr &= 0x3FFF;
      break;

   default:
      break;
   }
}

/* Builds a 256 color 8-bit palette based on a 64-color NES palette
** Note that we set it up 3 times so that we flip bits on the primary
** NES buffer for priorities
*/
void ppu_setpalette(rgb_t *pal)
{
   /* Set it up 3 times, for sprite priority/BG transparency trickery */
   for (int i = 0; i < 64; i++)
   {
      ppu.curpal[i].r = ppu.curpal[i + 64].r = ppu.curpal[i + 128].r = pal[i].r;
      ppu.curpal[i].g = ppu.curpal[i + 64].g = ppu.curpal[i + 128].g = pal[i].g;
      ppu.curpal[i].b = ppu.curpal[i + 64].b = ppu.curpal[i + 128].b = pal[i].b;
   }

   for (int i = 0; i < GUI_TOTALCOLORS; i++)
   {
      ppu.curpal[i + 192].r = gui_pal[i].r;
      ppu.curpal[i + 192].g = gui_pal[i].g;
      ppu.curpal[i + 192].b = gui_pal[i].b;
   }

   osd_setpalette((rgb_t *)&ppu.curpal);
}

const palette_t *ppu_getpalette(int n)
{
   return &nes_palettes[n % PPU_PAL_COUNT];
}

void ppu_setopt(ppu_option_t n, int val)
{
   // Some options need special care
   switch (n)
   {
      case PPU_PALETTE_RGB:
      ppu_setpalette((rgb_t*)nes_palettes[val % PPU_PAL_COUNT].data);
      break;

      default:
      break;
   }

   ppu.options[n] = val;
}

int ppu_getopt(ppu_option_t n)
{
   return ppu.options[n];
}

void ppu_setlatchfunc(ppu_latchfunc_t func)
{
   ppu.latchfunc = func;
}

void ppu_setvreadfunc(ppu_vreadfunc_t func)
{
   ppu.vreadfunc = func;
}

/* rendering routines */
INLINE uint16 get_patpix(uint16 tile_addr)
{
   uint8 pat1 = PPU_MEM_READ(tile_addr);
   uint8 pat2 = PPU_MEM_READ(tile_addr + 8);
   return ((pat2 & 0xAA) << 8) | ((pat2 & 0x55) << 1)
        | ((pat1 & 0xAA) << 7) | (pat1 & 0x55);
}

INLINE void build_tile_colors(bool flip, uint16 pattern, uint8 *colors)
{
   /* swap pixels around if our tile is flipped */
   if (flip)
   {
      colors[7] = (pattern >> 14) & 3;
      colors[6] = (pattern >> 6) & 3;
      colors[5] = (pattern >> 12) & 3;
      colors[4] = (pattern >> 4) & 3;
      colors[3] = (pattern >> 10) & 3;
      colors[2] = (pattern >> 2) & 3;
      colors[1] = (pattern >> 8) & 3;
      colors[0] = pattern & 3;
   }
   else
   {
      colors[0] = (pattern >> 14) & 3;
      colors[1] = (pattern >> 6) & 3;
      colors[2] = (pattern >> 12) & 3;
      colors[3] = (pattern >> 4) & 3;
      colors[4] = (pattern >> 10) & 3;
      colors[5] = (pattern >> 2) & 3;
      colors[6] = (pattern >> 8) & 3;
      colors[7] = pattern & 3;
   }
}

/* we render a scanline of graphics first so we know exactly
** where the sprite 0 strike is going to occur (in terms of
** cpu cycles), using the relation that 3 pixels == 1 cpu cycle
*/
INLINE void check_strike(uint8 *surface, uint8 attrib, uint16 pattern)
{
   uint8 colors[8];

   /* Flag already set */
   if (ppu.strikeflag)
      return;

   /* sprite is 100% transparent */
   if (0 == pattern)
      return;

   build_tile_colors(attrib & OAMF_HFLIP, pattern, colors);

   for (int i = 0; i < 8; i++)
   {
      if (colors[i] && (!surface || BG_SOLID(surface[i])))
      {
         /* 3 pixels per cpu cycle */
         ppu.strike_cycle = nes6502_getcycles() + (i / 3);
         ppu.strikeflag = true;
         return;
      }
   }
}

INLINE void draw_bgtile(uint8 *surface, uint16 pattern, const uint8 *colors)
{
   *surface++ = colors[(pattern >> 14) & 3];
   *surface++ = colors[(pattern >> 6) & 3];
   *surface++ = colors[(pattern >> 12) & 3];
   *surface++ = colors[(pattern >> 4) & 3];
   *surface++ = colors[(pattern >> 10) & 3];
   *surface++ = colors[(pattern >> 2) & 3];
   *surface++ = colors[(pattern >> 8) & 3];
   *surface   = colors[pattern & 3];
}

INLINE void draw_oamtile(uint8 *surface, uint8 attrib, uint16 pattern, const uint8 *col_tbl)
{
   uint8 colors[8];

   /* sprite is 100% transparent */
   if (0 == pattern)
      return;

   build_tile_colors(attrib & OAMF_HFLIP, pattern, colors);

   /* draw the character */
   if (attrib & OAMF_BEHIND)
   {
      if (colors[0])  surface[0] = SP_PIXEL | (BG_CLEAR(surface[0]) ? col_tbl[colors[0]] : surface[0]);
      if (colors[1])  surface[1] = SP_PIXEL | (BG_CLEAR(surface[1]) ? col_tbl[colors[1]] : surface[1]);
      if (colors[2])  surface[2] = SP_PIXEL | (BG_CLEAR(surface[2]) ? col_tbl[colors[2]] : surface[2]);
      if (colors[3])  surface[3] = SP_PIXEL | (BG_CLEAR(surface[3]) ? col_tbl[colors[3]] : surface[3]);
      if (colors[4])  surface[4] = SP_PIXEL | (BG_CLEAR(surface[4]) ? col_tbl[colors[4]] : surface[4]);
      if (colors[5])  surface[5] = SP_PIXEL | (BG_CLEAR(surface[5]) ? col_tbl[colors[5]] : surface[5]);
      if (colors[6])  surface[6] = SP_PIXEL | (BG_CLEAR(surface[6]) ? col_tbl[colors[6]] : surface[6]);
      if (colors[7])  surface[7] = SP_PIXEL | (BG_CLEAR(surface[7]) ? col_tbl[colors[7]] : surface[7]);
   }
   else
   {
      if (colors[0] && SP_CLEAR(surface[0]))  surface[0] = SP_PIXEL | col_tbl[colors[0]];
      if (colors[1] && SP_CLEAR(surface[1]))  surface[1] = SP_PIXEL | col_tbl[colors[1]];
      if (colors[2] && SP_CLEAR(surface[2]))  surface[2] = SP_PIXEL | col_tbl[colors[2]];
      if (colors[3] && SP_CLEAR(surface[3]))  surface[3] = SP_PIXEL | col_tbl[colors[3]];
      if (colors[4] && SP_CLEAR(surface[4]))  surface[4] = SP_PIXEL | col_tbl[colors[4]];
      if (colors[5] && SP_CLEAR(surface[5]))  surface[5] = SP_PIXEL | col_tbl[colors[5]];
      if (colors[6] && SP_CLEAR(surface[6]))  surface[6] = SP_PIXEL | col_tbl[colors[6]];
      if (colors[7] && SP_CLEAR(surface[7]))  surface[7] = SP_PIXEL | col_tbl[colors[7]];
   }
}

INLINE void ppu_renderbg(uint8 *vidbuf)
{
   /* draw a line of transparent background color if bg is disabled */
   if (!ppu.bg_on)
   {
      memset(vidbuf, FULLBG, NES_SCREEN_WIDTH);
      return;
   }

   uint8 *bmp_ptr = vidbuf - ppu.tile_xofs; /* scroll x */
   uint x_tile = ppu.vaddr & 0x1F;
   uint refresh_vaddr = 0x2000 + (ppu.vaddr & 0x0FE0); /* mask out x tile */
   uint bg_offset = ((ppu.vaddr >> 12) & 7) + ppu.bg_base; /* offset in y tile */
   uint attrib_base = (refresh_vaddr & 0x2C00) + 0x3C0 + (((ppu.vaddr >> 5) & 0x1C) << 1);
   uint attrib_addr = attrib_base + (x_tile >> 2);
   uint attrib = PPU_MEM_READ(attrib_addr); attrib_addr++;
   uint attrib_shift = (x_tile & 2) + (((ppu.vaddr >> 5) & 2) << 1);
   uint col_high = ((attrib >> attrib_shift) & 3) << 2;

   /* ppu fetches 33 tiles */
   for (int tile_num = 0; tile_num < 33; tile_num++)
   {
      /* Tile number from nametable */
      int tile_index = PPU_MEM_READ(refresh_vaddr + x_tile);

      /* Handle $FD/$FE tile VROM switching (PunchOut) */
      if (ppu.latchfunc)
         ppu.latchfunc(ppu.bg_base, tile_index);

      /* Fetch tile and draw it */
      draw_bgtile(bmp_ptr, get_patpix(bg_offset + (tile_index << 4)), ppu.palette + col_high);
      bmp_ptr += 8;

      x_tile++;

      if (0 == (x_tile & 1))     /* check every 2 tiles */
      {
         if (0 == (x_tile & 3))  /* check every 4 tiles */
         {
            if (32 == x_tile)    /* check every 32 tiles */
            {
               x_tile = 0;
               refresh_vaddr ^= (1 << 10); /* switch nametable */
               attrib_base ^= (1 << 10);
               attrib_addr = attrib_base;
            }

            /* Get the attribute byte */
            attrib = PPU_MEM_READ(attrib_addr); attrib_addr++;
         }

         attrib_shift ^= 2;
         col_high = ((attrib >> attrib_shift) & 3) << 2;
      }
   }

   /* Blank left hand column if need be */
   if (!ppu.left_bg_on)
   {
      memset(vidbuf, FULLBG, 8);
      ppu.left_bg_counter++;
   }
}

/* TODO: fetch valid OAM a scanline before, like the Real Thing */
INLINE void ppu_renderoam(uint8 *vidbuf, int scanline, bool draw)
{
   if (!ppu.obj_on)
      return;

   /* Save left hand column */
   uint32 savecol1 = ((uint32 *) vidbuf)[0];
   uint32 savecol2 = ((uint32 *) vidbuf)[1];

   int sprite_height = ppu.obj_height;
   int sprite_offset = ppu.obj_base;

   for (int sprite_num = 0, count = 0; sprite_num < 64; sprite_num++)
   {
      ppu_obj_t *sprite = (ppu_obj_t *)ppu.oam + sprite_num;

      int sprite_y = sprite->y_loc + 1;

      /* Check to see if sprite is out of range */
      if ((sprite_y > scanline) || (sprite_y <= (scanline - sprite_height))
          || (0 == sprite_y) || (sprite_y >= 240))
         continue;

      /* Handle $FD/$FE tile VROM switching (PunchOut) */
      if (ppu.latchfunc)
         ppu.latchfunc(sprite_offset, sprite->tile);

      int tile_addr, y_offset;

      /* 8x16 even sprites use $0000, odd use $1000 */
      if (16 == sprite_height)
         tile_addr = ((sprite->tile & 1) << 12) | ((sprite->tile & 0xFE) << 4);
      else
         tile_addr = sprite_offset + (sprite->tile << 4);

      /* Calculate offset (line within the sprite) */
      y_offset = scanline - sprite_y;
      if (y_offset > 7)
         y_offset += 8;

      /* Account for vertical flippage */
      if (sprite->attr & OAMF_VFLIP)
      {
         if (16 == sprite_height)
            y_offset -= 23;
         else
            y_offset -= 7;

         tile_addr -= y_offset;
      }
      else
      {
         tile_addr += y_offset;
      }

      /* Check for a strike on sprite 0 if strike flag isn't set */
      if (sprite_num == 0 && !ppu.strikeflag)
      {
         check_strike(draw ? vidbuf + sprite->x_loc : NULL, sprite->attr, get_patpix(tile_addr));
      }

      /* If we don't draw to buffer then we're done after sprite 0 */
      if (!draw)
         return;

      /* Fetch tile and draw it */
      draw_oamtile(
         vidbuf + sprite->x_loc,
         sprite->attr,
         get_patpix(tile_addr),
         ppu.palette + 16 + ((sprite->attr & 3) << 2));

      /* maximum of 8 sprites per scanline */
      if (OPT(PPU_LIMIT_SPRITES) && ++count == PPU_MAXSPRITE)
      {
         ppu.stat |= PPU_STATF_MAXSPRITE;
         break;
      }
   }

   /* Restore lefthand column */
   if (!ppu.left_obj_on)
   {
      ((uint32 *) vidbuf)[0] = savecol1;
      ((uint32 *) vidbuf)[1] = savecol2;
   }
}

IRAM_ATTR bool ppu_enabled(void)
{
   return (ppu.bg_on || ppu.obj_on);
}

IRAM_ATTR bool ppu_inframe(void)
{
   return (ppu.scanline < 240);
}

IRAM_ATTR void ppu_endscanline()
{
   /* modify vram address at end of scanline */
   if (ppu.scanline < 240 && (ppu.bg_on || ppu.obj_on))
   {
      /* check for max 3 bit y tile offset */
      if (7 == (ppu.vaddr >> 12))
      {
         switch ((ppu.vaddr >> 5) & 0x1F)
         {
            case 29:
               ppu.vaddr &= ~0x73E0;   /* clear y tile and offset */
               ppu.vaddr ^= 0x0800;    /* toggle nametable */
               break;
            case 31:
               ppu.vaddr &= ~0x73E0;   /* clear y tile and offset */
               break;
            default:
               ppu.vaddr &= ~0x7000;   /* clear y tile offset */
               ppu.vaddr += 0x20;      /* increment y tile */
         }
      }
      else
      {
         ppu.vaddr += 0x1000;       /* increment tile y offset */
      }
   }
}

IRAM_ATTR void ppu_scanline(bitmap_t *bmp, int scanline, bool draw_flag)
{
   ppu.scanline = scanline;

   // Draw visible line
   if (scanline < 240)
   {
      /* Lower the Max Sprite per scanline flag */
      ppu.stat &= ~PPU_STATF_MAXSPRITE;

      /* start scanline - transfer ppu latch into vaddr */
      if (ppu.bg_on || ppu.obj_on)
      {
         if (scanline == 0)
            ppu.vaddr = ppu.vaddr_latch;
         else
            ppu.vaddr = (ppu.vaddr & ~0x041F) | (ppu.vaddr_latch & 0x041F);
      }

      if (scanline == 0)
         ppu.left_bg_counter = 0;

      if (draw_flag && OPT(PPU_DRAW_BACKGROUND))
         ppu_renderbg(bmp->line[scanline]);

      /* TODO: fetch obj data 1 scanline before */
      ppu_renderoam(bmp->line[scanline], scanline, draw_flag && OPT(PPU_DRAW_SPRITES));
   }
   // Vertical Blank
   else if (scanline == 241)
   {
      ppu.stat |= PPU_STATF_VBLANK;
      ppu.vram_accessible = true;
   }
   // End of frame
   else if (scanline == ppu.scanlines_per_frame - 1)
   {
      ppu.stat &= ~PPU_STATF_VBLANK;
      ppu.strikeflag = false;
      ppu.strike_cycle = (uint32) -1;
      ppu.vram_accessible = false;
   }
}

bool ppu_checkzapperhit(bitmap_t *bmp, int x, int y)
{
   uint8 pixel = bmp->line[y][x] & 0x3F;

   if (0x20 == pixel || 0x30 == pixel)
      return true;

   return false;
}

/* reset state of ppu */
void ppu_reset()
{
   memset(ppu.nametab, 0, sizeof(ppu.nametab));
   memset(ppu.oam, 0, sizeof(ppu.oam));

   ppu.ctrl0 = 0;
   ppu.ctrl1 = PPU_CTRL1F_OBJON | PPU_CTRL1F_BGON;
   ppu.stat = 0;
   ppu.flipflop = 0;
   ppu.vaddr = ppu.vaddr_latch = 0x2000;
   ppu.oam_addr = 0;
   ppu.tile_xofs = 0;

   ppu.latch = 0;
   ppu.vram_accessible = true;
}

ppu_t *ppu_init(int region)
{
   memset(&ppu, 0, sizeof(ppu_t));

   ppu.scanlines_per_frame = (region == NES_PAL) ? NES_SCANLINES_PAL : NES_SCANLINES_NTSC;

   ppu_setopt(PPU_DRAW_BACKGROUND, true);
   ppu_setopt(PPU_DRAW_SPRITES, true);
   ppu_setopt(PPU_LIMIT_SPRITES, true);
   ppu_setopt(PPU_PALETTE_RGB, 0);

   return &ppu;
}

void ppu_shutdown()
{
   //
}




/*************************************************/
/* TODO: all this stuff should go somewhere else */
/*************************************************/
INLINE void draw_box(bitmap_t *bmp, int x, int y, int height)
{
   int i;
   uint8 *vid;

   vid = bmp->line[y] + x;

   for (i = 0; i < 10; i++)
      *vid++ = GUI_GRAY;
   vid += (bmp->pitch - 10);
   for (i = 0; i < height; i++)
   {
      vid[0] = vid[9] = GUI_GRAY;
      vid += bmp->pitch;
   }
   for (i = 0; i < 10; i++)
      *vid++ = GUI_GRAY;
}

INLINE void draw_deadsprite(bitmap_t *bmp, int x, int y, int height)
{
   int i, j, index;
   uint8 *vid;
   uint8 colbuf[8] = { GUI_BLACK, GUI_BLACK, GUI_BLACK, GUI_BLACK,
                       GUI_BLACK, GUI_BLACK, GUI_BLACK, GUI_DKGRAY };

   vid = bmp->line[y] + x;

   for (i = 0; i < height; i++)
   {
      index = i;

      if (height == 16)
         index >>= 1;

      for (j = 0; j < 8; j++)
      {
         *(vid + j) = colbuf[index++];
         index &= 7;
      }

      vid += bmp->pitch;
   }
}

INLINE void draw_sprite(bitmap_t *bmp, int x, int y, uint8 tile_num, uint8 attrib)
{
   int line, height;
   int col_high, tile_addr;
   uint8 *vid;

   vid = bmp->line[y] + x;

   /* Get upper two bits of color */
   col_high = ((attrib & 3) << 2);

   /* 8x16 even sprites use $0000, odd use $1000 */
   height = ppu.obj_height;
   if (16 == height)
      tile_addr = ((tile_num & 1) << 12) | ((tile_num & 0xFE) << 4);
   /* else just use the offset from $2000 */
   else
      tile_addr = ppu.obj_base + (tile_num << 4);

   for (line = 0; line < height; line++)
   {
      if (line == 8)
         tile_addr += 8;

      draw_bgtile(vid, get_patpix(tile_addr), ppu.palette + 16 + col_high);
      //draw_oamtile(vid, attrib, data_ptr[0], data_ptr[8], ppu.palette + 16 + col_high);

      tile_addr++;
      vid += bmp->pitch;
   }
}

void ppu_dumpoam(bitmap_t *bmp, int x_loc, int y_loc)
{
   int sprite, x_pos, y_pos, height;
   ppu_obj_t *spr_ptr;

   spr_ptr = (ppu_obj_t *) ppu.oam;
   height = ppu.obj_height;

   for (sprite = 0; sprite < 64; sprite++)
   {
      x_pos = ((sprite & 0x0F) << 3) + (sprite & 0x0F) + x_loc;
      if (height == 16)
         y_pos = (sprite & 0xF0) + (sprite >> 4) + y_loc;
      else
         y_pos = ((sprite & 0xF0) >> 1) + (sprite >> 4) + y_loc;

      draw_box(bmp, x_pos, y_pos, height);

      if (spr_ptr->y_loc && spr_ptr->y_loc < 240)
         draw_sprite(bmp, x_pos + 1, y_pos + 1, spr_ptr->tile, spr_ptr->attr);
      else
         draw_deadsprite(bmp, x_pos + 1, y_pos + 1, height);

      spr_ptr++;
   }
}

void ppu_dumpbg(bitmap_t *bmp, int x_loc, int y_loc)
{
   //
}
