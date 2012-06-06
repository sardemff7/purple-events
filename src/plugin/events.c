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
#include <glib/gi18n.h>
#include <purple.h>

#include <purple-events-context.h>

#include "ui.h"
#include "callbacks.h"

#include "events.h"


static void _purple_events_init(PurplePlugin *plugin);
static void _purple_events_destroy(PurplePlugin *plugin);
static gboolean _purple_events_load(PurplePlugin *plugin);
static gboolean _purple_events_unload(PurplePlugin *plugin);


static PurplePluginUiInfo _purple_events_plugin_ui_info = {
    .get_plugin_pref_frame = purple_events_ui_get_pref_frame
};

static PurplePluginInfo _purple_events_plugin_info = {
    .magic          = PURPLE_PLUGIN_MAGIC,
    .major_version  = PURPLE_MAJOR_VERSION,
    .minor_version  = PURPLE_MINOR_VERSION,
    .type           = PURPLE_PLUGIN_STANDARD,
    .ui_requirement = 0,
    .flags          = 0,
    .dependencies   = NULL,
    .priority       = PURPLE_PRIORITY_DEFAULT,

    .id             = "core-sardemff7-" PACKAGE_NAME,
    .name           = NULL,
    .version        = PACKAGE_VERSION,
    .summary        = NULL,
    .description    = NULL,
    .author         = "Quentin \"Sardem FF7\" Glidic <sardemff7+pidgin@sardemff7.net>",
    .homepage       = "http://sardemff7.github.com/" PACKAGE_TARNAME "/",

    .load           = _purple_events_load,
    .unload         = _purple_events_unload,
    .destroy        = _purple_events_destroy,

    .ui_info        = NULL,
    .extra_info     = NULL,
    .prefs_info     = &_purple_events_plugin_ui_info,
    .actions        = NULL
};

PURPLE_INIT_PLUGIN(purple_events, _purple_events_init, _purple_events_plugin_info)

