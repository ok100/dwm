/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "bstack.c"
#include "push.c"
#include "restart.c"

/* appearance */
static const char font[] = "-*-termsyn-medium-r-*-*-13-*-*-*-*-*-*-*";

#define NUMCOLORS 3
static const char colors[NUMCOLORS][ColLast][8] = {
	/* border     fg          bg */
	{ "#373B41", "#707880", "#1D1F21" },        /* 01 = normal */
	{ "#707880", "#C5C8C6", "#1D1F21" },        /* 02 = selected */
	{ "#A54242", "#CC6666", "#1D1F21" },        /* 03 = urgent/warning */
};

static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
static const char *tags[] = { "base", "web", "term", "work", "media", "irc" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class               instance    title           tags mask   isfloating   iscentred   monitor */
    {  NULL,               NULL,      "wget",          1 << 0,      False,       True,       -1 },
	{ "Gimp",              NULL,       NULL,           1 << 0,      False,       True,       -1 },
    { "Skype",             NULL,       NULL,           1 << 0,      True,        False,      -1 },
    { "Xsane",             NULL,       NULL,           1 << 0,      True,        False,      -1 },
	{ "Dwb",               NULL,       NULL,           1 << 1,      False,       True,       -1 },
	{ "Firefox",           NULL,       NULL,           1 << 1,      False,       True,       -1 },
	{  NULL,               NULL,      "tmux-netbook",  1 << 2,      False,       True,       -1 },
    {  NULL,               NULL,      "tmux-music",    1 << 4,      False,       True,       -1 },
    { "Lingot",            NULL,       NULL,           1 << 4,      True,        True,       -1 },
	{  NULL,               NULL,      "tmux-irc",      1 << 5,      False,       True,       -1 },
    { "GuitarPro",         NULL,       NULL,           1 << 5,      False,       True,       -1 },
	{ "Lxappearance",      NULL,       NULL,           0,           True,        True,       -1 },
	{ "Qalculate",         NULL,       NULL,           0,           True,        True,       -1 },
	{ "Stardict",          NULL,       NULL,           0,           True,        True,       -1 },
	{ "mplayer2",          NULL,       NULL,           0,           True,        True,       -1 },
    { "Qpass",             NULL,       NULL,           0,           True,        True,       -1 },
};

/* layout(s) */
static const float mfact      = 0.58;           /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;              /* number of clients in master area */
static const Bool resizehints = False;          /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[|]",      tile },                       /* first entry is default */
	{ "[ ]",      monocle },
	{ "[-]",      bstack },
	{ "><>",      NULL },                       /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *dmenucmd[] = { "dmenu_run", "-fn", font, "-nb", colors[0][ColBG], "-nf", colors[0][ColFG],
                                  "-sb", colors[1][ColBG], "-sf", colors[1][ColFG], NULL };
static const char *termcmd[]  = { "urxvtc", NULL };
static const char *dcalcmd[]  = { "dcal.sh", "-fn", font, "-bg", colors[0][ColBG], "-bd", colors[1][ColBorder],
                                  "-cf", colors[0][ColFG], "-of", colors[0][ColBorder],
								  "-tf", colors[1][ColFG], "-hf", colors[2][ColFG], "-y", "15", NULL };

