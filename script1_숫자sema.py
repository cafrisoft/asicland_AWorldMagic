import sysv_ipc
import time

class CShareMem:
    def __init__(self, key, size):
        self.key = key
        self.size = size
        self.memory = sysv_ipc.SharedMemory(key=self.key)
        self.semaphore = sysv_ipc.Semaphore(key=self.key, mode=0o666)  # IPC_CREAT 플래그를 삭제했습니다.

    def doReadShm(self):
          # 여기서 sizeof_sem_t 값을 실제로 알고 있다면, 그 값을 사용하시길 바랍니다.
        sizeof_sem_t = 32  # 예시 값
        self.semaphore.acquire()
        offset = sizeof_sem_t
        memory_value = self.memory.read()[offset:]  # 세마포어 데이터를 건너뛴 후부터 읽습니다.
        self.semaphore.release()
        print("I got: ", memory_value.rstrip(b'\0'))

    def doWriteShm(self):
          # 여기서 sizeof_sem_t 값을 실제로 알고 있다면, 그 값을 사용하시길 바랍니다.
        sizeof_sem_t = 32  # 예시 값
        text = "Message From Python - " + str(self.counter)
        self.semaphore.acquire()

          # 세마포어의 크기만큼 빈 바이트를 앞에 추가하고 실제 메시지를 그 다음에 추가합니다.
        data_to_write = b'\0' * sizeof_sem_t + text.encode()
        self.memory.write(data_to_write)
    
        self.semaphore.release()
        print("I sent: No ", str(self.counter))
        self.counter += 1


class CShmWriter:
    def __init__(self, key, size):
        self.key = key
        self.size = size
        # Here we ensure that the shared memory is created if it doesn't exist
        self.memory = sysv_ipc.SharedMemory(key=self.key, flags=sysv_ipc.IPC_CREAT, size=self.size)
        self.semaphore = sysv_ipc.Semaphore(key=self.key, flags=sysv_ipc.IPC_CREAT, mode=0o666, initial_value=1)
        self.counter = 1

    def doWriteShm(self):
        text = "Message From Python - " + str(self.counter)
        self.semaphore.acquire()
        self.memory.write(text.encode())
        self.semaphore.release()
        print("I sent: No ", str(self.counter))
        self.counter += 1

if __name__ == "__main__":
    reader = CShareMem(700, sysv_ipc.PAGE_SIZE)
    writer = CShareMem(701, sysv_ipc.PAGE_SIZE)
    writer.doWriteShm()

    while True:
        time.sleep(3)
        reader.doReadShm()
        time.sleep(3)
        writer.doWriteShm()

