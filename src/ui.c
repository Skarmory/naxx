#include "ui.h"

#include "colour.h"
#include "globals.h"
#include "log.h"
#include "map.h"
#include "message.h"
#include "monster.h"
#include "mon_equip.h"
#include "mon_inventory.h"
#include "mon_stats.h"
#include "object.h"
#include "player.h"
#include "player_class.h"
#include "term.h"
#include "ui_map.h"
#include "util.h"

#include <stdio.h>
#include <string.h>

#define STATUS_X 0
#define STATUS_Y 45
#define STATUS_W 80

struct UI* g_ui = NULL;

void draw_textbox(int x, int y, int w, int h, struct Colour* fg, struct Colour* bg, const char* text)
{
    term_draw_area(x, y, w, h, fg, bg, 0, ' ');

    int stridx   = 0;
    int textlen  = strlen(text);

    // Print text
    while(stridx < textlen)
    {
        int chars = w;
        if((stridx + w) < textlen)
        {
            chars = strrfindi(text, ' ', stridx + w);

            if(chars == -1)
            {
                chars = w;
            }
            else
            {
                chars -= stridx;
            }
        }

        term_draw_fntext(chars, x, y++, fg, bg, 0, "%s", text + stridx);
        stridx += chars;
    }
}

void draw_textbox_border(int x, int y, int w, int h, struct Colour* fg, struct Colour* bg, const char* text)
{
    int _w = w + 4;
    int _h = h + 4;

    term_draw_area(x, y, _w, _h, fg, bg, 0, ' ');

    for(int _x = x + 1; _x < (x+_w-1); ++_x)
    {
        term_draw_symbol(_x,        y, NULL, COL(CLR_DGREY), A_BOLD_BIT, '=');
        term_draw_symbol(_x, (y+_h-1), NULL, COL(CLR_DGREY), A_BOLD_BIT, '=');
    }

    for(int _y = y + 1; _y < (y+_h-1); ++_y)
    {
        term_draw_symbol(x,        _y, NULL, COL(CLR_DGREY), A_BOLD_BIT, '|');
        term_draw_symbol((x+_w-1), _y, NULL, COL(CLR_DGREY), A_BOLD_BIT, '|');
    }

    term_draw_symbol(x,           y, NULL, COL(CLR_DGREY), A_BOLD_BIT, '+');
    term_draw_symbol(x+_w-1, y+_h-1, NULL, COL(CLR_DGREY), A_BOLD_BIT, '+');
    term_draw_symbol(x+_w-1,      y, NULL, COL(CLR_DGREY), A_BOLD_BIT, '+');
    term_draw_symbol(x,      y+_h-1, NULL, COL(CLR_DGREY), A_BOLD_BIT, '+');

    draw_textbox(x+2, y+2, _w-3, _h-3, fg, bg, text);
}

bool prompt_yn(const char* msg)
{
    clear_msgs();
    display_fmsg_nolog("%s [yn] (n)", msg);
    flush_msg_buffer();
    term_refresh();

    bool decision = (enum YesNoCommand)get_key() == YES;

    display_fmsg_log("%s [yn] (n) %c", msg, decision ? YES : NO);
    flush_msg_buffer();
    term_refresh();
    return decision;
}

char prompt_choice(const char* title, char** choices, int length)
{
    char last_option_id = KEYCODE_a + length - 1;

    int x = (screen_cols / 2) - (g_option_name_max_size / 2);
    int y = (screen_rows / 2) - (length / 2);

    term_draw_ftext(x, y++, NULL, NULL, A_BOLD_BIT, "%s", title);

    for(int option_id = 0; option_id < length; ++option_id)
    {
        term_draw_ftext(x, y++, NULL, NULL, 0, "%c - %s", (KEYCODE_a + option_id), choices[option_id]);
    }

    term_refresh();

    char choice;
    do
    {
        choice = get_key();
    }
    while(choice != KEYCODE_ESC && (choice < KEYCODE_a || choice > last_option_id));

    return choice;
}

void display_main_screen(void)
{
    ui_map_draw(g_ui->ui_map);
    display_char_status();
}

/*
 * Prints a simple view of the player's stats at the bottom of the game area.
 */
void display_char_status(void)
{
    char tmp[STATUS_W];
    sprintf(tmp, "%s\tHP: %d/%d", g_you->name, g_you->mon->stats.hp, g_you->mon->stats.hp_max);

    term_clear_area(STATUS_X, STATUS_Y, MCOLS, 1);
    term_draw_text(STATUS_X, STATUS_Y, NULL, NULL, 0, tmp);
}
