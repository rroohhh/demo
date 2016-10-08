#include <thread>

#include "repl.h"
#include "ssh_file.h"
#include "sshd.h"

int main(int argc, char ** argv) {
    std::thread t([]() {
        sshd listener("0.0.0.0", 8000);
        while(true) {
            auto     chan = listener.accept();
            ssh_file sfile(&chan);
            repl(sfile.infile(), sfile.outfile(), CommandHandler());
        }
    });
    t.join();
}
