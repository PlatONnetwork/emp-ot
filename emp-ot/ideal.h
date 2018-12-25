#ifndef OT_IDEAL_H__
#define OT_IDEAL_H__
#include "emp-ot/ot.h"
/** @addtogroup OT
    @{
  */
namespace emp { 
class OTIdeal: public OT { public:
	int cnt = 0;
	IOChannel* io = nullptr;
	OTIdeal(IOChannel * io) {
		this->io = io;
	}

	void send(const block* data0, const block* data1, int length) {
		cnt+=length;
		io->send_block(data0, length);
		io->send_block(data1, length);
	}

	void recv(block* data, const bool* b, int length) {
		cnt+=length;
		block *data1 = new block[length];
		io->recv_block(data, length);
		io->recv_block(data1, length);
		for(int i = 0; i < length; ++i)
			if(b[i])
				data[i] = data1[i];
		delete []data1;
	}
};
/**@}*/
}
#endif// OT_IDEAL_H__
