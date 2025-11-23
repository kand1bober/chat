# Chat

### build
    gcc chat.c sig.c shm.c thread.c -o <name>

### Functions:

#### chat 
    create/join chat

    using: chat <num>

    note: if num = -1, new chat will be created
          if num >= 0, user will be added to chat where user with pid,
          that is equal to num is present

#### list 
    show list of users present in this chat

    using: list

#### tell
    send msg from one user(process) to another

    using:  tell \<pid\> \<phrase\>  

#### say 
    send msg from one user(process) to every process in the chat, where current process is present

    using:  say \<pid\> \<phrase\>  

#### bye
    quit chat

    using: bye