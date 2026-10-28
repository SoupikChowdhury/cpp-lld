
using namespace std;
class String
{
    public:
        String():res(nullptr), length(0) {} //default constructor
        String(const char *ch) //parameterized constructor
        {
            length = strlen(ch);
            res = new char[length + 1];
            strcpy(res, ch);
        }
        String(const String &other) //copy constructor
        {
            length = other.length;
            res = new char[length + 1];
            strcpy(res, other.res);
        }
        String(String &&other) noexcept //move constructor
        {
            res = other.res;
            length = other.length;
            other.res = nullptr;
            other.length = 0;
        }
       /* String& operator=(const String &other) //copy assignment operator
        {
            if(this != &other)
            {
                char* temp=res;
                length = other.length;
                res = new char[length + 1];
                strcpy(res, other.res);
                delete[] temp;
            }
            return *this;
        }*/
       swap(String& str1, String& str2) //swap function for copy and swap idiom
       {
            std::swap(str1.res, str2.res);
            std::swap(str1.length, str2.length);
       }
       String& operator=(String other) //better copy assignment operator with CAS(copy and swap idiom)
        {
            swap(*this,other);
            return *this;
        }
        String& operator=(String &&other) noexcept //move assignment operator
        {
            if(this != &other)
            {
                delete[] res;
                res = other.res;
                length = other.length;
                other.res = nullptr;
                other.length = 0;
            }
            return *this;
        }
        unsigned int length() const //length method
        {
            return length;
        }
        ~String() //destructor
        {
            if(res)
            {
                delete[] res;
                res = nullptr;
                length = 0;
            }            
        }
        friend ostream& operator<<(ostream &out, const String &str); //overload output(<<) operator
        friend istream& operator>>(istram &in, string &str); //overload >> operator

    private:
        char *res;
        unsigned int length;

}
ostream& operator<<(ostream &out, const String &str) //overload output(<<) operator
{
    out << str.res;
    return out;
}
istream& operator>>(istream& in,String &str)
{
    in>>str.res;
    return in;
}

int main() {
    String str1;//default constructor
    String str2 = "Hello";//parameterized constructor
    String str3 = str1;//copy constructor  String str3(str1) same thing
    str3 = str2;//copy assignment operator
    int len = str2.length();//length method
    cout<<str2;//overload output(<<) operator
    cin>>str1;//overload input(>>) operator
    String str6 = std::move(str2); //move constructor
    str6 = move(str3); //move assignment operator

    return 0;
}