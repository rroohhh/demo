/*
  Copyright (C) 2016 by Robin Heinemann. All Rights Reserved.
  repl.h -- Interactive simulation control
*/

#ifndef _REPL_H
#define _REPL_H

#include "CommandHandler.h"
#include <cstdio>
#include <iostream>
//#include <readline/history.h>
//#include <readline/readline.h>
#include "histedit.h"
#include <thread>

class repl {
public:
    repl(FILE * in, FILE * out, CommandHandler handler);

private:
    void init_editline(FILE * infile, FILE * outfile);
    EditLine * el = NULL;
    History *  hist;
    HistEvent  ev;
};

#endif
