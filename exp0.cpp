//��o_custkey����ģ100����������Ϊ0���ԣ��޷��ҵ� 
//��C/O/L���ű���еѿ��������ӣ����չ��������������ʽ��������� 
//c_mktsegment = s1, c_orderdate < s2, l_shipdate > s3 ,s1,s2,s3Ϊ����Ŀɱ���� 
//��������������Ľ��������l_orderkey�е�ֵ���з��飬����l_orderkey��ͬ�ļ�¼��l_extendedprice����ͣ�
//���������l_extendedprice���ֵsum���򣬲�����ǰn��topn�Ľ��������topnֵҲ������ı�Ρ�
//˼·������Ȼ�����������õ���COL��custkey��orderkey��mkgsegment��orderdate��shipdate��extendedprice����Ȼ��ɸѡ����ͣ��������l_orderkey|o_orderdate|revenue
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
const int MOD=100; 

typedef struct cust
{
	int c_custkey;//�˿ͱ�� 
	char c_mkgsegment[20];//�г����� 
}customer;//�˿� 100
typedef struct ord
{
	int o_orderkey; //������ 
	int o_custkey;//�˿ͺ� 
	char c_orderdate[10]; //�µ�ʱ�� ��ʽdate��2019-08-28 
}orders;//���� 4000
typedef struct item
{
	int l_orderkey;//������ 
	double l_extendedprice;//�۸� 
	char l_shipdate[10];//����ʱ�� 
}lineitem;//��Ʒ��Ŀ 1000
typedef struct
{
	int l_orderkey;
	char o_orderdate[10];
	double l_extendedprice;//sum(l_extendedprice)
}fresult;//���
 
customer * readfile_customer()
{
	FILE *fp;
	customer *p = NULL;
	p = (customer *)malloc(100 * sizeof(customer));
	char ch;
	int i=0;
	fp = fopen("customer.txt", "r");
	if (fp== NULL)
	{
		printf("ERROR\n");
		return NULL;
	}
	//printf("GET Customer\n");
	while (!feof(fp))
	{
		fscanf(fp, "%d%c%s", &p[i].c_custkey, &ch, p[i].c_mkgsegment);
	//	printf("%d%c%s\n", p[i].c_custkey, ch, p[i].c_mkgsegment);
		i++;
	}
	fclose(fp);
	return p;
}

orders * readfile_order() 
{
	FILE *fp=NULL; 
	orders * a=NULL;
	a = (orders * )malloc(4000*sizeof(orders));
	int i=0;
	char b,c;
	long long d;
	fp = fopen("orders.txt","r");
	if(fp == NULL)
	{
		printf("ERROR\n");
		return NULL;
	}
	while(!feof(fp))
	{	
		fscanf(fp,"%d%c%lld%c%s",&a[i].o_orderkey,&b,&d,&c,a[i].c_orderdate);
		a[i].o_custkey=d%MOD;
		i++;
	}
	fclose(fp);
	return a;
}
lineitem * readfile_lineitem()
{
	FILE * fp;
	lineitem * l=NULL;
	l = (lineitem *)malloc(1000*sizeof(lineitem));
	int i=0;
	char b,c;
	fp = fopen("lineitem.txt","r");
	if(fp==NULL)
	{
		printf("ERROR\n");
		return NULL;
	}
	while(!feof(fp))
	{
		fscanf(fp,"%d%c%lf%c%s",&l[i].l_orderkey,&c,&l[i].l_extendedprice,&b,l[i].l_shipdate);
		i++;
	}
	fclose(fp);
	return l; 
}
 //������������C�����ҵ��г�����ƥ�����Ŀ���õ��˿ͱ�ţ�����O���ҵ��˿ͺ�ƥ�����µ�ʱ��������������Ŀ�� �õ������ţ�
 //�پ���L�з���ʱ�������Ƚϣ��õ����Ķ����š��۸񣬶�������ͬ�ģ��Լ۸����revenue���������� l_orderkey|o_orderdate|revenue
