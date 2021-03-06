#include <bits/stdc++.h>
#include "../libs/bitarray.h"
using namespace std;

// mudar parametros para aceitar int ou unsigned
Bitarray::Bitarray(int l, int n)
{
    this->l = (unsigned) l;
    this->n = (unsigned) n;
    // W = (unsigned *)malloc(l * n / w * sizeof(unsigned));
    this->W_SIZE = ceil(l * n / (float)w);
    this->sz = W_SIZE * sizeof(unsigned);
    W = (unsigned*) malloc(sz);
    if (W == NULL)
    {
        printf("malloc error\n");
    }
}

Bitarray::Bitarray(int l, int n, unsigned *arr) : Bitarray::Bitarray(l, n)
{
    compress(arr);
}

Bitarray::~Bitarray()
{
    free(W);
}

void Bitarray::compress(unsigned *arr)
{
    for (int i = 0; i < n; i++)
        this->write(i, arr[i]);
}

bool Bitarray::bitread(int j)
{
    return (W[j / w] >> (j % w)) & 1;
}

unsigned Bitarray::bitsread(int j0, int j)
{

    if (j0 / w == j / w)
        return (W[j / w] >> (j0 % w)) & ((1 << (j - j0 + 1)) - 1);

    return (W[j0 / w] >> (j0 % w)) |
           (W[j / w] & ((1 << ((j + 1) % w)) - 1)) << (w - (j0 % w));
}

unsigned Bitarray::read(int i)
{
    bitsread(i * l, i * l + l - 1);
}

void Bitarray::bitset(int j)
{
    W[j / n] |= 1 << (j % w);
}

void Bitarray::bitclear(int j)
{
    W[j / w] &= ~(1 << (j % w));
}

void Bitarray::bitswrite(int j0, int j, unsigned x)
{
    if (j0 / w == j / w)
    {
        W[j / w] &= ~(((1 << (j0 - j + 1)) - 1) << (j0 % w));
        W[j / w] |= x << (j0 % w);
    }
    else
    {
        W[j0 / w] = (W[j0 / w] & ((1 << (j0 % w)) - 1)) | (x << (j0 % w));
        W[j / w] = (W[j / w] & ~((1 << ((j + 1) % w)) - 1)) | (x >> (w - (j0 % w)));
    }
}

void Bitarray::write(int i, unsigned x)
{
    bitswrite(i * l, i * l + l - 1, x);
}

int Bitarray::bitsPrint()
{
    int total_of_bits = l *n;
    int aux = 0;
    for (int i = 0; i < total_of_bits; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            printf("%d", bitread(32 * i + j));
            aux++;
            if (aux == 4)
            {
                aux = 0;
                printf(" ");
            }
        }
    }
    puts("");
}

void Bitarray::printW()
{
    int size = W_SIZE;
    for (int i = 0; i < size; i++)
        printf(" %u", W[i]);

    puts("");
}

long unsigned Bitarray::size()
{
    return this->sz;
    // TODO, valor errado o certo seria sz * sizeof unsigned
}

long unsigned Bitarray::length()
{
    return n;
}

// void printWbits()
// {
//     for (int i = 1; i <= l * n; i++)
//     {
//         printf("%d", bitread(i - 1));
//         if (i % l == 0)
//             printf(" ");
//     }
//     puts("");
// }
