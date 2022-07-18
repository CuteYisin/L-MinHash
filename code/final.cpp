#include<bits/stdc++.h>
#define ll long long
#pragma GCC diagnostic error "-std=c++11"
#pragma GCC optimize("-fdelete-null-pointer-checks,inline-functions-called-once,-funsafe-loop-optimizations,-fexpensive-optimizations,-foptimize-sibling-calls,-ftree-switch-conversion,-finline-small-functions,inline-small-functions,-frerun-cse-after-loop,-fhoist-adjacent-loads,-findirect-inlining,-freorder-functions,no-stack-protector,-fpartial-inlining,-fsched-interblock,-fcse-follow-jumps,-fcse-skip-blocks,-falign-functions,-fstrict-overflow,-fstrict-aliasing,-fschedule-insns2,-ftree-tail-merge,inline-functions,-fschedule-insns,-freorder-blocks,-fwhole-program,-funroll-loops,-fthread-jumps,-fcrossjumping,-fcaller-saves,-fdevirtualize,-falign-labels,-falign-loops,-falign-jumps,unroll-loops,-fsched-spec,-ffast-math,Ofast,inline,-fgcse,-fgcse-lm,-fipa-sra,-ftree-pre,-ftree-vrp,-fpeephole2",3)
#pragma GCC target("avx","sse2")
using namespace std;
const double eps=1e-8;
const int MAXN=1e6+10;
const int k=4;            //k-mer
const int l=2;            //最小l个hash值
const int L=500;          //哈希函数个数
const int p=1610612741;   //散列的大素数模数
const int pp=500;         //全域散列最后的模数

int n;

string_view s[MAXN];
vector< pair<string_view,int> >k_mer[MAXN];
string s2[MAXN];
unordered_map<string_view,int> mp,mpp;
unordered_map<int,int> f;
unordered_map<string_view,int>hash_table,emptyh;
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

void lsh()
{
    srand((unsigned)time(NULL));
    for(int i=0;i<L;i++)                 //生成不重复的L组hash函数
    {
        int x=randnum(1,p-1);
        while(f[x]) x=randnum(1,p-1);
        f[x]++;
        hasha[i]=x;
    }
    int num=1;
    for(int ii=0;ii<L;ii++)
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
                    h=(h*hasha[ii]+(x[jj]-'A'))%p;
                }
                hashval[j].val=h%pp;
                hashval[j].no=j;
            }
            sort(hashval,hashval+m,cmp);
            string ss;
            for(int j=0;j<l;j++)
                ss+=k_mer[i][hashval[j].no].first;
            std::string_view sss={ss};
            if(hash_table[sss]==0) hash_table[sss]=num++;
            hashset[hash_table[sss]].push_back(i);
        }
        for(int j=1;j<num;j++)
        {
            int ans=hashset[j].size();
            for(int jj=0;jj<ans;jj++)
                for(int jjj=jj+1;jjj<ans;jjj++)
                    preset.insert(make_pair(hashset[j][jj],hashset[j][jjj]));
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
    freopen("out.txt","w",stdout);

    find_kmer();
    lsh();

    check();

    finish = clock();
    cout <<"the time cost is:" << double(finish - start) / CLOCKS_PER_SEC<<"\n";
    return 0;
}
