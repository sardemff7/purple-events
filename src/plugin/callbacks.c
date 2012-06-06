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

    PurpleEventsHandler *handler;
    GList *handler_;
    for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
    {
        handler = handler_->data;
        if ( handler->signed_on != NULL )
            handler->signed_on(handler->plugin, buddy);
    }
}

void
purple_events_callback_signed_off(PurpleBuddy *buddy, PurpleEventsContext *context)
{
    if ( ! purple_events_utils_check_event_dispatch(context, buddy, "signed-off") )
        return;

    PurpleEventsHandler *handler;
    GList *handler_;
    for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
    {
        handler = handler_->data;
        if ( handler->signed_off != NULL )
            handler->signed_off(handler->plugin, buddy);
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

    if ( purple_status_is_independent(old_status) )
    {
        if ( ! purple_events_utils_check_event_dispatch(context, buddy, "specials") )
            return;

        for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
        {
            handler = handler_->data;
            if ( handler->special != NULL )
                handler->special(handler->plugin, buddy, PURPLE_EVENTS_EVENT_SPECIAL_TYPE_NONE);
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
            if ( handler->away != NULL )
                handler->away(handler->plugin, buddy, msg);
        }
    }
    else if ( ( ! old_avail ) && new_avail )
    {
        if ( ! purple_events_utils_check_event_dispatch(context, buddy, "back") )
            return;

        for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
        {
            handler = handler_->data;
            if ( handler->back != NULL )
                handler->back(handler->plugin, buddy, msg);
        }
    }
    else if ( g_strcmp0(msg, purple_status_get_attr_string(old_status, "message")) != 0 )
    {
        if ( ! purple_events_utils_check_event_dispatch(context, buddy, "status-message") )
            return;

        for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
        {
            handler = handler_->data;
            if ( handler->status != NULL )
                handler->status(handler->plugin, buddy, msg);
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

    PurpleEventsHandler *handler;
    GList *handler_;
    for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
    {
        handler = handler_->data;
        if ( newidle )
        {
            if ( handler->idle != NULL )
                handler->idle(handler->plugin, buddy);
        }
        else
        {
            if ( handler->idle_back != NULL )
                handler->idle_back(handler->plugin, buddy);
        }
    }
}

void
purple_events_callback_new_im_msg(PurpleAccount *account, const gchar *sender, const gchar *message, PurpleConversation *conv, PurpleMessageFlags flags, PurpleEventsContext *context)
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

    PurpleEventsHandler *handler;
    GList *handler_;
    for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
    {
        handler = handler_->data;
        if ( action )
        {
            if ( handler->im_action != NULL )
                handler->im_action(handler->plugin, buddy, message);
        }
        else
        {
            if ( handler->im_message != NULL )
                handler->im_message(handler->plugin, buddy, message);
        }
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

    PurpleEventsHandler *handler;
    GList *handler_;
    for ( handler_ = context->handlers ; handler_ != NULL ; handler_ = g_list_next(handler_) )
    {
        handler = handler_->data;
        if ( action )
        {
            if ( handler->chat_action != NULL )
                handler->chat_action(handler->plugin, buddy, message);
        }
        else
        {
            if ( handler->chat_message != NULL )
                handler->chat_message(handler->plugin, buddy, message);
        }
    }
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
