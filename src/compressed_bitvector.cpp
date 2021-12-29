#include <bits/stdc++.h>
#include "../libs/bitarray.h"
#include "../libs/sample_pointers.h"
#include "../libs/compressed_bitvector.h"
#include "../libs/combination.h"
#include "../libs/memprofile.h"
#include "../libs/constants.h"
using namespace std;

CompressedBitvector::CompressedBitvector(uint block_size, ulong blocks_total)
{
    this->block_size = block_size; // block size in bits
    this->blocks_total = blocks_total;
    this->bits = block_size * blocks_total;
    this->Comb = Combination::precomp(block_size); // represents variable K
    this->SUPER_BLOCK_SIZE = block_size * Constants::COMPRESSED_BITVECTOR_PARAM_K;
    this->SUPER_BLOCK_NUM = (bits / SUPER_BLOCK_SIZE + 1);
    this->R = (uint*) new uint[SUPER_BLOCK_NUM];
    this->ones = 0;
    this->zeros = 0;
    this->sz = Combination::size(block_size) + ((SUPER_BLOCK_NUM * 2) * sizeof(uint));
}
/*
    compress using a array B, the elements of B will be used as bit sequences
*/
CompressedBitvector::CompressedBitvector(uint block_size, ulong length, uint *B) : CompressedBitvector(block_size, length)
{
    compress(Bitarray(block_size, length, B));
}

CompressedBitvector::CompressedBitvector(uint block_size, ulong length, vector<bool> &bitvector) : CompressedBitvector(block_size, length)
{ 
    // first we need to convert all the bitvector in a array int with size of block_size bits

    uint *BLOCKS = (uint*) calloc(length, sizeof(uint));
    if(BLOCKS) {
        uint block_idx, bit_number;
        for(uint i = 0; i < bitvector.size(); i++) {
            if(i % block_size == 0) {
                bit_number =  block_size - 1;
                block_idx = i / block_size;
            } else bit_number--;
            
            if (bitvector[i])
                BLOCKS[block_idx] |= 1 << bit_number;
        }

        compress(Bitarray(block_size, length, BLOCKS));
        free(BLOCKS);
    }
}

CompressedBitvector::~CompressedBitvector()
{
    delete C;
    delete O;
    delete[] R;
    Combination::free(Comb, block_size);
}

pair<uint, uint> CompressedBitvector::encode(Bitarray &B, uint i)
{
    uint BLOCK = B.read(i);
    uint c = 0;
    uint b = block_size;

    while (b > 0)
    {
        if (BLOCK & (1 << (b - 1))) // if the bit is 1
        {
            c++;
            ones++;
        } else { // if it's a 0
            zeros++;
        }
        b--;
    }

    uint o = 0;
    uint c1 = c;
    b = block_size;

    while (c1 > 0)
    {
        if (BLOCK & (1 << (b - 1)))
        {
            o += Comb[b - 1][c1];
            c1--;
        }
        b--;
    }

    return make_pair(c, o);
}

uint CompressedBitvector::decode(uint i)
{
    uint c = C->read(i);
    uint o = O->read(i);
    uint b = block_size;

    uint BLOCK = 0;
    while (c > 0) // b = 0, c = 0, o = 0, Comb[b][c]
    {
        if (o >= Comb[b - 1][c])
        {
            BLOCK |= (1 << (b - 1));
            o -= Comb[b - 1][c];
            c--;
        }
        b--;
    }

    return BLOCK;
}

void CompressedBitvector::precompR()
{

    uint r = 0, r_idx = 1;
    R[0] = r;
    for (uint i = 1; i <= blocks_total; i++)
    {
        r += C->read(i - 1);
        if(i % Constants::COMPRESSED_BITVECTOR_PARAM_K == 0)
            R[r_idx++] = r;
    }

    // TODO
    // when there is no more R block to be filled break the loop

}

void CompressedBitvector::compress(Bitarray B)
{
    uint *C = (uint*) new uint[blocks_total];
    uint *O = (uint*) new uint[blocks_total];

    for (uint i = 0; i < blocks_total; i++) {
        const pair<uint, uint> enconded_block = encode(B, i);
        C[i] = enconded_block.first;
        O[i] = enconded_block.second;
    }

    this->C = new Bitarray(log2(block_size), blocks_total, C);
    delete[] C;
    // malloc_count_print_status();

    this->O = new SamplePointers(blocks_total, O);
    delete[] O;
    // delete this->C;
    // malloc_count_print_status();



    cout << "C size: " << this->C->size() << endl;
    cout << "O size: " << this->O->size() << endl;
    
    this->sz = this->sz + this->C->size() + this->O->size();
    // delete[] C;
    // delete[] O;
    precompR();
}

bool CompressedBitvector::access(uint i)
{
    if(i == 0 || i > bits) return 0;

    i--;
    uint block = decode(i / block_size);
    return block & (1 << (block_size - 1 - (i % block_size)));
}

