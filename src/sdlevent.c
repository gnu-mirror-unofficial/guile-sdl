/*******************************************************************
 *  sdlevent.c -- SDL input handling for Guile                     *
 *                                                                 *
 *  Copyright (C) 2001 Alex Shinn                                  *
 *                                                                 *
 *  This program is free software; you can redistribute it and/or  *
 * modify it under the terms of the GNU General Public License as  *
 * published by the Free Software Foundation; either version 2 of  *
 * the License, or (at your option) any later version.             *
 *                                                                 *
 * This program is distributed in the hope that it will be useful, *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of  *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the   *
 * GNU General Public License for more details.                    *
 *                                                                 *
 * You should have received a copy of the GNU General Public       *
 * License along with this program; if not, write to the Free      *
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,  *
 * MA 02111-1307 USA                                               *
 ******************************************************************/

/* event headers */
#include "sdlsmobs.h"
#include "sdlenums.h"
#include "sdlevent.h"

/* tags for SDL smobs */
long event_tag;
long keysym_tag;

/* enum/flag types */
SCM event_type_enum;
SCM event_state_enum;
SCM event_keysym_enum;
SCM event_mod_flags;

scm_sizet
free_event (SCM event)
{
  free ((SDL_Event*) SCM_SMOB_DATA (event));
  return sizeof (SDL_Event);
}

/* constructors */

SCM_DEFINE( make_event, "sdl-make-event", 0, 1, 0,
            (SCM s_type),
"Create a new SDL event.")
#define FUNC_NAME s_make_event
{
  SDL_Event *event;
  int type=SDL_NOEVENT;

  if (s_type != SCM_UNDEFINED) {
    type = scm_enum2long (s_type, event_type_enum, SCM_ARG1, "sdl-make-event");
  }

  event = (SDL_Event *) scm_must_malloc (sizeof (SDL_Event), "sdl-make-event");
  event->type = type;

  SCM_RETURN_NEWSMOB (event_tag, event);
}
#undef FUNC_NAME


SCM make_keysym (SCM sym, SCM mod)
{
  SDL_keysym *keysym;

  /* alloc the keysym */
  keysym = (SDL_keysym *) scm_must_malloc (sizeof (SDL_keysym),
                                           "sdl-make-keysym");

  /* set the sym if given */
  if (sym != SCM_UNDEFINED) {
    /* SCM_ASSERT (scm_exact_p (sym), sym, SCM_ARG1, "sdl-make-keysym"); */
    /* keysym->sym = (SDLKey) scm_num2long (sym, SCM_ARG1, "scm_num2long"); */
    keysym->sym = (SDLKey) scm_enum2long (sym, event_keysym_enum,
                                          SCM_ARG1, "sdl-make-keysym");
  }

  /* set the mod if given */
  if (mod != SCM_UNDEFINED) {
    SCM_ASSERT (scm_exact_p (mod), mod, SCM_ARG2, "sdl-make-keysym");
    keysym->mod = (SDLMod) scm_flags2ulong (mod, event_mod_flags,
                                            SCM_ARG2, "sdl-make-keysym");
  }

  /* return the new smob */
  SCM_RETURN_NEWSMOB (keysym_tag, keysym);
}

/* smob getters */

SCM_DEFINE_ENUM_GETTER ("sdl-event:type", event_type, event_tag,
                        SDL_Event*, type, event_type_enum)

SCM_DEFINE_NUMBER_GETTER ("sdl-event:active:gain", event_active_gain, event_tag,
                        SDL_Event*, active.gain)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:active:state", event_active_state, event_tag,
                        SDL_Event*, active.state)

SCM_DEFINE_NUMBER_GETTER ("sdl-event:key:state", event_key_state, event_tag,
                        SDL_Event*, key.state)

SCM_DEFINE_ENUM_GETTER ("sdl-event:key:keysym:sym", event_key_keysym_sym,
                        event_tag, SDL_Event*, key.keysym.sym, event_keysym_enum)

SCM_DEFINE_FLAG_GETTER ("sdl-event:key:keysym:mod", event_key_keysym_mod,
                        event_tag, SDL_Event*, key.keysym.mod, event_mod_flags)

