#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include<ctype.h>
/*忽略错误*/
#pragma warning(disable : 4996)
/*定义影厅最大容量*/
#define Nor_MAX 300 
#define ThreeD_MAX 200 
#define VIP_MAX 100
/*默认一排的人数*/
#define Nor_Default 25
#define TD_Default 20
#define VIP_Default 10
int n = 0; //用于计算文件中数据的数目
int Movie_N = 0;//用于计算文件中电影的数目 
int User_N = 0; //管理员账户数目
/*用于判断程序执行是否正确*/ 
typedef int Status;
#define OK 1
#define ERROR 0
/*预处理结束*/ 
/*加载登录信息*/
struct User
{
	char account[20];
	char password[20];
	struct User* next;
};
struct Movie
{
	int day;                      //放映日期 
	int month;
	char place[4];               //放映地点 
	char session[15];           //放映场次 
	char name[10];             //电影名 
	int price;                //电影价格 
	int num;                 // 订购人数
	int seat[30][30];		//座位情况
	float Seat_Percent;     //上座率情况 
};
struct info
{
	struct Movie Movieinfo;
	int id;                        //唯一放映编号 
	struct info* next;
};
/*用于统计电影数目*/
struct Movie_info
{
	int Movie_Num;
	char Movie_Name[10];
	struct Movie_info* next;
};
void SetColor(unsigned short ForeColor , unsigned short BackGroundColor )
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取当前窗口句柄
	SetConsoleTextAttribute(handle, ForeColor + BackGroundColor * 0x10);//设置颜色
	/*颜色代码：
	0 = 黑色       8 = 灰色
	1 = 蓝色       9 = 淡蓝色
	2 = 绿色       10 = 淡绿色
	3 = 浅绿色     11 = 淡浅绿色
	4 = 红色       12 = 淡红色
	5 = 紫色       13 = 淡紫色
	6 = 黄色       14 = 淡黄色
	7 = 白色       15 = 亮白色 */
}
/*从文件中读入数据并创建链表 */
struct User* LoadUser(void)
{
	FILE* fp;
	if ((fp = fopen("User_info.txt", "r")) == NULL)
	{
		printf("Can't open the file!\n"); exit(0);
	}
	else
	{
		struct User* p1, * p2;
		struct User* head;
		p1 = p2 = (struct User*)malloc(sizeof(struct User));
		head = p1;
		fscanf(fp, "%s    %s", p1->account, p1->password);
		while (!feof(fp))	//fp并不指向文件末尾时 
		{
			User_N++;
			if (User_N == 1)
				head = p1;
			else
				p2->next = p1;
			p2 = p1;
			p1 = (struct User*)malloc(sizeof(struct User));
			fscanf(fp, "%s    %s", p1->account, p1->password);
		}
		p2->next = NULL;
		fclose(fp);
		return head;
	}
}
struct info* LoadData(void)
{
	FILE* fp;
	if ((fp = fopen("Movie_info.txt", "r")) == NULL)  //打开Movie_info.txt
	{
		printf("Can't open the file!\n"); exit(0);
	}
	//判断文件是否为空 
	int ch;
	if ((ch = fgetc(fp)) == EOF)
	{
		printf("File is empty!\n"); exit(0);
	}
	else
	{
		rewind(fp); //重置文件指针 
		struct info* p1, * p2;
		struct info* head;
		p1 = p2 = (struct info*)malloc(sizeof(struct info));
		head = p1;
		fscanf(fp, "%d.%d %s %s %s %d"
			, &p1->Movieinfo.month, &p1->Movieinfo.day, p1->Movieinfo.place, p1->Movieinfo.session, p1->Movieinfo.name, &p1->Movieinfo.price);
		p1->id = n + 1; p1->Movieinfo.num = 0; p1->Movieinfo.Seat_Percent = 0;//放映编号、已售人数、上座率初始化 
		while (!feof(fp))	//fp并不指向文件末尾时 
		{
			n++;
			if (n == 1)
				head = p1;
			else
				p2->next = p1;
			p2 = p1;
			p1 = (struct info*)malloc(sizeof(struct info));
			fscanf(fp, "%d.%d %s %s %s %d"
				, &p1->Movieinfo.month, &p1->Movieinfo.day, p1->Movieinfo.place, p1->Movieinfo.session, p1->Movieinfo.name, &p1->Movieinfo.price);
			p1->Movieinfo.num = 0; p1->id = n + 1; p1->Movieinfo.Seat_Percent = 0;//放映编号、已售人数初始化 
		}
		p2->next = NULL;
		fclose(fp);
		return head;
	}
}
/*检查数据库中的文件是否正确*/ 
Status Check_Input( struct info *head )
{
	struct info *p1,*p2;/*p2用于记录当前判断的数据*/ 
	if(!head)	return ERROR;
	p1 = head;p2 = p1->next;
	int i,cnt = 1;//cnt用于记录没有问题的数据个数 
	while(p2)
	{
		for( i = 0; i < cnt; i++ )
		{
			if(p2->Movieinfo.day == p1->Movieinfo.day && p2->Movieinfo.month == p1->Movieinfo.month &&
			 !(strcmp(p1->Movieinfo.place,p2->Movieinfo.place)) && !(strcmp(p1->Movieinfo.session,p2->Movieinfo.session)))
			 	return ERROR;
			else
				p1 = p1->next;
		}
		p2 = p2->next;cnt++;p1 = head;
	}
	return OK;
}
/*开始统计电影数目*/
struct Movie_info* LoadMovie(struct info* head)
{
	struct Movie_info* p1, * p2, * p0;
	struct Movie_info* Movie_head;
	struct info* p = head;
	int flag = 0;
	Movie_head = NULL;
	/*录入第一个电影数据*/
	Movie_head = p1 = p2 = p0 = (struct Movie_info*)malloc(sizeof(struct Movie_info));
	p2->next = NULL; p1->Movie_Num = 0; Movie_N++;
	strcpy(p1->Movie_Name, p->Movieinfo.name); p1->Movie_Num++;
	p = p->next;
	/*逐个查找*/
	while (p)
	{
		p0 = Movie_head;
		while (p0)
		{
			if (!(strcmp(p0->Movie_Name, p->Movieinfo.name))) { p0->Movie_Num++; flag = 1; break; }
			p0 = p0->next;
		}
		if (!flag)
		{
			p2 = p1; Movie_N++;
			p1 = (struct Movie_info*)malloc(sizeof(struct Movie_info));
			p2->next = p1; p1->next = NULL;
			strcpy(p1->Movie_Name, p->Movieinfo.name); p1->Movie_Num = 1;
		}
		flag = 0;
		p = p->next;
	}
	return (Movie_head);
}
/*打印放映信息*/
/*不含上座率和已售信息*/
void Print(struct info* head)
{
	struct info* p = head;
	int cnt = 0;
	if (p == NULL)
		printf("ERROR! No info in the list!\n");
	else
	{
		printf("                            影片信息                           \n");
		printf("                       （当前共有%d条记录）                    \n", n);
		printf("======================================================================\n");
		printf(" 编号       日期      影厅           场次         片名       票价（元）  \n");
		printf("======================================================================\n");
		while (p != NULL)
		{
			cnt++;
			printf("|%03d|    |%02d月%02d日|  |%s|      |%s|   |%6s|       |%d|\n"
				, p->id, p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price);
			p = p->next;
		}
		printf("==================================================================\n");
	}
}
/*包含上座率和已售信息*/
void Print_Admin(struct info* head)
{
	struct info* p = head;
	int cnt = 0;
	if (p == NULL)
		printf("ERROR! No info in the list!\n");
	else
	{
		printf("                                           影片信息                                  \n");
		printf("                                     （当前共有%d条记录）                            \n", n);
		printf("==========================================================================================\n");
		printf("编号        日期      影厅           场次          片名       票价（元）    已售     上座率\n");
		printf("==========================================================================================\n");
		while (p != NULL)
		{
			cnt++;
			printf("|%03d|    |%02d月%02d日|  |%s|      |%s|    |%6s|      |%3d|       |%3d|     |%4.1f%%|\n"
				, p->id, p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num, p->Movieinfo.Seat_Percent * 100);
			p = p->next;
		}
		printf("==========================================================================================\n");
	}
}


