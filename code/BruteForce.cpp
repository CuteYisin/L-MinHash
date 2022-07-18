#include<bits/stdc++.h>
using namespace std;
const double eps=1e-8;

string str[100005];
int dis[5005][5005];
int ans=0;

void readTxt(string file)
{
    ifstream fin;
    fin.open(file.data());
    assert(fin.is_open());
    string s1,s2;int i=0;
    while(getline(fin,s1))
    {
        getline(fin,s2);
        str[ans++]=s2;
    }
}

double getsim(string_view a,string_view b)
{
    int l1=a.size(),l2=b.size();
    for(int i=0;i<l1;i++) dis[i][0]=i;
    for(int i=0;i<l2;i++) dis[0][i]=i;
    for(int i=1;i<=l1;i++)
    {
        for(int j=1;j<=l2;j++)
        {
            if(a[i]==b[j]) dis[i][j]=dis[i-1][j-1];
            else dis[i][j]=1+min(min(dis[i-1][j],dis[i][j-1]),dis[i-1][j-1]);
        }
    }
    return 1-dis[l1][l2]*1.0/max(l1,l2);
}

int main()
{
    clock_t start, finish;
    start = clock();
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    readTxt("testdata.txt");
    freopen("oo.txt","w",stdout);

    int total=0;
    for(int j=0;j<ans;j++)
    {
        for(int k=j+1;k<ans;k++)
        {
            double ss=getsim(str[j],str[k]);
            if(ss-0.5>=eps)
            {
                cout<<str[j]<<"\n"<<str[k]<<"\n"<<ss<<"\n";
                total++;
            }
        }
    }
    cout<<total<<"\n";

    finish = clock();
    cout <<"the time cost is:" << double(finish - start) / CLOCKS_PER_SEC<<"\n";
    return 0;
}
