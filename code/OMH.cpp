#include<bits/stdc++.h>
#define ll long long
using namespace std;
const double eps=1e-8;
const int MAXN=1e6+10;
const int k=3;            //k-mer
const int l=3;            //最小l个hash值
const int L=300;          //哈希函数个数
const int p=19260817;     //散列的大素数模数
const int pp=100;         //全域散列最后的模数

int n;

string_view s[MAXN];
vector< pair<string_view,int> >k_mer[MAXN];
string s2[MAXN];
unordered_map<string_view,int> mp,mpp;
map< vector<string_view>, int >hash_table,emptyh;
vector<int>hashset[MAXN],emptyhs;
set< pair<int,int> >preset;
int hasha[1005];
int dis[5005][5005];

struct hh
{
    int no,val;
}hashval[100005];

bool cmp(hh a,hh b)
{
    return a.val<b.val;
}

bool cmp2(hh a,hh b)
{
    return a.no<b.no;
}

void readTxt(string file)
{
    ifstream fin;
    fin.open(file.data());
    assert(fin.is_open());
    while(getline(fin,s2[n]))
    {
        std::string_view str {s2[n]};
        s[n++]=str;
    }
}

void find_kmer()
{
    for(int i=0;i<n;i++)
    {
        int m=s[i].size();
        mp=mpp;
        for(int j=0;j<m-k+1;j++)
        {
            std::string_view str=s[i].substr(j,k);
            int ans=mp[str];
            mp[str]++;
            k_mer[i].push_back(make_pair(str,ans));
        }
    }
}

int randnum(int a,int b)
{
    return (rand()%(b-a+1))+a;
}

void omh()
{
    srand((unsigned)time(NULL));
    for(int i=0;i<L;i++) hasha[i]=randnum(1,p-1);        //生成hash函数族
    //for(int i=0;i<L;i++) cout<<hasha[i]<<endl;
    int num=1;
    for(int ii=0;ii<L;ii++)                              //L组hash函数
    {
        hash_table=emptyh;
        for(int i=0;i<=num;i++) hashset[i]=emptyhs;
        num=1;
        for(int i=0;i<n;i++)
        {
            int m=k_mer[i].size();
            for(int j=0;j<m;j++)
            {
                string_view x=k_mer[i][j].first;
                ll h=1;
                for(int jj=0;jj<k;jj++)
                {
                    h=(h*hasha[ii]+x[jj])%p;
                }
                hashval[j].val=h%pp;
                hashval[j].no=j;
                //cout<<x<<" "<<hashval[j].val<<endl;
            }

            //找出最小l个hash值对应的k-mer
            sort(hashval,hashval+m,cmp);
            sort(hashval,hashval+l,cmp2);
            vector<string_view>v;
            for(int j=0;j<l;j++)
            {
                v.push_back(k_mer[i][hashval[j].no].first);
                //cout<<k_mer[i][hashval[j].no].first<<endl;
            }
            if(hash_table[v]==0) hash_table[v]=num++;
            hashset[hash_table[v]].push_back(i);
        }
        for(int j=1;j<num;j++)
        {
            int ans=hashset[j].size();
            for(int jj=0;jj<ans;jj++)
            {
                for(int jjj=jj+1;jjj<ans;jjj++)
                {
                    preset.insert(make_pair(hashset[j][jj],hashset[j][jjj]));
                    //cout<<s[hashset[j][jj]]<<" "<<s[hashset[j][jjj]]<<endl;
                }
            }
        }
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

void check()
{
    cout<<preset.size()<<"\n";
    int total=0;
    set< pair<int,int> >::iterator it=preset.begin();
    while(it!=preset.end())
    {
        int x=(*it).first,y=(*it).second;
        double result=getsim(s[x],s[y]);
        if(result-0.5>=eps)
        {
            cout<<s[x]<<"\n"<<s[y]<<"\n"<<result<<"\n";
            total++;
        }
        it++;
    }
    cout<<total<<"\n";
}

int main()
{
    clock_t start, finish;
    start = clock();
    ios::sync_with_stdio(false),cin.tie(0),cout.tie(0);
    readTxt("testdata.txt");
    freopen("o.txt","w",stdout);

    find_kmer();
    omh();

    check();

    finish = clock();
    cout <<"the time cost is:" << double(finish - start) / CLOCKS_PER_SEC<<"\n";
    return 0;
}
