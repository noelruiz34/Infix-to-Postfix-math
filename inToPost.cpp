#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct opNode
{
    char parth = ' ';
    string nums = " ";
    opNode *next = nullptr;
};
struct numNodes
{
    string sNum = "";
    unsigned long int num;
    numNodes *next = nullptr;
};
class stack
{
    public:
        stack();
        opNode *top;
        void pop();
        void push(char _new);
        void push(string _new);
        void print();
        bool isEmpty();
};
struct node
{
    string line = "";
    string postfix = "";
    string output = "";
    string og = " ";
    bool balanced = false;
    bool valid = true;
    bool checkBal();
    node *next = nullptr;
    stack *chars = new stack;
};
bool node::checkBal()
{
    return false;
}
stack::stack()
{
    top = nullptr;
}
bool stack::isEmpty()
{
    if(this->top == nullptr)
    {
        return true;
    }
    return false;
}
void stack::pop()
{
    opNode *temp;
    temp = this->top;
    if(!this->isEmpty())
    {
        this->top = this->top->next;
        delete temp;
    }
}
void stack::push(char _new)
{
    opNode *add = new opNode;
    add->parth = _new;
    add->next = this->top;
    this->top = add;
}
void stack::push(string _new)
{
    opNode *add = new opNode;
    add->nums = _new;
    add->next = this->top;
    this->top = add;
}
void stack::print()
{
    opNode *temp;

    temp = this->top;

    cout << "This is the stack of paranthesis\n-------------------------------\n";
    while(temp != nullptr)
    {
        cout << "Data: " << temp->parth << endl;
        temp = temp->next;
    }
}
void removeEnd(string &line)
{
    if (*line.rbegin() == '\r')
    {
        line.erase(line.length() - 1);
    }
}
node *readFile(ifstream &file)
{
    string line;
    node *cur = nullptr;
    node *head = nullptr;

    if(!file.is_open())
    {
        return head;
    }

    while(!file.eof())
    {
        getline(file, line);
        removeEnd(line);
        if(line != "\0")
        {
            if(cur == nullptr)
            {
                cur = new node;
                cur->line = line;
                cur->og = line;

                head = cur;
                cur->next = new node;
                cur = cur->next;
            }
            else
            {
                cur->line = line;
                cur->og = line;
                cur->next = new node;
                cur = cur->next;
            }
        }
    }
    return head;
}
void checkBal(node *cur)
{
    node *temp;
    temp = cur;
    for(int i = 0; i < temp->line.length(); i++)
    {
        if(temp->line.at(i) == '(')
        {
            temp->chars->push(temp->line.at(i));
        }
        else if(temp->line.at(i) == ')')
        {
            temp->chars->push(temp->line.at(i));
        }
        if(temp->chars->top != nullptr && temp->chars->top->next != nullptr)
        {
            if(temp->chars->top->parth == ')' && temp->chars->top->next->parth == '(')
            {
                temp->chars->pop();
                temp->chars->pop();
            }
        }
        
        
    }
    if(temp->chars->isEmpty())
    {
        temp->balanced = true;
    }
    else
    {
        temp->balanced = false;
    }
}
bool isOperator(char check)
{
    if(check == '+' || check == '-' || check == '*' || check == '/')
        return true;
    
    return false;
}
bool isOperator(string check)
{
    if(check == "+" || check == "-" || check == "*" || check == "/")
        return true;
    
    return false;
}
void checkValid(node *cur)
{
    string line;
    line = cur->line;
    node *temp;
    temp = cur;

    for(int i = 0; i < line.length(); i++)
    {
        if((i+1) != line.length() && line.at(i) == ')' && line.at(i+1) == '(')
        {
            temp->valid = false;
            break;
        }
        else if(!isdigit(line.at(i)) && (i+1) != line.length() && isOperator(line.at(i)) && isOperator(line.at(i+1)))
        {
            temp->valid = false;
            break;
        }
        else if(line.at(i) == '(' && line.at(i+1) == '*')
        {
            temp->valid = false;
            break;
        }
        else if(line.at(i) == '*' && line.at(i+1) == '*')
        {
            temp->valid = false;
            break;
        }
        else if(line.at(i) == '/' && line.at(i+1) == '/')
        {
            temp->valid = false;
            break;
        }
    }
}
void checkList(node *head)
{
    node *cur = head;
    while(cur->next  != nullptr)
    {
        checkValid(cur);
        if(cur->valid == true)
        {
            checkBal(cur);
        }
        cur = cur->next;
    }
}
int checkPre(char operand)
{
    if(operand == '+' || operand == '-')
    {
        return 1;
    }
    else if(operand == '*' || operand == '/')
    {
        return 2;
    }
    else if (operand == ')')
    {
        return 3;
    }
    return -1;
}
void inToPost(node *cur)
{
    node *temp;
    numNodes *numHead;
    numNodes *iterator;
    string line;
    string hold;
    temp = cur;

    while(temp->next != nullptr)
    {
        if(temp->balanced == true && temp->valid == true)
        {
            line = temp->line;
            int j;
            for(int i =0; i < line.length(); i++)
            {
                
                if(isdigit(line.at(i)))
                {
                    if(numHead == nullptr)
                    {
                        numHead = new numNodes;
                        iterator = numHead;
                    }
                    j = i;
                    while(j < line.length() && isdigit(line.at(j)))
                    {
                        temp->postfix += line.at(j);
                        j++;
                    }
                    temp->postfix += " ";
                    i = j-1;
                }
                if(line.at(i) == '+' || line.at(i) == '-' || line.at(i) == '*' || line.at(i) == '/' || line.at(i) == '(' || line.at(i) == ')')
                {
                    
                    if(temp->chars->isEmpty())
                    {
                        temp->chars->push(line.at(i));
                    }
                    else if(isOperator(line.at(i)))
                    {
                        if(checkPre(line.at(i)) > checkPre(temp->chars->top->parth))
                        {
                            temp->chars->push(line.at(i));
                        }
                        else if((checkPre(line.at(i)) == checkPre(temp->chars->top->parth)))
                        {
                            hold = temp->chars->top->parth;
                                temp->postfix += hold;
                                temp->postfix += " ";
                                temp->chars->pop();
                                temp->chars->push(line.at(i));
                        }
                        else
                        {
                            while(!temp->chars->isEmpty() && (checkPre(line.at(i)) < checkPre(temp->chars->top->parth)))
                            {
                                hold = temp->chars->top->parth;
                                temp->postfix += hold;
                                temp->postfix += " ";
                                temp->chars->pop();
                            }
                            temp->chars->push(line.at(i));
                        }
                    }
                    else if(line.at(i) == ')')
                    {
                        while (temp->chars->top->parth != '(')
                        {
                            hold = temp->chars->top->parth;
                            temp->chars->pop();
                            temp->postfix += hold;
                            temp->postfix += " ";
                        }
                        temp->chars->pop();
                    }
                    else
                    {
                        temp->chars->push(line.at(i));
                    }   
                }
            }
            //if stack not empty push everything to end
            if(!temp->chars->isEmpty())
            {
                while(!temp->chars->isEmpty())
                {
                    hold = temp->chars->top->parth;
                    temp->postfix += hold;
                    temp->postfix += " ";
                    temp->chars->pop();
                }
            }
        }
        cout << "Postfix: " << temp->postfix << endl;
        temp = temp->next;
    }
}
void insertZero(node *cur, string line, int key)
{
    string _after;
    char hold;
    for(int i = 0; i < line.length(); i++)
    {
        if(i == key)
        {
            _after += "0";
        }
        _after += line.at(i);
    }
    cur->line = _after;
}
void chargeCheck(node *head)
{
    node *temp;
    string line;
    int size;
    temp = head;

    while (temp->next != nullptr)
    {
        line = temp->line;
        size = line.length();
        for(int i = 0; i < size; i++)
        {
            line = temp->line;
            size = line.length();
            if(isOperator(line.at(i)) && line.at(i) == '(' && line.at(i+1) == '-')
            {
                insertZero(temp, line, i);
                i= i+1;
            }
            else if(isOperator(line.at(i)) && line.at(i) == '+' && line.at(i-1) == '(')
            {
                insertZero(temp, line, i);
                i= i+1;
            }
            else if(isOperator(line.at(i)) && line.at(i) == ')' && line.at(i+1) == '-' && line.at(i+2) == '(')
            {
                i = i+2;
            }
            else if(isOperator(line.at(i)) && line.at(i) == '-' && line.at(i+1) == '-')
            {
                string hold;
                for(int j = 0; j < size;j++)
                {
                    if(line.at(j) == '-' && line.at(j+1) == '-')
                    {
                        hold += '+';
                        j = j +2;
                    }
                    hold += line.at(j);
                }
                temp->line = hold;
            }
            else if(isOperator(line.at(i)) &&i!=0 && line.at(i) == '-' && line.at(i-1) == '(')
            {
                insertZero(temp, line, i);
                i = i + 2;
            }
            else if(isOperator(line.at(i)) && i==0 && line.at(i) == '-')
            {
                insertZero(temp, line, i);
                i += 2;
            }
        }
        temp = temp->next;
    }
    
}
bool isdigit(string word)
{
    std::string s = word;
    std::istringstream check(s);
    long long int num = 0;

    if ((check >> num).fail()) {
        return false;
    }
    return true; 

}
void reverse(string& str) 
{ 
    int n = str.length(); 
  
    for (int i = 0; i < n / 2; i++) 
        swap(str[i], str[n - i - 1]); 
} 
string longNum(node *cur, string &data)
{    
    string a,b;
    string endProd = "";
    int strLen;
    int carry = 0;

    a = cur->chars->top->nums;
    cur->chars->pop();
    b = cur->chars->top->nums;
    cur->chars->pop();

    if(data == "+")
    {
        if(a.length() > b.length())
        {
            string hold;
            hold = b;
            b = a;
            a = hold;
        }
        
        strLen = b.length() - a.length();
        for (int i = a.length() - 1;i >= 0; i--) 
        { 
            int sum = ((a[i] - '0') + (b[i + strLen]-'0') + carry); 
            endProd.push_back(sum%10 + '0'); 
            carry = sum / 10; 
        } 
        for (int i = b.length() - a.length() - 1; i >= 0; i--) 
        { 
            int sum = ((b[i] - '0') + carry); 
            endProd.push_back(sum % 10 + '0'); 
            cout << "End: " << endProd << endl;
            carry = sum/10; 
        } 
        if (carry)
        {
            endProd.push_back(carry+'0');
            cout << "End: " << endProd << endl;
        }
        reverse(endProd);  
        return endProd;  
    }
    else if(data == "-")
    {
        if(a.length() < b.length())
        {
            string hold;
            hold = b;
            b = a;
            a = hold;
        }

        strLen = a.length() - b.length();
        carry = 0;

        for (int i = b.length() - 1; i >= 0; i--) 
        {  
            int sub = ((a[i + strLen]-'0') - (b[i] - '0') - carry); 
            if (sub < 0) 
            { 
                sub = sub + 10; 
                carry = 1; 
            } 
            else
            {
                carry = 0;
            }
            endProd.push_back(sub + '0'); 
        }


        for (int i=a.length()-b.length()-1; i>=0; i--) 
        { 
            if (a[i]=='0' && carry) 
            { 
                endProd.push_back('9'); 
                continue; 
            } 
            int sub = ((a[i]-'0') - carry); 
            if (i > 0 || sub > 0)
            {
                endProd.push_back(sub+'0');
            } 
                 
            carry = 0; 
        }
        reverse(endProd); 
        cout << "End: " << endProd << endl;  
        return endProd;
    }
    else if(data == "*")
    {
       int len1 = a.size(); 
    int len2 = b.size(); 

    vector<int> result(len1 + len2, 0); 
    
    int i_n1 = 0;  
    int i_n2 = 0;  
      
    for (int i=len1-1; i>=0; i--) 
    { 
        int carry = 0; 
        int n1 = a[i] - '0'; 
  
        i_n2 = 0;  
          
        for (int j=len2-1; j>=0; j--) 
        { 
            int n2 = b[j] - '0';  
            int sum = n1*n2 + result[i_n1 + i_n2] + carry; 
  
            carry = sum/10; 
  
            result[i_n1 + i_n2] = sum % 10; 
  
            i_n2++; 
        } 
  
        if (carry > 0) 
            result[i_n1 + i_n2] += carry; 

        i_n1++; 
    } 
  
        int i = result.size() - 1; 
        while (i>=0 && result[i] == 0) 
            i--; 
        stringstream s; 
        
        while (i >= 0) 
            s << result[i--];
        
        endProd = s.str();
        cout << "End: " << endProd << endl;
        return endProd;
  
    }
    else if(data == "/")
    {
        if(a == b)
        {
            endProd += "1";
        }
    }
    return endProd;
}
void evaluate(node *head, ofstream &out)
{
    node *temp;
    string data;
    long long int a;
    long long int b;
    long long int result = 0;
    temp = head;

    while(temp->next != nullptr)
    {
        if(temp->balanced == false || temp->valid == false)
        {
            out << "error" << endl;
        }
        else
        {
            stringstream postfix(temp->postfix);
            while (postfix >> data)
            {
                if(isdigit(data))
                {
                    data.erase(0, min(data.find_first_not_of('0'), data.size()-1));
                    temp->chars->push(data);
                }
                else if(isOperator(data))
                {
                    if(temp->chars->top->nums.size() < 16 && temp->chars->top->next->nums.size() < 16)
                    {
                        data.erase(0, min(data.find_first_not_of('0'), data.size()-1));
                        a = stoll(temp->chars->top->nums, 0, 10);
                        temp->chars->pop();
                        b = stoll(temp->chars->top->nums, 0, 10);
                        temp->chars->pop();
                        if(data == "+") result = b + a;
                        else if(data == "-")result = b - a;
                        else if(data == "*") result = b * a;
                        else if(data == "/") result = b / a;
                        stringstream num;
                        num << result;
                        data = num.str();
                        temp->chars->push(data);
                    }
                    else
                    {
                        string tooLong;
                        tooLong = longNum(temp, data);
                        temp->chars->push(tooLong);
                    }                    
                    
                }
            }
            temp->output += temp->og + "=" + temp->chars->top->nums;;
            out << temp->output << endl;
            
        }
        temp = temp->next;
        
        
    }

}

int main()  //MAIN
{

    string input;
    string output;

    cout << "Enter input filename: ";
    cin >> input;
    cout << "Enter output filename: ";
    cin >> output;

    ifstream inFS(input);
    ofstream outFS(output);

    node *head;
    head = readFile(inFS);
    if(head == nullptr)
    {
        outFS << "error" << endl;
        return 0;
    }
    node *temp;
    temp = head;
    checkList(head);
    chargeCheck(head);
    checkValid(head);
    inToPost(head);

    evaluate(head, outFS);

    cout << "\n*VISIT OUTPUTFILE*\n";

    return 0;
}