#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <iostream>
using namespace std;

//#include <sys/ipc.h>
//#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>

class SharedMemory
{
private:
    int shmid;
    key_t key;
    char *data;
    sem_t *semaphore;
    size_t size;
    bool isOwner;
    char *cpData;
    int fd;
    std::string _name;
    std::string sem_name;

public:
    //SharedMemory(key_t key, size_t size, bool bOwner=true);

    SharedMemory(std::string name, size_t size, bool bOwner=true)
        :  size(size), isOwner(bOwner)
    {
        key = atoi(_name.c_str());
        // Convert key to string for POSIX IPC
//        sem_name = "/sem_" + name;
//        this->_name = "/sem_" + name;
        sem_name =  name;
        this->_name = name;

        if (bOwner)
        {
            // Open shared memory with O_CREAT flag
            fd = shm_open(name.c_str(), O_CREAT | O_RDWR, 0666);
            if (fd == -1)
            {
                perror("shm_open failed for owner");
                exit(1);
            }

            // Set the size of the shared memory
            ftruncate(fd, size);

            // Create the named semaphore
            semaphore = sem_open(sem_name.c_str(), O_CREAT, 0666, 1);
            if (semaphore == SEM_FAILED)
            {
                perror("sem_open failed for owner");
                exit(1);
            }
        }
        else
        {
            // Open shared memory without O_CREAT flag
            fd = shm_open(name.c_str(), O_RDWR, 0666);
            if (fd == -1)
            {
                perror("shm_open failed for non-owner");
                exit(1);
            }

            // Open the existing named semaphore
            semaphore = sem_open(sem_name.c_str(), 0);
            if (semaphore == SEM_FAILED)
            {
                perror("sem_open failed for non-owner");
                exit(1);
            }
        }

        // Map the shared memory
        data = (char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (data == MAP_FAILED)
        {
            perror("mmap");
            exit(1);
        }
    }

    ~SharedMemory()
    {
        if (data)
        {
            munmap(data, size);  // Unmap the shared memory
        }

        if (fd >= 0 && isOwner)
        {
            close(fd);
            shm_unlink(_name.c_str());  // Remove the shared memory
        }

        //std::string sem_name = "/sem_" + name;
        if (semaphore != SEM_FAILED)
        {
            sem_close(semaphore);
            if (isOwner)
            {
                sem_unlink(sem_name.c_str());
            }
        }

        destroySemaphore();
    }

    void destroySemaphore() {
        sem_t *temp_semaphore = sem_open(sem_name.c_str(), 0);
        if (temp_semaphore != SEM_FAILED) {
            sem_close(temp_semaphore);
            sem_unlink(sem_name.c_str());
        }
    }
/*
    void InitRetry() {
        if(data != nullptr)
            return;

        if ((shmid = shmget(key, size, 0)) < 0) {
            perror("shmget get");
            //exit(1);
        }
        else
        {
            if ((data = (char*)shmat(shmid, NULL, 0)) == (char*)-1) {
                perror("shmat");
                //exit(1);
            }
        }

        if(data != nullptr){
            semaphore = (sem_t*)data;
            data += sizeof(sem_t);

            // Initialize the semaphore
            sem_init(semaphore, 1, 1);
        }
    }
    bool IsHaveShareMem()
    {
        return (data != nullptr);
    }
*/

    std::string keyToString() {
        return std::to_string(key);
    }

    void writeData(const char *str) {
        // 길이 검사
        size_t len = strlen(str);
        if (len + 1 > size) {  // +1 to account for the null terminator
            std::cerr << "Input string is too large for shared memory!" << std::endl;
            return;
        }

        sem_t *named_semaphore = sem_open(sem_name.c_str(), 0);
        if (named_semaphore == SEM_FAILED) {
            perror("sem_open failed");
            return;
        }

        try {
            sem_wait(named_semaphore);
            memcpy(data, str, len + 1); // +1 to copy the null terminator
        } catch (const std::exception &e) {
            std::cerr << "Exception while writing to shared memory: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception while writing to shared memory." << std::endl;
        }

        sem_post(named_semaphore);
        sem_close(named_semaphore);

        std::cout << "WriteData MemKey : " << keyToString() << ", Memory Contents :" << data << std::endl;
    }

    void readData() {
        if(!data) {
            std::cout << "Not Connected ShareMemory" << std::endl;
            return;
        }

        sem_t *named_semaphore = sem_open(sem_name.c_str(), 0);
        if (named_semaphore == SEM_FAILED) {
            perror("sem_open failed");
            return;
        }

        try {
            sem_wait(named_semaphore);
            std::cout << "ReadData MemKey : " << keyToString() << ", Memory Contents :" << data << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Exception while reading from shared memory: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception while reading from shared memory." << std::endl;
        }

        sem_post(named_semaphore);
        sem_close(named_semaphore);
    }


    char* getData() {
        return data;
    }

/*
    ~SharedMemory() {
        if (data) {
            shmdt(data); // Detach from shared memory
        }

        if (shmid >= 0 && isOwner) {
            if (shmctl(shmid, IPC_RMID, NULL) == -1) {
                perror("shmctl remove");
                exit(1);
            }
        }

        std::string sem_name = "/sem_" + std::to_string(key);
        if (semaphore != SEM_FAILED) {
            sem_close(semaphore);
            if (isOwner) {
                sem_unlink(sem_name.c_str());
            }
        }
    }
*/
};

#endif // SHAREDMEMORY_H