SCM_DEFINE_NUMBER_GETTER ("sdl-event:key:keysym:scancode", event_key_keysym_scancode,
                        event_tag, SDL_Event*, key.keysym.scancode)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:key:keysym:unicode", event_key_keysym_unicode,
                        event_tag, SDL_Event*, key.keysym.unicode)

SCM_DEFINE_NUMBER_GETTER ("sdl-event:motion:state", event_motion_state, event_tag,
                        SDL_Event*, motion.state)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:motion:x", event_motion_x, event_tag,
                        SDL_Event*, motion.x)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:motion:y", event_motion_y, event_tag,
                        SDL_Event*, motion.y)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:motion:xrel", event_motion_xrel, event_tag,
                        SDL_Event*, motion.xrel)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:motion:yrel", event_motion_yrel, event_tag,
                        SDL_Event*, motion.yrel)

SCM_DEFINE_NUMBER_GETTER ("sdl-event:button:button", event_button_button, event_tag,
                        SDL_Event*, button.button)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:button:state", event_button_state, event_tag,
                        SDL_Event*, button.state)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:button:x", event_button_x, event_tag,
                        SDL_Event*, button.x)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:button:y", event_button_y, event_tag,
                        SDL_Event*, button.y)

SCM_DEFINE_NUMBER_GETTER ("sdl-event:jaxis:which", event_jaxis_which, event_tag,
                        SDL_Event*, jaxis.which)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:jaxis:axis", event_jaxis_axis, event_tag,
                        SDL_Event*, jaxis.axis)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:jaxis:value", event_jaxis_value, event_tag,
                        SDL_Event*, jaxis.value)

SCM_DEFINE_NUMBER_GETTER ("sdl-event:jbutton:which", event_jbutton_which, event_tag,
                        SDL_Event*, jbutton.which)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:jbutton:button", event_jbutton_button, event_tag,
                        SDL_Event*, jbutton.button)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:jbutton:state", event_jbutton_state, event_tag,
                        SDL_Event*, jbutton.state)

SCM_DEFINE_NUMBER_GETTER ("sdl-event:jball:which", event_jball_which, event_tag,
                        SDL_Event*, jball.which)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:jball:ball", event_jball_ball, event_tag,
                        SDL_Event*, jball.ball)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:jball:xrel", event_jball_xrel, event_tag,
                        SDL_Event*, jball.xrel)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:jball:yrel", event_jball_yrel, event_tag,
                        SDL_Event*, jball.yrel)

SCM_DEFINE_NUMBER_GETTER ("sdl-event:jhat:which", event_jhat_which, event_tag,
                        SDL_Event*, jhat.which)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:jhat:hat", event_jhat_hat, event_tag,
                        SDL_Event*, jhat.hat)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:jhat:value", event_jhat_state, event_tag,
                        SDL_Event*, jhat.value)

SCM_DEFINE_NUMBER_GETTER ("sdl-event:resize:w", event_resize_w, event_tag,
                        SDL_Event*, resize.w)
SCM_DEFINE_NUMBER_GETTER ("sdl-event:resize:h", event_resize_h, event_tag,
                        SDL_Event*, resize.h)

/* SCM_DEFINE_NUMBER_GETTER ("sdl-event:user:code", event_user_code, event_tag, */
/*                         SDL_Event*, user.code) */
/* SCM_DEFINE_NUMBER_GETTER ("event:user:data1", event_user_data1, event_tag,  */
/*                         SDL_Event*, user.data1)  */
/* SCM_DEFINE_NUMBER_GETTER ("event:user:data2", event_user_data2, event_tag,  */
/*                         SDL_Event*, user.data2)  */

/* SCM_DEFINE_NUMBER_GETTER ("keysym:scancode", keysym_scancode, keysym_tag,  */
/*                         SDL_keysym*, scancode)  */
/* SCM_DEFINE_NUMBER_GETTER ("keysym:sym", keysym_sym, keysym_tag,  */
/*                         SDL_keysym*, sym)  */
/* SCM_DEFINE_NUMBER_GETTER ("keysym:mod", keysym_mod, keysym_tag,  */
/*                         SDL_keysym*, mod)  */
/* SCM_DEFINE_NUMBER_GETTER ("keysym:unicode", keysym_unicode, keysym_tag,  */
/*                         SDL_keysym*, unicode)  */

