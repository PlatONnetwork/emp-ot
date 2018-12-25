#include <iostream>
#include "test/test.h"
#include "emp-ot/mextension_kos.h"
using namespace std;

template<typename T>
double test_cot_mal(IOChannel * io, int party, int length) {
	block *b0 = new block[length], *r = new block[length];
	bool *b = new bool[length];
	block *delta = new block[length];
	PRG prg(fix_key);
	prg.random_block(delta, length);
	prg.random_bool(b, length);
	
	io->sync();
	auto start = clock_start();
	T* ot = new T(io);
	if (party == ALICE) {
		ot->send_cot(b0, delta, length);
	} else {
		ot->recv_cot(r, b, length);
	}
	io->flush();
	long long t = time_from(start);
	if(party == ALICE)
			io->send_block(b0, length);
	else if(party == BOB)  {
		io->recv_block(b0, length);
		for(int i = 0; i < length; ++i) {
			block b1 = xorBlocks(b0[i], delta[i]); 
			if (b[i]) {
				if(!block_cmp(&r[i], &b1, 1))
					error("COT failed!");
			} else {
				if(!block_cmp(&r[i], &b0[i], 1))
					error("COT failed!");
			}
		}
	}
	io->flush();
	delete ot;
	delete[] b0;
	delete[] r;
	delete[] b;
	delete[] delta;
	return t;
}

int main(int argc, char** argv) {
#ifndef OT_NP_USE_MIRACL
	int port, party, length = 1<<24;
	parse_party_and_port(argv, 2, &party, &port);
	IOChannel * io = new NetIO(party==ALICE ? nullptr:"127.0.0.1", port);
	cout <<"COOT\t"<<10000.0/test_ot<OTCO>(io, party, 10000)*1e6<<" OTps"<<endl;
	cout <<"Malicious OT Extension\t"<<double(length)/test_ot<MOTExtension_KOS>(io, party, length)*1e6<<" OTps"<<endl;
	cout <<"Malicious COT Extension\t"<<double(length)/test_cot_mal<MOTExtension_KOS>(io, party, length)*1e6<<" OTps"<<endl;
    cout <<"Malicious ROT Extension\t"<<double(length)/test_rot<MOTExtension_KOS>(io, party, length)*1e6<<" OTps"<<endl;
	delete io;
#else
	cerr << "Not support for miracl version !" << endl;
#endif//
	return 0;
}
