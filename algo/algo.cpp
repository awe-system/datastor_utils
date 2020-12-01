#include <iostream>
#include "algo.h"
using namespace std;
using namespace ServerSan_Algo;

namespace ServerSan_Algo
{
env disk_info_sec("awe_algo","disk_info_sec");
env pool_vols_sec("awe_algo","pool_vols_sec");
env pool_info_sec("awe_algo","pool_info_sec");

std::string error_str(int err)
{
    switch ( abs(err))
    {
        case ERROR_TYPE_OK:
            return std::string("OK\n");
        case ERROR_TYPE_NOTSUPPORT:
            return std::string("Not Support\n");
        case ERROR_TYPE_UNKNOWN:
        default:
            return std::string("Unkown error\n");
    }
}
};

#ifdef TEST_ALGO

class algo_metadata_callback_service:public algo_metadata_callback
{
	void request_done(request_t *request, int err)
	{
		cout<<"algo_metadata_callback_service request_done"<<endl;
	}

	int handle_offline(block_event *block)
	{
		cout<<"algo_metadata_callback_service handle_offline"<<endl;
		return 0;
	}
	int handle_online(block_event *block)
	{
		cout<<"algo_metadata_callback_service handle_online"<<endl;
		return 0;
	}
};

int main(int argc, char const *argv[])
{
	algo_cluster cluster;
	string pool_name = "pool1";
	string vol_name = "vol";
	algo_metadata_callback_service callback_service;
	algo_device_list_t algo_device_list;
	algo_jbod_info info(pool_name,algo_device_list);
	int err = cluster.create_pool(pool_name,(algo_pool_info*)&info);
	if(err) cout<<ServerSan_Algo::error_str(err)<<endl;
	
	algo_pool_operation *pool =  cluster.get_pool(pool_name);
	if(pool){
		cout<<"Create Pool Success"<<endl;
	} 
	else cout<<"Create Pool Failed"<<endl;

	err = pool->create_vol(vol_name,1024,&callback_service);
	if(err) cout<<ServerSan_Algo::error_str(err)<<endl;
	
	algo_vol_operation *vol =  pool->get_vol(vol_name);
	if(vol){
		cout<<"Create Vol Success"<<endl;
	} 
	else cout<<"Create Vol Failed"<<endl;


	return 0;
}


#endif