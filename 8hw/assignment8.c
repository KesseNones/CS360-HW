//Jesse A. Jones
//9 Nov, 2022
//CS 360
//Assignment VIII
//Version: 1.11

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

#define PORTNUM 49999

//Represents one client address and how many times it has occured.
typedef struct nd{
    int count;
    char *address;
}AddrNode;

//Represents array of client addresses 
// and the overall number of addresses contacted.
typedef struct adrCnt{
    int numAddresses;
    AddrNode **addrArr;
}AddressCount;

AddressCount *createAddressStruct(){
    //This function creates the AddressCount Struct.

    AddressCount *adStrct = (AddressCount *)malloc(sizeof(AddressCount));

    //Error checks creation of AddressCount struct.
    if (adStrct == NULL){
        return NULL;
    }
    adStrct->numAddresses = 0;
    adStrct->addrArr = NULL;

    return adStrct;
}

AddrNode *createNode(int count, char *address){
    //This function creates a node that contains 
    // a client address and the number of times it was called.

    AddrNode *node = (AddrNode *)malloc(sizeof(AddrNode));

    //Error checks initial node creation.
    if (node == NULL){
        return NULL;
    }

    node->count = 1;
    node->address = strdup(address);

    //Error checks strdup.
    if (node->address == NULL){
        free(node);
        return NULL;
    }

    return node;
}

int resizeArray(AddressCount *addStruct){
    //Increases size of array by 1, 
    // making room for a new address.

    AddrNode **newArray = (AddrNode **)calloc((addStruct->numAddresses + 1), sizeof(AddrNode *));

    //Error checks calloc.
    if (newArray == NULL){
        return 1;
    }

    //Transfers contents of old array to new array.
    for (int i = 0; i < addStruct->numAddresses; i++){
        newArray[i] = addStruct->addrArr[i];
        addStruct->addrArr[i] = NULL;
    }

    //Attaches new array to the struct that was passed in.
    free(addStruct->addrArr);
    addStruct->addrArr = newArray;

    return 0;
}

int insertAddress(AddressCount *addStruct, char *address){
    //Iterates through array of addresses 
    // and inserts a new node or updates 
    // an existing node's count if address is already present.
    //Returns index of address that was inserted or updated.
    //Returns -1 on error.

    AddrNode *node;

    int resizeResult, comp;

    //Handles case of no recorded addresses.
    if (addStruct->addrArr == NULL){
        addStruct->addrArr = (AddrNode **)calloc(1, sizeof(AddrNode *));
        
        //Error checks allocation of array of node pointer array.
        if (addStruct->addrArr == NULL){
            return -1;
        }

        node = createNode(1, address);

        //Error checks node creation.
        if (node == NULL){
            return -1;
        }

        addStruct->addrArr[0] = node;
        addStruct->numAddresses++;

        return 0;

    }else{

        //Looks for matching address in array. 
        // If match is found, count is incremented.
        for (int i = 0; i < addStruct->numAddresses; i++){
            comp = strcmp(address, addStruct->addrArr[i]->address);
            if (comp == 0){
                addStruct->addrArr[i]->count++;
                return i;
            }
        }

        resizeResult = resizeArray(addStruct);

        //Error checks array resizing.
        if (resizeResult == 1){
            return -1;
        }

        node = createNode(1, address);

        //Error checks node creation.
        if (node == NULL){
            return -1;
        }

        //Inserts new address node to end 
        // of array and increases count of addresses.
        addStruct->addrArr[addStruct->numAddresses] = node;
        addStruct->numAddresses++;

        return (addStruct->numAddresses - 1);
    }

}

void delAddressArray(AddressCount *addStruct){
    //This function iterates through and deletes the array 
    // of client addresses and all other associated data.
    //This function is not directly used but would be useful 
    // in managing this basic data structure.

    //Frees all address pointers and then 
    // all node pointers in client address array.
    for (int i = 0; i < addStruct->numAddresses; i++){
        free(addStruct->addrArr[i]->address);
        free(addStruct->addrArr[i]);
    }

    free(addStruct->addrArr);
    free(addStruct);

}

