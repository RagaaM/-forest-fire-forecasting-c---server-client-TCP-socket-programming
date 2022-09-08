#include "pch.h"
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <time.h>
#pragma warning(disable : 4996)
#pragma comment (lib, "ws2_32.lib")

using namespace std;

string tempValue() {
	//srand(time(0));
	float num = 20 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30 - (20))));

	string str = to_string(num);
	return str;
}

void main()
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	// Close listening socket
	closesocket(listening);


	// Do-while loop to send and receive data
	char buf[4096];
	//string userInput;

	//do
	//{
	//	// Prompt the user for some text
	//	cout << "> ";
	//	getline(cin, userInput);

	//	if (userInput.size() > 0)		// Make sure the user has typed in something
	//	{
	//		// Send the text
	//		int sendResult = send(clientSocket, userInput.c_str(), userInput.size() + 1, 0);
	//		if (sendResult != SOCKET_ERROR)
	//		{
	//			// Wait for response
	//			ZeroMemory(buf, 4096);
	//			int bytesReceived = recv(clientSocket, buf, 4096, 0);
	//			if (bytesReceived > 0)
	//			{
	//				// Echo response to console
	//				cout << "client>/server: " << string(buf, 0, bytesReceived) << endl;
	//			}
	//		}
	//	}

	//} while (userInput.size() > 0);

	while (true)
	{
		

		string value = tempValue();
		int sendResult = send(clientSocket, value.c_str(), tempValue().size() + 1, 0);
		//if (sendResult != SOCKET_ERROR)
		//{
		//	// Wait for response
		//	ZeroMemory(buf, 4096);
		//	int bytesReceived = recv(clientSocket, buf, 4096, 0);
		//	if (bytesReceived > 0)
		//	{
		//		// Echo response to console
		//		cout << "client>/server: " << string(buf, 0, bytesReceived) << endl;
		//	}
		//}
		const time_t ttime = time(0);
		tm *local_time = localtime(&ttime);

		cout << "tempvalue each sec: " << value << "   " ;
		cout << "Time: " << 1 + local_time->tm_hour << ":";
		cout << 1 + local_time->tm_min << ":";
		cout << 1 + local_time->tm_sec << endl;

		Sleep(1000);
		
	}
	
	

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");
}