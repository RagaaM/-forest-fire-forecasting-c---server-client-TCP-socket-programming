#include "pch.h"
#include <iostream>
#include <string>
#include <time.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

using namespace std;



void main()
{
	string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 54000;						// Listening port # on the server

		
	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// While loop: accept and echo message back to client
	char buf[4096];

	//while (true)
	//{
	//	ZeroMemory(buf, 4096);

	//	// Wait for server to send data
	//	int bytesReceived = recv(sock, buf, 4096, 0);
	//	if (bytesReceived == SOCKET_ERROR)
	//	{
	//		cerr << "Error in recv(). Quitting" << endl;
	//		break;
	//	}

	//	if (bytesReceived == 0)
	//	{
	//		cout << "server disconnected " << endl;
	//		break;
	//	}
	//	/*cout << "avg : " << avg() << endl;
	//	cout << "Accumulation: " << Accum() << endl;*/
	//	cout << string(buf, 0, bytesReceived) << endl;
	//	

	//	// Echo message back to server
	//	send(sock, buf, bytesReceived + 1, 0);

	//}
	float sum= 0;
	int i = 0;
	while (true)
	{
		
		ZeroMemory(buf, 4096);
		int bytesReceived;
		// Wait for server to send data
		for (int r = 0; r < 5; r++) {
			bytesReceived = recv(sock, buf, 4096, 0);
			float temp = stof(buf);
			//cout << temp;
			sum += temp; 
			i++;
			Sleep(1000);
		}
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}

		if (bytesReceived == 0)
		{
			cout << "server disconnected " << endl;
			break;
		}
		
		//cout << "buf after int casting--> " << temp << endl;
		float avg = sum / i;
		cout << "avg : " << avg << endl;
		cout << "Accumulation: " << sum << endl;
		const time_t ttime = time(0);
		tm *local_time = localtime(&ttime);

		cout << "Time: " << 1 + local_time->tm_hour << ":";
		cout << 1 + local_time->tm_min << ":";
		cout << 1 + local_time->tm_sec << endl;
		cout << "-----------------------------------" << endl;
		//cout << string(buf, 0, bytesReceived) << endl;
		//Sleep(5000);

		// Echo message back to server
		//send(sock, buf, bytesReceived + 1, 0);

	}

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}
