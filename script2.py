import sysv_ipc, time

class CShmReader :
	def __init__(self, key, size):
		self.key = key
		self.size = size
		self.memory = sysv_ipc.SharedMemory(key=self.key)
		self.semaphore = sysv_ipc.Semaphore(key=self.key, flags=sysv_ipc.IPC_CREAT, mode=0o666, initial_value=1)
		self.counter = 1

	def doReadShm(self):
    		self.semaphore.acquire()
		offset = sizeof_sem_t  # 여기서 sizeof_sem_t는 세마포어의 크기. 대개는 32바이트지만, 환경에 따라 달라질 수 있습니다.
    		memory_value = self.memory.read()[offset:]  # 세마포어 데이터를 건너뛴 후부터 읽습니다.
    		self.semaphore.release()
		print("I got:", memory_value.rstrip(b'\0'))
		self.semaphore.acquire()
		#memory_value = self.memory.read(self.size)
		memory_value = self.memory.read()  
		self.semaphore.release()
		#print("I got:", memory_value.decode().rstrip('\0'))
		print("I got:", memory_value.rstrip(b'\0'))
		#print("I got:", memory_value.decode())

	def doWriteShm(self):
		text = "From Python Client - " + str(self.counter)
		self.semaphore.acquire()
		self.memory.write(text.encode())
		self.semaphore.release()
		print("I sent:", text)
		self.counter += 1

if __name__ == "__main__":
	you = CShmReader(777, 120 + sysv_ipc.PAGE_SIZE, false)
	my = CShmReader(1024, 120 + sysv_ipc.PAGE_SIZE)

	while True:
		time.sleep(2)
	    	print(f"Python")
		time.sleep(2)
		print(f"Python alive")

		line = sys.stdin.readline().strip()  # Read line from stdin
	    	if line == "exit":
			print(f"Good Bye!! Exit Command received: {line}")
			break
	    	print(f"Python received: {line}")

		reader.doReadShm()
		#reader.doWriteShm()		
		my.doWriteShm()

		print("Python Answer")

