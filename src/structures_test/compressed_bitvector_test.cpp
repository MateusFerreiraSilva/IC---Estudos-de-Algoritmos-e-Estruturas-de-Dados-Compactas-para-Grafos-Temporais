#include "../../libs/structures_test/compressed_bitvector_test.h"

class BitvectorTest {
    public:
        vector<bool> bitvector;
        
        BitvectorTest(int n) {
            bitvector.resize(n);
            for (int i = 0; i < bitvector.size(); i++)
                bitvector[i] = 0;            
        }

        uint access(uint i) {
            if (i == 0 || i > bitvector.size()) return 0;
            return bitvector[i - 1];
        }

        void fill(vector<uint> idxs) {
            for (auto idx : idxs)
                bitvector[idx - 1] = 1;
        }

        uint  rank1(uint i) {
            if (i == 0 || i > bitvector.size()) return 0;
            int sum = 0;
            for (int j = 1; j <= i; j++)
                sum += access(j);
            return sum;
        }
        
        uint  rank0(uint i) {
            if (i == 0 || i > bitvector.size()) return 0;
            return i - rank1(i);
        }

        uint ones() {
            return rank1(bitvector.size());
        }

        uint zeros() {
            return bitvector.size() - ones();
        }


        uint select1(uint i) {
            if (i == 0 || i > ones()) return 0;
            for (int j = 1; j <= bitvector.size(); j++)
                if (rank1(j) == i) return j;    
        }

        uint select0(uint i) {
            if (i == 0 || i > zeros()) return 0;
            for (int j = 1; j <= bitvector.size(); j++)
                if (rank0(j) == i) return j;    
        }

        void print() {
            for (int i = 0; i < bitvector.size(); i++)
                cout << bitvector[i];
            cout << endl;
        }
};

void test_access(BitvectorTest *bitvector_test, CompressedBitvector *bitvector, int n) {
    for (int i = 1; i <= n; i++)
        assert(bitvector_test->access(i) == bitvector->access(i));
    cout << "access OK" << endl;
}

void test_rank1(BitvectorTest *bitvector_test, CompressedBitvector *bitvector, int n) {
    for (int i = 1; i <= n; i++)
        assert(bitvector_test->rank1(i) == bitvector->rank1(i));
    cout << "rank1 OK" << endl;
}

void test_rank0(BitvectorTest *bitvector_test, CompressedBitvector *bitvector, int n) {
    for (int i = 1; i <= n; i++)
        assert(bitvector_test->rank0(i) == bitvector->rank0(i));
    cout << "rank0 OK" << endl;
}

void test_select1(BitvectorTest *bitvector_test, CompressedBitvector *bitvector, int n) {
    for (int i = 1; i <= bitvector_test->ones(); i++)
        assert(bitvector_test->select1(i) == bitvector->select1(i));
    cout << "select1 OK" << endl;
}

void test_select0(BitvectorTest *bitvector_test, CompressedBitvector *bitvector, int n) {
    for (int i = 1; i <= bitvector_test->zeros(); i++)
        assert(bitvector_test->select0(i) == bitvector->select0(i));
    cout << "select0 OK" << endl;
}

void compressed_bitvector_test() {
    int n = 32;
    vector<uint> idxs = {2, 7, 9, 23, 24, 25, 31};
    BitvectorTest *bitvector_test = new BitvectorTest(n);
    bitvector_test->fill(idxs);
    const uint block_size = 4;
    const uint total_of_bits = bitvector_test->bitvector.size() / block_size + 1;
    CompressedBitvector *bitvector = new CompressedBitvector(block_size, total_of_bits, bitvector_test->bitvector);

    bitvector_test->print();
    bitvector->print();

    test_access(bitvector_test, bitvector, n);
    test_rank1(bitvector_test, bitvector, n);
    test_rank0(bitvector_test, bitvector, n);
    test_select1(bitvector_test, bitvector, n);
    test_select0(bitvector_test, bitvector, n);

    delete bitvector_test;
    delete bitvector;

}


