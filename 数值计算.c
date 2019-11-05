#include<stdio.h>
int main()
{
	int num[8];
	int N,max=0,sum=0;
	printf("输入数组的大小\n");
	scanf("%d",&N);
	printf("输入数组的值\n");
	for(int i=0;i<N;i++)
	scanf("%d",&num[i]);
	for(int i=0;i<N;i++)
	{
	sum=0;
	for(int j=i;j<N;j++)
	{
		sum+=num[j];
		if(sum>max)
		max=sum;	
	}
	}
	printf("子数组数值最大为：%d\n",max);
}
