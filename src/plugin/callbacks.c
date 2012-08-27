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

#include <purple-events-context.h>
#include <purple-events-handler.h>

#include "events.h"
#include "utils.h"

#include "callbacks.h"


void
purple_events_callback_signed_on(PurpleBuddy *buddy, PurpleEventsContext *context)
{
    if ( ! purple_events_utils_check_event_dispatch(context, buddy, "signed-on") )
        return;

    gboolean stack;
    stack = purple_prefs_get_bool("/plugins/core/events/restrictions/stack-events");

    PurpleContact *contact;
    contact = purple_buddy_get_contact(buddy);

    PurpleEventsHandler *handler;
    GList *handler_;
    for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
    {
        handler = handler_->data;

        GList *events;
        gpointer event = NULL;
        gpointer old_event = NULL;

        events = g_hash_table_lookup(handler->events, contact);
        if ( ( ! stack ) && ( events != NULL ) )
            old_event = events->data;

        if ( handler->signed_on != NULL )
            event = handler->signed_on(handler->plugin, old_event, buddy);

        if ( ( event != NULL ) && ( event != old_event ) )
            g_hash_table_insert(handler->events, contact, g_list_prepend(events, event));
    }
}

void
purple_events_callback_signed_off(PurpleBuddy *buddy, PurpleEventsContext *context)
{
    if ( ! purple_events_utils_check_event_dispatch(context, buddy, "signed-off") )
        return;

    gboolean stack;
    stack = purple_prefs_get_bool("/plugins/core/events/restrictions/stack-events");

    PurpleContact *contact;
    contact = purple_buddy_get_contact(buddy);

    PurpleEventsHandler *handler;
    GList *handler_;
    for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
    {
        handler = handler_->data;

        GList *events;
        gpointer event = NULL;
        gpointer old_event = NULL;

        events = g_hash_table_lookup(handler->events, contact);
        if ( ( ! stack ) && ( events != NULL ) )
            old_event = events->data;

        if ( handler->signed_off != NULL )
            event = handler->signed_off(handler->plugin, old_event, buddy);

        if ( ( event != NULL ) && ( event != old_event ) )
            g_hash_table_insert(handler->events, contact, g_list_prepend(events, event));
    }
}

void
purple_events_callback_status_changed(PurpleBuddy *buddy, PurpleStatus *old_status, PurpleStatus *new_status, PurpleEventsContext *context)
{
    gboolean old_avail = purple_status_is_available(old_status);
    gboolean new_avail = purple_status_is_available(new_status);
    const gchar *msg = purple_status_get_attr_string(new_status, "message");
    PurpleEventsHandler *handler;
    GList *handler_;

    gboolean stack;
    stack = purple_prefs_get_bool("/plugins/core/events/restrictions/stack-events");

    PurpleContact *contact;
    contact = purple_buddy_get_contact(buddy);

    if ( purple_status_is_independent(old_status) )
    {
        if ( ! purple_events_utils_check_event_dispatch(context, buddy, "specials") )
            return;

        for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
        {
            handler = handler_->data;

            GList *events;
            gpointer event = NULL;
            gpointer old_event = NULL;

            events = g_hash_table_lookup(handler->events, contact);
            if ( ( ! stack ) && ( events != NULL ) )
                old_event = events->data;

            if ( handler->special != NULL )
                event = handler->special(handler->plugin, old_event, buddy, PURPLE_EVENTS_EVENT_SPECIAL_TYPE_NONE);

            if ( ( event != NULL ) && ( event != old_event ) )
                g_hash_table_insert(handler->events, contact, g_list_prepend(events, event));
        }

        /* TODO: make it work
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
        */
    }
    else if ( old_avail && ( ! new_avail ) )
    {
        if ( ! purple_events_utils_check_event_dispatch(context, buddy, "away") )
            return;

        for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
        {
            handler = handler_->data;

            GList *events;
            gpointer event = NULL;
            gpointer old_event = NULL;

            events = g_hash_table_lookup(handler->events, contact);
            if ( ( ! stack ) && ( events != NULL ) )
                old_event = events->data;

            if ( handler->away != NULL )
                event = handler->away(handler->plugin, old_event, buddy, msg);

            if ( ( event != NULL ) && ( event != old_event ) )
                g_hash_table_insert(handler->events, contact, g_list_prepend(events, event));
        }
    }
    else if ( ( ! old_avail ) && new_avail )
    {
        if ( ! purple_events_utils_check_event_dispatch(context, buddy, "back") )
            return;

        for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
        {
            handler = handler_->data;

            GList *events;
            gpointer event = NULL;
            gpointer old_event = NULL;

            events = g_hash_table_lookup(handler->events, contact);
            if ( ( ! stack ) && ( events != NULL ) )
                old_event = events->data;

            if ( handler->back != NULL )
                event = handler->back(handler->plugin, old_event, buddy, msg);

            if ( ( event != NULL ) && ( event != old_event ) )
                g_hash_table_insert(handler->events, contact, g_list_prepend(events, event));
        }
    }
    else if ( g_strcmp0(msg, purple_status_get_attr_string(old_status, "message")) != 0 )
    {
        if ( ! purple_events_utils_check_event_dispatch(context, buddy, "status-message") )
            return;

        for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
        {
            handler = handler_->data;

            GList *events;
            gpointer event = NULL;
            gpointer old_event = NULL;

            events = g_hash_table_lookup(handler->events, contact);
            if ( ( ! stack ) && ( events != NULL ) )
                old_event = events->data;

            if ( handler->status != NULL )
                event = handler->status(handler->plugin, old_event, buddy, msg);

            if ( ( event != NULL ) && ( event != old_event ) )
                g_hash_table_insert(handler->events, contact, g_list_prepend(events, event));
        }
    }
}

