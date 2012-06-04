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


typedef void (*PurpleEventsSignedOnFunc)(PurplePlugin *plugin, PurpleBuddy *buddy);
typedef void (*PurpleEventsSignedOffFunc)(PurplePlugin *plugin, PurpleBuddy *buddy);

typedef void (*PurpleEventsAwayFunc)(PurplePlugin *plugin, PurpleBuddy *buddy, const gchar *message);
typedef void (*PurpleEventsBackFunc)(PurplePlugin *plugin, PurpleBuddy *buddy, const gchar *message);

typedef void (*PurpleEventsStatusFunc)(PurplePlugin *plugin, PurpleBuddy *buddy, const gchar *message);
typedef void (*PurpleEventsSpecialFunc)(PurplePlugin *plugin, PurpleBuddy *buddy, PurpleEventsEventSpecialType type, ...);

typedef void (*PurpleEventsIdleFunc)(PurplePlugin *plugin, PurpleBuddy *buddy);
typedef void (*PurpleEventsIdleBackFunc)(PurplePlugin *plugin, PurpleBuddy *buddy);

typedef void (*PurpleEventsImMessageFunc)(PurplePlugin *plugin, PurpleBuddy *buddy, const gchar *message);
typedef void (*PurpleEventsImActionFunc)(PurplePlugin *plugin, PurpleBuddy *buddy, const gchar *message);

typedef void (*PurpleEventsChatMessageFunc)(PurplePlugin *plugin, PurpleBuddy *buddy, const gchar *message);
typedef void (*PurpleEventsChatActionFunc)(PurplePlugin *plugin, PurpleBuddy *buddy, const gchar *message);


typedef struct _PurpleEventsContext PurpleEventsContext;
typedef struct {
	PurplePlugin *plugin;

	PurpleEventsSignedOnFunc signed_on;
	PurpleEventsSignedOffFunc signed_off;

	PurpleEventsAwayFunc away;
	PurpleEventsBackFunc back;

	PurpleEventsStatusFunc status;
	PurpleEventsSpecialFunc special;

	PurpleEventsIdleFunc idle;
	PurpleEventsIdleBackFunc idle_back;

	PurpleEventsImMessageFunc im_message;
	PurpleEventsImActionFunc im_action;

	PurpleEventsChatMessageFunc chat_message;
	PurpleEventsChatActionFunc chat_action;
} PurpleEventsHandler;


void purple_events_context_connect_handler(PurpleEventsContext *context, PurpleEventsHandler *handler);
void purple_events_context_disconnect_handler(PurpleEventsContext *context, PurpleEventsHandler *handler);

const gchar *purple_events_utils_buddy_get_best_name(PurpleBuddy *buddy);
const gchar *purple_events_utils_buddy_get_protocol(PurpleBuddy *buddy);
gchar *purple_events_utils_protocol_get_icon_uri(const gchar *protocol_name, PurpleEventsUtilsIconFormat format);

G_END_DECLS

#endif /* __PURPLE_EVENTS_H__ */
