#include <bits/stdc++.h>

using namespace std;

class Page
{
public:
    int used;
    int size;
    vector<int> records;
    Page *next, *prev;

    Page(int s)
    {
        size = s;
        used = 4;
        next = nullptr;
        prev = nullptr;
    }

    int empty_space()
    {
        return size - 4 - used;
    }

    bool insert(int sz, int pk)
    {
        if (empty_space() >= sz + 4)
        {
            used += sz + 4;
            records.push_back(pk);
            return true;
        }
        return false;
    }

    int search(int pk)
    {
        for (int i = 0; i < records.size(); i++)
        {
            if (records[i] == pk)
                return i;
        }
        return -1;
    }
};

class File
{
public:
    Page *first;
    int pages;
    int page_size;

    File(int s)
    {
        page_size = s;
        first = nullptr;
        pages = 0;
    }

    void insert(int sz, int pk)
    {
        Page *curr = first, *last = nullptr;
        while (true)
        {
            if (!curr)
            {
                curr = new Page(page_size);
                if (!first)
                    first = curr;
                pages++;
                if (last)
                {
                    last->next = curr;
                    curr->prev = last;
                }
            }
            if (curr->insert(sz, pk))
                return;
            last = curr;
            curr = curr->next;
        }
    }

    void display_status()
    {
        cout << pages << " ";
        Page *curr = first;
        while (curr)
        {
            cout << curr->records.size() << " ";
            curr = curr->next;
        }
        cout << endl;
    }

    pair<int, int> search(int pk)
    {
        int page_number = 0;
        Page *curr = first;
        while (curr)
        {
            if (curr->search(pk) >= 0)
                return {page_number, curr->search(pk)};
            curr = curr->next;
            page_number++;
        }
        return {-1, -1};
    }
};

int main()
{

    int PAGE_SIZE;
    cin >> PAGE_SIZE;

    File file(PAGE_SIZE);

    int op, sz, pk;
    while (true)
    {
        cin >> op;
        switch (op)
        {
        case 1:
        {
            cin >> sz >> pk;
            file.insert(sz, pk);
            break;
        }

        case 2:
        {
            file.display_status();
            break;
        }

        case 3:
        {
            cin >> pk;
            pair<int, int> ans = file.search(pk);

            cout << ans.first << " " << ans.second << endl;

            break;
        }

        case 4:
        {
            return 0;
            break;
        }

        default:
        {
            cout << "Invalid Operation" << endl;
            break;
        }
        }
    }
}