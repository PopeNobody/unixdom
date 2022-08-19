c () 
{ 
    strace -o/dev/stdout -fF ./client
}
s () 
{ 
    strace -o/dev/stdout -fF ./server
}
