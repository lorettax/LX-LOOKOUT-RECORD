#include "socket.hh"
#include "util.hh"

#include <cstdlib>
#include <iostream>

using namespace std;

void get_URL(const string &host, const string &path) {

    FullStackSocket sock{};
    sock.connect(Address(host, "http"));
    sock.write("GET " + path + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n");
    sock.shutdown(SHUT_WR);
    while (!sock.eof())
    {
        cout << sock.read();
    }
    sock.close();
    sock.wait_until_closed();
    return;

}

int main(int argc, char *argv[]) {
    try {
        if (argc <= 0) {
            abort();  // For sticklers: don't try to access argv[0] if argc <= 0.
        }

        // The program takes two command-line arguments: the hostname and "path" part of the URL.
        if (argc != 3) {
            cerr << "Usage: " << argv[0] << " HOST PATH\n";
            cerr << "\tExample: " << argv[0] << " edu/class/\n";
            return EXIT_FAILURE;
        }

        const string host = argv[1];
        const string path = argv[2];

        get_URL(host, path);
    } catch (const exception &e) {
        cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
