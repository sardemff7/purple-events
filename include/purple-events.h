/*
 * purple-events - libpurple events handling plugin and library
 *
 * Copyright © 2011-2012 Quentin "Sardem FF7" Glidic
 *
 * This file is part of purple-events.
 *
 * purple-events is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * purple-events is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with purple-events. If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef __PURPLE_EVENTS_H__
#define __PURPLE_EVENTS_H__

G_BEGIN_DECLS


typedef enum
{
    PURPLE_EVENTS_EVENT_SPECIAL_TYPE_NONE,
    PURPLE_EVENTS_EVENT_SPECIAL_TYPE_TUNE,
} PurpleEventsEventSpecialType;

typedef enum
{
	PURPLE_EVENTS_UTILS_ICON_FORMAT_SVG,
	PURPLE_EVENTS_UTILS_ICON_FORMAT_PNG,
} PurpleEventsUtilsIconFormat;


typedef gpointer (*PurpleEventsSignedOnFunc)(PurplePlugin *plugin, gpointer event, PurpleBuddy *buddy);
typedef gpointer (*PurpleEventsSignedOffFunc)(PurplePlugin *plugin, gpointer event, PurpleBuddy *buddy);

typedef gpointer (*PurpleEventsAwayFunc)(PurplePlugin *plugin, gpointer event, PurpleBuddy *buddy, const gchar *message);
typedef gpointer (*PurpleEventsBackFunc)(PurplePlugin *plugin, gpointer event, PurpleBuddy *buddy, const gchar *message);

typedef gpointer (*PurpleEventsStatusFunc)(PurplePlugin *plugin, gpointer event, PurpleBuddy *buddy, const gchar *message);
typedef gpointer (*PurpleEventsSpecialFunc)(PurplePlugin *plugin, gpointer event, PurpleBuddy *buddy, PurpleEventsEventSpecialType type, ...);

typedef gpointer (*PurpleEventsIdleFunc)(PurplePlugin *plugin, gpointer event, PurpleBuddy *buddy);
typedef gpointer (*PurpleEventsIdleBackFunc)(PurplePlugin *plugin, gpointer event, PurpleBuddy *buddy);

typedef gpointer (*PurpleEventsImMessageFunc)(PurplePlugin *plugin, gpointer event, PurpleBuddy *buddy, const gchar *message);

typedef gpointer (*PurpleEventsChatMessageFunc)(PurplePlugin *plugin, gpointer event, PurpleConversation *conv, PurpleBuddy *buddy, const gchar *message);

typedef void (*PurpleEventsEmailFunc)(PurplePlugin *plugin, const gchar *subject, const gchar *from, const gchar *to, const gchar *url);

typedef void (*PurpleEventsEndEventFunc)(PurplePlugin *plugin, gpointer event);


typedef struct _PurpleEventsHandler PurpleEventsHandler;


const gchar *purple_events_get_plugin_id();
void purple_events_connect_handler(PurpleEventsHandler *handler);
void purple_events_disconnect_handler(PurpleEventsHandler *handler);


#define PURPLE_EVENTS_HANDLER_ADD_CALLBACK(event_name, EventName) void purple_events_handler_add_##event_name##_callback(PurpleEventsHandler *handler, PurpleEvents##EventName##Func callback)
PurpleEventsHandler *purple_events_handler_new(PurplePlugin *plugin);
void purple_events_handler_free(PurpleEventsHandler *handler);
void purple_events_handler_remove_event(PurpleEventsHandler *handler, gpointer attach, gpointer event);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(signed_on, SignedOn);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(signed_off, SignedOff);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(away, Away);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(back, Back);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(status, Status);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(special, Special);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(idle, Idle);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(idle_back, IdleBack);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(im_message, ImMessage);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(im_highlight, ImMessage);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(im_action, ImMessage);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(chat_message, ChatMessage);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(chat_highlight, ChatMessage);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(chat_action, ChatMessage);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(email, Email);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK(end_event, EndEvent);


const gchar *purple_events_utils_buddy_get_best_name(PurpleBuddy *buddy);
const gchar *purple_events_utils_buddy_get_protocol(PurpleBuddy *buddy);
gchar *purple_events_utils_protocol_get_icon_uri(const gchar *protocol_name, PurpleEventsUtilsIconFormat format);


G_END_DECLS

#endif /* __PURPLE_EVENTS_H__ */
