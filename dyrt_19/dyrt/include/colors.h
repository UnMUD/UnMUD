/*
 * Colors.h - this file contains information related to colors on the game
 */
typedef enum
{
    COLOR_NORMAL_BLACK,
    COLOR_NORMAL_RED,
    COLOR_NORMAL_GREEN,
    COLOR_NORMAL_YELLOW,
    COLOR_NORMAL_BLUE,
    COLOR_NORMAL_MAGENTA,
    COLOR_NORMAL_CYAN,
    COLOR_NORMAL_WHITE,

    COLOR_HILIGHT_BLACK,
    COLOR_HILIGHT_RED,
    COLOR_HILIGHT_GREEN,
    COLOR_HILIGHT_YELLOW,
    COLOR_HILIGHT_BLUE,
    COLOR_HILIGHT_MAGENTA,
    COLOR_HILIGHT_CYAN,
    COLOR_HILIGHT_WHITE,

    COLOR_BG_NORM_BLACK,
    COLOR_BG_NORM_RED,
    COLOR_BG_NORM_GREEN,
    COLOR_BG_NORM_YELLOW,
    COLOR_BG_NORM_BLUE,
    COLOR_BG_NORM_MAGENTA,
    COLOR_BG_NORM_CYAN,
    COLOR_BG_NORM_WHITE,

    COLOR_BG_HIGH_BLACK,
    COLOR_BG_HIGH_RED,
    COLOR_BG_HIGH_GREEN,
    COLOR_BG_HIGH_YELLOW,
    COLOR_BG_HIGH_BLUE,
    COLOR_BG_HIGH_MAGENTA,
    COLOR_BG_HIGH_CYAN,
    COLOR_BG_HIGH_WHITE,

    COLOR_NONE
} color_code_type;

static char *color_strings[] =
    {
        "&+l", "&+r", "&+g", "&+y", "&+b", "&+m", "&+c", "&+w",
        "&+L", "&+R", "&+G", "&+Y", "&+B", "&+M", "&+C", "&+W",
        "&-l", "&-r", "&-g", "&-y", "&-b", "&-m", "&-c", "&-w",
        "&-L", "&-R", "&-G", "&-Y", "&-B", "&-M", "&-C", "&-W",
        ""};
