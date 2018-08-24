//
// Created by root on 18-5-29.
//
#include <iostream>
using namespace std;
#include "json_obj.h"

const char *test_char = "{\"barrel\":{\"boards\":{\"host0\":{\"copy_len\":17407,\"copy_num\":2,\"main_len\":0,\"total_len\":34816},\"host1\":{\"copy_len\":17408,\"copy_num\":2,\"main_len\":17407,\"total_len\":34816}},\"copy_num\":2},\"vol_map\":{\"vol0\":{\"base\":{\"device_id\":\"poolvol0\",\"sec\":11605,\"vol_name\":\"vol0\"},\"jbods\":[{\"host_name\":\"host0\",\"vol_name\":\"host0vol0$7d9fa191-76be-48c5-af79-7f2a3d2b566c\"},{\"host_name\":\"host1\",\"vol_name\":\"host1vol0$47705c41-6889-4c27-9b5e-1465e636c0f7\"}],\"pieces\":{\"vol0$6925a095-f2df-4084-a61b-444f1d9db094\":{\"name\":\"vol0$6925a095-f2df-4084-a61b-444f1d9db094\",\"request_mapper\":{\"blocks\":[{\"host_name\":\"host0\",\"vol_name\":\"host0vol0$c5271076-2638-4100-8e90-9bc1b1706a6e\"}],\"len\":440361677,\"ranges\":{\"len\":\"11605\",\"vec\":[[\"0\",\"0\",\"11605\"]]}},\"vols\":[{\"host\":\"host0\",\"name\":\"host0vol0$c5271076-2638-4100-8e90-9bc1b1706a6e\"}]},\"vol0$72626e13-3bba-433f-8b5e-fd3227948afe\":{\"name\":\"vol0$72626e13-3bba-433f-8b5e-fd3227948afe\",\"request_mapper\":{\"blocks\":[{\"host_name\":\"host1\",\"vol_name\":\"host1vol0$4ac1d494-f3cc-41cd-974e-db7bc52d4fc7\"}],\"len\":440362805,\"ranges\":{\"len\":\"11605\",\"vec\":[[\"0\",\"0\",\"11605\"]]}},\"vols\":[{\"host\":\"host1\",\"name\":\"host1vol0$4ac1d494-f3cc-41cd-974e-db7bc52d4fc7\"}]}},\"scoop\":[[{\"len\":11605,\"name\":\"host0\"}],[{\"len\":11605,\"name\":\"host1\"}]]},\"vol1\":{\"base\":{\"device_id\":\"poolvol1\",\"sec\":11605,\"vol_name\":\"vol1\"},\"jbods\":[{\"host_name\":\"host0\",\"vol_name\":\"host0vol1$f1178d38-f7b5-46ca-9cb8-3a379a82cdc8\"},{\"host_name\":\"host1\",\"vol_name\":\"host1vol1$5eea3c11-3ecd-4a8a-91e1-94f85f661329\"},{\"host_name\":\"host1\",\"vol_name\":\"host1vol1$4bf8248a-bd4d-4caa-a1ce-9cd05d53cfd9\"},{\"host_name\":\"host0\",\"vol_name\":\"host0vol1$79b0bfc7-0a00-4407-b861-02cbef102acd\"}],\"pieces\":{\"vol1$08e3b360-dfc9-4bdf-aad3-d323857d012f\":{\"name\":\"vol1$08e3b360-dfc9-4bdf-aad3-d323857d012f\",\"request_mapper\":{\"blocks\":[{\"host_name\":\"host1\",\"vol_name\":\"host1vol1$2fba81f3-dcda-4aa3-8141-655cc1ddbdd4\"},{\"host_name\":\"host0\",\"vol_name\":\"host0vol1$8c1062e6-0f7a-4fcf-99af-0f2f7833dcbe\"}],\"len\":440366693,\"ranges\":{\"len\":\"11605\",\"vec\":[[\"0\",\"0\",\"5803\"],[\"5803\",\"0\",\"5802\"]]}},\"vols\":[{\"host\":\"host1\",\"name\":\"host1vol1$2fba81f3-dcda-4aa3-8141-655cc1ddbdd4\"},{\"host\":\"host0\",\"name\":\"host0vol1$8c1062e6-0f7a-4fcf-99af-0f2f7833dcbe\"}]},\"vol1$b8ae75d8-618b-43c2-8871-dfcffd79a69a\":{\"name\":\"vol1$b8ae75d8-618b-43c2-8871-dfcffd79a69a\",\"request_mapper\":{\"blocks\":[{\"host_name\":\"host0\",\"vol_name\":\"host0vol1$6c8a9ccb-2732-44b4-9636-5d7cc71a5144\"},{\"host_name\":\"host1\",\"vol_name\":\"host1vol1$53332ef6-db0c-4722-8dde-7d74db0c53a6\"}],\"len\":23210,\"ranges\":{\"len\":\"11605\",\"vec\":[[\"0\",\"0\",\"5803\"],[\"5803\",\"0\",\"5802\"]]}},\"vols\":[{\"host\":\"host0\",\"name\":\"host0vol1$6c8a9ccb-2732-44b4-9636-5d7cc71a5144\"},{\"host\":\"host1\",\"name\":\"host1vol1$53332ef6-db0c-4722-8dde-7d74db0c53a6\"}]}},\"scoop\":[[{\"len\":5803,\"name\":\"host0\"},{\"len\":5802,\"name\":\"host1\"}],[{\"len\":5803,\"name\":\"host1\"},{\"len\":5802,\"name\":\"host0\"}]]},\"vol2\":{\"base\":{\"device_id\":\"poolvol2\",\"sec\":11605,\"vol_name\":\"vol2\"},\"jbods\":[{\"host_name\":\"host0\",\"vol_name\":\"host0vol2$de84cc4b-cda5-4629-ba3f-3e22fb72545e\"},{\"host_name\":\"host1\",\"vol_name\":\"host1vol2$5187aaa6-8f8e-4dd3-a250-7ccfd56f5413\"}],\"pieces\":{\"vol2$2b975b12-3ce1-4bc3-9cec-a08928da7a9c\":{\"name\":\"vol2$2b975b12-3ce1-4bc3-9cec-a08928da7a9c\",\"request_mapper\":{\"blocks\":[{\"host_name\":\"host0\",\"vol_name\":\"host0vol2$5228d6da-0a8e-4bd9-8920-f563e9c58cb5\"}],\"len\":11605,\"ranges\":{\"len\":\"11605\",\"vec\":[[\"0\",\"0\",\"11605\"]]}},\"vols\":[{\"host\":\"host0\",\"name\":\"host0vol2$5228d6da-0a8e-4bd9-8920-f563e9c58cb5\"}]},\"vol2$7920b761-dfb0-4e8f-a16f-9d963d74c49b\":{\"name\":\"vol2$7920b761-dfb0-4e8f-a16f-9d963d74c49b\",\"request_mapper\":{\"blocks\":[{\"host_name\":\"host1\",\"vol_name\":\"host1vol2$9feec9ac-839a-4f10-a25d-54392271925b\"}],\"len\":440359957,\"ranges\":{\"len\":\"11605\",\"vec\":[[\"0\",\"0\",\"11605\"]]}},\"vols\":[{\"host\":\"host1\",\"name\":\"host1vol2$9feec9ac-839a-4f10-a25d-54392271925b\"}]}},\"scoop\":[[{\"len\":11605,\"name\":\"host1\"}],[{\"len\":11605,\"name\":\"host0\"}]]}}}";

