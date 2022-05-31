#include<iostream>
usining namespace std;
class Complex{
    int real;
    int img;
    void setvalue(){
        cin>>real;
        cin>>img;
    }
    void display(){
        cout<<real<<"+"<<img<<"i"<<endl;
    }
    void sum(Complex c1,Complex c2)
    {
        real=c1.real+c2.real;
        img=c1.img-c2.img;
    }
    void sub(Complex c1,Complex c2)
    {
        real=c1.real-c2.real;
        img=c1.img-c2.img;
    }
};
int main()
{
    Complex c1,c2,c3,c4;
    cout<<"enter the 1st complex number"<<endl;
    c1.setvalue();
    cout<<"enter the 2nd complex number "<<endl;
    c2.setvalue();
    c3.sum(c1,c2);
    c3.display();
    cout<<endl;
    c4.sub(c1,c2);
    c4.dispay();
    return 0;
}