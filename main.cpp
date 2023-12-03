#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sys/types.h>
#include <cmath>
#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
    int fileChild[2];
    int fileParent[2];
    string fileChildMessage = "message from parent to child";
    string fileParentMessage = "message from child to parent";
    char buffer[100]; //buffer to read from pipe
    ssize_t numBytes;  

    //-------------------------------------- if pipe creation fails for parent or child---------------------
    if (pipe(fileChild) < 0 || pipe(fileParent) < 0) {
        cerr << "Pipe failed" << endl;
        exit(1);
    }

    pid_t pid = fork(); //for fork() to return twice once in parent process once in child process
    //------------------------------------------------
    // Parent process
    if (pid > 0) {  

        close(fileParent[0]); //close reading end of parent-to child pipe since parent will only write to this pipe.
        numBytes = write(fileParent[1], fileChildMessage.c_str(), fileChildMessage.size() + 1);
        //error
        if (numBytes == -1) {
            cerr << "Write to child failed" << endl;
            exit(1);
        }
        close(fileParent[1]);
        //read from child
        close(fileChild[1]);
        numBytes = read(fileChild[0], buffer, 100);
        if (numBytes == -1) {
            cerr << "Read from child failed" << endl;
            exit(1);
        }
        cout << "Parent received: " << buffer << endl;
        close(fileChild[0]);
        //------------------------------------------------------------------
        // Child process
    } else {

        close(fileParent[1]);
        //read from parent
        numBytes = read(fileParent[0], buffer, 100);
        //error
        if (numBytes == -1) {
            cerr << "Read from parent failed" << endl;
            exit(1);
        }
        //close read pipe after reading
        cout << "Child received: " << buffer << endl;
        close(fileParent[0]);
        //write to parent
        close(fileChild[0]); //close read end of child to parent pipe
        numBytes = write(fileChild[1], fileParentMessage.c_str(), fileParentMessage.size() + 1);
        if (numBytes == -1) {
            cerr << "Write to parent failed" << endl;
            exit(1);
        }
        close(fileChild[1]); //close write end
    }
    return 0;
}
