#include <iostream>
#include <utility>

using namespace std;
template<typename T>
class UniquePtr
{
    private:
        T* ptr;
    public:
        UniquePtr():ptr(nullptr){} //default constructor

        explicit UniquePtr(T* p):ptr(p){} //parameterized constructor, explicit so a raw pointer can't become owned by accident

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
                //take other's pointer first: other may live inside the object we are about to delete (head = move(head->next))
                reset(other.release());
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

        explicit operator bool() const //so if(ptr) works
        {
            return ptr != nullptr;
        }

        void reset(T* p = nullptr) //reset method
        {
            if(p == ptr) //resetting to the pointer we already own must not delete it
                return;
            T* old = ptr;
            ptr = p; //update first: deleting old may run code that reaches this pointer
            delete old;
        }

        T* release() //release method
        {
            T* temp = ptr;
            ptr = nullptr;
            return temp;
        }
};

struct Node
{
    int value;
    UniquePtr<Node> next;
    explicit Node(int v):value(v){}
    ~Node() { cout<<"Node "<<value<<" destroyed"<<endl; }
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

    //a linked list: dropping the first node must keep the rest
    UniquePtr<Node> head(new Node(1));
    head->next = UniquePtr<Node>(new Node(2));
    head = std::move(head->next); //only node 1 is destroyed
    cout<<"head is now node "<<head->value<<endl;
}
