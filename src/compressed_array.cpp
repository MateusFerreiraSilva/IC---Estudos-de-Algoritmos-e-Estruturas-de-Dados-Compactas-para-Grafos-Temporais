#include <bits/stdc++.h>
#include "../libs/compressed_array.h"
using namespace std;

// mudar parametros para aceitar int ou unsigned
CompArray::CompArray(int l, int n)
{
    this->l = (unsigned)l;
    this->n = (unsigned)n;
    // W = (unsigned *)malloc(l * n / w * sizeof(unsigned));
    this->sz = ceil(l * n / (float)w) * sizeof(unsigned);
    W = (unsigned *)malloc(sz);
    if (W == NULL)
    {
        printf("malloc error\n");
    }
}

CompArray::CompArray(int l, int n, unsigned *arr) : CompArray(l, n)
{
    compress(arr);
}

CompArray::~CompArray()
{
    free(W);
}

void CompArray::compress(unsigned *arr)
{
    for (int i = 0; i < n; i++)
        this->write(i, arr[i]);
}

bool CompArray::bitread(int j)
{
    return (W[j / w] >> (j % w)) & 1;
}

unsigned CompArray::bitsread(int j0, int j)
{

    if (j0 / w == j / w)
        return (W[j / w] >> (j0 % w)) & ((1 << (j - j0 + 1)) - 1);

    return (W[j0 / w] >> (j0 % w)) |
           (W[j / w] & ((1 << ((j + 1) % w)) - 1)) << (w - (j0 % w));
}

unsigned CompArray::read(int i)
{
    bitsread(i * l, i * l + l - 1);
}

void CompArray::bitset(int j)
{
    W[j / n] |= 1 << (j % w);
}

void CompArray::bitclear(int j)
{
    W[j / w] &= ~(1 << (j % w));
}

void CompArray::bitswrite(int j0, int j, unsigned x)
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

void CompArray::write(int i, unsigned x)
{
    bitswrite(i * l, i * l + l - 1, x);
}

int CompArray::bitsPrint()
{
    int size = l * n / w;
    int aux = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            printf("%d", bitread(i * 32 + j));
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

void CompArray::printW()
{
    int size = sizeof(W) / sizeof(unsigned);
    for (int i = 0; i < size; i++)
        printf(" %u", W[i]);

    puts("");
}

long unsigned CompArray::size()
{
    return this->sz;
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