/* SCM event_key_keysym (SCM s_event) */
/* { */
/* } */


/* SDL event functions */

/* extern DECLSPEC void SDL_PumpEvents(void); */
SCM pump_events (void)
{
  SDL_PumpEvents();
  return SCM_UNSPECIFIED;
}

/* extern DECLSPEC int SDL_PeepEvents(SDL_Event *events, int numevents, */
/* 				SDL_eventaction action, Uint32 mask); */
SCM peep_events (SCM events, SCM numevents, SCM action, SCM mask)
{
  return SCM_UNSPECIFIED;
}

/* extern DECLSPEC int SDL_PollEvent(SDL_Event *event); */
SCM poll_event (SCM event)
{
  int result;

  if (event == SCM_UNDEFINED) {
    /* no args */
    result = SDL_PollEvent (NULL);
  } else {
    /* we're given an event smob - fill it */
    SCM_ASSERT_SMOB (event, event_tag, SCM_ARG1, "sdl-poll-event");
    result = SDL_PollEvent ((SDL_Event*) SCM_CDR (event));
  }

  return result ? SCM_BOOL_T : SCM_BOOL_F;
}

/* extern DECLSPEC int SDL_WaitEvent(SDL_Event *event); */
SCM wait_event (SCM event)
{
  int result;

  if (event == SCM_UNDEFINED) {
    /* no args */
    result = SDL_WaitEvent (NULL);
  } else {
    /* we're given an event smob - fill it */
    SCM_ASSERT_SMOB (event, event_tag, SCM_ARG1, "sdl-wait-event");
    result = SDL_WaitEvent ((SDL_Event*) SCM_CDR (event));
  }

  return result ? SCM_BOOL_T : SCM_BOOL_F;
}

/* extern DECLSPEC int SDL_PushEvent(SDL_Event *event); */
SCM push_event (SCM event)
{
  int result;

  SCM_ASSERT_SMOB (event, event_tag, SCM_ARG1, "sdl-push-event");

  result = SDL_PushEvent ((SDL_Event*) SCM_CDR (event));
  return scm_long2num (result);
}

/* extern DECLSPEC void SDL_SetEventFilter(SDL_EventFilter filter); */
SCM set_event_filter (SCM filter)
{
  return SCM_UNSPECIFIED;
}

/* extern DECLSPEC SDL_EventFilter SDL_GetEventFilter(void); */
SCM get_event_filter (SCM filter)
{
  return SCM_UNSPECIFIED;
}

/* extern DECLSPEC Uint8 SDL_EventState(Uint8 type, int state); */
SCM event_state (SCM type, SCM state)
{
  return SCM_UNSPECIFIED;
}


SCM
sdl_enable_unicode (SCM enable_p)
{
  int result;

  if (enable_p == SCM_UNDEFINED) {
    result = SDL_EnableUNICODE (-1);
  } else if (SCM_FALSEP (enable_p)) {
    result = SDL_EnableUNICODE (0);
  } else {
    result = SDL_EnableUNICODE (1);
  }

  return scm_long2num (result);
}

/*
 * If 'delay' is set to 0, keyboard repeat is disabled.
 */
SCM
sdl_enable_key_repeat (SCM s_delay, SCM s_interval)
{
  int interval, delay;

  SCM_ASSERT (scm_exact_p (s_delay), s_delay, SCM_ARG1, "sdl-enable-key-repeat");
  SCM_ASSERT (scm_exact_p (s_interval), s_interval, SCM_ARG2, "sdl-enable-key-repeat");

  delay    = scm_num2long (s_delay, SCM_ARG1, "scm_num2long");
  interval = scm_num2long (s_interval, SCM_ARG1, "scm_num2long");

  SCM_RETURN_TRUE_IF_0 (SDL_EnableKeyRepeat (delay, interval));
}

/*
 * Get a snapshot of the current state of the keyboard.
 * Returns an array of keystates, indexed by the SDLK_* syms.
 */
SCM
sdl_get_key_state (SCM numkeys)
{
  return SCM_UNSPECIFIED;
}

/*
 * Get the current key modifier state
 */