int main(int argc, char const *argv[]){
    int errnoTemp;

    //Error checks for no arguments.
    if (argc < 2){
        fprintf(stderr, "Error: %s\n", "No arguments given.");
        fflush(stderr);
        exit(1);
    }

    //Error checks too many arguments.
    if (argc > 3){
        fprintf(stderr, "Error: %s\n", "Too many arguments given.");
        fflush(stderr);
        exit(1);
    }

    //Handles client case of input.
    if (strcmp(argv[1], "client") == 0){
        
        int socketfd;
        struct addrinfo hints, *actualdata;
        memset(&hints, 0, sizeof(hints));
        int err;
        char readDateString[19] = {'\0'};

        hints.ai_socktype = SOCK_STREAM;
        hints.ai_family = AF_INET;

        err = getaddrinfo(argv[2],
                        "49999",
                        &hints,
                        &actualdata);

        //Error checks getting address information.
        if (err != 0){
            errnoTemp = err;
            fprintf(stderr, "Error: %s\n", gai_strerror(errnoTemp));
            fflush(stderr);
            exit(errnoTemp);
        }

        socketfd = socket(actualdata->ai_family, 
            actualdata->ai_socktype, 0);

        //Error checks socket.
        if (socketfd == -1){
            errnoTemp = errno;
            fprintf(stderr, "Error: %s\n", strerror(errnoTemp));
            fflush(stderr);
            exit(errnoTemp);
        }

        err = connect(socketfd, 
                    actualdata->ai_addr, 
                    actualdata->ai_addrlen);

        //Error checks connecting to server.
        if (err < 0){
            errnoTemp = errno;
            fprintf(stderr, "Error: %s\n", strerror(errnoTemp));
            exit(errnoTemp);
        }

        //Reads date written to socket and writes date string to stdout.
        read(socketfd, readDateString, 19);
        write(1, readDateString, 19);
        fflush(stdout);

        close(socketfd);
    }

    //Handles server case of input.
    else if (strcmp(argv[1], "server") == 0){
        
        //Error checks to see if too many arguments were given to server.
        if (argc > 2){
            fprintf(stderr, "Error: %s\n", "Too many arguments given.");
            fflush(stderr);
            exit(1);  
        }

        int listenfd, bindResult;

        listenfd = socket(AF_INET, SOCK_STREAM, 0);

        //Error checks socket.
        if (listenfd == -1){
            errnoTemp = errno;
            fprintf(stderr, "Error: %s\n", strerror(errnoTemp));
            fflush(stderr);
            exit(errnoTemp);
        }

        //Options set so socket can be cleared 
        // and made available in the event 
        // of a server being murdered.
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
            &(int){1}, sizeof(int));

        //Sets up server address struct and all its members.
        struct sockaddr_in serverAddress;
        memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(PORTNUM);
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

        bindResult = bind(listenfd, (struct sockaddr *)&serverAddress, 
            sizeof(serverAddress));

        //Error checks binding of address to port.
        if (bindResult < 0){
            perror("bind");
            exit(1);
        }

        pid_t forkResult;

        AddressCount *connectedAddresses = createAddressStruct();

        //Error checks connected client address array creation.
        if (connectedAddresses == NULL){
            fprintf(stderr, "Error: %s\n", 
                "Failed to create client address array, exiting...");
            fflush(stderr);
            exit(1);
        }

        while(1){

            //Creates connection queue of size 1.
            listen(listenfd, 1);

            int connectfd; 
            unsigned int length = sizeof(struct sockaddr_in);
            struct sockaddr_in clientAddress;

            //Creates file descriptor for given client's connection.
            connectfd = accept(listenfd, 
                                (struct sockaddr *) &clientAddress,
                                &length);

            //Error checks connection to client.
            if (connectfd == -1){
                errnoTemp = errno;
                fprintf(stderr, "Error: %s\n", strerror(errnoTemp));
                fflush(stderr);
                exit(errnoTemp);
            }

            forkResult = fork();

            //Error checks creation of child representing client.
            if (forkResult == -1){
                errnoTemp = errno;
                fprintf(stderr, "Error: %s\n", strerror(errnoTemp));
                fflush(stderr);
                close(connectfd);
                exit(errnoTemp);
            }

            //Child process gets time string and writes it to client.
            if (forkResult == 0){
                char *timeString;
                char writeTimeString[19] = {'\0'};
                time_t unixTime;

                //Creates time string to be written over to client.
                unixTime = time(NULL);
                timeString = ctime(&unixTime);

                //Derives new time string that is properly formatted.
                strncpy(writeTimeString, timeString, 19);
                writeTimeString[18] = '\n';

                //Writes formatted string to client.
                write(connectfd, writeTimeString, 19);

                exit(0);

            //Parent end handles data structure interface and printing 
            // of client hostname with connection count of given name.
            }else{
                int insertResult;
                char clientHostName[NI_MAXHOST];
                int hostEntry;

                //Gets client host name from client.
                hostEntry = getnameinfo((struct sockaddr *)&clientAddress,
                            sizeof(clientHostName), 
                            clientHostName,
                            sizeof(clientHostName),
                            NULL,
                            0,
                            NI_NUMERICSERV);

                //Error checks aquisition of host info from client.
                if (hostEntry != 0){
                    errnoTemp = hostEntry;
                    fprintf(stderr, "Error: %s\n", gai_strerror(hostEntry));
                    fflush(stderr);
                    close(connectfd);
                    exit(errnoTemp);
                }

                insertResult = insertAddress(connectedAddresses, clientHostName);

                //Error checks insertion of client host name.
                if (insertResult == -1){
                    fprintf(stderr, "Error: %s\n", 
                        "Failed to insert address into host name array.");
                    fflush(stderr);
                    close(connectfd);
                    exit(1);
                } 

                fprintf(stdout, "%s %d\n", 
                    connectedAddresses->addrArr[insertResult]->address, 
                    connectedAddresses->addrArr[insertResult]->count);
                fflush(stdout);

                //Waits for child to finish and cloeses connection to client.
                wait(NULL);
                close(connectfd);
            }
        }
    }

    //Handles case of invalid first argument. 
    else{
        fprintf(stderr, "Error: %s\n", 
            "Incorrect first argument given.");
        fflush(stderr);
        return 1;
    }
    return 0;
}