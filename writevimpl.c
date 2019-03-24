#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/uio.h>


#define _FILE_OFFSET_BITS 64
#define MAX 101
struct shatter{
    void *buffer;
    size_t length;
};
typedef struct shatter Shatter;

int cwritev(int _fd,const Shatter *mirror,int counter);
int creadv(int _fd,const Shatter *mirror,int counter);

int main(int argc, char const *argv[])
{
    if(argc != 2){
        perror("Wrong args: use ./write <file to open>");
        exit(EXIT_FAILURE);
    }
    int _fd = open(argv[1],O_RDONLY,0777);
    if(_fd == -1){
        perror("File does not exist");
        exit(EXIT_FAILURE);
    }
    Shatter mirror[3];
    char text1[MAX] = {0};
    char text2[MAX] = {0};
    char text3[MAX] = {0};   
    
    mirror[0].buffer = text1;
    mirror[0].length = MAX;
    mirror[1].buffer = text2;
    mirror[1].length = MAX;
    mirror[2].buffer = text3;
    mirror[2].length = MAX;

    creadv(_fd,mirror,3);
    cwritev(STDIN_FILENO,mirror,3);
    
    
    close(_fd);
    return 0;
}

int cwritev(int _fd,const Shatter *mirror,int counter){

    size_t writtenBytes,totalBytes;
    size_t tempBytes = 0;
    writtenBytes = 0;
    totalBytes = 0;
    for(int i = 0;i < counter;i++){
        writtenBytes = write(_fd,mirror[i].buffer,mirror[i].length);
        if(writtenBytes != mirror[i].length){
            return -1;
            }
        tempBytes += writtenBytes;
        totalBytes += mirror[i].length;
    }
    if(tempBytes != totalBytes){
        return -1;
    }


    return tempBytes;
}


int creadv(int _fd,const Shatter *mirror,int counter){


    size_t readBytes,totalBytes;
    size_t tempBytes = 0;
    readBytes = 0;
    totalBytes = 0;
    tempBytes = 0;
    for(int i = 0; i < counter; i++)
    {
        readBytes = read(_fd,mirror[i].buffer,mirror[i].length);
        if(readBytes != mirror[i].length){
            return -1;
        }
        totalBytes += mirror[i].length;
        tempBytes += readBytes;    
    }
    if(tempBytes != totalBytes){
        return -1;
    }

    return tempBytes;



}