SCM
sdl_get_mod_state (void)
{
  return scm_ulong2flags (SDL_GetModState (), event_mod_flags);
}

/*
 * Set the current key modifier state
 * This does not change the keyboard state, only the key modifier flags.
 */
SCM
sdl_set_mod_state (SCM modstate)
{
  /* SCM_ASSERT (scm_exact_p (modstate), modstate, SCM_ARG1, "sdl-set-mod-state"); */
  SDL_SetModState (scm_flags2ulong (modstate, event_mod_flags, SCM_ARG1,
                                    "sdl-set-mod-state"));
  return SCM_UNSPECIFIED;
}

/*
 * Retrieve the current state of the mouse.
 */
SCM
sdl_get_mouse_state (void)
{
  int buttons, x, y;
  buttons = SDL_GetMouseState (&x, &y);
  return SCM_LIST3 (scm_cons (scm_str2symbol ("state"),
                              scm_long2num (buttons)),
                    scm_cons (scm_str2symbol ("x"),
                              scm_long2num (x)),
                    scm_cons (scm_str2symbol ("y"),
                              scm_long2num (y)));
}

/*
 * Retrieve the current state of the mouse.
 */
SCM
sdl_get_relative_mouse_state ()
{
  int buttons, x, y;
  buttons = SDL_GetRelativeMouseState (&x, &y);
  return SCM_LIST3 (scm_cons (scm_str2symbol ("state"),
                              scm_long2num (buttons)),
                    scm_cons (scm_str2symbol ("x"),
                              scm_long2num (x)),
                    scm_cons (scm_str2symbol ("y"),
                              scm_long2num (y)));
}

SCM
sdl_button_p (SCM mask)
{
  SCM_ASSERT (scm_exact_p (mask), mask, SCM_ARG1, "sdl-button?");
  return SDL_BUTTON (scm_num2long (mask, SCM_ARG1, "sdl-button?"))
    ? SCM_BOOL_T : SCM_BOOL_F;
}

