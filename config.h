/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "bstack.c"
#include "push.c"
#include "restart.c"

/* appearance */
static const char font[] = "-*-tamsyn-medium-r-*-*-14-*-*-*-*-*-*-*";

#define NUMCOLORS 3
static const char colors[NUMCOLORS][ColLast][8] = {
	/* border     fg          bg */
	{ "#262626", "#757978", "#1C1C1C" },        /* 01 = normal */
	{ "#757978", "#C1C4BC", "#1C1C1C" },        /* 02 = selected */
	{ "#A04363", "#A04363", "#1C1C1C" },        /* 03 = urgent/warning */
};

static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
static const char *tags[] = { "1:base", "2:web", "3:term", "4:docs", "5:media", "6:mail", "7:dev" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class               instance    title           tags mask   isfloating   iscentred   monitor */
    {  NULL,               NULL,      "wget",          1 << 0,      False,       True,       -1 },
	{ "Gimp",              NULL,       NULL,           1 << 0,      False,       True,       -1 },
	{ "Dwb",               NULL,       NULL,           1 << 1,      False,       True,       -1 },
	{ "Firefox",           NULL,       NULL,           1 << 1,      False,       True,       -1 },
    {  NULL,               NULL,      "ranger",        1 << 2,      False,       True,       -1 },
    {  NULL,               NULL,      "tmux-music",    1 << 4,      False,       True,       -1 },
    { "Lingot",            NULL,       NULL,           1 << 4,      True,        True,       -1 },
	{  NULL,               NULL,      "irc",           1 << 5,      False,       True,       -1 },
	{  NULL,               NULL,      "mutt",          1 << 5,      False,       True,       -1 },
	{  NULL,               NULL,      "tmux-irc",      1 << 5,      False,       True,       -1 },
    { "Eclipse",           NULL,       NULL,           1 << 6,      False,       True,       -1 },
    { "Java",              NULL,       NULL,           1 << 6,      True,        True,       -1 },
	{ "Lxappearance",      NULL,       NULL,           0,           True,        True,       -1 },
	{ "Qalculate",         NULL,       NULL,           0,           True,        True,       -1 },
	{ "Stardict",          NULL,       NULL,           0,           True,        True,       -1 },
	{ "mpv",               NULL,       NULL,           0,           True,        True,       -1 },
    { "Qpass",             NULL,       NULL,           0,           True,        True,       -1 },
    { "Xdialog",           NULL,       NULL,           0,           True,        True,       -1 },
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
static const char *dmenucmd[]   = { "dmenu_run", "-fn", font, "-nb", colors[0][ColBG], "-nf", colors[0][ColFG],
                                    "-sb", colors[1][ColBG], "-sf", colors[1][ColFG], "-i", NULL };
static const char *findercmd[]  = { "finder", "-fn", font, "-nb", colors[0][ColBG], "-nf", colors[0][ColFG],
                                    "-sb", colors[1][ColBG], "-sf", colors[1][ColFG], "-i", NULL };
static const char *termcmd[]    = { "urxvtc", NULL };
static const char *dcalcmd[]    = { "dcal.sh", "-fn", font, "-bg", colors[0][ColBG], "-bd", colors[1][ColBorder],
                                    "-cf", colors[0][ColFG], "-of", colors[0][ColBorder],
							 	    "-tf", colors[1][ColFG], "-hf", colors[2][ColFG], "-y", "15", NULL };
static const char *connmancmd[] = { "sudo", "connman_dmenu", NULL };

static Key keys[] = {
	/* modifier            key                       function        argument */
	{ Mod4Mask,            XK_b,                     spawn,          SHCMD("dwb") },
	{ Mod4Mask,            XK_c,                     spawn,          SHCMD("lolictrl") },
	{ Mod4Mask,            XK_s,                     spawn,          SHCMD("lolictrl -spc") },
	{ Mod4Mask,            XK_d,                     spawn,          SHCMD("stardict") },
	{ Mod4Mask,            XK_i,                     spawn,          SHCMD("urxvtc -title tmux-irc -e ssh ok100@shell.bshellz.net -t tmux attach") },
	{ Mod4Mask,            XK_m,                     spawn,          SHCMD("urxvtc -title tmux-music -e music") },
	{ Mod4Mask,            XK_r,                     spawn,          SHCMD("urxvtc -title ranger -e ranger") },
	{ Mod4Mask,            XK_h,                     spawn,          SHCMD("qpass") },
	{ Mod4Mask,            XK_w,                     spawn,          {.v = connmancmd } },
	{ Mod4Mask,            XK_f,                     spawn,          {.v = findercmd } },
	{ Mod4Mask,            XK_F2,                    spawn,          SHCMD("connmanctl disable wifi") },
	{ 0,                   XF86XK_PowerOff,          spawn,          SHCMD("xautolock -locknow") },
	{ 0,                   XF86XK_AudioLowerVolume,  spawn,          SHCMD("volume down") },
	{ 0,                   XF86XK_AudioMute,         spawn,          SHCMD("volume toggle") },
	{ 0,                   XF86XK_AudioRaiseVolume,  spawn,          SHCMD("volume up") },
	{ 0,                   XF86XK_AudioPlay,         spawn,          SHCMD("mpc toggle") },
	{ 0,                   XF86XK_AudioNext,         spawn,          SHCMD("mpc next") },
	{ 0,                   XF86XK_AudioPrev,         spawn,          SHCMD("mpc prev") },
	{ 0,                   XF86XK_AudioStop,         spawn,          SHCMD("mpc stop") },
	{ 0,                   XK_Menu,                  spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,    XK_Return,                spawn,          {.v = termcmd } },
	{ Mod4Mask|ShiftMask,  XK_r,                     restart,        {0} },
	{ MODKEY|ShiftMask,    XK_b,                     togglebar,      {0} },
	{ MODKEY,              XK_Down,                  focusstack,     {.i = +1 } },
	{ MODKEY,              XK_Up,                    focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_Left,                  incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_Right,                 incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_Up,                    pushup,         {0} },
	{ MODKEY|ShiftMask,    XK_Down,                  pushdown,       {0} },
	{ MODKEY,              XK_Left,                  setmfact,       {.f = -0.01} },
	{ MODKEY,              XK_Right,                 setmfact,       {.f = +0.01} },
	{ MODKEY,              XK_Return,                zoom,           {0} },
	{ MODKEY,              XK_Tab,                   view,           {0} },
	{ MODKEY|ShiftMask,    XK_c,                     killclient,     {0} },
	{ MODKEY,              XK_t,                     setlayout,      {.v = &layouts[0]} },
	{ MODKEY,              XK_m,                     setlayout,      {.v = &layouts[1]} },
	{ MODKEY,              XK_b,                     setlayout,      {.v = &layouts[2]} },
	{ MODKEY,              XK_f,                     setlayout,      {.v = &layouts[3]} },
	{ MODKEY,              XK_space,                 setlayout,      {0} },
	{ MODKEY|ShiftMask,    XK_space,                 togglefloating, {0} },
	{ MODKEY,              XK_eacute,                view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,    XK_eacute,                tag,            {.ui = ~0 } },
	{ MODKEY,              XK_comma,                 focusmon,       {.i = -1 } },
	{ MODKEY,              XK_period,                focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_comma,                 tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_period,                tagmon,         {.i = +1 } },
	TAGKEYS(               XK_plus,                                  0)
	TAGKEYS(               XK_ecaron,                                1)
	TAGKEYS(               XK_scaron,                                2)
	TAGKEYS(               XK_ccaron,                                3)
	TAGKEYS(               XK_rcaron,                                4)
	TAGKEYS(               XK_zcaron,                                5)
	TAGKEYS(               XK_yacute,                                6)
	{ MODKEY|ShiftMask,    XK_q,                     quit,           {0} },
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

