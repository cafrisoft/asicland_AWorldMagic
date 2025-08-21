#include <GlobalBuildOptions.h>

#include "sharedmemory.h"

#include <fcntl.h>
#include <sys/stat.h>

/*
SharedMemory::SharedMemory(key_t key, size_t size, bool bOwner)
{
    this->key = key;
    this->size = size;
    data = nullptr;
    isOwner = bOwner;

    cpData = new char[size];

    // Convert key to string for named semaphore
    std::string sem_name = "/sem_" + std::to_string(key);

    if(bOwner)
    {
        destroySemaphore(sem_name); // 기존 세마포어 파괴

        shmid = shmget(key, size, 0666);

        if (shmid >= 0) {
            // If it exists, delete it
            if (shmctl(shmid, IPC_RMID, NULL) == -1) {
                perror("shmctl remove");
                exit(1);
            }
        }

        if ((shmid = shmget(key, size, IPC_CREAT | 0666)) < 0) {
            perror("shmget create");
            exit(1);
        }
        else
        {
            if ((data = (char*)shmat(shmid, NULL, 0)) == (char*)-1) {
                perror("shmat");
                exit(1);
            }
        }

        // Create the named semaphore
        semaphore = sem_open(sem_name.c_str(), O_CREAT, 0666, 1);
        if (semaphore == SEM_FAILED) {
            perror("sem_open failed");
            std::cerr << "Error number: " << errno << std::endl;
            exit(1);
        }
    }
    else
    {
        if ((shmid = shmget(key, size, 0)) < 0) {
            perror("shmget get");
            exit(1);
        }
        else
        {
            if ((data = (char*)shmat(shmid, NULL, 0)) == (char*)-1) {
                perror("shmat");
                exit(1);
            }
        }

        // Open the existing named semaphore
        semaphore = sem_open(sem_name.c_str(), 0);
        if (semaphore == SEM_FAILED) {
            perror("sem_open");
            exit(1);
        }
    }

    if(data != nullptr){
        // Use the semaphore as before
    }
}

*/
