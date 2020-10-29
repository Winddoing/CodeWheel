 
``` C
while(1) {
    struct sockaddr_storage client_addr;
    socklen_t addr_len = sizeof(client_addr);   
    char client_ip[NI_MAXHOST + 1]   = {0};
    char client_port[NI_MAXSERV + 1] = {0};

    /* wait for connection */
    cfd = accept(lfd, (struct sockaddr *)&client_addr, &addr_len);    
    if(cfd == -1) {
        /* Parent process must wait children, can't exit directly, or else zombie comes. */
        if(errno != EINTR) {
            printf("%d: Failure in accept(): %s\n", getpid(), strerror(errno));
            sleep(10);
        }

        continue;
    } else {
        inetAddressStr((struct sockaddr *)&client_addr, addr_len, client_ip, client_port);

        if(is_gpcli_ip_valid(client_ip) == 0) {
            close(cfd);
            printf("invalid gpclient ip: %s, refused!\n", client_ip);
            continue;
        }

        if(curr_gpclients > max_gpcli_count) {
            close(cfd);
            printf("Refuse connection from %s:%s, curr_clients %d, max %d\n", 
                client_ip, client_port, curr_gpclients, max_gpcli_count);
            continue;
        } 
        
        printf("Connecting from %s:%s\n", client_ip, client_port);
    }
    
    /* Handle each client requet in a new child process */
    switch(fork()) {
    //switch(0) {              // use this line if need debug
        /* Error happen */
        case -1:
            printf("Can't create child (%s)\n", strerror(errno));
            close(cfd);
            break;

        /* Child */
        case 0:  
            close(lfd);                /* Unneeded copy of listening socket */
            handle_clientreq(cfd, client_ip, client_port);
            break;

        /* Parent */
        default:
            curr_gpclients++;
            close(cfd);               /* Unneeded copy of connected socket */
            break;                    /* Loop to accept next connection */
    }
}
```
- fork出子进程后，需要在父进程关闭`accept`新建的socket描述符，在子进程关闭listen socket描述符（因为父进程一直在listen中）
