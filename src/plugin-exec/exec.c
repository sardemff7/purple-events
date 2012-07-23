/*
 * purple-events-exec - Simple exec plugin for purple-events
 *
 * Copyright © 2012 Quentin "Sardem FF7" Glidic
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
 */

#include <config.h>
#include <glib.h>
#include <glib/gi18n-lib.h>
#include <purple.h>
#include <purple-events.h>

static gpointer
_purple_events_exec_exec(const gchar *command_line)
{
    GError *error = NULL;
    if ( ! g_spawn_command_line_async(command_line, &error) )
    {
        g_warning("Couldn’t close notification: %s", error->message);
        g_clear_error(&error);
    }
    return NULL;

}

static gpointer
_purple_events_exec_signed_on(PurplePlugin *plugin, gpointer unused, PurpleBuddy *buddy)
{
    return _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/signed-on"));
}

static gpointer
_purple_events_exec_signed_off(PurplePlugin *plugin, gpointer unused, PurpleBuddy *buddy)
{
    return _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/signed-off"));
}

static gpointer
_purple_events_exec_away(PurplePlugin *plugin, gpointer unused, PurpleBuddy *buddy, const gchar *message)
{
    return _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/away"));
}

static gpointer
_purple_events_exec_back(PurplePlugin *plugin, gpointer unused, PurpleBuddy *buddy, const gchar *message)
{
    return _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/back"));
}

static gpointer
_purple_events_exec_status(PurplePlugin *plugin, gpointer unused, PurpleBuddy *buddy, const gchar *message)
{
    return _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/status-message"));
}

static gpointer
_purple_events_exec_special(PurplePlugin *plugin, gpointer unused, PurpleBuddy *buddy, PurpleEventsEventSpecialType type, ...)
{
    return _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/specials"));
}

static gpointer
_purple_events_exec_idle(PurplePlugin *plugin, gpointer unused, PurpleBuddy *buddy)
{
    return _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/idle"));
}

static gpointer
_purple_events_exec_idle_back(PurplePlugin *plugin, gpointer unused, PurpleBuddy *buddy)
{
    return _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/idle"));
}

static gpointer
_purple_events_exec_im_message(PurplePlugin *plugin, gpointer unused, PurpleBuddy *buddy, const gchar *message)
{
    return _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/message"));
}

static gpointer
_purple_events_exec_im_action(PurplePlugin *plugin, gpointer unused, PurpleBuddy *buddy, const gchar *message)
{
    return _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/action"));
}

static gpointer
_purple_events_exec_chat_message(PurplePlugin *plugin, gpointer unused, PurpleConversation *conv, PurpleBuddy *buddy, const gchar *message)
{
    return _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/message"));
}

static gpointer
_purple_events_exec_chat_action(PurplePlugin *plugin, gpointer unused, PurpleConversation *conv, PurpleBuddy *buddy, const gchar *message)
{
    return _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/action"));
}

static void
_purple_events_exec_email(PurplePlugin *plugin, const gchar *subject, const gchar *from, const gchar *to, const gchar *url)
{
    _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/emails"));
}

static void
_notify_plus_end_event(PurplePlugin *plugin, gpointer event)
{
}


static PurplePluginPrefFrame *
_purple_events_exec_get_pref_frame(PurplePlugin *plugin)
{
    PurplePluginPref *pref = NULL;
    PurplePluginPrefFrame *frame = purple_plugin_pref_frame_new();

    pref = purple_plugin_pref_new_with_label(
        _("Events commands:")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/events-exec/message",
        _("Messages")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/events-exec/action",
        _("Actions (/me messages)")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/events-exec/signed-on",
        _("Buddy signing on")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/events-exec/signed-off",
        _("Buddy signing off")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/events-exec/away",
        _("Buddy going away")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/events-exec/idle",
        _("Buddy going idle")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/events-exec/back",
        _("Buddy coming back")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/events-exec/status-message",
        _("Status message change (or removal)")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/events-exec/specials",
        _("Special events")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/events-exec/emails",
        _("E-mails")
    );
    purple_plugin_pref_frame_add(frame, pref);

    return frame;
}

static void _purple_events_exec_init(PurplePlugin *plugin);
static void _purple_events_exec_destroy(PurplePlugin *plugin);
static gboolean _purple_events_exec_load(PurplePlugin *plugin);
static gboolean _purple_events_exec_unload(PurplePlugin *plugin);

