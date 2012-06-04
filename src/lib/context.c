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

#include <purple-events-context.h>

void
purple_events_context_connect_handler(PurpleEventsContext *context, PurpleEventsHandler *handler)
{
    context->handlers = g_list_prepend(context->handlers, handler);
}

void
purple_events_context_disconnect_handler(PurpleEventsContext *context, PurpleEventsHandler *handler)
{
    context->handlers = g_list_remove(context->handlers, handler);
}
