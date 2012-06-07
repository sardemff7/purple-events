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

#include <glib.h>
#include <purple.h>

#include <purple-events-handler.h>

PurpleEventsHandler *
purple_events_handler_new(PurplePlugin *plugin)
{
    g_return_val_if_fail(plugin != NULL, NULL);

    PurpleEventsHandler *handler;

    handler = g_new0(PurpleEventsHandler, 1);

    handler->plugin = plugin;

    return handler;
}

void
purple_events_handler_free(PurpleEventsHandler *handler)
{
    g_return_if_fail(handler != NULL);

    g_free(handler);
}

#define PURPLE_EVENTS_HANDLER_ADD_CALLBACK_DEF(event_name, EventName) PURPLE_EVENTS_HANDLER_ADD_CALLBACK(event_name, EventName) { handler->event_name = callback; }

PURPLE_EVENTS_HANDLER_ADD_CALLBACK_DEF(signed_on, SignedOn);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK_DEF(signed_off, SignedOff);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK_DEF(away, Away);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK_DEF(back, Back);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK_DEF(status, Status);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK_DEF(special, Special);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK_DEF(idle, Idle);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK_DEF(idle_back, IdleBack);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK_DEF(im_message, ImMessage);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK_DEF(im_action, ImAction);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK_DEF(chat_message, ChatMessage);
PURPLE_EVENTS_HANDLER_ADD_CALLBACK_DEF(chat_action, ChatAction);
