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

#include <config.h>
#include <glib.h>
#include <purple.h>

#include <purple-events-context.h>
#include <purple-events-handler.h>

const gchar *
purple_events_get_plugin_id()
{
    return PURPLE_EVENTS_PLUGIN_ID;
}

void
purple_events_connect_handler(PurpleEventsHandler *handler)
{
    g_return_if_fail(handler != NULL);

    PurplePlugin *plugin;
    PurpleEventsContext *context;

    plugin = purple_plugins_find_with_id(PURPLE_EVENTS_PLUGIN_ID);
    g_return_if_fail(plugin != NULL);
    context = plugin->extra;

    context->handlers = g_list_prepend(context->handlers, handler);
}

void
purple_events_disconnect_handler(PurpleEventsHandler *handler)
{
    g_return_if_fail(handler != NULL);

    PurplePlugin *plugin;
    PurpleEventsContext *context;

    plugin = purple_plugins_find_with_id(PURPLE_EVENTS_PLUGIN_ID);
    g_return_if_fail(plugin != NULL);
    context = plugin->extra;

    context->handlers = g_list_remove(context->handlers, handler);
}