void
purple_events_callback_idle_changed(PurpleBuddy *buddy, gboolean oldidle, gboolean newidle, PurpleEventsContext *context)
{
    if ( oldidle == newidle )
        return;
    if ( ! purple_events_utils_check_event_dispatch(context, buddy, "idle") )
        return;

    gboolean stack;
    stack = purple_prefs_get_bool("/plugins/core/events/restrictions/stack-events");

    PurpleContact *contact;
    contact = purple_buddy_get_contact(buddy);

    PurpleEventsHandler *handler;
    GList *handler_;
    for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
    {
        handler = handler_->data;

        GList *events;
        gpointer event = NULL;
        gpointer old_event = NULL;

        events = g_hash_table_lookup(handler->events, contact);
        if ( ( ! stack ) && ( events != NULL ) )
            old_event = events->data;

        if ( newidle )
        {
            if ( handler->idle != NULL )
                event = handler->idle(handler->plugin, old_event, buddy);
        }
        else
        {
            if ( handler->idle_back != NULL )
                event = handler->idle_back(handler->plugin, old_event, buddy);
        }

        if ( ( event != NULL ) && ( event != old_event ) )
            g_hash_table_insert(handler->events, contact, g_list_prepend(events, event));
    }
}

void
purple_events_callback_new_im_msg(PurpleAccount *account, const gchar *sender, const gchar *message, PurpleConversation *conv, PurpleMessageFlags flags, PurpleEventsContext *context)
{
    PurpleBuddy *buddy = purple_find_buddy(account, sender);
    if ( buddy == NULL )
        return;

    if ( ( conv != NULL ) && purple_prefs_get_bool("/plugins/core/events/restrictions/new-conv-only") )
        return;

    gboolean action;
    gchar *stripped_message;

    stripped_message = purple_markup_strip_html(message);
    action = g_str_has_prefix(stripped_message, "/me ");
    g_free(stripped_message);

    if ( ! purple_events_utils_check_event_dispatch(context, buddy, action ? "action" : "message") )
        return;

    gboolean stack;
    stack = purple_prefs_get_bool("/plugins/core/events/restrictions/stack-events");

    PurpleContact *contact;
    contact = purple_buddy_get_contact(buddy);

    PurpleEventsHandler *handler;
    GList *handler_;
    for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
    {
        handler = handler_->data;

        GList *events;
        gpointer event = NULL;
        gpointer old_event = NULL;

        events = g_hash_table_lookup(handler->events, contact);
        if ( ( ! stack ) && ( events != NULL ) )
            old_event = events->data;

        if ( action )
        {
            if ( handler->im_action != NULL )
                event = handler->im_action(handler->plugin, old_event, buddy, message);
        }
        else
        {
            if ( handler->im_message != NULL )
                event = handler->im_message(handler->plugin, old_event, buddy, message);
        }

        if ( ( event != NULL ) && ( event != old_event ) )
            g_hash_table_insert(handler->events, contact, g_list_prepend(events, event));
    }
}