/* Initialize glue */
void sdl_init_event (void)
{
  /* tags */
  event_tag   = scm_make_smob_type ("SDL-Event",  sizeof (SDL_Event));
  /* keysym_tag  = scm_make_smob_type ("keysym", sizeof (SDL_keysym)); */

  scm_set_smob_free (event_tag, free_event);

  /* event type constants */
  event_type_enum = scm_c_define_enum (
    "sdl-event-types",
    "SDL_ACTIVEEVENT",      SDL_ACTIVEEVENT,
    "SDL_KEYDOWN",          SDL_KEYDOWN,
    "SDL_KEYUP",            SDL_KEYUP,
    "SDL_MOUSEMOTION",      SDL_MOUSEMOTION,
    "SDL_MOUSEBUTTONDOWN",  SDL_MOUSEBUTTONDOWN,
    "SDL_MOUSEBUTTONUP",    SDL_MOUSEBUTTONUP,
    "SDL_JOYAXISMOTION",    SDL_JOYAXISMOTION,
    "SDL_JOYBALLMOTION",    SDL_JOYBALLMOTION,
    "SDL_JOYHATMOTION",     SDL_JOYHATMOTION,
    "SDL_JOYBUTTONDOWN",    SDL_JOYBUTTONDOWN,
    "SDL_JOYBUTTONUP",      SDL_JOYBUTTONUP,
    "SDL_QUIT",             SDL_QUIT,
    "SDL_SYSWMEVENT",       SDL_SYSWMEVENT,
    "SDL_VIDEORESIZE",      SDL_VIDEORESIZE,
    "SDL_USEREVENT",        SDL_USEREVENT,
    NULL);

  /* keysyms */
  event_keysym_enum = scm_c_define_enum (
    "sdl-event-keys",
    "SDLK_BACKSPACE",  SDLK_BACKSPACE,
    "SDLK_TAB",  SDLK_TAB,
    "SDLK_CLEAR",  SDLK_CLEAR,
    "SDLK_RETURN",  SDLK_RETURN,
    "SDLK_PAUSE",  SDLK_PAUSE,
    "SDLK_ESCAPE",  SDLK_ESCAPE,
    "SDLK_SPACE",  SDLK_SPACE,
    "SDLK_EXCLAIM",  SDLK_EXCLAIM,
    "SDLK_QUOTEDBL",  SDLK_QUOTEDBL,
    "SDLK_HASH",  SDLK_HASH,
    "SDLK_DOLLAR",  SDLK_DOLLAR,
    "SDLK_AMPERSAND",  SDLK_AMPERSAND,
    "SDLK_QUOTE",  SDLK_QUOTE,
    "SDLK_LEFTPAREN",  SDLK_LEFTPAREN,
    "SDLK_RIGHTPAREN",  SDLK_RIGHTPAREN,
    "SDLK_ASTERISK",  SDLK_ASTERISK,
    "SDLK_PLUS",  SDLK_PLUS,
    "SDLK_COMMA",  SDLK_COMMA,
    "SDLK_MINUS",  SDLK_MINUS,
    "SDLK_PERIOD",  SDLK_PERIOD,
    "SDLK_SLASH",  SDLK_SLASH,
    "SDLK_0",  SDLK_0,
    "SDLK_1",  SDLK_1,
    "SDLK_2",  SDLK_2,
    "SDLK_3",  SDLK_3,
    "SDLK_4",  SDLK_4,
    "SDLK_5",  SDLK_5,
    "SDLK_6",  SDLK_6,
    "SDLK_7",  SDLK_7,
    "SDLK_8",  SDLK_8,
    "SDLK_9",  SDLK_9,
    "SDLK_COLON",  SDLK_COLON,
    "SDLK_SEMICOLON",  SDLK_SEMICOLON,
    "SDLK_LESS",  SDLK_LESS,
    "SDLK_EQUALS",  SDLK_EQUALS,
    "SDLK_GREATER",  SDLK_GREATER,
    "SDLK_QUESTION",  SDLK_QUESTION,
    "SDLK_AT",  SDLK_AT,
    "SDLK_LEFTBRACKET",  SDLK_LEFTBRACKET,
    "SDLK_BACKSLASH",  SDLK_BACKSLASH,
    "SDLK_RIGHTBRACKET",  SDLK_RIGHTBRACKET,
    "SDLK_CARET",  SDLK_CARET,
    "SDLK_UNDERSCORE",  SDLK_UNDERSCORE,
    "SDLK_BACKQUOTE",  SDLK_BACKQUOTE,
    "SDLK_a",  SDLK_a,
    "SDLK_b",  SDLK_b,
    "SDLK_c",  SDLK_c,
    "SDLK_d",  SDLK_d,
    "SDLK_e",  SDLK_e,
    "SDLK_f",  SDLK_f,
    "SDLK_g",  SDLK_g,
    "SDLK_h",  SDLK_h,
    "SDLK_i",  SDLK_i,
    "SDLK_j",  SDLK_j,
    "SDLK_k",  SDLK_k,
    "SDLK_l",  SDLK_l,
    "SDLK_m",  SDLK_m,
    "SDLK_n",  SDLK_n,
    "SDLK_o",  SDLK_o,
    "SDLK_p",  SDLK_p,
    "SDLK_q",  SDLK_q,
    "SDLK_r",  SDLK_r,
    "SDLK_s",  SDLK_s,
    "SDLK_t",  SDLK_t,
    "SDLK_u",  SDLK_u,
    "SDLK_v",  SDLK_v,
    "SDLK_w",  SDLK_w,
    "SDLK_x",  SDLK_x,
    "SDLK_y",  SDLK_y,
    "SDLK_z",  SDLK_z,
    "SDLK_DELETE",  SDLK_DELETE,
    "SDLK_KP0",  SDLK_KP0,
    "SDLK_KP1",  SDLK_KP1,
    "SDLK_KP2",  SDLK_KP2,
    "SDLK_KP3",  SDLK_KP3,
    "SDLK_KP4",  SDLK_KP4,
    "SDLK_KP5",  SDLK_KP5,
    "SDLK_KP6",  SDLK_KP6,
    "SDLK_KP7",  SDLK_KP7,
    "SDLK_KP8",  SDLK_KP8,
    "SDLK_KP9",  SDLK_KP9,
    "SDLK_KP_PERIOD",  SDLK_KP_PERIOD,
    "SDLK_KP_DIVIDE",  SDLK_KP_DIVIDE,
    "SDLK_KP_MULTIPLY",  SDLK_KP_MULTIPLY,
    "SDLK_KP_MINUS",  SDLK_KP_MINUS,
    "SDLK_KP_PLUS",  SDLK_KP_PLUS,
    "SDLK_KP_ENTER",  SDLK_KP_ENTER,
    "SDLK_KP_EQUALS",  SDLK_KP_EQUALS,
    "SDLK_UP",  SDLK_UP,
    "SDLK_DOWN",  SDLK_DOWN,
    "SDLK_RIGHT",  SDLK_RIGHT,
    "SDLK_LEFT",  SDLK_LEFT,
    "SDLK_INSERT",  SDLK_INSERT,
    "SDLK_HOME",  SDLK_HOME,
    "SDLK_END",  SDLK_END,
    "SDLK_PAGEUP",  SDLK_PAGEUP,
    "SDLK_PAGEDOWN",  SDLK_PAGEDOWN,
    "SDLK_F1",  SDLK_F1,
    "SDLK_F2",  SDLK_F2,
    "SDLK_F3",  SDLK_F3,
    "SDLK_F4",  SDLK_F4,
    "SDLK_F5",  SDLK_F5,
    "SDLK_F6",  SDLK_F6,
    "SDLK_F7",  SDLK_F7,
    "SDLK_F8",  SDLK_F8,
    "SDLK_F9",  SDLK_F9,
    "SDLK_F10",  SDLK_F10,
    "SDLK_F11",  SDLK_F11,
    "SDLK_F12",  SDLK_F12,
    "SDLK_F13",  SDLK_F13,
    "SDLK_F14",  SDLK_F14,
    "SDLK_F15",  SDLK_F15,
    "SDLK_NUMLOCK",  SDLK_NUMLOCK,
    "SDLK_CAPSLOCK",  SDLK_CAPSLOCK,
    "SDLK_SCROLLOCK",  SDLK_SCROLLOCK,
    "SDLK_RSHIFT",  SDLK_RSHIFT,
    "SDLK_LSHIFT",  SDLK_LSHIFT,
    "SDLK_RCTRL",  SDLK_RCTRL,
    "SDLK_LCTRL",  SDLK_LCTRL,
    "SDLK_RALT",  SDLK_RALT,
    "SDLK_LALT",  SDLK_LALT,
    "SDLK_RMETA",  SDLK_RMETA,
    "SDLK_LMETA",  SDLK_LMETA,
    "SDLK_LSUPER",  SDLK_LSUPER,
    "SDLK_RSUPER",  SDLK_RSUPER,
    "SDLK_MODE",  SDLK_MODE,
    "SDLK_HELP",  SDLK_HELP,
    "SDLK_PRINT",  SDLK_PRINT,
    "SDLK_SYSREQ",  SDLK_SYSREQ,
    "SDLK_BREAK",  SDLK_BREAK,
    "SDLK_MENU",  SDLK_MENU,
    "SDLK_POWER",  SDLK_POWER,
    "SDLK_EURO",  SDLK_EURO,
    NULL);

  event_mod_flags = scm_c_define_flag (
    "sdl-mod-flags",
    "KMOD_NONE",      KMOD_NONE,
    "KMOD_LSHIFT",    KMOD_LSHIFT,
    "KMOD_RSHIFT",    KMOD_RSHIFT,
    "KMOD_LCTRL",     KMOD_LCTRL,
    "KMOD_RCTRL",     KMOD_RCTRL,
    "KMOD_LALT",      KMOD_LALT,
    "KMOD_RALT",      KMOD_RALT,
    "KMOD_LMETA",     KMOD_LMETA,
    "KMOD_RMETA",     KMOD_RMETA,
    "KMOD_NUM",       KMOD_NUM,
    "KMOD_CAPS",      KMOD_CAPS,
    "KMOD_MODE",      KMOD_MODE,
    "KMOD_RESERVED",  KMOD_RESERVED,
    "KMOD_CTRL",      KMOD_CTRL,
    "KMOD_SHIFT",     KMOD_SHIFT,
    "KMOD_ALT",       KMOD_ALT,
    "KMOD_META",      KMOD_META,
    NULL);

  /* event states */
  event_state_enum = scm_c_define_enum (
    "sdl-event-states",
    "SDL_QUERY",  SDL_QUERY,
    "SDL_IGNORE",  SDL_IGNORE,
    "SDL_DISABLE",  SDL_DISABLE,
    "SDL_ENABLE",  SDL_ENABLE,
    NULL);

  /* functions */
  scm_c_define_gsubr ("sdl-make-event",      0, 1, 0, make_event);
  scm_c_define_gsubr ("sdl-make-keysym",     0, 2, 0, make_keysym);
  scm_c_define_gsubr ("sdl-poll-event",      0, 1, 0, poll_event);
  scm_c_define_gsubr ("sdl-wait-event",      0, 1, 0, wait_event);
  scm_c_define_gsubr ("sdl-push-event",      1, 0, 0, push_event);
  scm_c_define_gsubr ("sdl-enable-unicode",  0, 1, 0, sdl_enable_unicode);
  scm_c_define_gsubr ("sdl-enable-key-repeat",  2, 0, 0, sdl_enable_key_repeat);
  scm_c_define_gsubr ("sdl-get-key-state",   0, 0, 0, sdl_get_key_state);
  scm_c_define_gsubr ("sdl-get-mod-state",   0, 0, 0, sdl_get_mod_state);
  scm_c_define_gsubr ("sdl-set-mod-state",   1, 0, 0, sdl_set_mod_state);
  scm_c_define_gsubr ("sdl-get-mouse-state", 0, 0, 0, sdl_get_mouse_state);
  scm_c_define_gsubr ("sdl-get-relative-mouse-state", 0, 0, 0,
                      sdl_get_relative_mouse_state);
  scm_c_define_gsubr ("sdl-button?",         1, 0, 0, sdl_button_p);

  /* smob getters */
  scm_c_define_gsubr ("sdl-event:type", 1, 0, 0, event_type);
  scm_c_define_gsubr ("sdl-event:active:gain", 1, 0, 0, event_active_gain);
  scm_c_define_gsubr ("sdl-event:active:state", 1, 0, 0, event_active_state);
  scm_c_define_gsubr ("sdl-event:key:state", 1, 0, 0, event_key_state);
  scm_c_define_gsubr ("sdl-event:key:keysym:sym", 1, 0, 0, event_key_keysym_sym);
  scm_c_define_gsubr ("sdl-event:key:keysym:mod", 1, 0, 0, event_key_keysym_mod);
  scm_c_define_gsubr ("sdl-event:key:keysym:scancode", 1, 0, 0, event_key_keysym_scancode);
  scm_c_define_gsubr ("sdl-event:key:keysym:unicode", 1, 0, 0, event_key_keysym_unicode);
  scm_c_define_gsubr ("sdl-event:motion:state", 1, 0, 0, event_motion_state);
  scm_c_define_gsubr ("sdl-event:motion:x", 1, 0, 0, event_motion_x);
  scm_c_define_gsubr ("sdl-event:motion:y", 1, 0, 0, event_motion_y);
  scm_c_define_gsubr ("sdl-event:motion:xrel", 1, 0, 0, event_motion_xrel);
  scm_c_define_gsubr ("sdl-event:motion:yrel", 1, 0, 0, event_motion_yrel);
  scm_c_define_gsubr ("sdl-event:button:button", 1, 0, 0, event_button_button);
  scm_c_define_gsubr ("sdl-event:button:state", 1, 0, 0, event_button_state);
  scm_c_define_gsubr ("sdl-event:button:x", 1, 0, 0, event_button_x);
  scm_c_define_gsubr ("sdl-event:button:y", 1, 0, 0, event_button_y);
/*   scm_c_define_gsubr ("sdl-event:jaxis:which", 1, 0, 0, event_jaxis_which); */
/*   scm_c_define_gsubr ("sdl-event:jaxis:axis", 1, 0, 0, event_jaxis_axis); */
/*   scm_c_define_gsubr ("sdl-event:jaxis:value", 1, 0, 0, event_jaxis_value); */
/*   scm_c_define_gsubr ("sdl-event:jbutton:which", 1, 0, 0, event_jbutton_which); */
/*   scm_c_define_gsubr ("sdl-event:jbutton:button", 1, 0, 0, event_jbutton_button); */
/*   scm_c_define_gsubr ("sdl-event:jbutton:state", 1, 0, 0, event_jbutton_state); */
/*   scm_c_define_gsubr ("sdl-event:jball:which", 1, 0, 0, event_jball_which); */
/*   scm_c_define_gsubr ("sdl-event:jball:ball", 1, 0, 0, event_jball_ball); */
/*   scm_c_define_gsubr ("sdl-event:jball:xrel", 1, 0, 0, event_jball_xrel); */
/*   scm_c_define_gsubr ("sdl-event:jball:yrel", 1, 0, 0, event_jball_yrel); */
/*   scm_c_define_gsubr ("sdl-event:jhat:which", 1, 0, 0, event_jhat_which); */
/*   scm_c_define_gsubr ("sdl-event:jhat:hat", 1, 0, 0, event_jhat_hat); */
/*   scm_c_define_gsubr ("sdl-event:jhat:value", 1, 0, 0, event_jhat_value); */
/*   scm_c_define_gsubr ("sdl-event:resize:w", 1, 0, 0, event_resize_w); */
/*   scm_c_define_gsubr ("sdl-event:resize:h", 1, 0, 0, event_resize_h); */
/*    scm_c_define_gsubr ("sdl-event:user:code", 1, 0, 0, event_user_code); */
/* /\*    scm_c_define_gsubr ("sdl-event:user:data1", 1, 0, 0, event_user_data1); *\/ */
/* /\*    scm_c_define_gsubr ("sdl-event:user:data2", 1, 0, 0, event_user_data2); *\/ */
/*    scm_c_define_gsubr ("sdl-keysym:scancode", 1, 0, 0, keysym_scancode); */
/*    scm_c_define_gsubr ("sdl-keysym:sym", 1, 0, 0, keysym_sym); */
/*    scm_c_define_gsubr ("sdl-keysym:mod", 1, 0, 0, keysym_mod); */
/*    scm_c_define_gsubr ("sdl-keysym:unicode", 1, 0, 0, keysym_unicode); */

  /* exported symbols */
  scm_c_export (
    /* enums */
    "sdl-event-types", "sdl-event-keys", "sdl-event-states",
    "sdl-mod-flags",
    /* smob getters */
    "sdl-event:type",                "sdl-event:active:gain",
    "sdl-event:active:state",        "sdl-event:key:state",
    "sdl-event:key:keysym:sym",      "sdl-event:key:keysym:mod",
    "sdl-event:key:keysym:scancode", "sdl-event:key:keysym:unicode",
    "sdl-event:motion:state",        "sdl-event:motion:x",
    "sdl-event:motion:y",            "sdl-event:motion:xrel",
    "sdl-event:motion:yrel",         "sdl-event:button:button",
    "sdl-event:button:state",        "sdl-event:button:x",
    "sdl-event:button:y",            "sdl-event:jaxis:which",
    "sdl-event:jaxis:axis",          "sdl-event:jaxis:value",
    "sdl-event:jbutton:which",       "sdl-event:jbutton:button",
    "sdl-event:jbutton:state",       "sdl-event:jball:which",
    "sdl-event:jball:ball",          "sdl-event:jball:xrel",
    "sdl-event:jball:yrel",          "sdl-event:jhat:which",
    "sdl-event:jhat:hat",            "sdl-event:jhat:value",
    "sdl-event:resize:w",            "sdl-event:resize:h",
/*       "sdl-event:user:code",           "sdl-event:user:data1", */
/*       "sdl-event:user:data2",          "sdl-keysym:scancode", */
/*       "sdl-keysym:sym",                "sdl-keysym:mod", */
/*       "sdl-keysym:unicode", */
    /* event functions */
    "sdl-make-event",  "sdl-make-keysym",  "sdl-poll-event",  "sdl-wait-event",
    "sdl-push-event",
    /* keyboard functions */
    "sdl-enable-unicode",    "sdl-enable-key-repeat",     "sdl-get-key-state",
    "sdl-get-mod-state",     "sdl-set-mod-state",
    /* mouse functions */
    "sdl-get-mouse-state",   "sdl-get-relative-mouse-state", "sdl-button?",
    NULL);
}

