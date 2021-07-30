#include <bits/stdc++.h>

#include "../libs/compressed_array.h"
#include "../libs/sample_pointers.h"
#include "../libs/compressed_bitvector.h"
#include "../libs/wavelet_tree.h"

using namespace std;

#define sz 9

pair<unsigned, unsigned> findLimits(unsigned *seq, unsigned seq_size)
{
    unsigned seqMin, seqMax;
    if (seq_size >= 1)
        seqMin = seqMax = seq[0];
    for (int i = 1; i < seq_size; i++)
    {
        seqMin = min(seqMin, seq[i]);
        seqMax = max(seqMax, seq[i]);
    }
    return make_pair(seqMin, seqMax);
}

// int main() {
//     unsigned arr[sz] = {2, 1, 9, 3, 8, 4, 7, 5, 6};
//     pair<unsigned, unsigned> limits = findLimits(arr, sz);
//     WaveletTree wt(arr, arr + sz, limits.first, limits.second, "", NULL);

//     int n;
//     for (int i = 1; i <= sz; i++) cout << wt.access(i) << " ";
//     cout << endl;

//     while(1) {
//         unsigned c, i;
//         scanf("%u%u", &c, &i);
//         printf("rank-%u(%u) = %d\n", c, i, wt.rank(c, i));
//     }

//     return 0;
// }

int main()
{
    unsigned arr[10] = {2, 1, 4, 1, 3, 4, 1, 5, 2, 1};
    int arrSize = sizeof(arr) / sizeof(unsigned);
    pair<unsigned, unsigned> limits = findLimits(arr, arrSize);
    WaveletTree wt(arr, arr + arrSize, limits.first, limits.second, "", NULL);

    for (int i = 1; i <= arrSize; i++) printf("%u ", wt.access(i));
    puts("");

    while (1)
    {
        int c, i;
        scanf("%d%d", &c, &i);
        printf("rank-%u(%u) = %d\n", c, i, wt.rank(c, i));
    }

    // while (1)
    // {
    //     int c, x;
    //     scanf("%d%d", &c, &x);
    //     printf("%u\n", wt.select(c, x));
    // }
}