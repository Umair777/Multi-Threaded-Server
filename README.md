```markdown
# Multi-Threaded-Server — README

This repository implements a small multi-threaded server and a set of companion programs that demonstrate inter-process communication (IPC) using named pipes (FIFOs) and concurrent request handling. The README explains what each file does, how the pieces interact, how to build the code, and how to run simple examples. I write in active voice and keep the instructions practical — inspect the source files (links below) for exact argument formats.

Quick links
- Repository: https://github.com/Umair777/Multi-Threaded-Server
- Files referenced in this README:
  - adder.c — https://github.com/Umair777/Multi-Threaded-Server/blob/master/adder.c
  - sub.c — https://github.com/Umair777/Multi-Threaded-Server/blob/master/sub.c
  - creatfifo.c — https://github.com/Umair777/Multi-Threaded-Server/blob/master/creatfifo.c
  - rc1.c — https://github.com/Umair777/Multi-Threaded-Server/blob/master/rc1.c
  - rc2.c — https://github.com/Umair777/Multi-Threaded-Server/blob/master/rc2.c
  - server.c — https://github.com/Umair777/Multi-Threaded-Server/blob/master/server.c
  - request.h — https://github.com/Umair777/Multi-Threaded-Server/blob/master/request.h
  - headers.h — https://github.com/Umair777/Multi-Threaded-Server/blob/master/headers.h
  - declarations.h — https://github.com/Umair777/Multi-Threaded-Server/blob/master/declarations.h
  - Precompiled object files (binaries): server.o, creatfifo.o (present in repo as built artifacts)

Table of contents
- Purpose
- Project structure and file responsibilities
- How the components interact
- Build instructions
- Run / Usage examples
- Debugging tips and notes
- License & contact

Purpose
-------
I designed this repository to demonstrate a simple multi-threaded server that receives requests from clients through named FIFOs, performs arithmetic operations, and returns results. The code emphasizes basic IPC, request parsing, and thread-per-request handling.

Project structure and file responsibilities
-------------------------------------------

- server.c
  - Role: Implement the multi-threaded server core.
  - What it does: It opens the shared request FIFO(s), receives client requests, spawns worker threads to handle each request concurrently, coordinates access to shared resources, and sends responses back to clients (commonly through client-specific FIFOs).
  - Where to look: https://github.com/Umair777/Multi-Threaded-Server/blob/master/server.c

- request.h
  - Role: Define the request/response data structures and related constants.
  - What it does: It declares the request format (structure fields such as operation type, operands, client identifiers, and any result containers) and any protocol-level constants the server and clients share.
  - Where to look: https://github.com/Umair777/Multi-Threaded-Server/blob/master/request.h

- headers.h
  - Role: Centralize standard includes and shared macros.
  - What it does: It collects the common system headers (stdio.h, stdlib.h, string.h, unistd.h, sys/types.h, sys/stat.h, fcntl.h, pthread.h, errno.h, etc.) and defines convenience macros or small helpers used by multiple .c files.
  - Where to look: https://github.com/Umair777/Multi-Threaded-Server/blob/master/headers.h

- declarations.h
  - Role: Keep small declaration constants or prototypes.
  - What it does: It contains compact declarations (for example, macro values or forward declarations) that other modules include to avoid repetition.
  - Where to look: https://github.com/Umair777/Multi-Threaded-Server/blob/master/declarations.h

- creatfifo.c
  - Role: Create the named pipes used for IPC.
  - What it does: It creates one or more FIFOs (named pipes) that the server and clients use for communication. You can run this program to prepare the environment instead of using mkfifo manually.
  - Where to look: https://github.com/Umair777/Multi-Threaded-Server/blob/master/creatfifo.c
  - Note: The repository also contains a precompiled object (creatfifo.o). Use the source to rebuild on your platform.

- adder.c
  - Role: Client or worker module that performs addition requests.
  - What it does: It formats a request that asks the server to add given operands and waits for the response. Alternatively, it may implement the arithmetic routine the server invokes; inspect the file to confirm the exact usage pattern in this repo.
  - Where to look: https://github.com/Umair777/Multi-Threaded-Server/blob/master/adder.c

- sub.c
  - Role: Client or worker module that performs subtraction requests.
  - What it does: It formats a subtraction request, sends it to the server, and reads the reply. As with adder.c, inspect the source to see whether this file contains a client or server-side helper routine.
  - Where to look: https://github.com/Umair777/Multi-Threaded-Server/blob/master/sub.c

- rc1.c and rc2.c
  - Role: Sample request client programs / request-case examples.
  - What they do: Each program demonstrates a different request pattern (for example, different argument formats, payload layouts, or client FIFO naming). They act as simple clients that show how to construct requests the server expects.
  - Where to look:
    - rc1.c — https://github.com/Umair777/Multi-Threaded-Server/blob/master/rc1.c
    - rc2.c — https://github.com/Umair777/Multi-Threaded-Server/blob/master/rc2.c

- server.o and creatfifo.o
  - Role: Precompiled object files included in the repository.
  - What they are: They represent compiled artifacts. They are useful only on compatible systems; rebuilding from source is generally preferable for portability and security.
  - Where to look: server.o, creatfifo.o (present in repository root)

How the components interact
--------------------------
1. The repository defines a shared request format in request.h. Clients and the server include that header so both sides agree on the wire format.
2. You create the named FIFO(s) the server listens on. You can run the creatfifo utility or use mkfifo manually.
3. You start the server executable. The server opens the shared FIFO for reading requests and may open per-client FIFOs for sending replies (the client often provides its reply FIFO name inside the request).
4. Each time the server reads a request, it spawns a worker thread to process it. The worker thread:
   - Parses the request struct
   - Runs the requested operation (for example, addition or subtraction)
   - Opens the client-specific FIFO or file descriptor and writes the result back
   - Cleans up and exits
5. Clients (adder, sub, rc1, rc2) format requests, write them into the shared request FIFO, and wait for the server response on their reply FIFO.

Build instructions
------------------
I recommend building on a POSIX-compatible system (Linux, macOS with minor adjustments). The examples below assume GCC and pthreads.

1. Create object files and executables:
   - Build the server:
     gcc -pthread -o server server.c
   - Build helper clients:
     gcc -o adder adder.c
     gcc -o sub sub.c
     gcc -o rc1 rc1.c
     gcc -o rc2 rc2.c
   - Build the FIFO creator (optional):
     gcc -o creatfifo creatfifo.c

2. Alternatively, compile everything in one command:
   gcc -pthread -o server server.c adder.c sub.c rc1.c rc2.c creatfifo.c

3. Rebuild from the headers if you change them:
   - When you modify request.h, headers.h, or declarations.h, recompile all affected source files.

Run / Usage examples
--------------------
Note: Inspect the client programs' source to confirm the exact command-line arguments and FIFO names. The repository links above point to each source file for the definitive interface.

Basic sequence
1. Create the FIFOs
   - Run the helper:
     ./creatfifo
   - Or run manually:
     mkfifo /tmp/request_fifo
     mkfifo /tmp/client_reply_fifo_<PID>

2. Start the server (in one terminal)
   ./server

3. Send requests from clients (in other terminals)
   - Example (adder client):
     ./adder <operand1> <operand2>
     - The program typically writes a request and prints the response it receives from the server.
   - Example (subtraction client):
     ./sub <operand1> <operand2>
   - Example (rc1 / rc2):
     ./rc1 [args...]
     ./rc2 [args...]

4. Stop the server
   - Use Ctrl+C or send a termination signal. Ensure the server implements safe shutdown in server.c.

Notes and expectations
- The server uses threads; compile with -pthread.
- Clients usually pass the reply FIFO name inside the request struct so the server can return results to the correct client.
- The repository contains precompiled .o files (server.o, creatfifo.o); rebuild those from source for your platform.

Debugging tips
--------------
- If the server blocks on FIFO open/read:
  - Ensure at least one writer opens the FIFO, or open the FIFO in non-blocking mode for debugging.
- If clients hang waiting for replies:
  - Confirm that the server writes to the exact FIFO name the client created and that permissions allow both sides to open the FIFO.
- If you get compilation errors related to missing headers:
  - Confirm you compile with -pthread and include system development packages on your platform.

Security & portability
----------------------
- The code relies on POSIX APIs (mkfifo/open/read/write, pthread). It runs on Unix-like systems. Windows needs an equivalent IPC mechanism.
- Avoid running the server as root. Use proper file permissions for the FIFOs if multiple users share the system.

Where to look in the code for specifics
---------------------------------------
- For the request struct and wire-level details: request.h
- For the concurrency model and worker thread lifecycle: server.c
- For FIFO creation: creatfifo.c
- For example clients / operation examples: adder.c, sub.c, rc1.c, rc2.c
- For common includes and macros: headers.h and declarations.h

Contact
-------
- Author / repository owner: Umair777
- For clarification or changes, open an issue on the repository.

License
-------
- The repository does not include an explicit license file. If you plan to reuse this code publicly, add a LICENSE file or request clarification from the repository owner.

Acknowledgements
----------------
This README documents the repository layout and how to work with the multi-threaded server and example clients. For exact behavior, argument formats, and internal routines, please inspect the implementation files linked at the top of this README.
```
