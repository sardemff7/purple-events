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

#ifndef __PURPLE_EVENTS_CALLBACKS_H__
#define __PURPLE_EVENTS_CALLBACKS_H__

#include <purple-events.h>

G_BEGIN_DECLS


void purple_events_callback_signed_on(PurpleBuddy *buddy, PurpleEventsContext *context);
void purple_events_callback_signed_off(PurpleBuddy *buddy, PurpleEventsContext *context);
void purple_events_callback_status_changed(PurpleBuddy *buddy, PurpleStatus *old_status, PurpleStatus *new_status, PurpleEventsContext *context);
void purple_events_callback_idle_changed(PurpleBuddy *buddy, gboolean oldidle, gboolean newidle, PurpleEventsContext *context);
void purple_events_callback_new_im_msg(PurpleAccount *account, const gchar *sender, const gchar *message, PurpleConversation *conv, PurpleMessageFlags flags, PurpleEventsContext *context);
void purple_events_callback_new_chat_msg(PurpleAccount *account, const gchar *sender, const gchar *message, PurpleConversation *conv, PurpleMessageFlags flags, PurpleEventsContext *context);
void purple_events_callback_email_notification(const gchar *subject, const gchar *from, const gchar *to, const gchar *url, PurpleEventsContext *context);
void purple_events_callback_emails_notification(const gchar **subject, const gchar **from, const gchar **to, const gchar **url, guint count, PurpleEventsContext *context);
void purple_events_callback_conversation_updated(PurpleConversation *conv, PurpleConvUpdateType type, PurpleEventsContext *context);
void purple_events_callback_account_signed_on(PurpleConnection *conn, PurpleEventsContext *context);


G_END_DECLS

#endif /* __PURPLE_EVENTS_CALLBACKS_H__ */
