#pragma once
#include "Poco/Net/TCPServer.h"
#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Util/Application.h>
#include <Poco/Util/ServerApplication.h>


using namespace Poco::Net;
using namespace Poco::Util;

class TestConnection: public TCPServerConnection
{
public:
	TestConnection(const StreamSocket& s): 
	  TCPServerConnection(s)
	  {
		  socket().setBlocking(false);
	  }
	  void run()
	  {
		  StreamSocket& ss = socket();
		  try
		  {
			  char buffer[256];
			  int n = ss.receiveBytes(buffer, sizeof(buffer));
			  while (n > 0)
			  {
				  ss.sendBytes(buffer, n);
				  n = ss.receiveBytes(buffer, sizeof(buffer));
				  std::cout << "recv id: " << GetCurrentThreadId()<< std::endl;
			  }
		  }
		  catch (Poco::Exception& exc)
		  {
			  std::cerr << "EchoConnection: " << exc.displayText() << std::endl;
		  }
	  }
};

class TestConnectionFactory: public TCPServerConnectionFactory
{
public:
	TCPServerConnection* createConnection(const StreamSocket& socket)
	{
		TestConnection* tc = new TestConnection(socket);
		return tc;
	}
};

//ServerApplication
class Server: public ServerApplication
{
protected:
	void initialize(Application& self)
	{
		loadConfiguration();
		ServerApplication::initialize(self);
	}

	void uninitialize()
	{
		ServerApplication::uninitialize();
	}

	int main(const std::vector<std::string>& args)
	{
		ServerSocket serverSock(7000);
		TCPServer tcpServer(new TestConnectionFactory(), serverSock);
		tcpServer.start();
		waitForTerminationRequest();
		tcpServer.stop();
		return Application::EXIT_OK;
	}
};

class CLibPOCO
{
public:
	CLibPOCO(void);
	~CLibPOCO(void);
public:
	void Test(int argc, char **argv);
};
