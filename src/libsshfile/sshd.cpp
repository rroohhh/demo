/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  sshd.cpp -- simple sshd implementation
*/

#define KEYS_FOLDER "/etc/ssh/"

#include "sshd.h"

sshd::sshd(std::string addr, unsigned int port) {
    sshbind = ssh_bind_new();

    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDADDR, addr.c_str());
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT, &port);
    ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY,
                         KEYS_FOLDER "ssh_host_rsa_key");

    if(ssh_bind_listen(sshbind) < 0) {
        exit(EXIT_FAILURE);
    }
}

ssh_channel sshd::accept() {
    ssh_session session;
    ssh_message message;
    ssh_channel chan  = 0;
    int         auth  = 0;
    int         shell = 0;
    int         r;

    session = ssh_new();
    r       = ssh_bind_accept(sshbind, session);

    if(r == SSH_ERROR) {
        return NULL;
    }

    if(ssh_handle_key_exchange(session)) {
        return NULL;
    }

    /* proceed to authentication */
    auth = authenticate(session);
    if(!auth) {
        ssh_disconnect(session);
        return NULL;
    }

    /* wait for a channel session */
    do {
        message = ssh_message_get(session);
        if(message) {
            if(ssh_message_type(message) == SSH_REQUEST_CHANNEL_OPEN &&
               (ssh_message_subtype(message) == SSH_CHANNEL_REQUEST_SHELL ||
                ssh_message_subtype(message) == SSH_CHANNEL_REQUEST_PTY)) {
                chan = ssh_message_channel_request_open_reply_accept(message);
                ssh_message_free(message);
                break;
            } else {
                ssh_message_reply_default(message);
                ssh_message_free(message);
            }
        } else {
            if(!chan) {
                ssh_finalize();
                return NULL;
            }
        }
    } while(!chan);

    do {
        message = ssh_message_get(session);
        if(message != NULL) {
            if(ssh_message_type(message) == SSH_REQUEST_CHANNEL &&
               (ssh_message_subtype(message) == SSH_CHANNEL_REQUEST_SHELL ||
                ssh_message_subtype(message) == SSH_CHANNEL_REQUEST_PTY)) {
                shell = 1;
                ssh_message_channel_request_reply_success(message);
                ssh_message_free(message);
                break;
            }
            ssh_message_reply_default(message);
            ssh_message_free(message);
        } else {
            if(!shell) {
                return NULL;
            }
        }
    } while(!shell);

    return chan;
}

int sshd::authenticate(ssh_session session) {
    ssh_message message;

    do {
        message = ssh_message_get(session);
        if(!message) break;
        switch(ssh_message_type(message)) {
        case SSH_REQUEST_AUTH:
            switch(ssh_message_subtype(message)) {
            case SSH_AUTH_METHOD_NONE:
                ssh_message_auth_reply_success(message, 0);
                ssh_message_free(message);
                return 1;
            default:
                ssh_message_auth_set_methods(message, SSH_AUTH_METHOD_NONE);
                ssh_message_reply_default(message);
                break;
            }
            break;
        default:
            ssh_message_auth_set_methods(message, SSH_AUTH_METHOD_NONE);
            ssh_message_reply_default(message);
        }
        ssh_message_free(message);
    } while(1);
    return 0;
}

