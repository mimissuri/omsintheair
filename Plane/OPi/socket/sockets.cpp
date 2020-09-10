#include "Poco/ConsoleChannel.h"

#include "Poco/Thread.h"

#include "SIOClient.h"
#include "TestTarget.h"
#include "TestEndpointTarget.h"

#include <iostream>

using Poco::Thread;

int main(int argc, char* argv[])
{
	logger->setChannel(new Poco::ConsoleChannel());

	
	//Establish the socket.io connection
	//JS: var socket = io.connect("localhost:3000")
	SIOClient *sio = SIOClient::connect("http://localhost:8080");

	//Create a target and register object its method onUpdate for the Update event
	//JS: socket.on("Update", function(data) {...});
	TestTarget *target = new TestTarget();
	sio->on("message", target, callback(&TestTarget::onMessage));
	
	//setup is now complete, messages and events can be send and received
	logger->information("Socket.io client setup complete\n");

	//test the message sending
	logger->information("Sending message string");
	sio->send("Message - String");
  logger->information("Sending message json\n");
	sio->send("{\"name\":\"myname\",\"type\":\"mytype\"}");

	//test the event emitter, this will return the same event so let's register a callback too
	sio->on("chat", target, callback(&TestTarget::onEvent));
  logger->information("Emit \"chat\" event with string");
	sio->emit("chat","Event - String");
  logger->information("Emit \"chat\" event with json");
	sio->emit("chat", "[{\"name\":\"myname\",\"type\":\"mytype\"}]");
	
	//test connecting to an endpoint 'testpoint'
	//~ TestEndpointTarget *target2 = new TestEndpointTarget();
	//~ SIOClient *testpoint = SIOClient::connect("http://localhost:3000/testpoint");
	//~ testpoint->send("Hello Socket.IO Testpoint");
	//~ testpoint->on("Update", target2, callback(&TestEndpointTarget::onUpdate));
	//~ testpoint->on("testevent", target2, callback(&TestEndpointTarget::ontestevent));
	//~ testpoint->emit("testevent", "[{\"name\":\"myname\",\"type\":\"mytype\"}]");

	//wait for user input to move to next section of code
	//socket receiving occurs in another thread and will not be halted
	logger->information("Press ENTER to continue...");
	std::cin.get();

	//test disconnecting a single endpoint, other endpoints stay connected
	//~ testpoint->disconnect();

	//disconnecting the default socket with no endpoint will also disconnect all endpoints
	sio->disconnect();

	logger->information("Press ENTER to quit...");
	std::cin.get();

	return 0;
}