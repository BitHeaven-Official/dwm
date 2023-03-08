/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */

static const unsigned int borderpx  = 4;
static const unsigned int gappx     = 15;
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int showextrabar       = 1;        /* 0 means no bar */
// TODO: FIX THAT SHIT
static const int topbar             = 1;        /* 0 means bottom bar */
static const char statussep         = ';';
static const int horizpadbar        = 8;
static const int vertpadbar         = 12;
static const int vertpadstat        = 0;
static const char *fonts[]          = {
    "Ubuntu Mono:style=Bold:size=12:antialias=true:autohint=true",
    "Font Awesome 6 Free Solid:style=Solid:size=12:antialias=true:autohint=true",
    "Font Awesome 6 Brands Regular:style=Regular:size=12:antialias=true:autohint=true"
};
static char dmenufont[]          = "Ubuntu Mono:size=12";
static char normbgcolor[]        = "#222222";
static char normbordercolor[]    = "#444444";
static char normfgcolor[]        = "#bbbbbb";
static char selfgcolor[]         = "#eeeeee";
static char selbordercolor[]     = "#005577";
static char selbgcolor[]         = "#005577";
static char *colors[][3]         = {
	/*               fg           bg           border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "", "", "3", "", "5", "6", "7", "8", "" };

/* launcher commands (They must be NULL terminated) */
static const char* grabc[] = { "grabc", "|", "tr", "-d", "'\n'", "|", "xclip", "-selection", "clipboard", NULL };
static const char* bhev[] = { "firefox", "bhev.ru", NULL };
static const char* youtube[] = { "firefox", "youtube.com", NULL };

static const Launcher launchers[] = {
    /* command      name to display */
	{ grabc,        "" },
	{ bhev,         "" },
	{ youtube,      "" },
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class       instance    title       tags mask     isfloating   monitor */
	{ "Gimp",      NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",   NULL,       NULL,       1 << 8,       0,           -1 },
	{ "xwinwrap",  NULL,       NULL,       1 << 9,       0,           -1 },
	{ NULL,        NULL,       "broken",   1 << 9,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "###",      gaplessgrid },
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "HHH",      grid },
};

/* key definitions */
#define SUPERKEY Mod4Mask
#define ALTKEY Mod1Mask

#define TAGKEYS(KEY,TAG) \
	{ SUPERKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ SUPERKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ SUPERKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ SUPERKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */

static const char *screenshot[] = { "scrsht", NULL };
static const char *screenshotarea[] = { "scrsht", "-s", NULL };

static const char *volup[] = { "pactl", "set-sink-volume", "0", "+5%", NULL };
static const char *voldown[] = { "pactl", "set-sink-volume", "0", "-5%", NULL };
static const char *volmute[] = { "pactl", "set-sink-mute", "0", "toggle", NULL };

static const char *backlightup[] = { "s", "set-sink-mute", "0", "toggle", NULL };

static const char *dmenucmd[] = { "dmenu_run", "-g", "15", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key                       function        argument */
	{ 0,                            XK_Print,                 spawn,          {.v = screenshot } },
	{ ShiftMask,                    XK_Print,                 spawn,          {.v = screenshotarea } },
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn,          {.v = volup } },
	{ 0,                            XF86XK_AudioLowerVolume,  spawn,          {.v = voldown } },
	{ 0,                            XF86XK_AudioMute,         spawn,          {.v = volmute } },
	{ SUPERKEY,                     XK_p,                     spawn,          {.v = dmenucmd } },
	{ SUPERKEY,                     XK_Return,                spawn,          {.v = termcmd } },
	{ SUPERKEY,                     XK_b,                     togglebar,      {.i = 1} },
	{ SUPERKEY|ShiftMask,           XK_b,                     togglebar,      {.i = 2} },
	{ ALTKEY,                       XK_Tab,                   focusstack,     {.i = +1 } },
/*	{ SUPERKEY,                     XK_k,                     focusstack,     {.i = -1 } }, */
	{ SUPERKEY,                     XK_i,                     incnmaster,     {.i = +1 } },
	{ SUPERKEY,                     XK_d,                     incnmaster,     {.i = -1 } },
	{ SUPERKEY,                     XK_h,                     setmfact,       {.f = -0.05} },
	{ SUPERKEY,                     XK_l,                     setmfact,       {.f = +0.05} },
	{ SUPERKEY|ShiftMask,           XK_Return,                zoom,           {0} },
	{ SUPERKEY,                     XK_Tab,                   view,           {.ui =  0 } }, // next tab
	{ SUPERKEY|ShiftMask,           XK_Tab,                   view,           {.ui = -1 } }, // prev tab
	{ SUPERKEY|ControlMask,         XK_Tab,                   view,           {.ui = -2 } }, // last tab
	{ SUPERKEY,                     XK_q,                     killclient,     {0} },
	{ SUPERKEY,                     XK_t,                     setlayout,      {.v = &layouts[1]} },
	{ SUPERKEY,                     XK_f,                     setlayout,      {.v = &layouts[2]} },
	{ SUPERKEY,                     XK_m,                     setlayout,      {.v = &layouts[3]} },
	{ SUPERKEY|ShiftMask,           XK_g,			  setlayout,      {.v = &layouts[4]} },
	{ SUPERKEY,                     XK_g,			  setlayout,      {.v = &layouts[0]} },
/*	{ SUPERKEY,                     XK_space,                 setlayout,      {0} }, */
	{ SUPERKEY|ShiftMask,           XK_space,                 togglefloating, {0} },
	{ SUPERKEY,                     XK_0,                     view,           {.ui = ~0 } },
	{ SUPERKEY|ShiftMask,           XK_0,                     tag,            {.ui = ~0 } },
	{ SUPERKEY,                     XK_comma,                 focusmon,       {.i = -1 } },
	{ SUPERKEY,                     XK_period,                focusmon,       {.i = +1 } },
	{ SUPERKEY|ShiftMask,           XK_comma,                 tagmon,         {.i = -1 } },
	{ SUPERKEY|ShiftMask,           XK_period,                tagmon,         {.i = +1 } },	
	{ SUPERKEY,                     XK_minus,		  setgaps,        {.i = -1 } },
	{ SUPERKEY,                     XK_equal,		  setgaps,        {.i = +1 } },
	{ SUPERKEY|ShiftMask,           XK_equal, 		  setgaps,        {.i = 0  } },
	{ SUPERKEY|ShiftMask,           XK_t,			  togglealwaysontop, {0} },
	{ SUPERKEY|ShiftMask,           XK_f,			  togglefullscr,  {0} },
	{ SUPERKEY,                     XK_F5,			  xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                                     0)
	TAGKEYS(                        XK_2,                                     1)
	TAGKEYS(                        XK_3,                                     2)
	TAGKEYS(                        XK_4,                                     3)
	TAGKEYS(                        XK_5,                                     4)
	TAGKEYS(                        XK_6,                                     5)
	TAGKEYS(                        XK_7,                                     6)
	TAGKEYS(                        XK_8,                                     7)
	TAGKEYS(                        XK_9,                                     8)
	{ SUPERKEY|ShiftMask,           XK_q,                     quitprompt,     {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         SUPERKEY,       Button1,        movemouse,      {0} },
	{ ClkClientWin,         SUPERKEY,       Button2,        togglefloating, {0} },
	{ ClkClientWin,         SUPERKEY,       Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            SUPERKEY,       Button1,        tag,            {0} },
	{ ClkTagBar,            SUPERKEY,       Button3,        toggletag,      {0} },
};

