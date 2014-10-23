#include <iostream>
#include <omniORB4/CORBA.h>
#include "master.h"

using namespace std;
using namespace master;

class instance_i_impl : public POA_master::instance_i
{
	private:
	::CORBA::Boolean is_ready;
	::CORBA::Boolean is_idle;
	
	public:
	void get_status(const char* s_key, ::master::count_t& cnt, ::master::octet_sequence_t_out status)
	{
		
	}
    //::CORBA::Boolean request(const ::master::request_t& req);
    void disconnect()
    {
		// TODO: delete this connection
	}
    ::CORBA::Boolean idle()
    {
		::CORBA::Boolean result = is_idle;
		if (is_ready)
			 is_idle = true;
		return result;
	}
    ::CORBA::Boolean ready() { return is_ready; }
    void ready(::CORBA::Boolean _v) 
    { 
		is_ready = _v;
	}
	
	instance_i_impl() : is_ready(false), is_idle(false)
	{
	}
};
/*class server_i_impl : public POA_master::server_i
{
	public:
	::CORBA::Short ping(::CORBA::Short val)
	{
		return ++val;
	}
	instance_i_ptr connect(::CORBA::String_INOUT_arg peer, ::CORBA::LongLong& key) throw (POA_master::connection_e)
	{
		return instance_i_ptr(new instance_i_impl());
	}
	
};*/

int main (int iArgC, char *apArgV [])
{
	try
	{
		CORBA::ORB_var vORB;

		vORB = CORBA::ORB_init (iArgC, apArgV);

		CORBA::Object_var vRootPOABase;
		PortableServer::POA_var vRootPOA;
		PortableServer::POAManager_var vRootPOAManager;

		vRootPOABase = vORB->resolve_initial_references ("RootPOA");
		vRootPOA = PortableServer::POA::_narrow (vRootPOABase);
		vRootPOAManager = vRootPOA->the_POAManager ();

		/*server_i_impl *pMasterServer = new server_i_impl ();
		server_i_var vServer = pMasterServer->_this ();

		cout << vORB->object_to_string (vServer) << endl;
		
		vRootPOAManager->activate ();
		vORB->run ();*/
	}
	catch (const CORBA::SystemException &sEx)
	{
		cerr << sEx._name() << endl;
	}
}
