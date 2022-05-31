#include<iostream>
using namespace std;
int main()
{
    int number;
    int temp=1;
    cout<<"enter the numbers to determine its factors ";
    cin>>number;
    cout<<"the factors of number are "<<number <<":"<<endl;
    while(temp<=number)
    {
        if(not(number % temp))
            cout<<temp<<" ";
        temp++;
    }
    cout<<endl;
}