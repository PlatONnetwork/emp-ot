#include "test/test.h"
#include <iostream>
using namespace std;

#ifdef OT_NP_USE_MIRACL
#include "emp-tool/utils/sm2_params.h"
#endif//

int main(int argc, char** argv) {
	int length = 1<<24, port, party;
	parse_party_and_port(argv, 2, &party, &port);

#ifdef OT_NP_USE_MIRACL
	SM2_Init();
#endif//
	
	IOChannel * io = new NetIO(party==ALICE ? nullptr:"127.0.0.1", port);
	cout <<"NPOT\t"<<10000.0/test_ot<OTNP>(io, party, 10000)*1e6<<" OTps"<<endl;
	cout <<"Semi Honest OT Extension\t"<<double(length)/test_ot<SHOTExtension>(io, party, length)*1e6<<" OTps"<<endl;
	//cout <<"Semi Honest COT Extension\t"<<double(length)/test_cot<SHOTExtension>(io, party, length)*1e6<<" OTps"<<endl;
	//cout <<"Semi Honest ROT Extension\t"<<double(length)/test_rot<SHOTExtension>(io, party, length)*1e6<<" OTps"<<endl;
	delete io;
}

