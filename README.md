# ft_irc

A small IRC server written in C++98. It listens for IRC clients over TCP, checks the configured server password during registration, and supports nicknames, private and channel messages, and channel membership and moderation commands.

## Requirements

- A C++ compiler
- GNU Make
- Linux (the server uses `epoll`)

## Build

From the repository root:

```sh
make
```

This builds the server as `ircserv` and builds the bundled `libftpp` support library. The default build uses optimization and treats compiler warnings as errors.

Debug builds are available with `make debug1` (debug symbols) or `make debug2` (debug symbols and AddressSanitizer). `make clean` removes build files; `make fclean` also removes the generated executable(s).

## Run

Pass a TCP port and a non-empty connection password:

```sh
./ircserv 6667 change-me
```

The port must be an unsigned 16-bit value, and the password cannot contain whitespace. The server writes diagnostic output to `logs.txt` and serves its message of the day from `data/MOTD.txt`.

Connect with an IRC client to `127.0.0.1` on the selected port, using the same password. For example, with `nc`:

```sh
nc 127.0.0.1 6667
```

Then register by sending these IRC lines (each line must end with CRLF):

```text
PASS change-me
NICK alice
USER alice 0 * :Alice Example
```

The `make run` target builds and starts the server with the Makefile's example arguments (`6667 password`).

## Supported commands

The server implements these IRC commands:

- Registration and identity: `PASS`, `NICK`, `USER`
- Messaging and connection: `PRIVMSG`, `PING`, `QUIT`
- Channels: `JOIN`, `PART`, `TOPIC`, `LIST`, `WHO`, `ISON`
- Channel administration: `MODE`, `INVITE`, `KICK`

Channel modes, invitation rules, and command parameters follow the implementation's IRC handling. Channel names beginning with `#` and `&` have separate membership limits; each limit is 42 channels per user.

## Project layout

- `src/` and `include/` contain the server, socket/event loop, channel and user code, and command handlers.
- `lib/libftpp/` contains the bundled C++ utility library used by the server.
- `data/MOTD.txt` contains the server message of the day.
- `Makefile` is the primary build interface. A CMake configuration is also present.
