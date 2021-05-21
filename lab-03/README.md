# Hash Hash Hash

This code parallelizes hash tables.

## Building

Run make.

## Running

./hash-table-tester -t 4 -s 100000
Generation: 48,651 usec
Hash table base: 1,539,312 usec
  - 0 missing
Hash table v1: 1,861,028 usec
  - 0 missing
Hash table v2: 524,897 usec
  - 0 missing


## First Implementation

With a single mutex, the goal is to prevent all possible race conditions. Race 
conditions are nondeterministic behavior that are only possible when there are 
writes to the hash table's linked lists' entries. That means our lock must lock 
any code that handles nondeterministic behavior/writing to the hash table. 

In the add_entry function, we don't have to lock getting the hash table entry because 
the pointer for an entry of the hash table never changes. It is the same thing for 
getting the linked list head for a hash table entry, this linked list 
implementation utilizes a "dummy head" for the linked list head, so the head never 
changes. Since getting both the hash table entry pointer and linked list head is 
deterministic behavior and neither perform writes they do not need to be locked. 
get_list_entry on the other hand, can be nondeterministic because if there are 
two reads at the same time, they can be updated in different ways if the incoming 
key is the same. Therefore, our lock must start after getting the hash table and 
the linked list head, and before getting the list entry which is what is done.

Since our unlock for the mutex must come after the writes to the linked list which are 
done at the end of the function, that is where the correct unlocking call for the mutex 
must go. This is what is done, therefore the v1 strategy that employs using only one 
mutex is correct.

### Performance

As shown in the "Running" section's example, when ./hash-table-tester is run with 4 
cores and 100000 hash table entries added per thread, the v1 implementation is actually 
20% slower than the the base. This is because the v1 implementation is essentially a single 
threaded approach because it only allows one thread at a time to add an entry to the 
hash map which is essentially the serial version, and then on top of that we have added 
the overhead of locking.

## Second Implementation

With our first implementation, we mentioned that writes to the linked list as well 
as getting the list entry had to be locked. This is because if a list entry is being 
wrote to by one thread while being read from in another thread, there is 
nondeterminstic behavior. However, a problem with this v1 implementation is that
it causes all threads - even if the threads are working on different linked lists
to be locked and go one at a time when adding entries to the hash map. This is 
extremely slow (almost a single thread approached), and unnecessary as the process 
of two threads adding different entries from each other is deterministic.
So the idea developed is to lock every entry instead, so that overhead of 
adding elements to the hash map is greatly reduced when the entries are different, 
and we prevent data races that can occur when the linked lists being accessed by 
two or more different threads are the same.

So, we added a lock for every bucket. Just like v1, we do not have to lock getting 
the hash map entry or getting the linked list head - we have to lock the same region 
as v1. The difference however is, when a bucket is accessed we lock ONLY that 
bucket instead of the entire hash map (as we did in v1), and then we unlock 
the has bucket after making the writes to the linked list. This method is better than 
v1 because it reduces the overhead by allowing different hash map entry's to be added/modified, 
and it is correct because it prevents potential data races.

### Performance

As shown in the "Running" section's example, when ./hash-table-tester is run with 4 
cores and 100000 hash table entries added per thread, the v2 implementation is actually 
3x faster than the the base. This is because we are actually allowing the addition of hash 
table entries to happen in parallel across the 4 cores when the threads are not accessing the 
same hash table entry.


## Cleaning up

Run make clean.
