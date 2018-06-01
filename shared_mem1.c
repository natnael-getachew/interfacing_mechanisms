#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    int shm_id;
    size_t size = sizeof(char)*26;
    int shmflg = IPC_CREAT | 0666;

    key_t key = 12345;
    char *char_value;

    shm_id = shmget(key, size, shmflg);
    if(shm_id == -1){
        perror("ERROR: shmget has failed\n");
        exit(0);
    }
    char_value = shmat(shm_id, NULL, 0);
    if(char_value == (void *)-1){
        perror("ERROR: shmat has failed\n");
        exit(0);
    }

    for(int i=0;i<26;i++){
        *(char_value+i) = (char)(65+i);
    }

    printf("The value stored by the first process is %s\n", char_value);

    while(*char_value == 'A')
        sleep(1);
    printf("The value modified by the second process is %s\n",char_value);
    
    
    if(shmdt(char_value) == -1){
        perror("ERROR: shmdt has failed \n");
    }

    shmctl(shm_id,IPC_RMID,NULL);

    return 0;
}