#include <bits/stdc++.h>
using namespace std;

#define max_global_depth 20

// class bucket
class bucket
{
public:
    int creation_time;
    int bucket_capacity;
    int local_depth;
    int num_keys;
    int index;
    int *arr;
    bucket(int size, int ld, int ct, int idx);
    ~bucket();
    void sort_bucket(bucket *b);
};

// bucket constructor
bucket::bucket(int size, int ld, int ct, int idx)
{
    bucket_capacity = size;
    local_depth = ld;
    creation_time = ct;
    num_keys = 0;
    index = idx;
    arr = new int[size];
    for (int i = 0; i < size; i++)
    {
        arr[i] = -1; // -1 means empty
    }
}

// bucket destructor
bucket::~bucket()
{
    delete[] arr;
}

// class hash table
class ht
{
public:
    int global_depth;
    int bucket_capacity;
    int creation_clock;
    int num_buckets;
    bucket **dir;
    bucket *insert(int x);
    bucket *search(int x);
    void print();
    void delete_value(int x);
    ht(int gd, int bc);
    void print_all();

private:
    void split_bucket(bucket *b);
    bucket *merge_bucket(bucket *b1, bucket *b2);
    void double_dir();
};

// sort the bucket b so that all -1 are in the end
void bucket::sort_bucket(bucket *b)
{
    for(int i = 0; i < b->bucket_capacity; i++)
    {
        for (int j = i+1; j < b->bucket_capacity; j++)
        {
            if(b->arr[i] == -1 && b->arr[j] != -1)
            {
                int temp = b->arr[i];
                b->arr[i] = b->arr[j];
                b->arr[j] = temp;
            }
        }
        
    }
}

// hash table constructor
ht::ht(int gd, int bc)
{
    global_depth = gd;
    bucket_capacity = bc;
    dir = new bucket *[1 << global_depth];
    bucket *first_bucket = new bucket(bucket_capacity, 0, 0, 0);
    creation_clock = 0;
    num_buckets = 1;
    for (int i = 0; i < (1 << global_depth); i++)
    {
        dir[i] = first_bucket;
    }
}

// function to double the directory when local depth exceeds global depth
void ht::double_dir()
{
    global_depth++;
    bucket **new_dir = new bucket *[1 << global_depth];
    for (int i = 0; i < (1 << (global_depth - 1)); i++)
    {
        new_dir[i] = dir[i];
    }
    for (int i = (1 << (global_depth - 1)); i < (1 << global_depth); i++)
    {
        new_dir[i] = dir[i - (1 << (global_depth - 1))];
    }
    delete[] dir;
    dir = new_dir;
}

// function to split a bucket in a hash table
void ht::split_bucket(bucket *b)
{
    b->local_depth++;
    if (b->local_depth > global_depth)
    {
        double_dir();
    }
    creation_clock++;
    bucket *new_bucket = new bucket(bucket_capacity, b->local_depth, creation_clock, b->index + (1 << (b->local_depth - 1)));
    for (int i = 0; i < bucket_capacity; i++)
    {
        if ((b->arr[i] % (1 << (b->local_depth))) == new_bucket->index)
        {
            new_bucket->arr[new_bucket->num_keys] = b->arr[i];
            new_bucket->num_keys++;
            b->arr[i] = -1;
            b->num_keys--;
        }
    }
    // sort the new bucket so that all -1 are in the end
    new_bucket->sort_bucket(new_bucket);
    // sort the bucket b so that all -1 are in the end
    b->sort_bucket(b);
    // insert the new bucket in the directory
    for (int i = 0; i < (1 << global_depth); i++)
    {
        if (i % (1 << (b->local_depth)) == new_bucket->index)
        {
            dir[i] = new_bucket;
        }
    }
}

// fuction to merge two buckets
bucket *ht::merge_bucket(bucket *b1, bucket *b2)
{
    creation_clock++;
    bucket *new_bucket = new bucket(bucket_capacity, b1->local_depth - 1, creation_clock, min(b1->index, b2->index));
    for (int i = 0; i < bucket_capacity; i++)
    {
        if (b1->arr[i] != -1)
        {
            new_bucket->arr[i] = b1->arr[i];
            new_bucket->num_keys++;
        }
        else
        {
            new_bucket->arr[i] = b2->arr[i];
            new_bucket->num_keys++;
        }
    }
    return new_bucket;
}

// search from hash table
bucket *ht::search(int x)
{
    int index = x % (1 << global_depth);
    bucket *b = dir[index];
    for (int i = 0; i < b->num_keys; i++)
    {
        if (b->arr[i] == x)
        {
            return b;
        }
    }
    return NULL;
}

// insert into hash table
bucket *ht::insert(int x)
{
    int hash_value = x % (1 << global_depth);
    bucket *b = dir[hash_value];
    if (b->num_keys == bucket_capacity)
    {
        if (global_depth < max_global_depth)
        {
            split_bucket(b);
            num_buckets++;
            return insert(x);
        }
        else
        {
            cout << "Hash table is full" << endl;
            return NULL;
        }
    }
    else
    {
        b->arr[b->num_keys] = x;
        b->num_keys++;
        return b;
    }
}

// output values for the Global Depth, Number of buckets, and list of pairs <number of keys, local depth> for each bucket
void ht::print()
{
    cout << "Global Depth: " << global_depth << endl;
    cout << "Number of buckets: " << (num_buckets) << endl;

    for (int c = 0; c < creation_clock + 1; c++)
    {
        for (int i = 0; i < (1 << global_depth); i++)
        {
            if (dir[i]->creation_time == c)
            {
                cout << "Bucket " << i << ": " << dir[i]->num_keys << " keys, " << dir[i]->local_depth << " local depth" << endl;
                break;
            }
        }
    }
}

// delete from hash table
void ht::delete_value(int x)
{
    int hash_value = x % (1 << global_depth);
    bucket *b = dir[hash_value];
    for (int i = 0; i < b->num_keys; i++)
    {
        if (b->arr[i] == x)
        {
            b->arr[i] = -1;
            b->num_keys--;
            break;
        }
    }
    if (b->num_keys == 0)
    {
        dir[hash_value] = NULL;
    }
    else
    {
        // sort the bucket so that all -1 are in the end
        b->sort_bucket(b);
    }
}

// print all the values in order of creation time
void ht::print_all()
{
    for (int c = 0; c < creation_clock + 1; c++)
    {
        for (int i = 0; i < (1 << global_depth); i++)
        {
            if (dir[i]->creation_time == c)
            {
                for (int j = 0; j < dir[i]->num_keys; j++)
                {
                    cout << dir[i]->arr[j] << " ";
                }
                cout << endl;
                break;
            }
        }
    }
}

int main()
{
    int g_depth, b_capacity;
    cin >> g_depth >> b_capacity;

    ht h(g_depth, b_capacity);
    int input;

    while (1)
    {
        cin >> input;
        switch (input)
        {
            case 2: // insert
            {
                int x;
                cin >> x;
                h.insert(x);
                break;
            }
            case 3: // search
            {
                int x;
                cin >> x;
                h.search(x);
                break;
            }
            case 4: // delete
            {
                int x;
                cin >> x;
                h.delete_value(x);
                break;
            }

            case 5: // print
            {
                h.print();
                break;
            }
            case 6: // print all
            {
                h.print_all();
                break;
            }
            case 7: // exit
            {
                return 0;
            }
            default:
            {
                cout << "Invalid input" << endl;
            }
        }
    }
}