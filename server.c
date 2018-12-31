#include<stdio.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>

int main()
{
    /* code */
    int welcome,new_soc,fd,n;
    char buffer[1024],fname[50];
    struct sockaddr_in addr;
    welcome = socket(PF_INET,SOCK_STREAM,0);
    addr.sin_family=AF_INET;
    addr.sin_port=htons(7891);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    bind(welcome,(struct sockaddr *) &addr,sizeof(addr));
    printf("server online\n");
    listen(welcome,5);
    new_soc = accept(welcome,NULL,NULL);
    recv(new_soc,fname,50,0);
    printf(" requesting for file: %s",fname);
    fd = open(fname,O_RDONLY);
    if(fd<0){
        send(new_soc,"file not found",20,0);
    }
    else{
        while((n=read(fd,buffer,sizeof(buffer)))>0)
            send(new_soc,buffer,sizeof(buffer),0);
            printf("file sent");
            
    }
    close(fd);


    return 0;
}

