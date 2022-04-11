#include <iostream>
using namespace std;

#define index 1
#define data 2

// defining the node class for the use in BPTree
class node
{
public:
    int type;  // 1 for index node, 2 for data node
    int *keys; // array of keys
    int capacity;
    node *parent;                // parent of the node
    node **child_ptr;            // pointer to the children of the node or maybe adjacent node for the data nodes
    int num_of_keys;             // number of keys in the node
    node(int t, int s, node *p); // constructor for the node
    void insert(int x);          // insert a key in the node
    int is_full();               // check if the node is full
    int is_root();
    void correct_child_ptr();
    node *split(); // splits the node and returns the new node
    //destructor
    ~node();
};

// constructor for the node
node::node(int t, int c, node *p)
{
    type = t;
    capacity = c;
    parent = p;
    keys = new int[capacity];
    if (type == index)
    {
        child_ptr = new node *[capacity + 1];
        for (int i = 0; i < capacity + 1; i++)
            child_ptr[i] = NULL;
    }
    if (type == data)
    {
        {
            child_ptr = new node *[2];
            for (int i = 0; i < 2; i++)
                child_ptr[i] = NULL;
        }
    }
    num_of_keys = 0;
}

// destructor for the node
node::~node()
{
    delete[] keys;
    delete[] child_ptr;
}

// insert a key in the node
void node::insert(int x)
{
    int i = num_of_keys - 1;      // i is the index of the last key
    while (i >= 0 && keys[i] > x) // if the key is smaller than the last key
    {
        keys[i + 1] = keys[i]; // shift the keys
        i--;
    }
    keys[i + 1] = x; // insert the key
    num_of_keys++;   // increment the number of keys
}

int node::is_full()
{
    if (num_of_keys == capacity)
        return 1;
    else
        return 0;
}

// splits the node and returns the new node
node *node::split()
{
    node *new_node = new node(type, capacity, parent);
    int mid = capacity / 2;
    for (int i = mid; i < capacity; i++)
    {
        new_node->keys[i - mid] = keys[i];
        new_node->num_of_keys++;
    }
    if (type == index)
    {
        for (int i = mid + 1; i < capacity + 1; i++)
        {
            new_node->child_ptr[i - mid - 1] = child_ptr[i];
            child_ptr[i]->parent = new_node;
        }
    }
    if (type == data)
    {
        new_node->child_ptr[0] = this;
        new_node->child_ptr[1] = child_ptr[1];
        child_ptr[1] = new_node;
    }
    num_of_keys = mid;
    return new_node;
}

int node::is_root()
{
    if (parent == NULL)
        return 1;
    else
        return 0;
}

// defining the B+ tree class
class BPTree
{
public:
    node *root;
    int min_data_keys;
    int max_data_keys;
    int min_index_keys;
    int max_index_keys;
    BPTree(int d, int t);
    void insert(node *nodeptr, int entry, node *new_child_entry);
    void print(node* subtree_root);
};

// constructor for the B+ tree
BPTree::BPTree(int d, int t)
{
    min_data_keys = d;
    max_data_keys = d * 2;
    min_index_keys = t;
    max_index_keys = t * 2 + 1;
    root = new node(data, max_data_keys, NULL);
}

void BPTree::insert(node *nodeptr, int entry, node *new_child_entry)
{
    if (nodeptr->type == index)
    {
        int i = nodeptr->num_of_keys - 1;
        while (i >= 0 && nodeptr->keys[i] > entry) // if the key is smaller than the last key
        {
            i--;
        }
        // the value of i thus obtained satisfies the property key[i] <= entry < key[i+1]
        insert(nodeptr->child_ptr[i + 1], entry, new_child_entry);
        if (new_child_entry == NULL)
            return;
        else
        {
            if (nodeptr->is_full() != 1)
            {
                nodeptr->insert(new_child_entry->keys[0]);
                new_child_entry = NULL;
                return;
            }
            else
            {
                new_child_entry = nodeptr->split();
                if (nodeptr->is_root() == 1)
                {
                    root = new node(index, max_index_keys, NULL); // creating a new index node as the root node
                    root->keys[0] = new_child_entry->keys[0];     // the smallest of the right node gets entered as the seperator key
                    root->num_of_keys = 1;
                    root->child_ptr[0] = nodeptr;
                    root->child_ptr[1] = new_child_entry;
                    nodeptr->parent = root;
                    new_child_entry->parent = root;
                    return;
                }
            }
        }
    }
    if (nodeptr->type == data)
    {
        if (nodeptr->is_full() != 1)
        {
            nodeptr->insert(entry);
            new_child_entry = NULL;
            return;
        }
        else
        {
            new_child_entry = nodeptr->split();
            return;
        }
    }
}

//recursively print the B+ tree
void BPTree::print(node* subtree_root)
{
    if (subtree_root == NULL)
        return;
    if (subtree_root->type == data)
    {
        for (int i = 0; i < subtree_root->num_of_keys; i++)
            cout << subtree_root->keys[i] << " ";
        cout << endl;
    }
    if (subtree_root->type == index)
    {
        for (int i = 0; i < subtree_root->num_of_keys; i++)
            cout << subtree_root->keys[i] << " ";
        cout << endl;
        for (int i = 0; i < subtree_root->num_of_keys + 1; i++)
            print(subtree_root->child_ptr[i]);
    }

}

int main()
{
    int n, d, t;
    cin >> d >> t;
    BPTree bpt(d, t);
    while (cin >> n)
    {
        switch (n)
        {
        case 1:
            // insert
            int entry;
            cin >> entry;
            bpt.insert(bpt.root, entry, NULL);
            break;

        case 2:
            // print
            bpt.print(bpt.root);
            break;

        case 3:
            // exit
            return 0;

        default:
            break;
        }
    }
}
