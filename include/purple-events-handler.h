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

#ifndef __PURPLE_EVENTS_HANDLER_H__
#define __PURPLE_EVENTS_HANDLER_H__

#include <purple-events.h>

G_BEGIN_DECLS


struct _PurpleEventsHandler {
	PurplePlugin *plugin;
	GHashTable *events;

	PurpleEventsSignedOnFunc signed_on;
	PurpleEventsSignedOffFunc signed_off;

	PurpleEventsAwayFunc away;
	PurpleEventsBackFunc back;

	PurpleEventsStatusFunc status;
	PurpleEventsSpecialFunc special;

	PurpleEventsIdleFunc idle;
	PurpleEventsIdleBackFunc idle_back;

	PurpleEventsImMessageFunc im_message;

	PurpleEventsChatMessageFunc chat_message;

	PurpleEventsEmailFunc email;

	PurpleEventsConversationGotFocusFunc conversation_got_focus;
};


G_END_DECLS

#endif /* __PURPLE_EVENTS_HANDLER_H__ */