uint CompressedBitvector::rank1(uint i)
{
    if (i == 0 || i > bits) return 0;

    uint super_block_idx = ceil( i / (float) SUPER_BLOCK_SIZE);
    if(i % SUPER_BLOCK_SIZE == 0) return R[super_block_idx]; // allready know rank

    uint r = R[super_block_idx - 1];
    for (int t = (super_block_idx - 1) * Constants::COMPRESSED_BITVECTOR_PARAM_K; t < floor(i / (float) block_size); t++) // walk in blocks
        r += C->read(t);

    uint aux = i % block_size;
    if(aux == 0) return r; // aux is in the end of block the block
    // else aux = number of bit to be readed for the next block

    uint x = decode(ceil(i / (float) block_size) - 1);
    uint b = 0;
    for (uint j = 0, pos = block_size - 1; j < aux; j++, pos--)
        b += x & (1 << pos) ? 1 : 0;
    return r + b;
}

uint CompressedBitvector::rank0(uint i)
{
    if (i == 0 || i > bits) return 0;
    return i - rank1(i);
}

uint CompressedBitvector::find_first_rank(uint idx) {
    if (idx == 0 || idx > bits) return 0;

    while (idx > 0 && R[idx] == R[idx - 1]) idx--;
    return idx > 0 ? idx - 1 : idx;
}

/*
    return idx o mid position closer of a R stored value return allways the greather
*/


/*
    do a binary search in R then return smallest closest idx of the searched rank
*/
uint CompressedBitvector::rank1_binary_search(uint lo_idx, uint hi_idx, uint rank)
{
    if(lo_idx < 0 || hi_idx >= SUPER_BLOCK_NUM) return 0;
    
    if (lo_idx >= hi_idx) {
        const uint idx = min(lo_idx, hi_idx);
        return find_first_rank(idx);
    }

    const uint mid_idx = lo_idx + (hi_idx - lo_idx) / 2;

    if (rank < R[mid_idx])
        return rank1_binary_search(lo_idx, mid_idx - 1, rank);
    else if (rank > R[mid_idx])
        return rank1_binary_search(mid_idx + 1, hi_idx, rank);
    return find_first_rank(mid_idx);
}

uint CompressedBitvector::select1(uint i)
{
    if (i > ones) return 0;

    uint idx = rank1_binary_search(0, SUPER_BLOCK_NUM - 1, i);
    uint c, r = R[idx];
    idx = idx * SUPER_BLOCK_SIZE;

    while(true) {
        c = C->read(idx / block_size); // walk in C blocks
        if (r + c >= i) break;
        r += c;
        idx += block_size;
    }

    // erro, select(5, 1), x = 1, deveria ser x = 2
    const uint x = decode(idx / block_size);
    uint b = block_size;
    while (b > 0) {
        idx++;
        if(x & (1 << (b - 1))) r++;
        if (r == i) break;
        b--;
    }

    return idx;
}

uint CompressedBitvector::rank0_binary_search(uint lo_idx, uint hi_idx, uint rank)
{
    if (lo_idx < 0 || hi_idx >= SUPER_BLOCK_NUM)
        return 0;

    if (lo_idx >= hi_idx)
    {
        const uint idx = min(lo_idx, hi_idx);
        return find_first_rank(idx);
    }

    const uint mid_idx = lo_idx + (hi_idx - lo_idx) / 2;
    const uint offset = mid_idx * SUPER_BLOCK_SIZE;

    if (rank < offset - R[mid_idx])
        return rank0_binary_search(lo_idx, mid_idx - 1, rank);
    else if (rank > offset - R[mid_idx])
        return rank0_binary_search(mid_idx + 1, hi_idx, rank);
    return find_first_rank(mid_idx);
}

uint CompressedBitvector::select0(uint i)
{
    if (i > zeros)
        return 0;

    uint idx = rank0_binary_search(0, SUPER_BLOCK_NUM - 1, i);
    uint c, r = idx * SUPER_BLOCK_SIZE - R[idx];
    idx = idx * SUPER_BLOCK_SIZE;

    while (true)
    {
        c = block_size - C->read(idx / block_size); // walk in C blocks
        if (r + c >= i) break;
        r += c;
        idx += block_size;
    }

    const uint x = decode(idx / block_size);
    uint b = block_size;
    while (b > 0)
    {
        idx++;
        if (~x & (1 << (b - 1))) r++;
        if (r == i) break;
        b--;
    }

    return idx;
}

void CompressedBitvector::print_blocks()
{
    for (uint i = 0; i < blocks_total; i++)
    {
        printf("%u ", decode(i));
    }
    puts("");
}

void CompressedBitvector::print()
{
    for (uint i = 1; i <= bits; i++)
        cout << access(i);
    cout << endl;
}

ulong CompressedBitvector::size()
{
    return this->sz;
}