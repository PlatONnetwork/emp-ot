#ifdef OT_NP_USE_MIRACL

#ifndef MIRACL_OT_NP_H__
#define MIRACL_OT_NP_H__

#include "emp-ot/ot.h"
#include "emp-tool/utils/prg.h"
#include "emp-tool/utils/miracl_otnp_utils.h"
#include <cstddef>


namespace emp
{

		class OTNP : public OT
		{
		public:
			OTNP(IOChannel* io);
			~OTNP();

			void send(const block* data0, const block* data1, int length);

			void recv(block* data, const bool* b, int length);

		private:
			void init();

		private:
			int cnt = 0;
			void* epointC = nullptr;
			IOChannel* io;
			PRG prg;
		};

		OTNP::OTNP(IOChannel* io)
		{
			this->io = io;
		}

		OTNP::~OTNP()
		{
			otnp_uninit(&epointC);
		}

		void OTNP::init()
		{
			otnp_init(&epointC);
		}

		//bob's send promitive
		void OTNP::send(const block* data0, const block* data1, int length)
		{
			otnp_send(io, &epointC, data0, data1, length);
		}

		//alice's recv promitive
		void OTNP::recv(block* data, const bool* b, int length)
		{
			otnp_recv(io, &epointC, data, b, length);
		}

}//emp

#endif//MIRACL_OT_NP_H__
#endif//OT_NP_USE_MIRACL

