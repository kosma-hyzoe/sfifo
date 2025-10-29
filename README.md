# sfifo

## Server

* creates and opens connection, listens for clients in loop
  * ...opens `/tmp/fifo-listener`?
  * handles client PID and text message
  * opens `fifo<PID>`, writes n of chars
  * close client fifo

## Client

* creates `fifo<PID>` if not exists (recieve end)
* opens `fifo-listener`, sends message
* then opens `fifo<pid>` and waits

## Both

if exists and not fifo, >:(
