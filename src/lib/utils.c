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

#include <purple-events.h>

const gchar *
purple_events_utils_buddy_get_best_name(PurpleBuddy *buddy)
{
    g_return_val_if_fail(buddy != NULL, NULL);

    const gchar *name;

    name = purple_buddy_get_contact_alias(buddy);
    if ( name != NULL )
        return name;

    name = purple_buddy_get_alias(buddy);
    if ( name != NULL )
        return name;

    name = purple_buddy_get_server_alias(buddy);
    if ( name != NULL )
        return name;

    return purple_buddy_get_name(buddy);
}


const gchar *
purple_events_utils_buddy_get_protocol(PurpleBuddy *buddy)
{
    g_return_val_if_fail(buddy != NULL, NULL);

    PurplePluginProtocolInfo *info;
    const gchar *protocol_name = NULL;

    info = PURPLE_PLUGIN_PROTOCOL_INFO(purple_find_prpl(purple_account_get_protocol_id(buddy->account)));
    if ( info->list_icon != NULL )
        protocol_name = info->list_icon(buddy->account, NULL);

    return protocol_name;
}

gchar *
purple_events_utils_protocol_get_icon_uri(const gchar *protocol_name, PurpleEventsUtilsIconFormat format)
{
    g_return_val_if_fail(protocol_name != NULL, NULL);

    gchar *tmp;
    gchar *protocol_icon_uri = NULL;

    switch ( format )
    {
    case PURPLE_EVENTS_UTILS_ICON_FORMAT_SVG:
        tmp = g_strconcat(protocol_name, ".svg", NULL);
        protocol_icon_uri = g_build_filename("file://" PURPLE_DATADIR, "pixmaps", "pidgin", "protocols", "scalable", tmp, NULL);
        g_free(tmp);
    break;
    case PURPLE_EVENTS_UTILS_ICON_FORMAT_PNG:
        tmp = g_strconcat(protocol_name, ".png", NULL);
        protocol_icon_uri = g_build_filename("file://" PURPLE_DATADIR, "pixmaps", "pidgin", "protocols", "48", tmp, NULL);
        g_free(tmp);
    break;
    }

    return protocol_icon_uri;
}
