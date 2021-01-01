# Distributed-Systems-Remote-Fork-Project
#### Note: I'm using \* as a wild card

## Description
In this project, we tackle the problem of allowing a process to fork a child on a remote
machine. This forking method takes an additional parameter that identifies on which machine
this child process should run. Remote forking can be implemented on the user-level using
checkpointing and restart.

## Dependencies
- This project depends on DMTCP library for handling checkpointing and restarting a process, so it's essential to install this library befor running this project
- DMTCP Library: https://github.com/dmtcp/dmtcp


## In order to install DMTCP library
1. Clone the repo in the provided link to some local directory.
2. Open a terminal in your local dmtcp folder. If you cloned it from home you will most probably find it thier, but sometimes it can be hidden, so make sure to view hidden files and folders.
3. Run the following commands in terminal:
    - ./configure
    - make
    - make install
4. Those commands should install the library in your "/usr/local/include" and enable you to include "dmtcp.h" in your projects.
5. Some times an include error might occur, if this happend you need tp:
    - go to your include folder using: cd /usr/local/include
    - open the "dmtcp.h" to edit using: sudo nano dmtcp.h
    - now change #include "dmtcp/version.h" to #include "version.h"
6. Now you should have a working "dmtcp.h" that you can include in your project and gives you access to the DMTCP library.

    -- For any further clarification see this install guide: https://github.com/dmtcp/dmtcp/blob/master/INSTALL.md --

# Linux Distribution
Ubuntu 20.04.1 LTS (DMTCP supports backward and forward compatibilty so the linux distro shouldn't be a probelm)

# Running the Program
- You will need to have a *_send.cpp running on 1 node and *_recieve.cpp running on the other. So compile both files using g++
- Before you run the send.out executable, run "dmtcp_coordinator" on another terminal (This coordinator controls the checkpointing done inside the parent node code)
- After that, run your recieve.out executable so it starts to listen for any incoming forking requests.
- Finally, run "dmtcp_launch --join ./*_send.out" (this runs your parent node sender executable and connects it to the DMTCP coordinator so it can checkpoint correctly)

And that's it, you should now have a remote forking program up and running.