static Key keys[] = {
	/* modifier          key                       function        argument */
	{ Mod4Mask,          XK_b,                     spawn,          SHCMD("dwb") },
	{ Mod4Mask,          XK_c,                     spawn,          SHCMD("lolictrl") },
	{ Mod4Mask,          XK_s,                     spawn,          SHCMD("lolictrl -spc") },
	{ Mod4Mask,          XK_d,                     spawn,          SHCMD("stardict") },
	{ Mod4Mask,          XK_i,                     spawn,          SHCMD("urxvtc -title tmux-irc -e ssh ok100@shell.bshellz.net -t tmux attach") },
	{ Mod4Mask,          XK_m,                     spawn,          SHCMD("urxvtc -title tmux-music -e music") },
	{ Mod4Mask,          XK_t,                     spawn,          SHCMD("urxvtc -title tmux-netbook -e tmux attach") },
	{ Mod4Mask,          XK_h,                     spawn,          SHCMD("qpass") },
	{ Mod4Mask,          XK_w,                     spawn,          SHCMD("sudo connman_dmenu") },
	{ Mod4Mask,          XK_F2,                    spawn,          SHCMD("connmanctl disable wifi") },
	{ 0,                 XF86XK_Launch1,           spawn,          SHCMD("xset dpms force off") },
	{ 0,                 XF86XK_PowerOff,          spawn,          SHCMD("dmenu-powerbutton") },
	{ 0,                 XF86XK_AudioLowerVolume,  spawn,          SHCMD("volume 2-") },
	{ 0,                 XF86XK_AudioMute,         spawn,          SHCMD("volume toggle") },
	{ 0,                 XF86XK_AudioRaiseVolume,  spawn,          SHCMD("volume 2+") },
	{ 0,                 XF86XK_ScreenSaver,       spawn,          SHCMD("mpc toggle") },
	{ 0,                 XF86XK_Launch2,           spawn,          SHCMD("mpc prev") },
	{ 0,                 XF86XK_Launch3,           spawn,          SHCMD("mpc next") },
	{ 0,                 XK_Menu,                  spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,  XK_Return,                spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,  XK_r,                     restart,        {0} },
	{ MODKEY|ShiftMask,  XK_b,                     togglebar,      {0} },
	{ MODKEY,            XK_Down,                  focusstack,     {.i = +1 } },
	{ MODKEY,            XK_Up,                    focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,  XK_Left,                  incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,  XK_Right,                 incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,  XK_Up,                    pushup,         {0} },
	{ MODKEY|ShiftMask,  XK_Down,                  pushdown,       {0} },
	{ MODKEY,            XK_Left,                  setmfact,       {.f = -0.01} },
	{ MODKEY,            XK_Right,                 setmfact,       {.f = +0.01} },
	{ MODKEY,            XK_Return,                zoom,           {0} },
	{ MODKEY,            XK_Tab,                   view,           {0} },
	{ MODKEY|ShiftMask,  XK_c,                     killclient,     {0} },
	{ MODKEY,            XK_t,                     setlayout,      {.v = &layouts[0]} },
	{ MODKEY,            XK_m,                     setlayout,      {.v = &layouts[1]} },
	{ MODKEY,            XK_b,                     setlayout,      {.v = &layouts[2]} },
	{ MODKEY,            XK_f,                     setlayout,      {.v = &layouts[3]} },
	{ MODKEY,            XK_space,                 setlayout,      {0} },
	{ MODKEY|ShiftMask,  XK_space,                 togglefloating, {0} },
	{ MODKEY,            XK_eacute,                view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,  XK_eacute,                tag,            {.ui = ~0 } },
	{ MODKEY,            XK_comma,                 focusmon,       {.i = -1 } },
	{ MODKEY,            XK_period,                focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,  XK_comma,                 tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,  XK_period,                tagmon,         {.i = +1 } },
	TAGKEYS(             XK_plus,                                  0)
	TAGKEYS(             XK_ecaron,                                1)
	TAGKEYS(             XK_scaron,                                2)
	TAGKEYS(             XK_ccaron,                                3)
	TAGKEYS(             XK_rcaron,                                4)
	TAGKEYS(             XK_zcaron,                                5)
	{ MODKEY|ShiftMask,  XK_q,                     quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkWinTitle,          0,              Button1,        focusstack,     {.i = +1 } },
	{ ClkWinTitle,          0,              Button3,        focusstack,     {.i = -1 } },
	{ ClkStatusText,        0,              Button3,        spawn,          SHCMD("mygtkmenu /home/ok/.menu") },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = dcalcmd } },
	{ ClkRootWin,           0,              Button3,        spawn,          SHCMD("mygtkmenu /home/ok/.menu") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

