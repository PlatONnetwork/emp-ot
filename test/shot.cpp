#include "test/test.h"
using namespace std;

void task1(int party, int port);

int main(int argc, char** argv) {

		int port, party;
		parse_party_and_port(argv, 2, &party, &port);


		if(party == 0) {
			// this does not work
			thread t1(task1, ALICE, port);

			//for firewall
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			thread t2(task1, BOB, port);


			t1.join();
			t2.join();
		}else{
			//this works (two processes)
			thread t1(task1, party, 22488);
			t1.join();
		}
}

void task1(int party, int port)
{
	NetIO * io = new NetIO(party==ALICE ? nullptr:"127.0.0.1", port);
	int length = 10000;
	cout <<"NPOT\t"<<10000.0/test_ot<NetIO, OTNP>(io, party, length)*1e6<<" OTps"<<endl;
	cout <<"Semi Honest OT Extension\t"<<double(length)/test_ot<NetIO, SHOTExtension>(io, party, length)*1e6<<" OTps"<<endl;
	cout <<"Semi Honest COT Extension\t"<<double(length)/test_cot<NetIO, SHOTExtension>(io, party, length)*1e6<<" OTps"<<endl;
	cout <<"Semi Honest ROT Extension\t"<<double(length)/test_rot<NetIO, SHOTExtension>(io, party, length)*1e6<<" OTps"<<endl;
	delete io;
}
