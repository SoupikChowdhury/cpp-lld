#include <iostream>
#include <cstring>
#include <string>
#include <utility>
using namespace std;
class String
{
    public:
        String():res(new char[1]{'\0'}), len(0) {} //default constructor
        String(const char *ch) //parameterized constructor
        {
            len = strlen(ch);
            res = new char[len + 1];
            strcpy(res, ch);
        }
        String(const String &other) //copy constructor
        {
            len = other.len;
            res = new char[len + 1];
            strcpy(res, other.res ? other.res : "");
        }
        String(String &&other) noexcept //move constructor
        {
            res = other.res;
            len = other.len;
            other.res = nullptr;
            other.len = 0;
        }
       /* String& operator=(const String &other) //copy assignment operator
        {
            if(this != &other)
            {
                char* temp=res;
                len = other.len;
                res = new char[len + 1];
                strcpy(res, other.res);
                delete[] temp;
            }
            return *this;
        }*/
       friend void swap(String& str1, String& str2) noexcept //swap function for copy and swap idiom
       {
            std::swap(str1.res, str2.res);
            std::swap(str1.len, str2.len);
       }
       String& operator=(const String &other) //better copy assignment operator with CAS(copy and swap idiom)
        {
            String temp(other);
            swap(*this, temp);
            return *this;
        }
        String& operator=(String &&other) noexcept //move assignment operator
        {
            if(this != &other)
            {
                delete[] res;
                res = other.res;
                len = other.len;
                other.res = nullptr;
                other.len = 0;
            }
            return *this;
        }
        unsigned int length() const //length method
        {
            return len;
        }
        ~String() //destructor
        {
            if(res)
            {
                delete[] res;
                res = nullptr;
                len = 0;
            }
        }
        friend ostream& operator<<(ostream &out, const String &str); //overload output(<<) operator
        friend istream& operator>>(istream &in, String &str); //overload >> operator

    private:
        char *res;
        unsigned int len;

};
ostream& operator<<(ostream &out, const String &str) //overload output(<<) operator
{
    if(str.res)
        out << str.res;
    return out;
}
istream& operator>>(istream& in,String &str)
{
    string word;
    in >> word;
    str = String(word.c_str());
    return in;
}

int main() {
    String str1;//default constructor
    String str2 = "Hello";//parameterized constructor
    String str3 = str1;//copy constructor  String str3(str1) same thing
    str3 = str2;//copy assignment operator
    int len = str2.length();//length method
    cout<<str2<<" "<<len<<endl;//overload output(<<) operator
    cin>>str1;//overload input(>>) operator
    String str6 = std::move(str2); //move constructor
    str6 = move(str3); //move assignment operator

    return 0;
}