void
purple_events_callback_new_chat_msg(PurpleAccount *account, const gchar *sender, const gchar *message, PurpleConversation *conv, PurpleEventsContext *context)
{
    PurpleBuddy *buddy = purple_find_buddy(account, sender);
    if ( buddy == NULL )
        return;

    gboolean action;
    gchar *stripped_message;

    stripped_message = purple_markup_strip_html(message);
    action = g_str_has_prefix(stripped_message, "/me ");
    g_free(stripped_message);

    if ( ! purple_events_utils_check_event_dispatch(context, buddy, action ? "action" : "message") )
        return;

    gboolean stack;
    stack = purple_prefs_get_bool("/plugins/core/events/restrictions/stack-events");

    PurpleEventsHandler *handler;
    GList *handler_;
    for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
    {
        handler = handler_->data;

        GList *events;
        gpointer event = NULL;
        gpointer old_event = NULL;

        events = g_hash_table_lookup(handler->events, conv);
        if ( ( ! stack ) && ( events != NULL ) )
            old_event = events->data;

        if ( action )
        {
            if ( handler->chat_action != NULL )
                event = handler->chat_action(handler->plugin, old_event, conv, buddy, message);
        }
        else
        {
            if ( handler->chat_message != NULL )
                event = handler->chat_message(handler->plugin, old_event, conv, buddy, message);
        }

        if ( ( event != NULL ) && ( event != old_event ) )
            g_hash_table_insert(handler->events, conv, g_list_prepend(events, event));
    }
}


void
purple_events_callback_email_notification(const gchar *subject, const gchar *from, const gchar *to, const gchar *url, PurpleEventsContext *context)
{
    if ( ! purple_prefs_get_bool("/plugins/core/events/events/emails") )
        return;

    PurpleEventsHandler *handler;
    GList *handler_;
    for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
    {
        handler = handler_->data;

        if ( handler->email != NULL )
            handler->email(handler->plugin, subject, from, to, url);
    }
}

void
purple_events_callback_emails_notification(const gchar **subject, const gchar **from, const gchar **to, const gchar **url, guint count, PurpleEventsContext *context)
{
    if ( ( count == 0 ) || ( ! purple_prefs_get_bool("/plugins/core/events/events/emails") ) )
        return;

    if ( ! purple_prefs_get_bool("/plugins/core/events/restrictions/stack-emails") )
        count = 1;

    PurpleEventsHandler *handler;
    GList *handler_;
    guint i;
    for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
    {
        handler = handler_->data;

        if ( handler->email != NULL )
        {
            for ( i = 0 ; i < count ; ++i )
                handler->email(handler->plugin, subject[i], from[i], to[i], url[i]);
        }
    }
}


void
purple_events_callback_conversation_updated(PurpleConversation *conv, PurpleConvUpdateType type, PurpleEventsContext *context)
{
    PurpleEventsHandler *handler;
    GList *handler_;
    switch ( type )
    {
    case PURPLE_CONV_UPDATE_UNSEEN:
        if ( purple_conversation_has_focus(conv) && ( ! GPOINTER_TO_UINT(purple_conversation_get_data(conv, "purple-events-last-focus")) ) )
        {
            gpointer attach = NULL;
            switch ( purple_conversation_get_type(conv) )
            {
            case PURPLE_CONV_TYPE_IM:
                attach = purple_buddy_get_contact(purple_find_buddy(purple_conversation_get_account(conv), purple_conversation_get_name(conv)));
            break;
            case PURPLE_CONV_TYPE_CHAT:
                attach = conv;
            break;
            default:
            break;
            }
            if ( attach == NULL )
                break;
            for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
            {
                handler = handler_->data;
                GList *event;
                for ( event = g_hash_table_lookup(handler->events, attach) ; event != NULL ; event = g_list_next(event) )
                    handler->end_event(handler->plugin, event->data);
            }
        }
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
    data->context->just_signed_on_accounts = g_list_remove(data->context->just_signed_on_accounts, data->account);

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
