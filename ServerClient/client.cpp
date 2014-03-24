#include <assert.h>
#include <iostream>
#include <unistd.h>

#include <omniORB4/CORBA.h>
#include "master.h"

using namespace std;
using namespace master;

typedef long long key_t;


bool connect(server_i_var &server, instance_i_var &conn, key_t &key, bounded_string_t_var &peer)
{
	try {
		conn = server->connect(peer, key);
	} catch (const server_i::connection_e &ex) {
		cerr << ex.cause << endl;
		return false;
	}
	cout << "Your connection key is: " << key << endl;
	return true;
}
void wait_for_idle(instance_i_var &conn)
{
	// tell server we are ready
	conn->ready(true);
	cout << "waiting ";
	
	// wait until server becomes idle
	while (!conn->idle()) {
		cout << ".";
		cout.flush();
		sleep(1);
	}
	cout << endl;
}
bool make_request(instance_i_var &conn, key_t &key, bounded_string_t_var &peer)
{
	octet_sequence_t_var stream;
	count_t count;
	count.long_long_value(key);			// initialize with previously acquired key-vaulue

	try {
		conn->get_status(peer, count, stream);
	} catch (const instance_i::protocol_e &ex) {
		cerr << ex.cause << endl;
		return false;
	}
	
	::CORBA::Long index = 0;
	switch(count._d())
	{
		case vshort: index = count.short_value(); break;
		case vlong: index = count.long_value(); break;
	}
	
	cout << "count value is: " << index << endl;
	
	request_t req;
	req.index = count;
	req.data <<= ::CORBA::Any::from_octet(stream[index]);
	
	try {
		::CORBA::Boolean result = conn->request(req);
		cout << "Result of request: " << result << endl;
	} catch (const instance_i::protocol_e &ex) {
		cerr << ex.cause << endl;
		return false;
	}
}

int main (int iArgC, char *apArgV [])
{
	if (iArgC != 2) {
		cout << "Provide server IOR" << endl;	
		return 1;
	}
	
	try
	{
		// corba initialization
		CORBA::ORB_var vORB;
		vORB = CORBA::ORB_init (iArgC, apArgV);

		CORBA::Object_var serverBase;
		server_i_var server;

		// acquire reference to server
		serverBase = vORB->string_to_object (apArgV [1]);
		server = server_i::_narrow (serverBase);
		
		// ping server
		short pong = server->ping(123);
		cout << "Server ping resopnse: " << pong << endl;
		
		// create a connection to the server (a unique key is returned for the connection)
		key_t key = 1038209528;
		bounded_string_t_var peer = CORBA::string_dup("kovaco");
		instance_i_var conn;
		if (!connect(server, conn, key, peer))
			return 1;

		// wait for server to be idle - wait actively by poolig
		wait_for_idle(conn);
		
		// finally make desired request to server
		make_request(conn, key, peer);

		// release connection
		conn->disconnect();

		// free corba resources
		vORB->destroy ();
	}
	catch (const CORBA::SystemException &sEx)
	{
		cerr << sEx._name() << endl;
	}
}