//'{"barrel":{"boards":{"host0":{"copy_len":17407,"copy_num":2,"main_len":17408,"total_len":34816},"host1":{"copy_len":17408,"copy_num":2,"main_len":17407,"total_len":34816}},"copy_num":2},"vol_map":{"vol0":{"base":{"device_id":"poolvol0","sec":11605,"vol_name":"vol0"},"jbods":[{"host_name":"host0","vol_name":"host0vol0$7d9fa191-76be-48c5-af79-7f2a3d2b566c"},{"host_name":"host1","vol_name":"host1vol0$47705c41-6889-4c27-9b5e-1465e636c0f7"}],"pieces":{"vol0$05829fa5-e8cc-4956-9357-d500e84053ed":{"name":"vol0$05829fa5-e8cc-4956-9357-d500e84053ed","request_mapper":{"blocks":[{"host_name":"host0","vol_name":"host0vol0$7d9fa191-76be-48c5-af79-7f2a3d2b566c"}],"len":426263245,"ranges":{"len":"11605","vec":[["0","0","11605"]]}},"vols":[{"host":"host0","name":"host0vol0$7d9fa191-76be-48c5-af79-7f2a3d2b566c"}]},"vol0$d731c869-11fd-4a6e-a53a-6e87bb488865":{"name":"vol0$d731c869-11fd-4a6e-a53a-6e87bb488865","request_mapper":{"blocks":[{"host_name":"host1","vol_name":"host1vol0$47705c41-6889-4c27-9b5e-1465e636c0f7"}],"len":426264373,"ranges":{"len":"11605","vec":[["0","0","11605"]]}},"vols":[{"host":"host1","name":"host1vol0$47705c41-6889-4c27-9b5e-1465e636c0f7"}]}},"scoop":[[{"len":11605,"name":"host0"}],[{"len":11605,"name":"host1"}]]},"vol1":{"base":{"device_id":"poolvol1","sec":11605,"vol_name":"vol1"},"jbods":[{"host_name":"host0","vol_name":"host0vol1$f1178d38-f7b5-46ca-9cb8-3a379a82cdc8"},{"host_name":"host1","vol_name":"host1vol1$5eea3c11-3ecd-4a8a-91e1-94f85f661329"},{"host_name":"host1","vol_name":"host1vol1$4bf8248a-bd4d-4caa-a1ce-9cd05d53cfd9"},{"host_name":"host0","vol_name":"host0vol1$79b0bfc7-0a00-4407-b861-02cbef102acd"}],"pieces":{"vol1$aa099dde-0b44-48df-8a4e-8fd322b78b41":{"name":"vol1$aa099dde-0b44-48df-8a4e-8fd322b78b41","request_mapper":{"blocks":[{"host_name":"host0","vol_name":"host0vol1$f1178d38-f7b5-46ca-9cb8-3a379a82cdc8"},{"host_name":"host1","vol_name":"host1vol1$5eea3c11-3ecd-4a8a-91e1-94f85f661329"}],"len":23210,"ranges":{"len":"11605","vec":[["0","0","5803"],["5803","0","5802"]]}},"vols":[{"host":"host0","name":"host0vol1$f1178d38-f7b5-46ca-9cb8-3a379a82cdc8"},{"host":"host1","name":"host1vol1$5eea3c11-3ecd-4a8a-91e1-94f85f661329"}]},"vol1$c5764605-fac0-43ea-9a79-11b228f8db38":{"name":"vol1$c5764605-fac0-43ea-9a79-11b228f8db38","request_mapper":{"blocks":[{"host_name":"host1","vol_name":"host1vol1$4bf8248a-bd4d-4caa-a1ce-9cd05d53cfd9"},{"host_name":"host0","vol_name":"host0vol1$79b0bfc7-0a00-4407-b861-02cbef102acd"}],"len":426268261,"ranges":{"len":"11605","vec":[["0","0","5803"],["5803","0","5802"]]}},"vols":[{"host":"host1","name":"host1vol1$4bf8248a-bd4d-4caa-a1ce-9cd05d53cfd9"},{"host":"host0","name":"host0vol1$79b0bfc7-0a00-4407-b861-02cbef102acd"}]}},"scoop":[[{"len":5803,"name":"host0"},{"len":5802,"name":"host1"}],[{"len":5803,"name":"host1"},{"len":5802,"name":"host0"}]]},"vol2":{"base":{"device_id":"poolvol2","sec":11605,"vol_name":"vol2"},"jbods":[{"host_name":"host0","vol_name":"host0vol2$de84cc4b-cda5-4629-ba3f-3e22fb72545e"},{"host_name":"host1","vol_name":"host1vol2$5187aaa6-8f8e-4dd3-a250-7ccfd56f5413"}],"pieces":{"vol2$00ebdb51-d610-4857-a067-480aa9e238e3":{"name":"vol2$00ebdb51-d610-4857-a067-480aa9e238e3","request_mapper":{"blocks":[{"host_name":"host1","vol_name":"host1vol2$5187aaa6-8f8e-4dd3-a250-7ccfd56f5413"}],"len":426288309,"ranges":{"len":"11605","vec":[["0","0","11605"]]}},"vols":[{"host":"host1","name":"host1vol2$5187aaa6-8f8e-4dd3-a250-7ccfd56f5413"}]},"vol2$63eb9a99-9c0e-4fe2-a783-d28b5e02960b":{"name":"vol2$63eb9a99-9c0e-4fe2-a783-d28b5e02960b","request_mapper":{"blocks":[{"host_name":"host0","vol_name":"host0vol2$de84cc4b-cda5-4629-ba3f-3e22fb72545e"}],"len":4294978900,"ranges":{"len":"11605","vec":[["0","0","11605"]]}},"vols":[{"host":"host0","name":"host0vol2$de84cc4b-cda5-4629-ba3f-3e22fb72545e"}]}},"scoop":[[{"len":11605,"name":"host1"}],[{"len":11605,"name":"host0"}]]}}}'


