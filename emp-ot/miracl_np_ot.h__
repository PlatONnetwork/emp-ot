#ifdef OT_NP_USE_MIRACL

#ifndef MIRACL_NP_OT_H__
#define MIRACL_NP_OT_H__


#include "emp-ot/ot.h"
#include "emp-tool/utils/sm2_params.h"
#include "emp-tool/utils/prg.h"
#include "emp-tool/utils/miracl_hash.h"
#include <cstddef>


namespace emp
{
		class OTNP: public OT
		{
		public:
			OTNP(IOChannel* io);
			virtual ~OTNP();

			virtual void send(const block* data0, const block* data1, int length);

			virtual void recv(block* data, const bool* b, int length);

		private:
			void init();

		private:
			int cnt = 0;
			epoint *C = nullptr;
			IOChannel* io;
			PRG prg;


		};

#include "emp-ot/miracl_np_ot.hpp"
}//emp

#endif//MIRACL_NP_OT_H__

#endif//OT_NP_USE_MIRACL_H__
