# sfifo

## Design

### Server

* Creates and opens a FIFO file `srv`, listens for clients in loop
  * receives PID and message from client
  * counts N of characters
  * opens client's FIFO file at `fifo<PID>`, writes N
  * closes client's FIFO

### Client

* creates `fifo<PID>` FIFO file or handles error if already exists
* opens the `srv`, sends it's PID and message
* then reads it's `fifo<pid>` file

### Constraints

If client or server attempts to create a FIFO file and a file at given path
already exists and is not a FIFO file, write an error message to `stdout`.

## Usage

```
make prepare
make

bin/sfifo-srv

# in another session
bin/sfifo-cli
```
