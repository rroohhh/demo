/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  ssh_file.h -- c file over ssh
*/

#ifndef _SSH_FILE_H
#define _SSH_FILE_H

#include "libssh/libssh.h"
#include <fcntl.h>
#include <functional>
//#include <regex>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <pty.h>
#include <stdlib.h>

// a file, relayed over a tcp socket
class ssh_file {
public:
    ssh_file(ssh_channel * chan);

	void disconnect();

    FILE * outfile();
    FILE * infile();

private:
    int client_input[2], client_output[2];
    FILE * infile_;
    FILE * outfile_;
};

#endif
