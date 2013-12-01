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

static void
_purple_events_exec_exec(const gchar *command_line)
{
    GError *error = NULL;
    if ( ! g_spawn_command_line_async(command_line, &error) )
    {
        g_warning("Couldn’t close notification: %s", error->message);
        g_clear_error(&error);
    }
}

static void
_purple_events_exec_signed_on(PurpleBuddy *buddy, PurplePlugin *plugin)
{
    _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/signed-on"));
}

static void
_purple_events_exec_signed_off(PurpleBuddy *buddy, PurplePlugin *plugin)
{
    _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/signed-off"));
}

static void
_purple_events_exec_away(PurpleBuddy *buddy, const gchar *message, PurplePlugin *plugin)
{
    _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/away"));
}

static void
_purple_events_exec_back(PurpleBuddy *buddy, const gchar *message, PurplePlugin *plugin)
{
    _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/back"));
}

static void
_purple_events_exec_status(PurpleBuddy *buddy, const gchar *message, PurplePlugin *plugin)
{
    _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/status-message"));
}

static void
_purple_events_exec_idle(PurpleBuddy *buddy, PurplePlugin *plugin)
{
    _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/idle"));
}

static void
_purple_events_exec_message(PurpleAccount *account, const gchar *sender, const gchar *message, PurpleConversation *conv, PurpleMessageFlags flags, PurplePlugin *plugin)
{
    _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/message"));
}

static void
_purple_events_exec_highlight(PurpleAccount *account, const gchar *sender, const gchar *message, PurpleConversation *conv, PurpleMessageFlags flags, PurplePlugin *plugin)
{
    _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/highlight"));
}

static void
_purple_events_exec_email(const gchar *subject, const gchar *from, const gchar *to, const gchar *url, PurplePlugin *plugin)
{
    _purple_events_exec_exec(purple_prefs_get_string("/plugins/core/events-exec/emails"));
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
        "/plugins/core/events-exec/highlight",
        _("Highlights")
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
        "/plugins/core/events-exec/emails",
        _("E-mails")
    );
    purple_plugin_pref_frame_add(frame, pref);

    return frame;
}

static void _purple_events_exec_init(PurplePlugin *plugin);
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

    .id             = PURPLE_EVENTS_PLUGIN_ID "-exec",
    .name           = NULL,
    .version        = PACKAGE_VERSION,
    .summary        = NULL,
    .description    = NULL,
    .author         = "Quentin \"Sardem FF7\" Glidic <sardemff7+pidgin@sardemff7.net>",
    .homepage       = "http://" PACKAGE_TARNAME ".sardemff7.net/",

    .load           = _purple_events_exec_load,
    .unload         = _purple_events_exec_unload,
    .destroy        = NULL,

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

    purple_prefs_add_none("/plugins/core/events-exec");
    purple_prefs_add_string("/plugins/core/events-exec/message", "");
    purple_prefs_add_string("/plugins/core/events-exec/highlight", "");
    purple_prefs_add_string("/plugins/core/events-exec/signed-on", "");
    purple_prefs_add_string("/plugins/core/events-exec/signed-off", "");
    purple_prefs_add_string("/plugins/core/events-exec/away", "");
    purple_prefs_add_string("/plugins/core/events-exec/idle", "");
    purple_prefs_add_string("/plugins/core/events-exec/back", "");
    purple_prefs_add_string("/plugins/core/events-exec/status-message", "");
    purple_prefs_add_string("/plugins/core/events-exec/emails", "");
}

static gboolean
_purple_events_exec_load(PurplePlugin *plugin)
{
    gpointer handle;

    handle = purple_plugins_find_with_id(purple_events_get_plugin_id());
    g_return_val_if_fail(handle != NULL, FALSE);

    purple_signal_connect(
        handle, "user_presence-online", plugin,
        (PurpleCallback)_purple_events_exec_signed_on, plugin
    );
    purple_signal_connect(
        handle, "user_presence-offline", plugin,
        (PurpleCallback)_purple_events_exec_signed_off, plugin
    );
    purple_signal_connect(
        handle, "user_presence-away", plugin,
        (PurpleCallback)_purple_events_exec_away, plugin
    );
    purple_signal_connect(
        handle, "user_presence-back", plugin,
        (PurpleCallback)_purple_events_exec_back, plugin
    );
    purple_signal_connect(
        handle, "user_presence-idle", plugin,
        (PurpleCallback)_purple_events_exec_idle, plugin
    );
    purple_signal_connect(
        handle, "user_presence-idle-back", plugin,
        (PurpleCallback)_purple_events_exec_idle, plugin
    );
    purple_signal_connect(
        handle, "user_presence-message", plugin,
        (PurpleCallback)_purple_events_exec_status, plugin
    );

    purple_signal_connect(
        handle, "user_im-received", plugin,
        (PurpleCallback)_purple_events_exec_message, plugin
    );
    purple_signal_connect(
        handle, "user_im-highlight", plugin,
        (PurpleCallback)_purple_events_exec_highlight, plugin
    );
    purple_signal_connect(
        handle, "user_chat-received", plugin,
        (PurpleCallback)_purple_events_exec_message, plugin
    );
    purple_signal_connect(
        handle, "user_chat-highlight", plugin,
        (PurpleCallback)_purple_events_exec_highlight, plugin
    );

    purple_signal_connect(
        handle, "user_email-arrived", plugin,
        (PurpleCallback)_purple_events_exec_email, plugin
    );

    return TRUE;
}

static gboolean
_purple_events_exec_unload(PurplePlugin *plugin)
{
    gpointer handle;

    handle = purple_plugins_find_with_id(purple_events_get_plugin_id());
    g_return_val_if_fail(handle != NULL, FALSE);

    purple_signal_disconnect(
        handle, "user_presence-online", plugin,
        (PurpleCallback)_purple_events_exec_signed_on
    );
    purple_signal_disconnect(
        handle, "user_presence-offline", plugin,
        (PurpleCallback)_purple_events_exec_signed_off
    );
    purple_signal_disconnect(
        handle, "user_presence-away", plugin,
        (PurpleCallback)_purple_events_exec_away
    );
    purple_signal_disconnect(
        handle, "user_presence-back", plugin,
        (PurpleCallback)_purple_events_exec_back
    );
    purple_signal_disconnect(
        handle, "user_presence-idle", plugin,
        (PurpleCallback)_purple_events_exec_idle
    );
    purple_signal_disconnect(
        handle, "user_presence-idle-back", plugin,
        (PurpleCallback)_purple_events_exec_idle
    );
    purple_signal_disconnect(
        handle, "user_presence-message", plugin,
        (PurpleCallback)_purple_events_exec_status
    );

    purple_signal_disconnect(
        handle, "user_im-received", plugin,
        (PurpleCallback)_purple_events_exec_message
    );
    purple_signal_disconnect(
        handle, "user_im-highlight", plugin,
        (PurpleCallback)_purple_events_exec_highlight
    );
    purple_signal_disconnect(
        handle, "user_chat-received", plugin,
        (PurpleCallback)_purple_events_exec_message
    );
    purple_signal_disconnect(
        handle, "user_chat-highlight", plugin,
        (PurpleCallback)_purple_events_exec_highlight
    );

    purple_signal_disconnect(
        handle, "user_email-arrived", plugin,
        (PurpleCallback)_purple_events_exec_email
    );

    return TRUE;
}



