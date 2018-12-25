#ifndef OT_H__
#define OT_H__
#include <emp-tool/emp-tool.h>
#include "emp-tool/io/io_channel.h"
namespace emp {

class OT {
public:
	OT(){}
	virtual ~OT(){}
	virtual void send(const block* data0, const block* data1, int length) = 0;
	virtual void recv(block* data, const bool* b, int length) = 0;
};
}
#endif// OT_H__
