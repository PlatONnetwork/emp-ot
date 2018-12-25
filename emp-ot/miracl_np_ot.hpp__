#ifndef MIRACL_OT_NP_HPP__
#define MIRACL_OT_NP_HPP__


OTNP::OTNP(IOChannel* io) 
{
    this->io = io;
    init();
}

OTNP::~OTNP()
{
	if (C)
		epoint_free(C);
}

void OTNP::init()
{
	if (C == nullptr)
	{
		SM2_Init();
		big c;
		c = mirvar(0);
		bigrand(getP(), c);
		C = epoint_init();
		ecurve_mult(c, getG(), C);  // C = c*G
		//big value should be free
		mirkill(c);
	}
}

//bob's send promitive
void OTNP::send(const block* data0, const block* data1, int length) 
{
    block *m0 = new block[length];
    block *m1 = new block[length];
    big r;
    r = mirvar(0);
    epoint *h, *h1;
    h = epoint_init();
    h1 = epoint_init();

    epoint **gr = (epoint **)mr_alloc(length, sizeof(epoint*));
	for (int i = 0; i < length; ++i)
	{
		gr[i] = epoint_init();
	}
    
	int ret = -1;
    for(int i = 0; i < length; i++) 
    {
        bigrand(getP(), r);
        ret = io->recv_ep(h, 1);   // h
		if (ret != 0)
			return;//exception

        ecurve_mult(r, getG(), gr[i]);  // g^r  
        epoint_copy(C, h1);   //h1 = C
        ecurve_sub(h, h1);    // h1 = C-h
        ecurve_mult(r, h, h);   // h' = h^r
        ecurve_mult(r, h1, h1);   // h1 = (C-h)^r
        m0[i] = Hash::hash_for_epoint(h);   //H(h^r)
        m1[i] = Hash::hash_for_epoint(h1);  // H((C/h)^r) 
    }

    block m[2];
    //printf("*** OTNP send blocks: \n");
    for(int i = 0; i < length; i++) 
    {
        ret = io->send_ep(gr[i], 1);
		if (ret != 0)
			return;//exception
        m[0] = xorBlocks(data0[i], m0[i]);
        m[1] = xorBlocks(data1[i], m1[i]);
        ret = io->send_data(m, 2 * sizeof(block));
		if (ret != 0)
			return;//exception
    }

    for(int i = 0; i < length; ++i) 
    {
        epoint_free(gr[i]);
    }
	mr_free(gr);

    mirkill(r);
    epoint_free(h);
    epoint_free(h1);
	
    delete[] m0;
    delete[] m1;

	return;
}

//alice's recv promitive
void OTNP::recv(block* data, const bool* b, int length) 
{
    big *k = new big[length];
	epoint *h, *pk1;
	epoint *gr ;
	gr = epoint_init();
	block m[2];

	//big value should free
	for(int i = 0; i < length; ++i) 
	{
		k[i] = mirvar(0);
	}
	h = epoint_init();
	pk1 = epoint_init();

	for(int i = 0; i < length; i++) 
	{
		bigrand(getP(), k[i]);
	}
	
	int ret = -1;
	for(int i = 0; i < length; i++) 
	{
		if(b[i]) {
			ecurve_mult(k[i], getG(), pk1);   // pk1 = g^ki
			epoint_copy(C, h);  // h = C
			ecurve_sub(pk1, h);  // h = C - pk1 = C - g^ki
		} else {
			ecurve_mult(k[i], getG(), h);  // h = g^ki
		}
		ret = io->send_ep(h, 1); 
		if (ret != 0)
			return;//exception
	}

	//printf("*** OTNP recv blocks: \n");
	for(int i = 0; i < length; ++i) 
	{
		ret = io->recv_ep(gr, 1);
		if (ret != 0)
			return;//exception

		ret = io->recv_data(m, 2 * sizeof(block));
		if (ret != 0)
			return;//exception

        //printBlock(m, 2);
        
		int ind = b[i]?1:0;

		ecurve_mult(k[i], gr, gr);  // gr = gr^ki
		// data_b =H(gr^ki) + m_b
		block tempB1 = Hash::hash_for_epoint(gr);
		block tempB2 = Hash::hash_for_epoint(gr);
		data[i] = xorBlocks(m[ind], Hash::hash_for_epoint(gr));
	}

	//free big values
	for(int i = 0; i < length; ++i) 
	{
		mirkill(k[i]);
	}
	
	epoint_free(gr);
	epoint_free(h);
	epoint_free(pk1);
	delete[] k;

	return;//ok
}

#endif//MIRACL_OT_NP_HPP__