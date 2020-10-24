#Insecure Web Server


- [Design](#Design)
    - [Overview](#Overview)
    - [Threads](##Threads)
    - [Sending and receiving](##Send)
    - [Buffers](##Buffers)
    - [Dependencies](##Dependencies)
- [Why?](#Why)

# Design
This is a basic HTTP1/1 C Server which should have multiple buffer vulnerabilities.  
It's only existence is to be a dependant of mstring and to learn about http. 
It's also a possibility to learn about correct multithreading.  
## Overview
The Server can handle up to two Https Requests. 
It also has one DB thread which is responsible for reading/writing.

###Threads

There are four thread types:
 - the main thread 
 - the writer thread 
 - the listener thread 
 - the database thread 
 
The main thread is responsible for scheduling and handles the other threads.
The listener and writers are tightly coupled,, since each receiving thread must post an answer.
The database thread writes in a background thread all the requests to the database.   

They may be named differently, but the thread types are all implemented as WebThread. 
The only context they need, is their thread_task and done_task, which are all callbacks.

The main loop is is this: 
```
    while main_thread is alive
    if db_thread not exists
        create db_thread
        db_thread.open_db()
    if(no request)
        main_thread.block()
    for each request in requests
        create listener if not exists
        listener.read(request)
        needs_answer := listener.result()
        listener.may_need_db().
        if(need_answer and writer not exists)
            create_writer
        writer.send(message)
        writer.postdone()
        keep listener alive until default_timeout or new request
        if(not new_request or default_timeout)
            listener.postdone(); //which invokes writer.destroy()
            main_thread.destroy(listener)
    main_thread.destroy(all_listeners)
    main_thread.destroy(db_thread)
    main_thread.exit() 
```
  
WebThreads have a OS dependant implementation, but they appear to the clients as generic as possible.

###Sending and Receiving


At the end of each task, the WebThread posts a done() and marks it as reusable for the main thread.
 
 
###Dependencies
The only external Dependency of this project is [mstring](mstring).  
Since this is a real implementation, I get to understand which API's are lacking mstring. 