static PurplePluginUiInfo _purple_events_exec_ui_info = {
    .get_plugin_pref_frame = _purple_events_exec_get_pref_frame
};

static PurplePluginInfo _purple_events_exec_plugin_info = {
    .magic          = PURPLE_PLUGIN_MAGIC,
    .major_version  = PURPLE_MAJOR_VERSION,
    .minor_version  = PURPLE_MINOR_VERSION,
    .type           = PURPLE_PLUGIN_STANDARD,
    .ui_requirement = 0,
    .flags          = 0,
    .dependencies   = NULL,
    .priority       = PURPLE_PRIORITY_DEFAULT,

    .id             = "core-sardemff7-" PACKAGE_NAME "-exec",
    .name           = NULL,
    .version        = PACKAGE_VERSION,
    .summary        = NULL,
    .description    = NULL,
    .author         = "Quentin \"Sardem FF7\" Glidic <sardemff7+pidgin@sardemff7.net>",
    .homepage       = "http://" PACKAGE_TARNAME ".sardemff7.net/",

    .load           = _purple_events_exec_load,
    .unload         = _purple_events_exec_unload,
    .destroy        = _purple_events_exec_destroy,

    .ui_info        = NULL,
    .extra_info     = NULL,
    .prefs_info     = &_purple_events_exec_ui_info,
    .actions        = NULL
};

PURPLE_INIT_PLUGIN(purple_events_exec, _purple_events_exec_init, _purple_events_exec_plugin_info)

static void
_purple_events_exec_init(PurplePlugin *plugin)
{
#if ENABLE_NLS
    bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
#endif

    _purple_events_exec_plugin_info.name = _("Events to order");
    _purple_events_exec_plugin_info.summary = _("Exec a command on specific event");
    _purple_events_exec_plugin_info.dependencies = g_list_prepend(_purple_events_exec_plugin_info.dependencies, (gpointer) purple_events_get_plugin_id());

    PurpleEventsHandler *handler;

    handler = purple_events_handler_new(plugin);
    plugin->extra = handler;

    purple_events_handler_add_signed_on_callback(handler, _purple_events_exec_signed_on);
    purple_events_handler_add_signed_off_callback(handler, _purple_events_exec_signed_off);

    purple_events_handler_add_away_callback(handler, _purple_events_exec_away);
    purple_events_handler_add_back_callback(handler, _purple_events_exec_back);

    purple_events_handler_add_status_callback(handler, _purple_events_exec_status);
    purple_events_handler_add_special_callback(handler, _purple_events_exec_special);

    purple_events_handler_add_idle_callback(handler, _purple_events_exec_idle);
    purple_events_handler_add_idle_back_callback(handler, _purple_events_exec_idle_back);

    purple_events_handler_add_im_message_callback(handler, _purple_events_exec_im_message);
    purple_events_handler_add_im_action_callback(handler, _purple_events_exec_im_action);

    purple_events_handler_add_chat_message_callback(handler, _purple_events_exec_chat_message);
    purple_events_handler_add_chat_action_callback(handler, _purple_events_exec_chat_action);

    purple_events_handler_add_email_callback(handler, _purple_events_exec_email);

    purple_events_handler_add_end_event_callback(handler, _notify_plus_end_event);

    purple_prefs_add_none("/plugins/core/events-exec");
    purple_prefs_add_string("/plugins/core/events-exec/message", "");
    purple_prefs_add_string("/plugins/core/events-exec/action", "");
    purple_prefs_add_string("/plugins/core/events-exec/signed-on", "");
    purple_prefs_add_string("/plugins/core/events-exec/signed-off", "");
    purple_prefs_add_string("/plugins/core/events-exec/away", "");
    purple_prefs_add_string("/plugins/core/events-exec/idle", "");
    purple_prefs_add_string("/plugins/core/events-exec/back", "");
    purple_prefs_add_string("/plugins/core/events-exec/status-message", "");
    purple_prefs_add_string("/plugins/core/events-exec/specials", "");
    purple_prefs_add_string("/plugins/core/events-exec/emails", "");
}

static void
_purple_events_exec_destroy(PurplePlugin *plugin)
{
    purple_events_handler_free(plugin->extra);
}

static gboolean
_purple_events_exec_load(PurplePlugin *plugin)
{
    purple_events_connect_handler(plugin->extra);

    return TRUE;
}

static gboolean
_purple_events_exec_unload(PurplePlugin *plugin)
{
    purple_events_disconnect_handler(plugin->extra);

    return TRUE;
}