static void
_purple_events_init(PurplePlugin *plugin)
{
#if ENABLE_NLS
    bindtextdomain(GETTEXT_PACKAGE, LOCALEDIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
#endif /* ENABLE_NLS */

    _purple_events_plugin_info.name = _("Events handling");
    _purple_events_plugin_info.summary = _("Allows to set a fine-grained event handling");
    _purple_events_plugin_info.description = _("This plugin should be used as a base for event-based plugins");

    PurpleEventsContext *context;
    context = g_new0(PurpleEventsContext, 1);

    context->plugin = plugin;
    plugin->extra = context;

    purple_prefs_add_none("/plugins/core/events");

    purple_prefs_add_none("/plugins/core/events/events");
    purple_prefs_add_bool("/plugins/core/events/events/message", TRUE);
    purple_prefs_add_bool("/plugins/core/events/events/action", TRUE);
    purple_prefs_add_bool("/plugins/core/events/events/signed-on", TRUE);
    purple_prefs_add_bool("/plugins/core/events/events/signed-off", FALSE);
    purple_prefs_add_bool("/plugins/core/events/events/away", TRUE);
    purple_prefs_add_bool("/plugins/core/events/events/idle", TRUE);
    purple_prefs_add_bool("/plugins/core/events/events/back", TRUE);
    purple_prefs_add_bool("/plugins/core/events/events/status-message", FALSE);
    purple_prefs_add_bool("/plugins/core/events/events/specials", FALSE);

    purple_prefs_add_none("/plugins/core/events/restrictions");
    purple_prefs_add_bool("/plugins/core/events/restrictions/blocked", TRUE);
    purple_prefs_add_bool("/plugins/core/events/restrictions/new-conv-only", FALSE);
    purple_prefs_add_bool("/plugins/core/events/restrictions/only-available", FALSE);
    purple_prefs_add_bool("/plugins/core/events/restrictions/stack-events", FALSE);
}

static void
_purple_events_destroy(PurplePlugin *plugin)
{
    PurpleEventsContext *context = plugin->extra;

    g_list_free(context->handlers);
    g_free(context);

    plugin->extra = NULL;
}

static gboolean
_purple_events_load(PurplePlugin *plugin)
{
    void *conv_handle = purple_conversations_get_handle();
    void *blist_handle = purple_blist_get_handle();
    void *conn_handle = purple_connections_get_handle();

    purple_signal_connect(
        blist_handle, "buddy-signed-on", plugin,
        (PurpleCallback)purple_events_callback_signed_on, plugin->extra
    );

    purple_signal_connect(
        blist_handle, "buddy-signed-off", plugin,
        (PurpleCallback)purple_events_callback_signed_off, plugin->extra
    );

    purple_signal_connect(
        blist_handle, "buddy-status-changed", plugin,
        (PurpleCallback)purple_events_callback_status_changed, plugin->extra
    );

    purple_signal_connect(
        blist_handle, "buddy-idle-changed", plugin,
        (PurpleCallback)purple_events_callback_idle_changed, plugin->extra
    );

    purple_signal_connect(
        conv_handle, "received-im-msg", plugin,
        (PurpleCallback)purple_events_callback_new_im_msg, plugin->extra
    );

    purple_signal_connect(
        conv_handle, "received-chat-msg", plugin,
        (PurpleCallback)purple_events_callback_new_chat_msg, plugin->extra
    );


    purple_signal_connect(
        conv_handle, "conversation-updated", plugin,
        (PurpleCallback)purple_events_callback_conversation_updated, plugin->extra
    );


    purple_signal_connect(
        conn_handle, "signed-on", plugin,
        (PurpleCallback)purple_events_callback_account_signed_on, plugin->extra
    );


    purple_signal_connect(
        blist_handle, "blist-node-extended-menu", plugin,
        (PurpleCallback)purple_events_ui_menu_add, plugin->extra
    );

    return TRUE;
}

static void
_purple_events_just_signed_on_account_free(gpointer data)
{
    PurpleEventsJustSignedOnAccount *just_signed_on_account = data;
    purple_timeout_remove(just_signed_on_account->handle);
    g_free(just_signed_on_account);
}

static gboolean
_purple_events_unload(PurplePlugin *plugin)
{
    PurpleEventsContext *context = plugin->extra;

    void *conv_handle = purple_conversations_get_handle();
    void *blist_handle = purple_blist_get_handle();
    void *conn_handle = purple_connections_get_handle();

    g_list_free_full(context->just_signed_on_accounts, _purple_events_just_signed_on_account_free);

    purple_signal_disconnect(
        blist_handle, "buddy-signed-on", plugin,
        (PurpleCallback)purple_events_callback_signed_on
    );

    purple_signal_disconnect(
        blist_handle, "buddy-signed-off", plugin,
        (PurpleCallback)purple_events_callback_signed_off
    );

    purple_signal_disconnect(
        blist_handle, "buddy-status-changed", plugin,
        (PurpleCallback)purple_events_callback_status_changed
    );

    purple_signal_disconnect(
        blist_handle, "buddy-idle-changed", plugin,
        (PurpleCallback)purple_events_callback_idle_changed
    );

    purple_signal_disconnect(
        conv_handle, "received-im-msg", plugin,
        (PurpleCallback)purple_events_callback_new_im_msg
    );

    purple_signal_disconnect(
        conv_handle, "received-chat-msg", plugin,
        (PurpleCallback)purple_events_callback_new_chat_msg
    );


    purple_signal_disconnect(
        conn_handle, "signed-on", plugin,
        (PurpleCallback)purple_events_callback_account_signed_on
    );


    purple_signal_disconnect(
        blist_handle, "blist-node-extended-menu", plugin,
        (PurpleCallback)purple_events_ui_menu_add
    );

    return TRUE;
}
