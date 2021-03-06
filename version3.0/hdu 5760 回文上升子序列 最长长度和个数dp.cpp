题意: 给出n个数，要找到一个合法的最长子序列s，输出其长度，并且输出不同的s的个数。s序列必须是回文的，并且中间最小，往两边依次不减。
s1与s2不同当且仅当长度不同或者存在某位s1[i]!=s2[i] 
以dp[l][r]表示从l开始以r结束的最长合法子序列的长度及数量（first表示长度，second表示数量），以nxt[i][j]表示序列中i之后第一个等于j的数的位置，
以pre[i][j]表示序列中i之前最后一个等于j的数的位置（求这两个数组需要对a序列离散化），那么显然有一个朴素的O(n^3)的转移： 
dp[l][r]=max(dp[nxt[l][c]][pre[r][c]]+2)（first+2,second不变） 
考虑到时间限制，需要优化上述转移，若固定l，那么随着r的右移，dp[l][r]一定是越来越优，用ans临时记录最优解，当r右移时就可以用ans更新dp数组，
如果a[i]=a[j]则令dp[i][j].first=ans.first+2,dp[i][j].second=ans.second，如果a[i]>=a[j]，则令ii=nxt[i][a[j]]表示找到i右边一个与a[j]值
相等的位置，如果dp[ii][j].first>ans.first则更新ans，如果两者相同说明和ans相同长度的序列又出现了，但是不能直接将dp[ii][j].second加到ans.second
上去，因为还有重复的问题，令jj=pre[j][a[j]]，若dp[ii][jj].first==dp[ii][j].first说明ii~jj的所有最长序列必然都会出现在ii~j中，而ii~jj中的所有
最长序列已经累加到ans.second中过，故要将这部分减掉之后再在ans.second上加上dp[ii][j].second，最后枚举最长子序列两端的值即可得到最优解
#include<cstdio>
#include<iostream>
#include<cstring>
#include<algorithm>
using namespace std;
#define mod 1000000007
#define maxn 5555
int n,a[maxn],h[maxn],pre[maxn][maxn],nxt[maxn][maxn];
typedef pair<int,int>P;
P dp[maxn][maxn];
int main(){
    while(~scanf("%d",&n)) {
        for(int i=1;i<=n;i++)scanf("%d",&a[i]),h[i]=a[i];
        sort(h+1,h+n+1);
        int cnt=unique(h+1,h+n+1)-(h+1);
        for(int i=1;i<=n;i++)a[i]=lower_bound(h+1,h+cnt+1,a[i])-h;
        memset(pre,-1,sizeof(pre)); memset(nxt,-1,sizeof(nxt));
        for(int i=0;i<=n+1;i++) {
            for(int j=i+1;j<=n;j++)
                if(nxt[i][a[j]]==-1)nxt[i][a[j]]=j;
            for(int j=i-1;j>=1;j--)
                if(pre[i][a[j]]==-1)pre[i][a[j]]=j;
        }
        for(int i=n;i>=1;i--) {
            dp[i][i]=P(1,1);
            P ans=P(0,1);
            for(int j=i+1;j<=n;j++) {
                dp[i][j]=P(0,0);
                if(a[i]==a[j])dp[i][j]=P(ans.first+2,ans.second);
                if(a[i]>=a[j]) {
                    int ii=nxt[i][a[j]];
                    if(ii==-1)continue;
                    if(dp[ii][j].first>ans.first)ans=dp[ii][j];
                    else if(dp[ii][j].first==ans.first) {
                        int jj=pre[j][a[j]];
                        if(jj!=-1&&dp[ii][jj].first==dp[ii][j].first)
                            ans.second-=dp[ii][jj].second;
                        if(ans.second<0)ans.second+=mod;
                        ans.second=(ans.second+dp[ii][j].second)%mod;
                    }
                }
            }
        }
        P ans=P(0,0); 
        for(int i=1;i<=n;i++) {
            int l=nxt[0][i],r=pre[n+1][i];
            if(l==-1||r==-1)continue;
            if(dp[l][r].first>ans.first)ans=dp[l][r];
            else if(dp[l][r].first==ans.first)
                ans.second=(ans.second+dp[l][r].second)%mod;
        }
        printf("%d %d\n",ans.first,ans.second);
    }
    return 0;
}