fresult * operate(customer * cus,orders * ord,lineitem * item,char *c_mkgsegment, char * order_date,char * ship_date)
{
	int i,j,k,l=0,m=0;
	fresult * result1=NULL;
	fresult * result2=NULL;
	fresult temp;
	result1 = (fresult *)malloc(1000*sizeof(fresult));
	result2 = (fresult *)malloc(1000*sizeof(fresult));
	for(i=0;i<100;i++)
	{
		if(strcmp(cus[i].c_mkgsegment,c_mkgsegment)==0)
		{
			for(j=0;j<4000;j++)
			{
				for(k=0;k<1000;k++)
				if(cus[i].c_custkey==ord[j].o_custkey&&ord[j].o_orderkey==item[k].l_orderkey&&(strcmp(ord[j].c_orderdate,order_date)<0)&&(strcmp(item[k].l_shipdate,ship_date)>0))
				{
				//	printf("%d,%s,%lf\n",item[k].l_orderkey,ord[j].c_orderdate,item[k].l_extendedprice);
					result1[l].l_orderkey=item[k].l_orderkey;
					strcpy(result1[l].o_orderdate,ord[j].c_orderdate);
					result1[l].l_extendedprice=item[k].l_extendedprice;
					l++;
				}
			}
		}
	}
//	printf("SUM:\n");
	result2[0].l_orderkey = result1[0].l_orderkey;
	strcpy(result2[0].o_orderdate,result1[0].o_orderdate);
	result2[0].l_extendedprice = result1[0].l_extendedprice;
	for(i=1;i<l;i++)
	{
		if(result2[m].l_orderkey==result1[i].l_orderkey)
		{
			result2[m].l_extendedprice += result1[i].l_extendedprice;
		}
		else
		{
			m++;
			result2[m].l_orderkey = result1[i].l_orderkey;
			strcpy(result2[m].o_orderdate,result1[i].o_orderdate);
			result2[m].l_extendedprice = result1[i].l_extendedprice;
		}
	}
	for(i=0;i<m-1;i++)//ð������ 
	{
		for(j=0;j<m-i;j++)
		{
			if(result2[j].l_extendedprice<result2[j+1].l_extendedprice)
			{
			//	printf("SORT:\n");
				temp.l_extendedprice=result2[j].l_extendedprice;
				temp.l_orderkey=result2[j].l_orderkey;
				strcpy(temp.o_orderdate,result2[j].o_orderdate);
				result2[j].l_extendedprice=result2[j+1].l_extendedprice;
				result2[j].l_orderkey=result2[j+1].l_orderkey;
				strcpy(result2[j].o_orderdate,result2[j+1].o_orderdate);
				result2[j+1].l_extendedprice=temp.l_extendedprice;
				result2[j+1].l_orderkey=temp.l_orderkey;
				strcpy(result2[j+1].o_orderdate,temp.o_orderdate);
			}
		}
	}
	return result2;
}

int output(char *a,char *b,char *c,int limit)
{
	customer *cus=NULL;
	orders *ord=NULL;
	lineitem *item=NULL;
	cus=readfile_customer();
	ord=readfile_order();
	item=readfile_lineitem();
	fresult *res=NULL;
	res=operate(cus,ord,item,a,b,c);
	printf("l_orderkey|o_orderdate|revenue\n");
	int cnt=0;
	for(int i=0;i<limit;i++)
	{
		if(res[i].l_orderkey>0)
		{
			printf("%-10d|%-11s|%-20.2lf\n",res[i].l_orderkey,res[i].o_orderdate,res[i].l_extendedprice);
			cnt++;
		}
	}
	if(cnt==0)
		printf("No Records!\n");
	return 0;
 } 
int main(int argc,char **argv)
{
	int i,func;
	unsigned int n=atoi(argv[4]);
	for(i=1;i<=n;i++)
	{
		unsigned int limit=atoi(argv[4*i+4]);
		func=output(argv[4*i+1],argv[4*i+2],argv[4*i+3],limit);
//	customer *test=NULL;
//	test=readfile_customer();
	}
	return 0;
}
