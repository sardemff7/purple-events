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

#include "context.h"
#include "events.h"
#include "utils.h"

#include "callbacks.h"

void
purple_events_callback_signed_on(PurpleBuddy *buddy, PurpleEventsContext *context)
{
    if ( ! purple_events_utils_check_buddy_event_dispatch(context, buddy, "signed-on") )
        return;

    purple_signal_emit(context->plugin, "user_presence-online", buddy);
}

void
purple_events_callback_signed_off(PurpleBuddy *buddy, PurpleEventsContext *context)
{
    if ( ! purple_events_utils_check_buddy_event_dispatch(context, buddy, "signed-off") )
        return;

    purple_signal_emit(context->plugin, "user_presence-offline", buddy);
}

void
purple_events_callback_status_changed(PurpleBuddy *buddy, PurpleStatus *old_status, PurpleStatus *new_status, PurpleEventsContext *context)
{
    gboolean old_avail = purple_status_is_available(old_status);
    gboolean new_avail = purple_status_is_available(new_status);
    const gchar *msg = purple_status_get_attr_string(new_status, "message");

    /* TODO: Add real events here
    if ( purple_status_is_independent(old_status) )
    {
        foreach ( unowned PurpleStatusAttr attr in old_status.get_type().get_attrs() )
        {
            var name = attr.get_name();
            unowned purple_Value @value = attr.get_value();
            switch ( @value.type )
            {
            case purple_Type.STRING:
                data.insert(name, @value.get_string());
            break;
            default:
            break;
            }
        }
    }
    else */ if ( old_avail && ( ! new_avail ) )
    {
        if ( ! purple_events_utils_check_buddy_event_dispatch(context, buddy, "away") )
            return;

        purple_signal_emit(context->plugin, "user_presence-away", buddy, msg);
    }
    else if ( ( ! old_avail ) && new_avail )
    {
        if ( ! purple_events_utils_check_buddy_event_dispatch(context, buddy, "back") )
            return;

        purple_signal_emit(context->plugin, "user_presence-back", buddy, msg);
    }
    else if ( g_strcmp0(msg, purple_status_get_attr_string(old_status, "message")) != 0 )
    {
        if ( ! purple_events_utils_check_buddy_event_dispatch(context, buddy, "status-message") )
            return;

        purple_signal_emit(context->plugin, "user_presence-status", buddy, msg);
    }
}

void
purple_events_callback_idle_changed(PurpleBuddy *buddy, gboolean oldidle, gboolean newidle, PurpleEventsContext *context)
{
    if ( oldidle == newidle )
        return;
    if ( ! purple_events_utils_check_buddy_event_dispatch(context, buddy, "idle") )
        return;

    if ( newidle )
        purple_signal_emit(context->plugin, "user_presence-idle", buddy);
    else
        purple_signal_emit(context->plugin, "user_presence-idle-back", buddy);
}

void
purple_events_callback_new_im_msg(PurpleAccount *account, const gchar *sender, const gchar *message, PurpleConversation *conv, PurpleMessageFlags flags, PurpleEventsContext *context)
{
    PurpleBuddy *buddy = purple_find_buddy(account, sender);

    gboolean highlight;

    highlight = flags & PURPLE_MESSAGE_NICK;

    if ( buddy == NULL )
    {
        if ( ! purple_events_utils_check_event_dispatch(context, account, conv, highlight ? "anonymous-highlight" : "anonymous-message") )
            return;
    }
    else if ( ! purple_events_utils_check_buddy_event_dispatch(context, buddy, highlight ? "highlight" : "message") )
        return;

    if ( highlight )
        purple_signal_emit(context->plugin, "user_im-highlight", account, sender, message, conv, flags);
    else
        purple_signal_emit(context->plugin, "user_im-received", account, sender, message, conv, flags);
}

void
purple_events_callback_new_chat_msg(PurpleAccount *account, const gchar *sender, const gchar *message, PurpleConversation *conv, PurpleMessageFlags flags, PurpleEventsContext *context)
{
    PurpleBuddy *buddy = purple_find_buddy(account, sender);

    gboolean highlight;

    highlight = flags & PURPLE_MESSAGE_NICK;

    if ( buddy == NULL )
    {
        if ( ! purple_events_utils_check_event_dispatch(context, account, conv, highlight ? "anonymous-highlight" : "anonymous-message") )
            return;
    }
    else if ( ! purple_events_utils_check_buddy_event_dispatch(context, buddy, highlight ? "highlight" : "message") )
        return;

    if ( highlight )
        purple_signal_emit(context->plugin, "user_chat-highlight", account, sender, message, conv, flags);
    else
        purple_signal_emit(context->plugin, "user_chat-received", account, sender, message, conv, flags);
}


void
purple_events_callback_email_notification(const gchar *subject, const gchar *from, const gchar *to, const gchar *url, PurpleEventsContext *context)
{
    if ( ! purple_prefs_get_bool("/plugins/core/events/events/emails") )
        return;

    purple_signal_emit(context->plugin, "user_email-arrived", subject, from, to, url);
}

void
purple_events_callback_emails_notification(const gchar **subject, const gchar **from, const gchar **to, const gchar **url, guint count, PurpleEventsContext *context)
{
    if ( ( count == 0 ) || ( ! purple_prefs_get_bool("/plugins/core/events/events/emails") ) )
        return;

    if ( ! purple_prefs_get_bool("/plugins/core/events/restrictions/stack-emails") )
        count = 1;

    guint i;
    for ( i = 0 ; i < count ; ++i )
        purple_signal_emit(context->plugin, "user_email-arrived", subject[i], from[i], to[i], url[i]);
}


void
purple_events_callback_conversation_updated(PurpleConversation *conv, PurpleConvUpdateType type, PurpleEventsContext *context)
{
    switch ( type )
    {
    case PURPLE_CONV_UPDATE_UNSEEN:
        if ( purple_conversation_has_focus(conv) && ( ! GPOINTER_TO_UINT(purple_conversation_get_data(conv, "purple-events-last-focus")) ) )
            purple_signal_emit(context->plugin, "conversation-got-focus", conv);
    break;
    case PURPLE_CONV_UPDATE_TOPIC:
    default:
    break;
    }
    purple_conversation_set_data(conv, "purple-events-last-focus", GUINT_TO_POINTER(purple_conversation_has_focus(conv)));
}


static gboolean
_purple_events_callback_account_signed_on_timeout(gpointer user_data)
{
    PurpleEventsJustSignedOnAccount *data = user_data;

    if ( ( purple_account_get_connection(data->account) != NULL ) && ( ! purple_account_is_connected(data->account) ) )
        return TRUE;
    data->context->just_signed_on_accounts = g_list_remove(data->context->just_signed_on_accounts, data);

    g_free(data);

    return FALSE;
}

void
purple_events_callback_account_signed_on(PurpleConnection *conn, PurpleEventsContext *context)
{
    g_assert(conn != NULL);

    PurpleEventsJustSignedOnAccount *data;
    data = g_new(PurpleEventsJustSignedOnAccount, 1);
    data->context = context;
    data->account = purple_connection_get_account(conn);

    context->just_signed_on_accounts = g_list_prepend(context->just_signed_on_accounts, data);


    data->handle = purple_timeout_add_seconds(5, _purple_events_callback_account_signed_on_timeout, data);
}

void
purple_events_callback_auth_requested(PurpleAccount *account, const char *sender, const char *message, PurpleEventsContext *context)
{
    if ( ! purple_events_utils_check_event_dispatch(context, account, NULL, "authorization-requested") )
        return;
    
    purple_signal_emit(context->plugin, "user_authorization-requested", account, sender, message);
}