/*管理员管理系统*/
/*将影片信息先按照日期再按照场次排序*/
struct info* Sort(struct info* head)
{
	if (head == NULL)
		printf("NO data in the list!\n");
	else
	{
		int i, j;
		struct Movie tmp;
		for (i = 0; i < n; i++)
		{
			struct info* cur = head;
			for (j = 0; j < n - 1 - i; j++)
			{
				if (cur->Movieinfo.month > cur->next->Movieinfo.month)  //按月份排序
				{
					tmp = cur->Movieinfo; cur->Movieinfo = cur->next->Movieinfo; cur->next->Movieinfo = tmp;
				}
				if ((cur->Movieinfo.month == cur->next->Movieinfo.month) && (cur->Movieinfo.day > cur->next->Movieinfo.day)) //月份相同，按日期排序 
				{
					tmp = cur->Movieinfo; cur->Movieinfo = cur->next->Movieinfo; cur->next->Movieinfo = tmp;
				}
				if ((cur->Movieinfo.month == cur->next->Movieinfo.month) && (cur->Movieinfo.day == cur->next->Movieinfo.day)
					&& (strcmp(cur->Movieinfo.session, cur->next->Movieinfo.session) > 0)) //月份、日期相同场次排序 
				{
					tmp = cur->Movieinfo; cur->Movieinfo = cur->next->Movieinfo; cur->next->Movieinfo = tmp;
				}
				cur = cur->next;
			}
		}
		return head;
	}
}

