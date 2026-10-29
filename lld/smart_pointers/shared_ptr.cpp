#include<bits/stdc++.h>

using namespace std;
template<typename T>
class SharedPtr
{
    private:
        T* ptr;
        atomic<unsigned int>* refCount; //shared by every SharedPtr that owns the same object, atomic so threads can copy/destroy safely

        void release() //drop this owner, delete the object when the last owner goes
        {
            //fetch_sub returns the old value, so only the thread that drops the count from 1 to 0 deletes
            if(refCount && refCount->fetch_sub(1, memory_order_acq_rel) == 1)
            {
                delete ptr;
                delete refCount;
            }
            ptr = nullptr;
            refCount = nullptr;
        }
    public:
        SharedPtr():ptr(nullptr), refCount(nullptr){} //default constructor

        explicit SharedPtr(T* p):ptr(p), refCount(p ? new atomic<unsigned int>(1) : nullptr){} //parameterized constructor

        ~SharedPtr() //destructor
        {
            release();
        }

        //copy constructor: share ownership, increase count
        SharedPtr(const SharedPtr& other)
        {
            ptr = other.ptr;
            refCount = other.refCount;
            if(refCount)
                refCount->fetch_add(1, memory_order_relaxed);
        }

        //copy assignment operator
        SharedPtr& operator=(const SharedPtr& other)
        {
            if(this != &other)
            {
                release();
                ptr = other.ptr;
                refCount = other.refCount;
                if(refCount)
                    refCount->fetch_add(1, memory_order_relaxed);
            }
            return *this;
        }

        //move constructor: take ownership, count stays the same
        SharedPtr(SharedPtr&& other) noexcept
        {
            ptr = other.ptr;
            refCount = other.refCount;
            other.ptr = nullptr;
            other.refCount = nullptr;
        }

        //move assignment operator
        SharedPtr& operator=(SharedPtr&& other) noexcept
        {
            if(this != &other)
            {
                release();
                ptr = other.ptr;
                refCount = other.refCount;
                other.ptr = nullptr;
                other.refCount = nullptr;
            }
            return *this;
        }

        T& operator*() const //dereference operator
        {
            return *ptr;
        }

        T* operator->() const //arrow operator
        {
            return ptr;
        }

        T* get() const //get method
        {
            return ptr;
        }

        unsigned int use_count() const //number of owners
        {
            return refCount ? refCount->load(memory_order_relaxed) : 0;
        }

        void reset(T* p = nullptr) //reset method
        {
            release();
            if(p)
            {
                ptr = p;
                refCount = new atomic<unsigned int>(1);
            }
        }
};

struct Demo
{
    void func() { cout<<"Demo::func called"<<endl; }
    ~Demo() { cout<<"Demo destroyed"<<endl; }
};

int main(){
    SharedPtr<int> ptr1(new int(10));
    cout<<"ptr1 count: "<<ptr1.use_count()<<endl; //1
    SharedPtr<int> ptr2(ptr1); //copy constructor
    SharedPtr<int> ptr3 = ptr1; //copy constructor
    cout<<"ptr1 count after 2 copies: "<<ptr1.use_count()<<endl; //3
    SharedPtr<int> ptr4(new int(500));
    ptr4 = ptr1; //copy assignment operator, old 500 is deleted
    cout<<"ptr1 count after copy assign: "<<ptr1.use_count()<<endl; //4
    SharedPtr<int> ptr5 = std::move(ptr2); //move constructor, count unchanged
    cout<<"ptr1 count after move: "<<ptr1.use_count()<<", ptr2 count: "<<ptr2.use_count()<<endl; //4, 0
    ptr3 = std::move(ptr5); //move assignment operator, ptr3 drops one share
    cout<<"ptr1 count after move assign: "<<ptr1.use_count()<<endl; //3
    cout<<*ptr1<<endl;
    ptr1.get();
    ptr1.reset(new int(20)); //ptr1 now owns a new object
    cout<<*ptr1<<" count: "<<ptr1.use_count()<<", old object count: "<<ptr3.use_count()<<endl; //20 1, 2

    {
        SharedPtr<Demo> d1(new Demo());
        SharedPtr<Demo> d2 = d1;
        d2->func(); //arrow operator
        cout<<"d1 count: "<<d1.use_count()<<endl; //2
    } //both go out of scope, Demo destroyed once

    {
        //thread safety test: 8 threads each make and destroy 100000 copies of the same pointer
        SharedPtr<Demo> shared(new Demo());
        vector<thread> threads;
        for(int i = 0; i < 8; i++)
        {
            threads.emplace_back([&shared]() {
                for(int j = 0; j < 100000; j++)
                {
                    SharedPtr<Demo> local = shared; //copy: count +1
                } //destroy: count -1
            });
        }
        for(auto& t : threads)
            t.join();
        cout<<"count after threads: "<<shared.use_count()<<endl; //1
    } //Demo destroyed once

    return 0;
}
