#include<bits/stdc++.h>
#include <windows.h>
#include <string>

using namespace std;

int matrix[100][100];
int storeBit[1000][1000];
int row=0;
int columnSize=0;
int column=0;
vector<int>tempStore;
int numberofCount;
//string rem;
int powerOfTwo[12]= {1,2,4,8,16,32,64,128,256,512,1024,2048};

void strToBinary(string s,int m)
{
    int n = s.length();

    int count=0;

    for (int i = 0; i <n; i++)
    {

        // convert each char to
        // ASCII value
        int val = int(s[i]);

        // Convert ASCII value to binary
        string bin = "";
        while (val > 0)
        {
            (val % 2)? bin.push_back('1'):
            bin.push_back('0');
            val /= 2;
        }
        //cout<<bin<<endl;
        //cout<<bin.length()<<endl;
        if(bin.length()<8)
        {
            while(bin.length()!=8)
            {
                bin=bin+'0';
            }
        }
        reverse(bin.begin(), bin.end());
        //cout<<bin<<endl;
        //cout<<bin<<endl;

        for(int j=0; j<bin.length(); j++)
        {
            matrix[row][column++]=int(bin[j])-48;
        }
        count++;
        if(count==m)
        {
            row++;
            column=0;
            count=0;
        }

    }
}

void makeParity(vector<int>v,int r)
{

    int len=v.size();
    int cnt=0;
    while(len>0)
    {
        len=len/2;
        cnt++;
    }
    numberofCount=cnt;
    for(int i=0; i<cnt; i++)
    {
        v.insert(v.begin()+powerOfTwo[i]-1,0);
    }


    /* for(int i=0;i<tempStore.size();i++){
         cout<<tempStore[i];
     }
     cout<<endl;*/
    int skipFlag=0;
    int oneCount=0;

    for(int i=0; i<cnt; i++)
    {

        int position=powerOfTwo[i];
        int j=position-1;
        oneCount=0;
        skipFlag=0;
        //cout<<tempStore.size()<<endl;
        while(j<v.size())
        {
            int decrement=position;

            if(skipFlag==0)
            {
                while(decrement && j<v.size())
                {
                    if((position-1)==j)
                    {
                        /*if(i==2 && r==1)
                            cout<<j<<" ";*/
                        decrement--;
                        j++;
                    }
                    else if(v[j]==1)
                    {
                        /*if(i==2 && r==1)
                            cout<<j<<" ";*/
                        decrement--;
                        oneCount++;
                        j++;
                    }
                    else
                    {
                        /*if(i==2 && r==1)
                            cout<<j<<" ";*/
                        decrement--;
                        j++;
                    }
                    //cout<<"OK"<<endl;
                }
                skipFlag=1;
            }
            else
            {
                while(decrement && j<v.size())
                {
                    if((position-1)==j)
                    {
                        decrement--;
                        j++;
                    }
                    else
                    {
                        decrement--;
                        j++;
                    }
                    //cout<<"OK2"<<endl;
                }
                skipFlag=0;
            }



        }
        if(oneCount%2==0)
        {
            v[position-1]=0;
            //cout<<tempStore[position-1];
        }
        else
        {
            v[position-1]=1;
            //cout<<tempStore[position-1];
        }
    }
    columnSize=v.size();
    for(int i=0; i<v.size(); i++)
    {
        storeBit[r][i]=v[i];
    }

}

int hammingCodeCorrection(vector<int>v)
{

    int len=v.size();
    int cnt=0;

    int c[numberofCount];
    int powerPos;

    for(int i=0; i<numberofCount; i++)
    {
        c[i]=0;
    }

    int skipFlag=0;
    int oneCount=0;
    int sum=0;
    for(int i=0; i<numberofCount; i++)
    {

        int position=powerOfTwo[i];
        int j=position-1;
        oneCount=0;
        skipFlag=0;

        while(j<v.size())
        {
            int decrement=position;

            if(skipFlag==0)
            {
                while(decrement)
                {
                    if(v[j]==1)
                    {
                        decrement--;
                        oneCount++;
                        j++;
                    }
                    else
                    {
                        decrement--;
                        j++;
                    }
                    //cout<<"OK"<<endl;
                }
                skipFlag=1;
            }
            else
            {
                while(decrement)
                {
                    if((position-1)==j)
                    {
                        decrement--;
                        j++;
                    }
                    else
                    {
                        decrement--;
                        j++;
                    }
                    //cout<<"OK2"<<endl;
                }
                skipFlag=0;
            }



        }
        if(oneCount%2==0)
        {
            sum+=powerOfTwo[i]*0;
        }
        else
        {
            sum+=powerOfTwo[i]*1;
        }

    }
    return sum;
}

