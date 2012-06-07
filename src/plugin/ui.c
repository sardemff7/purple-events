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


PurplePluginPrefFrame *
purple_events_ui_get_pref_frame(PurplePlugin *plugin)
{
    PurplePluginPref *pref = NULL;
    PurplePluginPrefFrame *frame = purple_plugin_pref_frame_new();

    pref = purple_plugin_pref_new_with_label(
        _("Events:")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/eventc/events/message",
        _("Messages")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/eventc/events/action",
        _("Actions (/me messages)")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/eventc/events/signed-on",
        _("Buddy signing on")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/eventc/events/signed-off",
        _("Buddy signing off")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/eventc/events/away",
        _("Buddy going away")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/eventc/events/idle",
        _("Buddy going idle")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/eventc/events/back",
        _("Buddy coming back")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/eventc/events/status-message",
        _("Status message change (or removal)")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/eventc/events/specials",
        _("Special events")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_label(
        _("Restrictions:")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/eventc/restrictions/blocked",
        _("Even for a blocked buddy")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/eventc/restrictions/new-conv-only",
        _("Only from new conversation")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/eventc/restrictions/only-available",
        _("Only when available")
    );
    purple_plugin_pref_frame_add(frame, pref);

    pref = purple_plugin_pref_new_with_name_and_label(
        "/plugins/core/libnotify+/stack-events",
        _("Stack events (do not ignore a buddy action if an event is already attached to)")
    );
    purple_plugin_pref_frame_add(frame, pref);

    return frame;
}

static PurpleBlistNode *
_purple_events_ui_get_good_node(PurpleBlistNode *node)
{
    if ( PURPLE_BLIST_NODE_IS_CONTACT(node) )
        return node;

    if ( PURPLE_BLIST_NODE_IS_BUDDY(node) )
        return PURPLE_BLIST_NODE(purple_buddy_get_contact(PURPLE_BUDDY(node)));

    if ( PURPLE_BLIST_NODE_IS_GROUP(node) )
        return node;

    return NULL;
}

static void
_purple_events_ui_reset_setting(PurpleBlistNode *node)
{
    node = _purple_events_ui_get_good_node(node);
    if ( node == NULL )
        return;

    purple_blist_node_remove_setting(node, "events");
}

static void
_purple_events_ui_disable_all_events(PurpleBlistNode *node)
{
    node = _purple_events_ui_get_good_node(node);
    if ( node == NULL )
        return;

    purple_blist_node_set_int(node, "events", -1);
}

static const gchar *_purple_events_ui_events[] =
{
    "events/message",
    "events/action",
    "events/signed-on",
    "events/signed-off",
    "events/away",
    "events/idle",
    "events/back",
    "events/status-message",
    "events/specials",
    NULL
};

static const gchar *_purple_events_ui_events_labels[] =
{
    N_("Messages"),
    N_("Actions (/me messages)"),
    N_("Buddy signing on"),
    N_("Buddy signing off"),
    N_("Buddy going away"),
    N_("Buddy going idle"),
    N_("Buddy coming back"),
    N_("Status message change (or removal)"),
    N_("Special events"),
    NULL
};

static void
_purple_events_ui_events_selection(PurpleBlistNode *node, PurpleRequestFields *request)
{
    guint i;
    gboolean v;
    for ( i = 0 ; _purple_events_ui_events[i] != NULL ; ++i )
    {
        v = purple_request_fields_get_bool(request, _purple_events_ui_events[i]);
        purple_blist_node_set_int(node, _purple_events_ui_events[i], v ? 1 : -1);
    }
    purple_blist_node_set_int(node, "events", 1);
}

static void
_purple_events_ui_events_selection_dialog(PurpleBlistNode *node, PurpleEventsContext *context)
{
    node = _purple_events_ui_get_good_node(node);
    if ( node == NULL )
        return;

    PurpleRequestFields *request;
    PurpleRequestFieldGroup *group;
    PurpleRequestField *field;

    group = purple_request_field_group_new(NULL);

    gint current;
    guint i;
    gboolean v;
    for ( i = 0 ; _purple_events_ui_events[i] != NULL ; ++i )
    {
        current = purple_blist_node_get_int(node, _purple_events_ui_events[i]);
        if ( current != 0 )
            v = ( current > 0 );
        else
        {
            gchar *pref_name;

            pref_name = g_strconcat("/plugins/core/events/", _purple_events_ui_events[i], NULL);
            v = purple_prefs_get_bool(pref_name);
            g_free(pref_name);
        }

        field = purple_request_field_bool_new(_purple_events_ui_events[i], _(_purple_events_ui_events_labels[i]), v);
        purple_request_field_group_add_field(group, field);
    }

    request = purple_request_fields_new();
    purple_request_fields_add_group(request, group);

    purple_request_fields(context->plugin,
        _("Events selection"),
        PURPLE_BLIST_NODE_IS_CONTACT(node) ? _("Select events for contact") : _("Select events for group"),
        NULL,
        request,
        PURPLE_BLIST_NODE_IS_CONTACT(node) ? _("_Set events for contact") : _("_Set events for group"),
        G_CALLBACK(_purple_events_ui_events_selection),
        _("_Cancel"),
        NULL,
        NULL,
        PURPLE_BLIST_NODE_IS_CONTACT(node) ? purple_buddy_get_name(purple_contact_get_priority_buddy(PURPLE_CONTACT(node))) : NULL,
        NULL,
        node);

}

void
purple_events_ui_menu_add(PurpleBlistNode *node, GList **menu, PurpleEventsContext *context)
{
    PurpleBlistNode *group = NULL;

    node = _purple_events_ui_get_good_node(node);
    if ( ( node == NULL ) || ( ( purple_blist_node_get_flags(node) & PURPLE_BLIST_NODE_FLAG_NO_SAVE ) > 0 ) )
        return;

    if ( PURPLE_BLIST_NODE_IS_CONTACT(node) )
        group = PURPLE_BLIST_NODE(purple_buddy_get_group(purple_contact_get_priority_buddy(PURPLE_CONTACT(node))));


    gint current = purple_blist_node_get_int(node, "events");
    GList *actions = NULL;
    PurpleMenuAction *action = NULL;

    const gchar *label = "";
    if ( current != 0 )
    {
        if ( PURPLE_BLIST_NODE_IS_GROUP(node) || ( purple_blist_node_get_int(group, "events") == 0 ) )
            label = _("Use global settings");
        else
            label = _("Use group settings");
        action = purple_menu_action_new(label, (PurpleCallback)_purple_events_ui_reset_setting, NULL, NULL);
        actions = g_list_prepend(actions, action);
    }
    if ( current >= 0 )
    {
        action = purple_menu_action_new(_("Disable all events"), (PurpleCallback)_purple_events_ui_disable_all_events, NULL, NULL);
        actions = g_list_prepend(actions, action);
    }

    action = purple_menu_action_new(_("Select events"), (PurpleCallback)_purple_events_ui_events_selection_dialog, context, NULL);
    actions = g_list_prepend(actions, action);

    switch ( current )
    {
    case -1:
        label = _("Current events: all disabled");
    break;
    case 0:
        if ( PURPLE_BLIST_NODE_IS_GROUP(node) || ( purple_blist_node_get_int(group, "events") == 0 ) )
            label = _("Current events: global settings");
        else
            label = _("Current events: group settings");
    break;
    case 1:
        label = _("Current events: selection");
    break;
    }
    actions = g_list_prepend(actions, NULL);
    action = purple_menu_action_new(label, NULL, NULL, NULL);
    actions = g_list_prepend(actions, action);

    *menu = g_list_append(*menu, NULL);
    action = purple_menu_action_new(_("Events"), NULL, NULL, actions);
    *menu = g_list_append(*menu, action);
}