// void compressed_bitvector_test() {
//     int n = 10, b = 4;
//     unsigned arr[10] = {8, 2, 0, 6, 0, 10, 0, 11, 8, 1};
//     CompressedBitvector cb(4, 10, arr);

//     // ################# ONES #####################

//     unsigned rank_test[10 * 4] = {0};
//     int c = 0;
//     for(int i = 0; i < n; i++) {
//         for (int j = 0; j < b; j++)
//         {
//             unsigned idx = i * b + j;
//             if (idx > 0) rank_test[idx] = rank_test[idx - 1];
//             rank_test[idx] += arr[i] & (1 << (b - 1) - j) ? 1 : 0; 
//             printf("rank[%d] = %d, rank_teste[%d] =  %d\n", idx, cb.rank1(idx + 1), idx, rank_test[idx]);
//             if (cb.rank1(idx + 1) == rank_test[idx]) c++;
//         }
//     }
//     if(c == n * b) printf("OK\n");

//     int select_test[10 * 4];
//     memset(select_test, -1, sizeof select_test);
//     c = 0;
//     int r = 0;
//     for (int i = 0, j = 0; i < n * b; i++)
//     {
//         if (rank_test[i] > r)
//         {
//             select_test[j] = i + 1;
//             r++;
//             j++;
//         }
//     }

//     for (int i = 0; i < n * b; i++)
//     {
//         int select = cb.select1(i + 1);
//         printf("select[%d] =  %d, select_test[%d] = %d\n", i + 1, select, i + 1, select_test[i]);
//         if (select == select_test[i])
//             c++;
//     }
//     if (c == n * b)
//         printf("OK\n");


    // ################# ZERO #####################

    // unsigned rank_test[10 * 4] = {0};
    // int c = 0;
    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < b; j++)
    //     {
    //         unsigned idx = i * b + j;
    //         if (idx > 0)
    //             rank_test[idx] = rank_test[idx - 1];
    //         rank_test[idx] += arr[i] & (1 << (b - 1) - j) ? 0 : 1;
    //         printf("rank[%d] = %d, rank_teste[%d] =  %d\n", idx, cb.rank0(idx + 1), idx, rank_test[idx]);
    //         if (cb.rank0(idx + 1) == rank_test[idx])
    //             c++;
    //     }
    // }
    // if (c == n * b)
    //     printf("OK\n");

    // int select_test[10 * 4];
    // memset(select_test, -1, sizeof select_test);
    // c = 0;
    // int r = 0;
    // for (int i = 0, j = 0; i < n * b; i++)
    // {
    //     if (rank_test[i] > r)
    //     {
    //         select_test[j] = i + 1;
    //         r++;
    //         j++;
    //     }
    // }

    // for (int i = 0; i < n * b; i++)
    // {
    //     int select = cb.select0(i + 1);
    //     printf("select[%d] =  %d, select_test[%d] = %d\n", i + 1, select, i + 1, select_test[i]);
    //     if (select == select_test[i])
    //         c++;
    // }
    // if (c == n * b)
    //     printf("OK\n");
// }

void compressed_bitvector_mem_test() {
    // cout << "Compact Bitvector\n";

    srand(time(NULL));

    const uint n = 10000 * 5;
    // vector<bool>* bs = new vector<bool>(n);
    BitvectorTest *bitvector_test = new BitvectorTest(n);
    for (int i = 0; i < n; i++)
        bitvector_test->bitvector.at(i) = (rand() % 100 + 1) <= 50;
    // cout << "Uncompact:\n";
    malloc_count_print_status();
    const uint block_size = 32;
    const uint block_num = bitvector_test->bitvector.size() / block_size + 1;
    CompressedBitvector bitvector(block_size, block_num, bitvector_test->bitvector);
    delete bitvector_test;
    // cout << "Compact:\n";
    malloc_count_print_status();
}