string makeChecksum(string inputString,string keyString)
{
    int i,j,keylen,msglen;
    char input[1000], key[300],temp[300],quot[1000],rem[300],key1[300];

    strcpy(input,inputString.c_str());
    strcpy(key,keyString.c_str());
    keylen=strlen(key);
    msglen=strlen(input);
    strcpy(key1,key);

    for (i=0; i<keylen-1; i++)
    {
        input[msglen+i]='0';
    }

    for (i=0; i<keylen; i++)
        temp[i]=input[i];

    for (i=0; i<msglen; i++)
    {
        quot[i]=temp[0];
        if(quot[i]=='0')
            for (j=0; j<keylen; j++)
                key[j]='0';
        else
            for (j=0; j<keylen; j++)
                key[j]=key1[j];
        for (j=keylen-1; j>0; j--)
        {
            if(temp[j]==key[j])
                rem[j-1]='0';
            else
                rem[j-1]='1';
        }
        rem[keylen-1]=input[i+keylen];
        strcpy(temp,rem);
    }
    strcpy(rem,temp);
    //printf("\nRemainder is ");
    string send="";
    for (i=0; i<keylen-1; i++)
        send=send+rem[i];
    //printf("\n");*/

    return send;
}

bool checkChecksum(string inputString,string keyString)
{
    int i,j,keylen,msglen;
    char input[1000], key[300],temp[300],quot[1000],rem[300],key1[300];

    strcpy(input,inputString.c_str());
    strcpy(key,keyString.c_str());
    keylen=strlen(key);
    msglen=strlen(input);
    strcpy(key1,key);
    msglen=msglen-keylen+1;
    /* for (i=0; i<keylen-1; i++)
     {
         input[msglen+i]='0';
     }*/

    for (i=0; i<keylen; i++)
        temp[i]=input[i];

    for (i=0; i<msglen; i++)
    {
        quot[i]=temp[0];
        if(quot[i]=='0')
            for (j=0; j<keylen; j++)
                key[j]='0';
        else
            for (j=0; j<keylen; j++)
                key[j]=key1[j];
        for (j=keylen-1; j>0; j--)
        {
            if(temp[j]==key[j])
                rem[j-1]='0';
            else
                rem[j-1]='1';
        }
        rem[keylen-1]=input[i+keylen];
        strcpy(temp,rem);
    }
    strcpy(rem,temp);
    //printf("\nRemainder is ");

    /*for (i=0; i<keylen-1; i++){
        cout<<rem[i];
    }*/
    cout<<endl;
    bool yesSame=true;
    for (i=0; i<keylen-1; i++)
    {
        if(rem[i]!='0')
        {
            yesSame=false;
            break;
        }
    }
    //send=send+rem[i];
    //printf("\n");*/

    return yesSame;
}

void addCheckBits(int m)
{

    for(int i=0; i<row; i++)
    {
        for(int j=0; j<m*8; j++)
        {
            //printf("%d",matrix[i][j]);

            tempStore.push_back(matrix[i][j]);

        }
        makeParity(tempStore,i);
        tempStore.clear();
        //printf("\n");
    }
}

