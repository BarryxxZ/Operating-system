#include<iostream>
#include<ctime>
#include<cstdlib>
#include<Windows.h>

using namespace std;

constexpr auto BUFFER_SIZE = 10;
int buffer[BUFFER_SIZE];
bool isfull;
int front, tail;

void printBuffer() {
	cout << "Buffer = ";
	if (front != tail || isfull) {
		int temp = front;
		do {
			cout << buffer[temp] << " ";
			temp = (temp + 1) % BUFFER_SIZE;
		}while (temp != tail);
	}
	else {
		cout << "Empty!!";
	}
	cout << endl;
	cout << "************************" << endl;
}

int produce(int _item) {
	int flag = -1;
	if (!isfull) {
		buffer[tail] = _item;
		tail = (tail + 1) % BUFFER_SIZE;
		if (tail == front) {
			isfull = true;
		}
		flag = 0;
		cout << "Producer produces " << _item << endl;
		printBuffer();
	}
	return flag;
}

int consume(int _item) {
	int flag = -1;
	if (front != tail || isfull) {
		_item = buffer[front];
		front = (front + 1) % BUFFER_SIZE;
		isfull = false;
		flag = 0;
		cout << "Consumer consumes " << _item << endl;
		printBuffer();
	}
	return flag;
}

DWORD WINAPI producer(LPVOID lpParameter) {
	srand((unsigned)time(0));
	for (int i = 0; ; i++) {
		produce(rand() % BUFFER_SIZE);
		Sleep((rand() % 100) + 1000);
	}
}

DWORD WINAPI consumer(LPVOID lpParameter) {
	srand((unsigned)time(0));
	for (int j = 0; ; j++) {
		consume(rand() % BUFFER_SIZE);
		Sleep((rand() % 50) + 500);
	}
}

int main(int argc, char *argv[]) {
	isfull = false;
	front, tail = 0;

	HANDLE thread_Producter = CreateThread(NULL, 0, producer, NULL, 0, NULL);
	HANDLE thread_Consumer = CreateThread(NULL, 0, consumer, NULL, 0, NULL);

	Sleep(INFINITE);

	CloseHandle(thread_Producter);
	CloseHandle(thread_Consumer);

	system("pause");
	return 0;
}