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

namespace PurpleEvents
{
    [CCode (cheader_filename = "purple-events.h")]
    unowned string get_plugin_id();

    [CCode (cheader_filename = "purple-events.h")]
    public enum EventSpecialType
    {
        NONE,
        TUNE,
    }

    [CCode (cheader_filename = "purple-events.h")]
    public enum UtilsIconFormat
    {
        SVG,
        PNG,
    }

    namespace Utils
    {
        [CCode (cheader_filename = "purple-events.h")]
        public static unowned string buddy_get_best_name(Purple.Buddy buddy);
        [CCode (cheader_filename = "purple-events.h")]
        public static unowned string buddy_get_protocol(Purple.Buddy buddy);
        [CCode (cheader_filename = "purple-events.h")]
        public static string protocol_get_icon_uri(string protocol_name, UtilsIconFormat format);
    }
}