bool canToggle(double p)
{
    double r;
    r=((double) rand() / (RAND_MAX))+1;
   //r=(r)*p*r/(((double) rand() / (RAND_MAX))+1);
    if(p>(r-1))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{

    srand(9999);
    string dataString;
    int m;
    double p;
    string polynomial;

    cout<<"Enter data string : ";
    getline(cin,dataString);
    cout<<"Enter number of data bytes in a row <m> : ";
    cin>>m;
    cout<<"Enter probability <p> : ";
    cin>>p;

    cout<<"Enter generator polynomial : ";
    cin>>polynomial;

    cout<<endl;

    if(dataString.size()%m!=0)
    {
        while(dataString.size()%m!=0)
        {
            dataString+="~";
        }
    }

    cout<<"Data string after padding : "<<dataString<<endl<<endl;
    cout<<"Data block (ASCII code of m characters per row) : "<<endl;

    strToBinary(dataString,m);

    for(int i=0; i<row; i++)
    {
        for(int j=0; j<m*8; j++)
        {
            printf("%d",matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    addCheckBits(m);
    cout<<"Data blocks after adding checkbits: "<<endl;
    for(int i=0; i<row; i++)
    {
        int colorPos=0;
        for(int j=0; j<columnSize; j++)
        {
            if(powerOfTwo[colorPos]-1==j)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                printf("%d",storeBit[i][j]);
                colorPos++;
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
                printf("%d",storeBit[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");

    string tempBin="";
    cout<<"Data bits after column-wise serialization : "<<endl;
    for(int i=0; i<columnSize; i++)
    {
        for(int j=0; j<row; j++)
        {
            printf("%d",storeBit[j][i]);
            ostringstream str1;
            str1<<storeBit[j][i];
            tempBin=tempBin+str1.str();
        }
        //printf("\n");
    }
    printf("\n");
    printf("\n");
    //cout<<tempBin<<endl;

    string checksum;
    checksum=makeChecksum(tempBin,polynomial);
    int lenChecksum=checksum.size();
    //cout<<checksum<<endl;
    tempBin=tempBin+checksum;
    //cout<<tempBin<<endl;
    int colorPos=tempBin.size()-lenChecksum;
    cout<<"Data bits after appending CRC checksum (sent frame): "<<endl;
    for(int i=0; i<tempBin.size(); i++)
    {
        if(i>=colorPos)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
            cout<<tempBin[i];
        }
        else
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            cout<<tempBin[i];
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    printf("\n");
    printf("\n");

    //step 6
    vector<int>changeMade;
    if(p==0 || p==0.0)
    {
        cout<<"Received frame : "<<endl;
        for(int i=0; i<tempBin.size(); i++)
        {
            cout<<tempBin[i];
        }
        cout<<endl;
        cout<<endl;
        cout<<"Result of CRC checksum matching : ";
        if(checkChecksum(tempBin,polynomial))
        {
            cout<<"No error detected"<<endl<<endl;
        }
        else
        {
            cout<<"Error detected"<<endl<<endl;
        }

    }
    else
    {
        cout<<"Received frame : "<<endl;
        for(int i=0; i<tempBin.size(); i++)
        {
            if(canToggle(p))
            {
                if(tempBin[i]=='0')
                {
                    tempBin[i]='1';
                }
                else
                {
                    tempBin[i]='0';
                }
                changeMade.push_back(i);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                cout<<tempBin[i];
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            }
            else
            {
                cout<<tempBin[i];
            }
        }
        cout<<endl;
        cout<<endl;
        cout<<"Result of CRC checksum matching : ";
        if(checkChecksum(tempBin,polynomial))
        {
            cout<<"No error detected"<<endl<<endl;
        }
        else
        {
            cout<<"Error detected"<<endl<<endl;
        }
    }

    //step 8
    int r=0,c=0;
    int vectorpos=0;
    int showError[100][100];

    for(int i=0; i<100; i++)
    {
        for(int j=0; j<100; j++)
        {
            showError[i][j]=0;
        }
    }
    //cout<<"OK"<<endl;
    for(int i=0; i<(tempBin.size()-lenChecksum); i++)
    {
        //cout<<"OK1"<<endl;
        if(!(p==0 || p==0.0))
        {
            if(changeMade[vectorpos]==i)
            {
                showError[r][c]=1;
                vectorpos++;
            }
        }
        //cout<<"OK"<<endl;
        storeBit[r][c]=(int)tempBin[i]-48;
        r++;
        if(r==row)
        {
            r=0;
            c++;

        }

    }

    cout<<"Data block after removing CRC checksum bits :"<<endl;
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<columnSize; j++)
        {
            if(showError[i][j]==1)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                cout<<storeBit[i][j];
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            }
            else
            {
                cout<<storeBit[i][j];
            }
        }
        cout<<endl;
    }
    cout<<endl;

    //step 9

    vector<int>vec;
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<columnSize; j++)
        {
            vec.push_back(storeBit[i][j]);

        }
        int makePos=hammingCodeCorrection(vec);
        //cout<<makePos<<endl;
        if(makePos<=columnSize && makePos>0)
        {
            if(storeBit[i][makePos-1]==0)
            {
                storeBit[i][makePos-1]=1;
            }
            else
            {
                storeBit[i][makePos-1]=0;
            }
        }
        vec.clear();
        //cout<<hammingCodeCorrection(vec)<<endl;
    }

    int pPos=0;
    int col=0;
    for(int i=0; i<row; i++)
    {
        col=0;
        pPos=0;
        for(int j=0; j<columnSize; j++)
        {
            if((j)==(powerOfTwo[pPos]-1))
            {
                pPos++;
                continue;
            }
            else
            {
                matrix[i][col]=storeBit[i][j];
                col++;
            }
        }
    }

    cout<<"Data blocks after removing check bits : "<<endl;
    for(int i=0; i<row; i++)
    {
        for(int j=0; j<m*8; j++)
        {
            cout<<matrix[i][j];
        }
        cout<<endl;
    }
    cout<<endl;

    //step 10
    cout<<"Output frame : ";
    string output="";
    int sm=0;
    int smCount=7;
    for(int i=0; i<row; i++)
    {
        int ct=0;
        sm=0;
        smCount=7;
        for(int j=0; j<m*8; j++)
        {
            sm+=matrix[i][j]*powerOfTwo[smCount];
            smCount--;
            if(smCount==-1){
                smCount=7;
                output=output+(char)sm;
                sm=0;
            }

        }
        //cout<<endl;
    }
    cout<<output<<endl;

}