/*各房间平均上座率*/
void Room_Per(struct info* head)
{
	struct info* p = head;
	int Nor1_Num, Nor2_Num, Nor3_Num, TD1_Num, TD2_Num, VIP_Num;
	float Nor1_Per, Nor2_Per, Nor3_Per, TD1_Per, TD2_Per, VIP_Per;
	Nor1_Num = Nor2_Num = Nor3_Num = TD1_Num = TD2_Num = VIP_Num = 0;
	Nor1_Per = Nor2_Per = Nor3_Per = TD1_Per = TD2_Per = VIP_Per = 0;
	while (p)
	{
		if (!(strcmp(p->Movieinfo.place, "普1"))) { Nor1_Num++; Nor1_Per += p->Movieinfo.Seat_Percent; }
		else if (!(strcmp(p->Movieinfo.place, "普2"))) { Nor2_Num++; Nor2_Per += p->Movieinfo.Seat_Percent; }
		else if (!(strcmp(p->Movieinfo.place, "普3"))) { Nor3_Num++; Nor3_Per += p->Movieinfo.Seat_Percent; }
		else if (!(strcmp(p->Movieinfo.place, "三1"))) { TD1_Num++; TD1_Per += p->Movieinfo.Seat_Percent; }
		else if (!(strcmp(p->Movieinfo.place, "三2"))) { TD2_Num++; TD2_Per += p->Movieinfo.Seat_Percent; }
		else if (!(strcmp(p->Movieinfo.place, "贵1"))) { VIP_Num++; VIP_Per += p->Movieinfo.Seat_Percent; }
		p = p->next;
	}
	//计算个房间平均上座率 
	Nor1_Per /= Nor1_Num; Nor2_Per /= Nor2_Num; Nor3_Per /= Nor3_Num; TD1_Per /= TD1_Num; TD2_Per /= TD2_Num; VIP_Per /= VIP_Num;
	//输出到屏幕
	printf("                                   各房间平均上座率统计                                    \n");
	printf("===========================================================================================\n");
	printf("普通影厅1      普通影厅2        普通影厅3        3D影厅1         3D影厅2            VIP影厅\n");
	printf(" %3.1f%%          %3.1f%%            %3.1f%%             %3.1f%%           %3.1f%%             %3.1f%%\n",
		Nor1_Per * 100, Nor2_Per * 100, Nor3_Per * 100, TD1_Per * 100, TD2_Per * 100, VIP_Per * 100);
	printf("===========================================================================================\n");
}
/*所有电影的上座率和票房*/
void Movie_PAS(struct info* head, struct Movie_info* Movie_head)
{
	struct info* p = head;
	struct Movie_info* p0 = Movie_head;
	int Sale_Num = 0, Sale_Money = 0;
	float Movie_Per = 0;
	int cnt = 0, tmp = 0;
	printf("======================================================================\n");
	printf("                           电影票房统计                           \n");
	printf("                      (当前共有%d场影片上映)                      \n", Movie_N);
	printf("======================================================================\n");
	printf("电影名    放映场次      总平均上座率       总观看人数       总票房\n");
	printf("======================================================================\n");
	while (p0)
	{
		while (p)
		{
			if (!(strcmp(p->Movieinfo.name, p0->Movie_Name)))
			{
				cnt++;
				Sale_Num += p->Movieinfo.num;
				tmp = p->Movieinfo.num;
				Sale_Money += tmp * p->Movieinfo.price;
				Movie_Per += p->Movieinfo.Seat_Percent;
			}
			p = p->next;
		}
		Movie_Per /= cnt;
		printf("%s       %2d             %3.1f%%             %-3d(人)        %-6d(元)\n",
			p0->Movie_Name, p0->Movie_Num, Movie_Per * 100, Sale_Num, Sale_Money);
		Movie_Per = cnt = Sale_Num = Sale_Money = 0; p = head;
		p0 = p0->next;
	}
	printf("======================================================================\n");
}
/*将所有影片ID初始化*/
void ID_Initi(struct info* head)
{
	struct info* p = head;
	int cnt = 1;
	while (p)
	{
		p->id = cnt;
		cnt++;
		p = p->next;
	}
}
/*对所有结构体中的数组成员初始化*/
void Initi(struct info* head)
{
	struct info* p = head;
	int i, j;
	while (p)
	{
		for (i = 0; i < 30; i++)
			for (j = 0; j < 30; j++)
				p->Movieinfo.seat[i][j] = 0;
		p = p->next;
	}
}
/*插入影片信息*/
struct info* Insert(struct info* head)
{
	if (head == NULL)
		printf("No Data in the list!\n");
	else
	{
		int index = 1;/*1表示正确插入影片信息，0表示未正确插入*/
		struct info* p0, * p1, * p2;
		p1 = p2 = head;
		p0 = (struct info*)malloc(sizeof(struct info));
		printf("Input Movie info:\n");
		printf("Date:");
		scanf("%d%d", &p0->Movieinfo.month, &p0->Movieinfo.day);
		int i;
		printf("================================\n");
		printf("=          1.普通影厅1         =\n");
		printf("=          2.普通影厅2         =\n");
		printf("=          3.普通影厅3         =\n");
		printf("=          4.3D影厅1           =\n");
		printf("=          5.3D影厅2           =\n");
		printf("=          6.VIP影厅           =\n");
		printf("================================\n");
		while (1)
		{
			printf("Place:");
			scanf("%d", &i);
			switch (i)
			{
			case 1:strcpy(p0->Movieinfo.place, "普1"); index = 1; break;
			case 2:strcpy(p0->Movieinfo.place, "普2"); index = 1; break;
			case 3:strcpy(p0->Movieinfo.place, "普3"); index = 1; break;
			case 4:strcpy(p0->Movieinfo.place, "三1"); index = 1; break;
			case 5:strcpy(p0->Movieinfo.place, "三2"); index = 1; break;
			case 6:strcpy(p0->Movieinfo.place, "贵1"); index = 1; break;
			default:index = 0; printf("ERROR Input!\n"); break;
			}
			if (index)
				break;
		}
		printf("=====================================\n");
		printf("=           1.08:00-10:00           =\n");
		printf("=           2.10:00-12:00           =\n");
		printf("=           3.12:30-14:30           =\n");
		printf("=           4.14:30-16:30           =\n");
		printf("=           5.16:30-18:30           =\n");
		printf("=           6.19:00-21:00           =\n");
		printf("=           7.21:00-23:00           =\n");
		printf("=====================================\n");
		while (1)
		{
			printf("Session:");
			scanf("%d", &i);
			switch (i)
			{
			case 1:strcpy(p0->Movieinfo.session, "08:00-10:00"); index = 1; break;
			case 2:strcpy(p0->Movieinfo.session, "10:00-12:00"); index = 1; break;
			case 3:strcpy(p0->Movieinfo.session, "12:30-14:30"); index = 1; break;
			case 4:strcpy(p0->Movieinfo.session, "14:30-16:30"); index = 1; break;
			case 5:strcpy(p0->Movieinfo.session, "16:30-18:30"); index = 1; break;
			case 6:strcpy(p0->Movieinfo.session, "19:00-21:00"); index = 1; break;
			case 7:strcpy(p0->Movieinfo.session, "21:00-23:00"); index = 1; break;
			default:index = 0; printf("ERROR Input!\n"); break;
			}
			if (index)
				break;
		}
		printf("Name:");
		scanf("%s", p0->Movieinfo.name);
		printf("Price:");
		scanf("%d", &p0->Movieinfo.price);
		p0->Movieinfo.num = 0;
		/*将插入的影片信息放在最后一位 */
		while (p1)
		{
			p2 = p1; p1 = p1->next;
		}
		p2->next = p0; p0->next = NULL; n++;
		p0->id = n; p0->Movieinfo.num = 0;//id,num初始化
		SetColor(15, 12); printf("Successfully Insert!"); SetColor(7, 0); printf("\n");
	}
	return head;
}
/*删除影片信息*/
struct info* Del(struct info* head)
{
	if (head == NULL)
		printf("No Data in the list!\n");
	else
	{
		struct info *p0, *bfp0;
		printf("影片信息已加载，输入对应序号删除\n");
		Print(head);
		int i, Input_ID = 1;		
		while (Input_ID)
		{
			printf("Input Number(0exit):");
			scanf("%d", &Input_ID);
			if (!Input_ID)
				break;
			p0 = bfp0 = head;
			while (p0)
			{
				if (p0->id == Input_ID)	break;
				else
				{
					bfp0 = p0;
					p0 = p0->next;
				}
			}
			if (p0)
			{
				if (p0 == head)	head = p0->next;
				else
					bfp0->next = p0->next;
				n--; free(p0);
				system("cls");
				printf("影片信息已加载，输入对应序号删除\n");
				Print(head);
			}
			else
				printf("Error Input!\n");
		}
	}
	return head;
}
/*按电影名称查找影片信息*/
void Find(struct info* head)
{
	char Key[10]; int cnt = 0;
	printf("Input Movie Name to Find:");
	scanf("%s", Key);
	struct info* p0 = head;
	while (p0)
	{
		if (!strcmp(Key, p0->Movieinfo.name))
		{
			cnt++;
			printf("%02d月%02d日  %s      %s    %6s       %d\n",
				p0->Movieinfo.month, p0->Movieinfo.day, p0->Movieinfo.place, p0->Movieinfo.session, p0->Movieinfo.name, p0->Movieinfo.price);
		}
		p0 = p0->next;
	}
	SetColor(15, 12); printf("共找到%d条记录", cnt); SetColor(7, 0); printf("\n");
}
/*上座率情况统计*/
void Seat_Percent(struct info* head)
{
	struct info* p = head;
	while (p)
	{
		if (strcmp(p->Movieinfo.place, "普1") == 0 || strcmp(p->Movieinfo.place, "普2") == 0 || strcmp(p->Movieinfo.place, "普3") == 0)
			p->Movieinfo.Seat_Percent = (1.0) * p->Movieinfo.num / Nor_MAX;
		else if (strcmp(p->Movieinfo.place, "三1") == 0 || strcmp(p->Movieinfo.place, "三2") == 0)
			p->Movieinfo.Seat_Percent = (1.0) * p->Movieinfo.num / ThreeD_MAX;
		else if (!(strcmp(p->Movieinfo.place, "贵1")))
			p->Movieinfo.Seat_Percent = (1.0) * p->Movieinfo.num / VIP_MAX;
		p = p->next;
	}
}
/*将信息保存到文档*/
void Save(struct info* head)
{
	struct info* p = head; int cnt = 0;
	p = head;
	FILE* fp;
	if ((fp = fopen("Movieinfo_Output.txt", "w")) == NULL)
	{
		printf("Can't Open The File!\n"); return;
	}
	else
	{
		fputs("                                                           影片信息     \n", fp);
		fputs("==================================================================================================================================================\n", fp);
		fputs("编号     日期         影厅               场次               片名          票价（元）    已售        上座率\n", fp);
		while (p)
		{
			cnt++;
			fprintf(fp, "%02d    %02d月%02d日     %s         %s       %6s         %3d              %3d           %4.1f%%\n",
				cnt, p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num, p->Movieinfo.Seat_Percent * 100);
			p = p->next;
		}
		SetColor(15, 12); printf("Successfully Save to The Doc."); SetColor(7, 0); printf("\n");
		fclose(fp);
	}
}
/*查看版本信息*/
void Ver_Print(void)
{
	FILE* fp; char str[300];
	if ((fp = fopen("Version_info.txt", "r")) == NULL)
	{
		printf("Can't open the file!"); return;
	}
	else
	{
		while (!feof(fp))
		{
			fgets(str, 300, fp);
			printf("%s", str);
		}
		fclose(fp);
	}
}

