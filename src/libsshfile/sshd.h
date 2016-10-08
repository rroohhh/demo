/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  sshd.h -- simple sshd implementation
*/

#ifndef _SSHD_H
#define _SSHD_H

#include <string>
#include <stdlib.h>
#include "libssh/libssh.h"
#include "libssh/server.h"

class sshd {
public:
    sshd(std::string addr, unsigned int port);
    ssh_channel accept();

private:
    ssh_bind sshbind;
    int authenticate(ssh_session session);
};

#endif
