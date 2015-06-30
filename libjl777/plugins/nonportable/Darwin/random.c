#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>

int32_t OS_getpid() { return(getpid()); }
int32_t OS_getppid() { return(getppid()); }
int32_t OS_waitpid(int32_t childpid,int32_t *statusp,int32_t flags) { return(waitpid(childpid,statusp,flags)); }

int32_t OS_launch_process(char *args[])
{
    pid_t child_pid;
    if ( (child_pid= fork()) >= 0 )
    {
        if ( child_pid == 0 )
        {
            printf("plugin PID =  %d, parent pid = %d (%s, %s, %s, %s, %s)\n",getpid(),getppid(),args[0],args[1],args[2],args[3],args[4]);
            return(execv(args[0],args));
        }
        else
        {
            printf("parent PID =  %d, child pid = %d\n",getpid(),child_pid);
            return(child_pid);
        }
    }
    else return(-1);
}

// from tweetnacl
void randombytes(unsigned char *x,long xlen)
{
    static int fd = -1;
    int32_t i;
    if (fd == -1) {
        for (;;) {
            fd = open("/dev/urandom",O_RDONLY);
            if (fd != -1) break;
            sleep(1);
        }
    }
    while (xlen > 0) {
        if (xlen < 1048576) i = (int32_t)xlen; else i = 1048576;
        i = (int32_t)read(fd,x,i);
        if (i < 1) {
            sleep(1);
            continue;
        }
        x += i;
        xlen -= i;
    }
}

