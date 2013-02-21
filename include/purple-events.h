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
	PURPLE_EVENTS_UTILS_ICON_FORMAT_SVG,
	PURPLE_EVENTS_UTILS_ICON_FORMAT_PNG,
} PurpleEventsUtilsIconFormat;

const gchar *purple_events_get_plugin_id(void);

const gchar *purple_events_utils_buddy_get_best_name(PurpleBuddy *buddy);
const gchar *purple_events_utils_buddy_get_protocol(PurpleBuddy *buddy);
gchar *purple_events_utils_protocol_get_icon_uri(const gchar *protocol_name, PurpleEventsUtilsIconFormat format);


G_END_DECLS

#endif /* __PURPLE_EVENTS_H__ */