int main(int argc, const char *argv[])
{
//    int test = 3;
//    json_obj obj("test",test);
//    obj.merge(json_obj("test1",(long long )test));
//    cout<<obj["test"].get_number()<<endl;

    json_obj obj;
    obj.loads(test_char);
    if(memcmp(obj.dumps().c_str(),test_char,strlen(test_char)) ==0)
    {
        cout<<"OK"<<endl;
    }
//    cout << obj.dumps().c_str()<<endl;
    // json_obj obj("key","val");
    // json_obj obj1("key","val");
    // json_obj obj2("key","val1");

    // json_obj obj3;
    // obj3.set_array();
    // obj3.append("heee");

    // json_obj obj4;
    // obj4.set_array();
    // obj4.append("heee");
    // obj4.append("eee");

    // json_obj obj5;
    // obj5.set_array();
    // obj5.append("heee");
    // obj5.append("eee");


    // if(obj == obj1) cout<<"right"<<endl;
    // else cout<<"wrong"<<endl;
    // if(obj1 != obj2) cout<<"right"<<endl;
    // else cout<<"wrong"<<endl;
    // if(obj3 != obj2) cout<<"right"<<endl;
    // else cout<<"wrong"<<endl;
    // if(obj3 != obj4) cout<<"right"<<endl;
    // else cout<<"wrong"<<endl;
    // if(obj4 == obj5) cout<<"right"<<endl;
    // else cout<<"wrong"<<endl;

    if (argc < 2){
        cout<<"d                    ;dumps"<<endl;
        cout<<"l                    ;loads"<<endl;
        return -1;
    }
    try
    {
        if(strcmp(argv[1],"d")==0)
        {
            json_obj obj("hello");
            json_obj ar;
            ar.set_array();
            ar.append(obj);
            ar.append(2333);
            ar.append(true);

            // // cout<<obj.dumps()<<endl;
            json_obj obj_map("key","pair");
            json_obj obj_map_2("key1","pair");
            // cout<<obj_map.dumps()<<endl;
            obj_map.merge(obj_map_2);
            obj_map_2 = json_obj("key2",true);
            obj_map.merge(obj_map_2);
            obj_map_2 = json_obj("key3",22222);
            obj_map.merge(obj_map_2);
            // cout<<ar.dumps()<<endl;
            obj_map_2 = json_obj("key4",ar);
            // cout<<obj_map_2.dumps()<<endl;
            obj_map.merge(obj_map_2);
            ar.append(obj_map);
            obj_map = json_obj("test",ar);
            cout<<obj_map.dumps();
        }
        else if (strcmp(argv[1],"l") == 0)
        {
            cout<<argv[2]<<endl;
            json_obj obj;
            obj.loads(argv[2]);
            cout<<obj.dumps()<<endl;
            cout<<obj["test"].dumps()<<endl;;
            cout<<obj["test"][0].dumps()<<endl;
            cout<<obj["test"][3]["key"].dumps()<<endl;
        }
    }
    catch(int &err)
    {
        cout<<err<<endl;
    }
}