/*用户管理系统*/
//打印座位表 
void Print_Seat(struct info* p, int SUM_Row, int SUM_Col)
{
	int cnt = 1, Row = 0, Col = 0;//初始化 
	int i, j;
	for (i = 0; i < 2 * SUM_Row + 1; i++)
	{
		for (j = 0; j < SUM_Col; j++)
		{
			if (!(i % 2))	printf("——");
			else
			{
				printf("|");
				if (p->Movieinfo.seat[Row][Col] == 0)
				{
					SetColor(15, 2);
					printf("%3d", cnt);
				}
				else
				{
					SetColor(15, 4);
					printf("%3d", cnt);
				}
				SetColor(7, 0);
				cnt++; Col++;
			}
		}
		if (i % 2) { printf("|"); Row++; Col = 0; }
		printf("\n");
	}
}
//输入电影序号打印座位表 
void Input_Print(struct info* head, int Nor_Row, int Nor_Col, int TD_Row, int TD_Col, int VIP_Row, int VIP_Col)
{
	struct info* p = head;
	if (p == NULL)
	{
		printf("No Data in the List!"); system("pause"); system("cls"); return;
	}
	else
	{
		int N;
		Print(head);
		printf("请输入序号："); scanf("%d", &N);
		while( N > n )
		{
			printf("输入序号有误，请重新输入！\n");
			printf("请输入序号：");scanf("%d",&N);
		 }
		for (int i = 1; i < N; i++)
			p = p->next;
		/*根据序号输出文件信息*/
		printf("该影院的信息为：");
		SetColor(8, 14);
		printf("%02d月%02d日  %s      %s    %6s       %d元    %d(人)",
			p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num);
		SetColor(7, 0); printf("\n");
		if (strcmp(p->Movieinfo.place, "普1") == 0 || strcmp(p->Movieinfo.place, "普2") == 0 || strcmp(p->Movieinfo.place, "普3") == 0)
			Print_Seat(p, Nor_Row, Nor_Col);
		else if (strcmp(p->Movieinfo.place, "三1") == 0 || strcmp(p->Movieinfo.place, "三2") == 0)
			Print_Seat(p, TD_Row, TD_Col);
		else if (!strcmp(p->Movieinfo.place, "贵1"))
			Print_Seat(p, VIP_Row, VIP_Col);
	}
}
/*以下是手动输入系统部分*/
int ManSelec(struct info* p, int SUM_Row, int SUM_Col)
{
	/*返回值说明：
	返回0代表退出，-1代表座位已经被选，1代表选座成功*/ 
	int N, MAX, Default;
	int Col = 0, Row = 0;
	/*判断房间*/
	if (SUM_Col == Nor_Default) { MAX = Nor_MAX; Default = Nor_Default; }
	else if (SUM_Col == TD_Default) { MAX = ThreeD_MAX; Default = TD_Default; }
	else if (SUM_Col == VIP_Default) { MAX = VIP_MAX; Default = VIP_Default; }
	else { printf("Can't Find the Room!\n"); return 0; }
	printf("请输入座位号(0:exit):");
	scanf("%d", &N);
	/*错误检查*/
	while (N < 0 || N > MAX)
	{
		SetColor(4, 14); printf("输入错误！请重试!\n");SetColor(7, 0);printf("请输入座位号(0:exit)：");scanf("%d", &N);
	}
	if( N == 0 )	return 0;
	Col = (N - 1) % Default;
	if (N % Default)	Row = (N / Default);
	else 	Row = (N / Default) - 1;
	if (p->Movieinfo.seat[Row][Col] == 0)
	{
		p->Movieinfo.seat[Row][Col] = 1;
		system("cls");
		printf("您现在选择的是:");
		SetColor(8, 14);
		printf("%02d月%02d日  %s      %s    %6s       %d元    %d(人)",
			p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num);
		SetColor(7, 0); printf("\n");
		Print_Seat(p, SUM_Row, SUM_Col);
		SetColor(8, 14); printf("选座成功！\n"); SetColor(7, 0); return 1;
	}
	else if (p->Movieinfo.seat[Row][Col] == 1)
	{
		SetColor(4, 14); printf("座位已被选！请更换"); SetColor(7, 0);printf("\n");return -1;
	}
}
/*用户选座系统*/
/*以下是手动选座系统部分*/
void Man(struct info* head, int Nor_Row, int Nor_Col, int TD_Row, int TD_Col, int VIP_Row, int VIP_Col)
{
	struct info* p = head;
	int i, j, index;
	Print(head);
	int Key;
	while (1)
	{
		printf("输入电影序号:");
		scanf("%d", &Key);
		if (Key > 0 && Key <= n)
		{
			while (p)
			{
				if (Key == p->id)
				{
					system("cls");
					printf("您现在选择的是:");
					SetColor(8, 14);
					printf("%02d月%02d日  %s      %s    %6s       %d元    %d(人)",
						p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num);
					SetColor(7, 0); printf("\n");
					/*开始打印座位*/
					if (strcmp(p->Movieinfo.place, "普1") == 0 || strcmp(p->Movieinfo.place, "普2") == 0 || strcmp(p->Movieinfo.place, "普3") == 0)
					{
						index = 1;
						Print_Seat(p, Nor_Row, Nor_Col);
						while (index == 1 || index == -1)
						{
							index =  ManSelec(p, Nor_Row, Nor_Col);	//调用ManSelec函数并且判断是否成功选座 	
							if (index == 1)
							{
								p->Movieinfo.num++;
								system("cls");
								printf("您现在选择的是:");
								SetColor(8, 14);
								printf("%02d月%02d日  %s      %s    %6s       %d元    %d(人)",
									p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num);
								SetColor(7, 0); printf("\n");
								Print_Seat(p, Nor_Row, Nor_Col);
							}
						}
					}
					else if (strcmp(p->Movieinfo.place, "三1") == 0 || strcmp(p->Movieinfo.place, "三2") == 0)
					{
						index = 1;
						Print_Seat(p, TD_Row, TD_Col);
						while (index == 1 || index == -1)
						{
							index = ManSelec(p, TD_Row, TD_Col);	//调用ManSelec函数并且判断是否成功选座 
							if (index == 1)
							{
								p->Movieinfo.num++;
								system("cls");
								printf("您现在选择的是:");
								SetColor(8, 14);
								printf("%02d月%02d日  %s      %s    %6s       %d元    %d(人)",
									p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num);
								SetColor(7, 0); printf("\n");
								Print_Seat(p, TD_Row, TD_Col);
							}
						}
					}
					else if (!strcmp(p->Movieinfo.place, "贵1"))
					{
						index = 1;
						Print_Seat(p, VIP_Row, VIP_Col);
						while (index == 1 || index == -1)
						{
							index = ManSelec(p, VIP_Row, VIP_Col);	//调用ManSelec函数并且判断是否成功选座 
							if (index == 1)
							{
								p->Movieinfo.num++;
								system("cls");
								printf("您现在选择的是:");
								SetColor(8, 14);
								printf("%02d月%02d日  %s      %s    %6s       %d元    %d(人)",
									p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num);
								SetColor(7, 0); printf("\n");
								Print_Seat(p, VIP_Row, VIP_Col);
							}
						}
					}
				}
				p = p->next;
			}
			break;
		}
		else printf("输入无效,请重新输入!\n");
	}
}
/*手动输入座位部分结束*/
/*以下是自动生成座位部分*/
void AutoSelec(struct info* p, int SUM_Row, int SUM_Col, int Ran_People)
{
	srand(time(NULL)); // 设置随机数种子 
	int cnt;
	int Row, Col;
	for (cnt = 0; cnt < Ran_People; )
	{
		Row = abs(rand() % SUM_Row); Col = abs(rand() % SUM_Col);
		if (p->Movieinfo.seat[Row][Col] == 0)
		{
			p->Movieinfo.seat[Row][Col] = 1; cnt++;
		}
	}
}
void Auto(struct info* head, int Nor_Row, int Nor_Col, int TD_Row, int TD_Col, int VIP_Row, int VIP_Col)
{
	struct info* p = head;
	srand(time(NULL)); //设置随机数种子
	SetColor(15, 12); printf("正在生成随机座位……\n"); SetColor(7, 0);
	int Ran_People = 0;
	int cnt = 0;
	while (p)
	{
		if (strcmp(p->Movieinfo.place, "普1") == 0 || strcmp(p->Movieinfo.place, "普2") == 0 || strcmp(p->Movieinfo.place, "普3") == 0)
		{
			Ran_People = abs(rand() % (Nor_MAX + 1 - p->Movieinfo.num));
			AutoSelec(p, Nor_Row, Nor_Col, Ran_People);
			p->Movieinfo.num += Ran_People;
		}
		else if (strcmp(p->Movieinfo.place, "三1") == 0 || strcmp(p->Movieinfo.place, "三2") == 0)
		{
			Ran_People = abs(rand() % (ThreeD_MAX + 1 - p->Movieinfo.num));
			AutoSelec(p, TD_Row, TD_Col, Ran_People);
			p->Movieinfo.num += Ran_People;
		}
		else if (!strcmp(p->Movieinfo.place, "贵1"))
		{
			Ran_People = abs(rand() % (VIP_MAX + 1 - p->Movieinfo.num));
			AutoSelec(p, VIP_Row, VIP_Col, Ran_People);
			p->Movieinfo.num += Ran_People;
		}

		cnt += Ran_People; p = p->next;
	}
	SetColor(15, 12); printf("随机用户产生完成，共产生%d位用户\n", cnt); SetColor(7, 0); system("pause");
}
/*自动生成座位部分结束*/
/*用户菜单*/
void User_Menu(struct info* head, struct Movie_info* Movie_head, int Nor_Row, int Nor_Col, int TD_Row, int TD_Col, int VIP_Row, int VIP_Col)
{
	int i;
	while (1)
	{
		printf("===================================================================\n");
		printf("=                          欢迎来到用户系统                       =\n");
		printf("===================================================================\n");
		printf("=                                                                 =\n");
		printf("=                         0.返回主菜单                            =\n");
		printf("=                         1.自动生成座位数据                      =\n");
		printf("=                            (测试时使用)                         =\n");
		printf("=                         2.人工购票                              =\n");
		printf("=                         3.查看座位信息                          =\n");
		printf("=                                                                 =\n");
		printf("===================================================================\n");
		SetColor(8, 14);
		printf("Input Code:"); scanf("%d", &i);
		SetColor(7, 0);
		switch (i)
		{
		case 0: system("cls"); return; break;
		case 1: Auto(head, Nor_Row, Nor_Col, TD_Row, TD_Col, VIP_Row, VIP_Col);Seat_Percent(head);Save(head);system("cls"); break;
		case 2: system("cls"); Man(head, Nor_Row, Nor_Col, TD_Row, TD_Col, VIP_Row, VIP_Col);Seat_Percent(head);Save(head);system("cls"); break;
		case 3: system("cls"); Input_Print(head, Nor_Row, Nor_Col, TD_Row, TD_Col, VIP_Row, VIP_Col); system("pause"); system("cls"); break;
		default: printf("ERROE INPUT!!\n"); system("pause"); system("cls");
		}
	}
}
/*管理员菜单*/
/*管理员登录系统*/
//管理员分菜单——数据统计菜单 
void DataSta_Menu(struct info* head, struct Movie_info* Movie_head, int Nor_Row, int Nor_Col, int TD_Row, int TD_Col, int VIP_Row, int VIP_Col)
{
	int i;
	while (1)
	{
		printf("================================================================\n");
		printf("=                       欢迎来到票房统计系统                   =\n");
		printf("================================================================\n");
		printf("=                                                              =\n");
		printf("=                        0.返回管理员系统                      =\n");
		printf("=                        1.查看各场次上座率                    =\n");
		printf("=                        2.查看各房间上座率                    =\n");
		printf("=                        3.查看电影上座率和票房                =\n");
		printf("=                                                              =\n");
		printf("================================================================\n");
		SetColor(8, 14);
		printf("Input Code:"); scanf("%d", &i);
		SetColor(7, 0);
		switch (i)
		{
		case 0: system("cls"); return; break;
		case 1: system("cls"); Print_Admin(head); system("pause"); system("cls"); break;
		case 2: system("cls"); Room_Per(head); system("pause"); system("cls"); break;
		case 3:system("cls"); Movie_PAS(head, Movie_head); system("pause"); system("cls"); break;
		default: printf("ERROE INPUT!!\n"); system("pause"); system("cls"); break;
		}
	}
}
//管理员主菜单 
void Admin_Menu(struct info* head, struct Movie_info* Movie_head, int Nor_Row, int Nor_Col, int TD_Row, int TD_Col, int VIP_Row, int VIP_Col)
{
	int i;
	while (1)
	{
		printf("================================================================\n");
		printf("=                        欢迎来到管理员系统                    =\n");
		printf("================================================================\n");
		printf("=                                                              =\n");
		printf("=                      0.退出登录并返回主菜单                  =\n");
		printf("=                      1.查看电影放映信息                      =\n");
		printf("=                      2.查看座位信息                          =\n");
		printf("=                      3.插入放映信息                          =\n");
		printf("=                      4.查找放映信息                          =\n");
		printf("=                      5.删除放映信息                          =\n");
		printf("=                      6.将放映信息保存到文件                  =\n");
		printf("=                      7.进入票房统计系统                      =\n");
		printf("=                      8.自动生成座位数据                      =\n");
		printf("=                          (测试时使用)                        =\n");
		printf("=                      9.查看版本信息                          =\n");
		printf("=                                                              =\n");
		printf("================================================================\n");
		SetColor(8, 14);
		printf("Input Code:"); scanf("%d", &i);
		SetColor(7, 0);
		Seat_Percent(head);
		switch (i)
		{
		case 0: system("cls"); return; break;
		case 1: system("cls"); Sort(head); Print_Admin(head);Save(head);system("pause"); system("cls"); break;
		case 2:	system("cls"); Input_Print(head, Nor_Row, Nor_Col, TD_Row, TD_Col, VIP_Row, VIP_Col); system("pause"); system("cls"); break;
		case 3: head = Insert(head); Sort(head); ID_Initi(head);Seat_Percent(head);Save(head);system("pause"); system("cls"); break;
		case 4: Find(head); system("pause"); system("cls"); break;
		case 5: system("cls"); head = Del(head); Sort(head); ID_Initi(head);Seat_Percent(head);Save(head);system("pause"); system("cls"); break;
		case 6: Sort(head); ID_Initi(head); Save(head); system("pause"); system("cls"); break;
		case 7: system("cls"); DataSta_Menu(head, Movie_head, Nor_Row, Nor_Col, TD_Row, TD_Col, VIP_Row, VIP_Col); break;
		case 8: Auto(head, Nor_Row, Nor_Col, TD_Row, TD_Col, VIP_Row, VIP_Col);Seat_Percent(head);Save(head);system("cls"); break;
		case 9: system("cls"); Ver_Print(); printf("\n"); system("pause"); system("cls"); break;
		default: printf("ERROE INPUT!!\n"); system("pause"); system("cls");
		}
	}
}
//管理员分菜单——登录系统主菜单
void Signin_Menu_UI(void)
{
	system("cls");
	printf("========================================\n");
	printf("=       欢迎使用管理员登陆系统          \n");
	printf("========================================\n");
	printf("=             0.返回主菜单             =\n");
	printf("=             1.登录系统               =\n");
	printf("=             2.创建账号               =\n");
	printf("=             3.修改密码               =\n");
	printf("========================================\n");
}
int Signin_Menu(struct User* User_head)
{
	int i;
	void Signin(struct User* User_head);
	void Creat(struct User* head);
	void Change_Pass(struct User* head);
	Signin_Menu_UI();
	SetColor(8, 14); printf("请按键选择，回车确定:");
	scanf("%d", &i); SetColor(7, 0);
	switch (i)
	{
	case 0:return 0; break;
	case 1:Signin(User_head); system("cls"); break;
	case 2:Creat(User_head); system("cls"); Signin_Menu(User_head); break;
	case 3:Change_Pass(User_head); break;
	default:printf("输入了错误的字符！\n"); system("pause"); Signin_Menu(User_head); break;
	}
	return 1;
}
//简单的密码加密
void Secretword(char password[])
{
	unsigned char c;
	int i = 0;
	while ((c = getch()) != '\r')
	{
		if (i < 20 && isprint(c))
		{
			password[i] = c;
			i++;
			putchar('*');
		}
		else if (i > 0 && c == '\b') {
			i--;
			putchar('\b');
			putchar(' ');
			putchar('\b');
		}
	}
	printf("\n");
	password[i] = '\0';
}
//创建新的账号 
void Creat(struct User* User_head)
{
	char account[20] = { '\0' }, password1[20] = { '\0' }, password2[20] = { '\0' };
	printf("请输入你的账号:\n");
	scanf("%s", account);
	while (1)
	{
		printf("请输入密码:\n");
		Secretword(password1);
		printf("请再输入一遍密码：\n");
		Secretword(password2);
		if (!(strcmp(password1, password2)))
		{
			printf("账号创建成功。\n");
			break;
		}
		else
		{
			printf("两次输入密码不一致，请重新输入！\n");
		}
	}
	User_N++;
	//写入文件
	FILE* fp;
	if ((fp = fopen("User_info.txt", "r")) == NULL)
	{
		fp = fopen("User_info.txt", "w");
		fclose(fp);
	}
	fp = fopen("User_info.txt", "a");
	fprintf(fp, "%s    %s\n", account, password1);
	fclose(fp);
	//写入链表
	struct User* p0, * p1, * p2;
	p1 = p2 = User_head;
	p0 = (struct User*)malloc(sizeof(struct User));
	strcpy(p0->account, account); strcpy(p0->password, password1);
	while (p1)
	{
		p2 = p1; p1 = p1->next;
	}
	p2->next = p0; p0->next = NULL; User_N++;
	SetColor(15, 12); printf("保存成功，按任意键返回上一层!\n"); SetColor(7, 0); getch();
}
//匹配数据库中的账号密码
int Match(char Input_account[], char Input_password[])
{
	FILE* fp;
	int n = 0, i = 0;
	char password[20] = { '\0' }, account[20] = { '\0' };
	fp = fopen("User_info.txt", "r");
	while (!feof(fp))
	{
		fscanf(fp, "%s    %s\n", account, password);
		if (strcmp(Input_account, account) == 0)
		{
			if (strcmp(Input_password, password) == 0)
			{
				fclose(fp); return 1;
			}
			else
			{
				fclose(fp); return -1;
			}
		}
	}
	fclose(fp);
	return 0;
}
//登陆管理员系统函数 
void Signin(struct User* User_head)
{
	if (User_N == 0)
	{
		SetColor(15, 12); printf("数据库为空，请选择添加账号!!\n"); SetColor(7, 0); system("pause"); Signin_Menu(User_head);
	}
	char Input_account[20] = { '\0' }, Input_password[20] = { '\0' };
	int flag; //用于判断是否登录成功 
	printf("请输入账号：\n");
	scanf("%s", Input_account);
	printf("请输入密码：\n");
	Secretword(Input_password);
	flag = Match(Input_account, Input_password);
	if (flag == 1)
	{
		SetColor(15, 12); printf("登陆成功!\n"); SetColor(7, 0);
		system("pause");
	}
	else
	{
		if (flag == -1)
		{
			printf("密码错误!\n");
			system("pause"); system("cls"); Signin_Menu_UI();
			Signin(User_head);
		}
		if (flag == 0)
		{
			printf("不存在此用户,请新建该账户！\n");
			system("pause"); system("cls");
			Signin_Menu(User_head);
		}
	}
}
//将链表中的密码保存到文件
void Print_PassWord(struct User* User_head)
{
	FILE* fp;
	struct User* p = User_head;
	if ((fp = fopen("User_info.txt", "w")) == NULL)
	{
		printf("Can't Open The File!\n"); return;
	}
	else
	{
		while (p)
		{
			fprintf(fp, "%s    %s\n", p->account, p->password);
			p = p->next;
		}
		fclose(fp);
	}
}
//修改密码
void Change_Pass(struct User* User_head)
{
	if (User_N == 0)
	{
		SetColor(15, 12); printf("数据库为空，请选择添加账号!!\n"); SetColor(7, 0); system("pause"); Signin_Menu(User_head);
	}
	struct User* p1;
	struct User* p2;
	p2 = p1 = User_head;
	int i;
	char account1[20], password1[20], password2[20], password3[20];
	printf("请输入你的账号:\n");
	scanf("%s", account1);
	printf("请输入原密码:\n");
	Secretword(password1);
	i = Match(account1, password1);
	if (i == 1 || i == -1)
	{
		while (p1 != NULL)
		{
			if (!strcmp(p1->account, account1))	break;
			p1 = p1->next;
		}
	}
	if (i == 1)
	{
		while (1)
		{
			SetColor(15, 12); printf("密码正确!\n"); SetColor(7, 0);
			printf("请输入新的密码:\n");
			Secretword(password2);
			printf("请再输入一遍密码:\n");
			Secretword(password3);
			if (strcmp(password2, password3) == 0)
			{
				strcpy(p1->password, password2);
				Print_PassWord(User_head);
				break;
			}
			else
			{
				SetColor(15, 12); printf("两次输入密码不一致，请重新输入"); SetColor(7, 0);
			}
		}
		SetColor(15, 12); printf("密码修改成功。\n"); SetColor(7, 0); system("pause"); Signin_Menu(User_head);
	}
	else
	{
		if (i == -1)
		{
			SetColor(15, 12); printf("密码错误,请重新输入!\n"); SetColor(7, 0);
			Change_Pass(User_head);
		}
		if (i == 0)
		{
			SetColor(15, 12); printf("不存在此用户,请创建账号\n"); SetColor(7, 0);
			system("pause"); system("cls");
			Signin_Menu(User_head);
		}
	}
}
int main()
{
	/*下面开始计算各影厅总的行和列*/
	int flag = 0;
	//普通座位 
	int Nor_Row, Nor_Col;
	/*判断是否要加行*/
	if (Nor_MAX % Nor_Default)	flag = 1;
	else	flag = 0;
	Nor_Col = Nor_Default;
	Nor_Row = (Nor_MAX / Nor_Default) + flag;

	//3D座位
	int TD_Row, TD_Col;
	/*判断是否要加行*/
	if (ThreeD_MAX % TD_Default) flag = 1;
	else	flag = 0;
	TD_Col = TD_Default;
	TD_Row = (ThreeD_MAX / TD_Default) + flag;

	//VIP座位 
	int VIP_Row, VIP_Col;
	/*判断是否需要加行*/
	if (VIP_MAX % VIP_Default) flag = 1;
	else	flag = 0;
	VIP_Col = VIP_Default;
	VIP_Row = (VIP_MAX / VIP_Default) + flag;
	/*影厅总的行和列计算完成*/
	struct info* head;
	struct Movie_info* Movie_head;
	struct User* User_head;
	User_head = LoadUser();
	head = LoadData();
	/*初始化*/
	Initi(head); Sort(head);
	Movie_head = LoadMovie(head);
	/*检查数据文件是否有错误*/
	if(!Check_Input(head))	{printf("请检查数据库文件是否有错误！\n");return 0;}
	/*主菜单选项*/
	int i;
	while (1)
	{
		system("title 影院管理系统V3.4");
		printf("===============================================\n");
		printf("=           欢迎来到影院管理系统系统          =\n");
		printf("===============================================\n");
		printf("=                                             =\n");
		printf("=              0.退出                         =\n");
		printf("=              1.进入用户系统                 =\n");
		printf("=              2.进入管理员系统               =\n");
		printf("=                                             =\n");
		printf("===============================================\n");
		SetColor(8, 14);
		printf("Input Code:"); scanf("%d", &i);
		SetColor(7, 0);
		switch (i)
		{
		case 0: exit(0); break;
		case 1: system("cls"); User_Menu(head, Movie_head, Nor_Row, Nor_Col, TD_Row, TD_Col, VIP_Row, VIP_Col); break;
		case 2: system("cls");
			if (Signin_Menu(User_head))
				Admin_Menu(head, Movie_head, Nor_Row, Nor_Col, TD_Row, TD_Col, VIP_Row, VIP_Col);
			else system("cls");
			break;
		default: printf("ERROE INPUT!!\n"); system("pause"); system("cls");
		}
	}
	return 0;
}
