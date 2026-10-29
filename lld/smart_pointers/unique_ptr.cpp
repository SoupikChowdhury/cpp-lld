#include<bits/stdc++.h>

using namespace std;
template<typename T>
class UniquePtr
{
    private:
        T* ptr;
    public:
        UniquePtr():ptr(nullptr){} //default constructor

        UniquePtr(T* p):ptr(p){} //parameterized constructor

        ~UniquePtr() //destructor
        {
            delete ptr;
        }

        //delete copy constructor and copy assignment operator
        UniquePtr(const UniquePtr& other) = delete;
        UniquePtr& operator=(const UniquePtr& other) = delete;

        //move constructor
        UniquePtr(UniquePtr&& other) noexcept
        {
            ptr = other.ptr;
            other.ptr = nullptr;
        }

        //move assignment operator
        UniquePtr& operator=(UniquePtr&& other) noexcept
        {
            if(this != &other)
            {
                delete ptr;
                ptr = other.ptr;
                other.ptr = nullptr;
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

        void reset(T* p = nullptr) //reset method
        {
            delete ptr;
            ptr = p;
        }

        T* release() //release method
        {
            T* temp = ptr;
            ptr = nullptr;
            return temp;
        }
};
int main(){
    UniquePtr<int> ptr1(new int(10));
   // UniquePtr<int> ptr2(ptr1); //compilation error
   // UniquePtr<int> ptr3 = ptr1;//compilation error
    UniquePtr<int> ptr4(new int(500));
   // ptr4=ptr1;//compilation error
    UniquePtr<int> ptr3 = std::move(ptr1); //move constructor
    ptr4 = std::move(ptr3); //move assignment operator
   // ptr1->func(); //compilation error: int has no members, -> needs a class type
    ptr1.reset(new int(20)); //ptr1 is empty after the move, give it a new value before using it
    cout<<*ptr1<<endl;
    ptr1.get();


}