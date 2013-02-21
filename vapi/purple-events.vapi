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

    [CCode (cheader_filename = "purple-events.h")]
    public enum MessageType
    {
        NORMAL,
        HIGHLIGHT,
        ACTION,
    }

    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void SignedOnFunc(Purple.Plugin plugin, void *event, Purple.Buddy buddy);
    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void SignedOffFunc(Purple.Plugin plugin, void *event, Purple.Buddy buddy);

    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void AwayFunc(Purple.Plugin plugin, void *event, Purple.Buddy buddy, string? message);
    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void BackFunc(Purple.Plugin plugin, void *event, Purple.Buddy buddy, string? message);

    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void StatusFunc(Purple.Plugin plugin, void *event, Purple.Buddy buddy, string? message);
    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void SpecialFunc(Purple.Plugin plugin, void *event, Purple.Buddy buddy, EventSpecialType type, ...);

    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void IdleFunc(Purple.Plugin plugin, void *event, Purple.Buddy buddy);
    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void IdleBackFunc(Purple.Plugin plugin, void *event, Purple.Buddy buddy);

    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void ImMessageFunc(Purple.Plugin plugin, void *event, MessageType type, Purple.Buddy? buddy, string sender, string message);
    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void ChatMessageFunc(Purple.Plugin plugin, void *event, MessageType type, Purple.Conversation conv, Purple.Buddy? buddy, string sender, string message);

    [CCode (cheader_filename = "purple-events.h", has_target = false)]
    public delegate void EndEventFunc(Purple.Plugin plugin, void *event);

    [CCode (cheader_filename = "purple-events.h")]
    [Compact]
    public class Handler
    {
        public Handler(Purple.Plugin plugin);
        public void remove_event(void *attach, void *event);

        public void add_signed_on_callback(SignedOnFunc @callback);
        public void add_signed_off_callback(SignedOffFunc @callback);

        public void add_away_callback(AwayFunc @callback);
        public void add_back_callback(BackFunc @callback);

        public void add_status_callback(StatusFunc @callback);

        public void add_idle_callback(IdleFunc @callback);
        public void add_idle_back_callback(IdleBackFunc @callback);

        public void add_im_message_callback(ImMessageFunc @callback);
        public void add_chat_message_callback(ChatMessageFunc @callback);

        public void add_end_event_callback(EndEventFunc @callback);
    }

    [CCode (cheader_filename = "purple-events.h")]
    public static void connect_handler(Handler handler);
    [CCode (cheader_filename = "purple-events.h")]
    public static void disconnect_handler(Handler handler);

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
