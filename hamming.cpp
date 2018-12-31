#include<iostream>
using namespace std;
int main(){
    int data[10],datarec[10],c,c1,c2,c3,i;
    cout<<"enter 4 bits of data one by one"<<endl;
    cin>>data[0];
    cin >> data[1];
    cin >> data[2];
    cin >> data[4];
    data[6]=data[0]^data[2]^data[4];
    data[5] = data[0] ^ data[1] ^ data[4];
    data[3] = data[0] ^ data[1] ^ data[2];
    cout<<"encoded data is:"<<endl;
    for(i=0;i<7;i++)
        cout<<data[i];
    
    cout << "enter recived data:" << endl;
    for (i = 0; i < 7; i++)
    	cin>>datarec[i];
    
    c1=datarec[6]^datarec[4]^datarec[2]^datarec[0];
    c2 = datarec[5] ^ datarec[4] ^ datarec[1] ^ datarec[0];
    c3 = datarec[3] ^ datarec[1] ^ datarec[1] ^ datarec[0];
    c=c3*4+c2*2+c1;
    if(c==0){
        cout<<"No error in transmission"<<endl;
    }
    else{
        cout<<"error on position"<<c<<endl;
        cout<<"data sent:"<<endl;
        for(i=0;i<7;i++)
            cout<<data[i];
        
        cout<<"data recived:"<<endl;
        for (i = 0; i < 7; i++)
             cout << datarec[i];
        
        cout<<"correct message is:"<<endl;
        if(datarec[7-c]==0)
            datarec[7-c]=1;
        else
            datarec[7-c]=0;
            
        
        
        for (i = 0; i < 7; i++)
        {
            cout << datarec[i];
        }
    }
    return 0;
}
