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

    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void SignedOnFunc(Purple.Plugin plugin, Purple.Buddy buddy);
    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void SignedOffFunc(Purple.Plugin plugin, Purple.Buddy buddy);

    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void AwayFunc(Purple.Plugin plugin, Purple.Buddy buddy, string? message);
    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void BackFunc(Purple.Plugin plugin, Purple.Buddy buddy, string? message);

    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void StatusFunc(Purple.Plugin plugin, Purple.Buddy buddy, string? message);
    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void SpecialFunc(Purple.Plugin plugin, Purple.Buddy buddy, EventSpecialType type, ...);

    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void IdleFunc(Purple.Plugin plugin, Purple.Buddy buddy);
    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void IdleBackFunc(Purple.Plugin plugin, Purple.Buddy buddy);

    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void ImActionFunc(Purple.Plugin plugin, Purple.Buddy buddy, string message);
    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void ImMessageFunc(Purple.Plugin plugin, Purple.Buddy buddy, string message);

    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void ChatActionFunc(Purple.Plugin plugin, Purple.Buddy buddy, string message);
    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void ChatMessageFunc(Purple.Plugin plugin, Purple.Buddy buddy, string message);

    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public struct Handler
    {
        public unowned Purple.Plugin plugin;

        public SignedOnFunc signed_on;
        public SignedOffFunc signed_off;

        public AwayFunc away;
        public BackFunc back;

        public StatusFunc status;
        public SpecialFunc special;

        public IdleFunc idle;
        public IdleBackFunc idle_back;

        public ImMessageFunc im_message;
        public ImActionFunc im_action;

        public ChatMessageFunc chat_message;
        public ChatActionFunc chat_action;
    }

    [CCode (cheader_filename = "purple-events.h")]
    [Compact]
    public class Context
    {
        public void connect_handler(Handler *handler);
        public void disconnect_handler(Handler *handler);
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
