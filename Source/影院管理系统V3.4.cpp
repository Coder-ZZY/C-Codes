#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include<ctype.h>
/*���Դ���*/
#pragma warning(disable : 4996)
/*����Ӱ���������*/
#define Nor_MAX 300 
#define ThreeD_MAX 200 
#define VIP_MAX 100
/*Ĭ��һ�ŵ�����*/
#define Nor_Default 25
#define TD_Default 20
#define VIP_Default 10
int n = 0; //���ڼ����ļ������ݵ���Ŀ
int Movie_N = 0;//���ڼ����ļ��е�Ӱ����Ŀ 
int User_N = 0; //����Ա�˻���Ŀ
/*�����жϳ���ִ���Ƿ���ȷ*/ 
typedef int Status;
#define OK 1
#define ERROR 0
/*Ԥ�������*/ 
/*���ص�¼��Ϣ*/
struct User
{
	char account[20];
	char password[20];
	struct User* next;
};
struct Movie
{
	int day;                      //��ӳ���� 
	int month;
	char place[4];               //��ӳ�ص� 
	char session[15];           //��ӳ���� 
	char name[10];             //��Ӱ�� 
	int price;                //��Ӱ�۸� 
	int num;                 // ��������
	int seat[30][30];		//��λ���
	float Seat_Percent;     //��������� 
};
struct info
{
	struct Movie Movieinfo;
	int id;                        //Ψһ��ӳ��� 
	struct info* next;
};
/*����ͳ�Ƶ�Ӱ��Ŀ*/
struct Movie_info
{
	int Movie_Num;
	char Movie_Name[10];
	struct Movie_info* next;
};
void SetColor(unsigned short ForeColor , unsigned short BackGroundColor )
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ��ǰ���ھ��
	SetConsoleTextAttribute(handle, ForeColor + BackGroundColor * 0x10);//������ɫ
	/*��ɫ���룺
	0 = ��ɫ       8 = ��ɫ
	1 = ��ɫ       9 = ����ɫ
	2 = ��ɫ       10 = ����ɫ
	3 = ǳ��ɫ     11 = ��ǳ��ɫ
	4 = ��ɫ       12 = ����ɫ
	5 = ��ɫ       13 = ����ɫ
	6 = ��ɫ       14 = ����ɫ
	7 = ��ɫ       15 = ����ɫ */
}
/*���ļ��ж������ݲ��������� */
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
		while (!feof(fp))	//fp����ָ���ļ�ĩβʱ 
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
	if ((fp = fopen("Movie_info.txt", "r")) == NULL)  //��Movie_info.txt
	{
		printf("Can't open the file!\n"); exit(0);
	}
	//�ж��ļ��Ƿ�Ϊ�� 
	int ch;
	if ((ch = fgetc(fp)) == EOF)
	{
		printf("File is empty!\n"); exit(0);
	}
	else
	{
		rewind(fp); //�����ļ�ָ�� 
		struct info* p1, * p2;
		struct info* head;
		p1 = p2 = (struct info*)malloc(sizeof(struct info));
		head = p1;
		fscanf(fp, "%d.%d %s %s %s %d"
			, &p1->Movieinfo.month, &p1->Movieinfo.day, p1->Movieinfo.place, p1->Movieinfo.session, p1->Movieinfo.name, &p1->Movieinfo.price);
		p1->id = n + 1; p1->Movieinfo.num = 0; p1->Movieinfo.Seat_Percent = 0;//��ӳ��š����������������ʳ�ʼ�� 
		while (!feof(fp))	//fp����ָ���ļ�ĩβʱ 
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
			p1->Movieinfo.num = 0; p1->id = n + 1; p1->Movieinfo.Seat_Percent = 0;//��ӳ��š�����������ʼ�� 
		}
		p2->next = NULL;
		fclose(fp);
		return head;
	}
}
/*������ݿ��е��ļ��Ƿ���ȷ*/ 
Status Check_Input( struct info *head )
{
	struct info *p1,*p2;/*p2���ڼ�¼��ǰ�жϵ�����*/ 
	if(!head)	return ERROR;
	p1 = head;p2 = p1->next;
	int i,cnt = 1;//cnt���ڼ�¼û����������ݸ��� 
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
/*��ʼͳ�Ƶ�Ӱ��Ŀ*/
struct Movie_info* LoadMovie(struct info* head)
{
	struct Movie_info* p1, * p2, * p0;
	struct Movie_info* Movie_head;
	struct info* p = head;
	int flag = 0;
	Movie_head = NULL;
	/*¼���һ����Ӱ����*/
	Movie_head = p1 = p2 = p0 = (struct Movie_info*)malloc(sizeof(struct Movie_info));
	p2->next = NULL; p1->Movie_Num = 0; Movie_N++;
	strcpy(p1->Movie_Name, p->Movieinfo.name); p1->Movie_Num++;
	p = p->next;
	/*�������*/
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
/*��ӡ��ӳ��Ϣ*/
/*���������ʺ�������Ϣ*/
void Print(struct info* head)
{
	struct info* p = head;
	int cnt = 0;
	if (p == NULL)
		printf("ERROR! No info in the list!\n");
	else
	{
		printf("                            ӰƬ��Ϣ                           \n");
		printf("                       ����ǰ����%d����¼��                    \n", n);
		printf("======================================================================\n");
		printf(" ���       ����      Ӱ��           ����         Ƭ��       Ʊ�ۣ�Ԫ��  \n");
		printf("======================================================================\n");
		while (p != NULL)
		{
			cnt++;
			printf("|%03d|    |%02d��%02d��|  |%s|      |%s|   |%6s|       |%d|\n"
				, p->id, p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price);
			p = p->next;
		}
		printf("==================================================================\n");
	}
}
/*���������ʺ�������Ϣ*/
void Print_Admin(struct info* head)
{
	struct info* p = head;
	int cnt = 0;
	if (p == NULL)
		printf("ERROR! No info in the list!\n");
	else
	{
		printf("                                           ӰƬ��Ϣ                                  \n");
		printf("                                     ����ǰ����%d����¼��                            \n", n);
		printf("==========================================================================================\n");
		printf("���        ����      Ӱ��           ����          Ƭ��       Ʊ�ۣ�Ԫ��    ����     ������\n");
		printf("==========================================================================================\n");
		while (p != NULL)
		{
			cnt++;
			printf("|%03d|    |%02d��%02d��|  |%s|      |%s|    |%6s|      |%3d|       |%3d|     |%4.1f%%|\n"
				, p->id, p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num, p->Movieinfo.Seat_Percent * 100);
			p = p->next;
		}
		printf("==========================================================================================\n");
	}
}


/*����Ա����ϵͳ*/
/*��ӰƬ��Ϣ�Ȱ��������ٰ��ճ�������*/
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
				if (cur->Movieinfo.month > cur->next->Movieinfo.month)  //���·�����
				{
					tmp = cur->Movieinfo; cur->Movieinfo = cur->next->Movieinfo; cur->next->Movieinfo = tmp;
				}
				if ((cur->Movieinfo.month == cur->next->Movieinfo.month) && (cur->Movieinfo.day > cur->next->Movieinfo.day)) //�·���ͬ������������ 
				{
					tmp = cur->Movieinfo; cur->Movieinfo = cur->next->Movieinfo; cur->next->Movieinfo = tmp;
				}
				if ((cur->Movieinfo.month == cur->next->Movieinfo.month) && (cur->Movieinfo.day == cur->next->Movieinfo.day)
					&& (strcmp(cur->Movieinfo.session, cur->next->Movieinfo.session) > 0)) //�·ݡ�������ͬ�������� 
				{
					tmp = cur->Movieinfo; cur->Movieinfo = cur->next->Movieinfo; cur->next->Movieinfo = tmp;
				}
				cur = cur->next;
			}
		}
		return head;
	}
}

/*������ƽ��������*/
void Room_Per(struct info* head)
{
	struct info* p = head;
	int Nor1_Num, Nor2_Num, Nor3_Num, TD1_Num, TD2_Num, VIP_Num;
	float Nor1_Per, Nor2_Per, Nor3_Per, TD1_Per, TD2_Per, VIP_Per;
	Nor1_Num = Nor2_Num = Nor3_Num = TD1_Num = TD2_Num = VIP_Num = 0;
	Nor1_Per = Nor2_Per = Nor3_Per = TD1_Per = TD2_Per = VIP_Per = 0;
	while (p)
	{
		if (!(strcmp(p->Movieinfo.place, "��1"))) { Nor1_Num++; Nor1_Per += p->Movieinfo.Seat_Percent; }
		else if (!(strcmp(p->Movieinfo.place, "��2"))) { Nor2_Num++; Nor2_Per += p->Movieinfo.Seat_Percent; }
		else if (!(strcmp(p->Movieinfo.place, "��3"))) { Nor3_Num++; Nor3_Per += p->Movieinfo.Seat_Percent; }
		else if (!(strcmp(p->Movieinfo.place, "��1"))) { TD1_Num++; TD1_Per += p->Movieinfo.Seat_Percent; }
		else if (!(strcmp(p->Movieinfo.place, "��2"))) { TD2_Num++; TD2_Per += p->Movieinfo.Seat_Percent; }
		else if (!(strcmp(p->Movieinfo.place, "��1"))) { VIP_Num++; VIP_Per += p->Movieinfo.Seat_Percent; }
		p = p->next;
	}
	//���������ƽ�������� 
	Nor1_Per /= Nor1_Num; Nor2_Per /= Nor2_Num; Nor3_Per /= Nor3_Num; TD1_Per /= TD1_Num; TD2_Per /= TD2_Num; VIP_Per /= VIP_Num;
	//�������Ļ
	printf("                                   ������ƽ��������ͳ��                                    \n");
	printf("===========================================================================================\n");
	printf("��ͨӰ��1      ��ͨӰ��2        ��ͨӰ��3        3DӰ��1         3DӰ��2            VIPӰ��\n");
	printf(" %3.1f%%          %3.1f%%            %3.1f%%             %3.1f%%           %3.1f%%             %3.1f%%\n",
		Nor1_Per * 100, Nor2_Per * 100, Nor3_Per * 100, TD1_Per * 100, TD2_Per * 100, VIP_Per * 100);
	printf("===========================================================================================\n");
}
/*���е�Ӱ�������ʺ�Ʊ��*/
void Movie_PAS(struct info* head, struct Movie_info* Movie_head)
{
	struct info* p = head;
	struct Movie_info* p0 = Movie_head;
	int Sale_Num = 0, Sale_Money = 0;
	float Movie_Per = 0;
	int cnt = 0, tmp = 0;
	printf("======================================================================\n");
	printf("                           ��ӰƱ��ͳ��                           \n");
	printf("                      (��ǰ����%d��ӰƬ��ӳ)                      \n", Movie_N);
	printf("======================================================================\n");
	printf("��Ӱ��    ��ӳ����      ��ƽ��������       �ܹۿ�����       ��Ʊ��\n");
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
		printf("%s       %2d             %3.1f%%             %-3d(��)        %-6d(Ԫ)\n",
			p0->Movie_Name, p0->Movie_Num, Movie_Per * 100, Sale_Num, Sale_Money);
		Movie_Per = cnt = Sale_Num = Sale_Money = 0; p = head;
		p0 = p0->next;
	}
	printf("======================================================================\n");
}
/*������ӰƬID��ʼ��*/
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
/*�����нṹ���е������Ա��ʼ��*/
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
/*����ӰƬ��Ϣ*/
struct info* Insert(struct info* head)
{
	if (head == NULL)
		printf("No Data in the list!\n");
	else
	{
		int index = 1;/*1��ʾ��ȷ����ӰƬ��Ϣ��0��ʾδ��ȷ����*/
		struct info* p0, * p1, * p2;
		p1 = p2 = head;
		p0 = (struct info*)malloc(sizeof(struct info));
		printf("Input Movie info:\n");
		printf("Date:");
		scanf("%d%d", &p0->Movieinfo.month, &p0->Movieinfo.day);
		int i;
		printf("================================\n");
		printf("=          1.��ͨӰ��1         =\n");
		printf("=          2.��ͨӰ��2         =\n");
		printf("=          3.��ͨӰ��3         =\n");
		printf("=          4.3DӰ��1           =\n");
		printf("=          5.3DӰ��2           =\n");
		printf("=          6.VIPӰ��           =\n");
		printf("================================\n");
		while (1)
		{
			printf("Place:");
			scanf("%d", &i);
			switch (i)
			{
			case 1:strcpy(p0->Movieinfo.place, "��1"); index = 1; break;
			case 2:strcpy(p0->Movieinfo.place, "��2"); index = 1; break;
			case 3:strcpy(p0->Movieinfo.place, "��3"); index = 1; break;
			case 4:strcpy(p0->Movieinfo.place, "��1"); index = 1; break;
			case 5:strcpy(p0->Movieinfo.place, "��2"); index = 1; break;
			case 6:strcpy(p0->Movieinfo.place, "��1"); index = 1; break;
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
		/*�������ӰƬ��Ϣ�������һλ */
		while (p1)
		{
			p2 = p1; p1 = p1->next;
		}
		p2->next = p0; p0->next = NULL; n++;
		p0->id = n; p0->Movieinfo.num = 0;//id,num��ʼ��
		SetColor(15, 12); printf("Successfully Insert!"); SetColor(7, 0); printf("\n");
	}
	return head;
}
/*ɾ��ӰƬ��Ϣ*/
struct info* Del(struct info* head)
{
	if (head == NULL)
		printf("No Data in the list!\n");
	else
	{
		struct info *p0, *bfp0;
		printf("ӰƬ��Ϣ�Ѽ��أ������Ӧ���ɾ��\n");
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
				printf("ӰƬ��Ϣ�Ѽ��أ������Ӧ���ɾ��\n");
				Print(head);
			}
			else
				printf("Error Input!\n");
		}
	}
	return head;
}
/*����Ӱ���Ʋ���ӰƬ��Ϣ*/
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
			printf("%02d��%02d��  %s      %s    %6s       %d\n",
				p0->Movieinfo.month, p0->Movieinfo.day, p0->Movieinfo.place, p0->Movieinfo.session, p0->Movieinfo.name, p0->Movieinfo.price);
		}
		p0 = p0->next;
	}
	SetColor(15, 12); printf("���ҵ�%d����¼", cnt); SetColor(7, 0); printf("\n");
}
/*���������ͳ��*/
void Seat_Percent(struct info* head)
{
	struct info* p = head;
	while (p)
	{
		if (strcmp(p->Movieinfo.place, "��1") == 0 || strcmp(p->Movieinfo.place, "��2") == 0 || strcmp(p->Movieinfo.place, "��3") == 0)
			p->Movieinfo.Seat_Percent = (1.0) * p->Movieinfo.num / Nor_MAX;
		else if (strcmp(p->Movieinfo.place, "��1") == 0 || strcmp(p->Movieinfo.place, "��2") == 0)
			p->Movieinfo.Seat_Percent = (1.0) * p->Movieinfo.num / ThreeD_MAX;
		else if (!(strcmp(p->Movieinfo.place, "��1")))
			p->Movieinfo.Seat_Percent = (1.0) * p->Movieinfo.num / VIP_MAX;
		p = p->next;
	}
}
/*����Ϣ���浽�ĵ�*/
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
		fputs("                                                           ӰƬ��Ϣ     \n", fp);
		fputs("==================================================================================================================================================\n", fp);
		fputs("���     ����         Ӱ��               ����               Ƭ��          Ʊ�ۣ�Ԫ��    ����        ������\n", fp);
		while (p)
		{
			cnt++;
			fprintf(fp, "%02d    %02d��%02d��     %s         %s       %6s         %3d              %3d           %4.1f%%\n",
				cnt, p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num, p->Movieinfo.Seat_Percent * 100);
			p = p->next;
		}
		SetColor(15, 12); printf("Successfully Save to The Doc."); SetColor(7, 0); printf("\n");
		fclose(fp);
	}
}
/*�鿴�汾��Ϣ*/
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

/*�û�����ϵͳ*/
//��ӡ��λ�� 
void Print_Seat(struct info* p, int SUM_Row, int SUM_Col)
{
	int cnt = 1, Row = 0, Col = 0;//��ʼ�� 
	int i, j;
	for (i = 0; i < 2 * SUM_Row + 1; i++)
	{
		for (j = 0; j < SUM_Col; j++)
		{
			if (!(i % 2))	printf("����");
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
//�����Ӱ��Ŵ�ӡ��λ�� 
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
		printf("��������ţ�"); scanf("%d", &N);
		while( N > n )
		{
			printf("��������������������룡\n");
			printf("��������ţ�");scanf("%d",&N);
		 }
		for (int i = 1; i < N; i++)
			p = p->next;
		/*�����������ļ���Ϣ*/
		printf("��ӰԺ����ϢΪ��");
		SetColor(8, 14);
		printf("%02d��%02d��  %s      %s    %6s       %dԪ    %d(��)",
			p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num);
		SetColor(7, 0); printf("\n");
		if (strcmp(p->Movieinfo.place, "��1") == 0 || strcmp(p->Movieinfo.place, "��2") == 0 || strcmp(p->Movieinfo.place, "��3") == 0)
			Print_Seat(p, Nor_Row, Nor_Col);
		else if (strcmp(p->Movieinfo.place, "��1") == 0 || strcmp(p->Movieinfo.place, "��2") == 0)
			Print_Seat(p, TD_Row, TD_Col);
		else if (!strcmp(p->Movieinfo.place, "��1"))
			Print_Seat(p, VIP_Row, VIP_Col);
	}
}
/*�������ֶ�����ϵͳ����*/
int ManSelec(struct info* p, int SUM_Row, int SUM_Col)
{
	/*����ֵ˵����
	����0�����˳���-1������λ�Ѿ���ѡ��1����ѡ���ɹ�*/ 
	int N, MAX, Default;
	int Col = 0, Row = 0;
	/*�жϷ���*/
	if (SUM_Col == Nor_Default) { MAX = Nor_MAX; Default = Nor_Default; }
	else if (SUM_Col == TD_Default) { MAX = ThreeD_MAX; Default = TD_Default; }
	else if (SUM_Col == VIP_Default) { MAX = VIP_MAX; Default = VIP_Default; }
	else { printf("Can't Find the Room!\n"); return 0; }
	printf("��������λ��(0:exit):");
	scanf("%d", &N);
	/*������*/
	while (N < 0 || N > MAX)
	{
		SetColor(4, 14); printf("�������������!\n");SetColor(7, 0);printf("��������λ��(0:exit)��");scanf("%d", &N);
	}
	if( N == 0 )	return 0;
	Col = (N - 1) % Default;
	if (N % Default)	Row = (N / Default);
	else 	Row = (N / Default) - 1;
	if (p->Movieinfo.seat[Row][Col] == 0)
	{
		p->Movieinfo.seat[Row][Col] = 1;
		system("cls");
		printf("������ѡ�����:");
		SetColor(8, 14);
		printf("%02d��%02d��  %s      %s    %6s       %dԪ    %d(��)",
			p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num);
		SetColor(7, 0); printf("\n");
		Print_Seat(p, SUM_Row, SUM_Col);
		SetColor(8, 14); printf("ѡ���ɹ���\n"); SetColor(7, 0); return 1;
	}
	else if (p->Movieinfo.seat[Row][Col] == 1)
	{
		SetColor(4, 14); printf("��λ�ѱ�ѡ�������"); SetColor(7, 0);printf("\n");return -1;
	}
}
/*�û�ѡ��ϵͳ*/
/*�������ֶ�ѡ��ϵͳ����*/
void Man(struct info* head, int Nor_Row, int Nor_Col, int TD_Row, int TD_Col, int VIP_Row, int VIP_Col)
{
	struct info* p = head;
	int i, j, index;
	Print(head);
	int Key;
	while (1)
	{
		printf("�����Ӱ���:");
		scanf("%d", &Key);
		if (Key > 0 && Key <= n)
		{
			while (p)
			{
				if (Key == p->id)
				{
					system("cls");
					printf("������ѡ�����:");
					SetColor(8, 14);
					printf("%02d��%02d��  %s      %s    %6s       %dԪ    %d(��)",
						p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num);
					SetColor(7, 0); printf("\n");
					/*��ʼ��ӡ��λ*/
					if (strcmp(p->Movieinfo.place, "��1") == 0 || strcmp(p->Movieinfo.place, "��2") == 0 || strcmp(p->Movieinfo.place, "��3") == 0)
					{
						index = 1;
						Print_Seat(p, Nor_Row, Nor_Col);
						while (index == 1 || index == -1)
						{
							index =  ManSelec(p, Nor_Row, Nor_Col);	//����ManSelec���������ж��Ƿ�ɹ�ѡ�� 	
							if (index == 1)
							{
								p->Movieinfo.num++;
								system("cls");
								printf("������ѡ�����:");
								SetColor(8, 14);
								printf("%02d��%02d��  %s      %s    %6s       %dԪ    %d(��)",
									p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num);
								SetColor(7, 0); printf("\n");
								Print_Seat(p, Nor_Row, Nor_Col);
							}
						}
					}
					else if (strcmp(p->Movieinfo.place, "��1") == 0 || strcmp(p->Movieinfo.place, "��2") == 0)
					{
						index = 1;
						Print_Seat(p, TD_Row, TD_Col);
						while (index == 1 || index == -1)
						{
							index = ManSelec(p, TD_Row, TD_Col);	//����ManSelec���������ж��Ƿ�ɹ�ѡ�� 
							if (index == 1)
							{
								p->Movieinfo.num++;
								system("cls");
								printf("������ѡ�����:");
								SetColor(8, 14);
								printf("%02d��%02d��  %s      %s    %6s       %dԪ    %d(��)",
									p->Movieinfo.month, p->Movieinfo.day, p->Movieinfo.place, p->Movieinfo.session, p->Movieinfo.name, p->Movieinfo.price, p->Movieinfo.num);
								SetColor(7, 0); printf("\n");
								Print_Seat(p, TD_Row, TD_Col);
							}
						}
					}
					else if (!strcmp(p->Movieinfo.place, "��1"))
					{
						index = 1;
						Print_Seat(p, VIP_Row, VIP_Col);
						while (index == 1 || index == -1)
						{
							index = ManSelec(p, VIP_Row, VIP_Col);	//����ManSelec���������ж��Ƿ�ɹ�ѡ�� 
							if (index == 1)
							{
								p->Movieinfo.num++;
								system("cls");
								printf("������ѡ�����:");
								SetColor(8, 14);
								printf("%02d��%02d��  %s      %s    %6s       %dԪ    %d(��)",
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
		else printf("������Ч,����������!\n");
	}
}
/*�ֶ�������λ���ֽ���*/
/*�������Զ�������λ����*/
void AutoSelec(struct info* p, int SUM_Row, int SUM_Col, int Ran_People)
{
	srand(time(NULL)); // ������������� 
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
	srand(time(NULL)); //�������������
	SetColor(15, 12); printf("�������������λ����\n"); SetColor(7, 0);
	int Ran_People = 0;
	int cnt = 0;
	while (p)
	{
		if (strcmp(p->Movieinfo.place, "��1") == 0 || strcmp(p->Movieinfo.place, "��2") == 0 || strcmp(p->Movieinfo.place, "��3") == 0)
		{
			Ran_People = abs(rand() % (Nor_MAX + 1 - p->Movieinfo.num));
			AutoSelec(p, Nor_Row, Nor_Col, Ran_People);
			p->Movieinfo.num += Ran_People;
		}
		else if (strcmp(p->Movieinfo.place, "��1") == 0 || strcmp(p->Movieinfo.place, "��2") == 0)
		{
			Ran_People = abs(rand() % (ThreeD_MAX + 1 - p->Movieinfo.num));
			AutoSelec(p, TD_Row, TD_Col, Ran_People);
			p->Movieinfo.num += Ran_People;
		}
		else if (!strcmp(p->Movieinfo.place, "��1"))
		{
			Ran_People = abs(rand() % (VIP_MAX + 1 - p->Movieinfo.num));
			AutoSelec(p, VIP_Row, VIP_Col, Ran_People);
			p->Movieinfo.num += Ran_People;
		}

		cnt += Ran_People; p = p->next;
	}
	SetColor(15, 12); printf("����û�������ɣ�������%dλ�û�\n", cnt); SetColor(7, 0); system("pause");
}
/*�Զ�������λ���ֽ���*/
/*�û��˵�*/
void User_Menu(struct info* head, struct Movie_info* Movie_head, int Nor_Row, int Nor_Col, int TD_Row, int TD_Col, int VIP_Row, int VIP_Col)
{
	int i;
	while (1)
	{
		printf("===================================================================\n");
		printf("=                          ��ӭ�����û�ϵͳ                       =\n");
		printf("===================================================================\n");
		printf("=                                                                 =\n");
		printf("=                         0.�������˵�                            =\n");
		printf("=                         1.�Զ�������λ����                      =\n");
		printf("=                            (����ʱʹ��)                         =\n");
		printf("=                         2.�˹���Ʊ                              =\n");
		printf("=                         3.�鿴��λ��Ϣ                          =\n");
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
/*����Ա�˵�*/
/*����Ա��¼ϵͳ*/
//����Ա�ֲ˵���������ͳ�Ʋ˵� 
void DataSta_Menu(struct info* head, struct Movie_info* Movie_head, int Nor_Row, int Nor_Col, int TD_Row, int TD_Col, int VIP_Row, int VIP_Col)
{
	int i;
	while (1)
	{
		printf("================================================================\n");
		printf("=                       ��ӭ����Ʊ��ͳ��ϵͳ                   =\n");
		printf("================================================================\n");
		printf("=                                                              =\n");
		printf("=                        0.���ع���Աϵͳ                      =\n");
		printf("=                        1.�鿴������������                    =\n");
		printf("=                        2.�鿴������������                    =\n");
		printf("=                        3.�鿴��Ӱ�����ʺ�Ʊ��                =\n");
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
//����Ա���˵� 
void Admin_Menu(struct info* head, struct Movie_info* Movie_head, int Nor_Row, int Nor_Col, int TD_Row, int TD_Col, int VIP_Row, int VIP_Col)
{
	int i;
	while (1)
	{
		printf("================================================================\n");
		printf("=                        ��ӭ��������Աϵͳ                    =\n");
		printf("================================================================\n");
		printf("=                                                              =\n");
		printf("=                      0.�˳���¼���������˵�                  =\n");
		printf("=                      1.�鿴��Ӱ��ӳ��Ϣ                      =\n");
		printf("=                      2.�鿴��λ��Ϣ                          =\n");
		printf("=                      3.�����ӳ��Ϣ                          =\n");
		printf("=                      4.���ҷ�ӳ��Ϣ                          =\n");
		printf("=                      5.ɾ����ӳ��Ϣ                          =\n");
		printf("=                      6.����ӳ��Ϣ���浽�ļ�                  =\n");
		printf("=                      7.����Ʊ��ͳ��ϵͳ                      =\n");
		printf("=                      8.�Զ�������λ����                      =\n");
		printf("=                          (����ʱʹ��)                        =\n");
		printf("=                      9.�鿴�汾��Ϣ                          =\n");
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
//����Ա�ֲ˵�������¼ϵͳ���˵�
void Signin_Menu_UI(void)
{
	system("cls");
	printf("========================================\n");
	printf("=       ��ӭʹ�ù���Ա��½ϵͳ          \n");
	printf("========================================\n");
	printf("=             0.�������˵�             =\n");
	printf("=             1.��¼ϵͳ               =\n");
	printf("=             2.�����˺�               =\n");
	printf("=             3.�޸�����               =\n");
	printf("========================================\n");
}
int Signin_Menu(struct User* User_head)
{
	int i;
	void Signin(struct User* User_head);
	void Creat(struct User* head);
	void Change_Pass(struct User* head);
	Signin_Menu_UI();
	SetColor(8, 14); printf("�밴��ѡ�񣬻س�ȷ��:");
	scanf("%d", &i); SetColor(7, 0);
	switch (i)
	{
	case 0:return 0; break;
	case 1:Signin(User_head); system("cls"); break;
	case 2:Creat(User_head); system("cls"); Signin_Menu(User_head); break;
	case 3:Change_Pass(User_head); break;
	default:printf("�����˴�����ַ���\n"); system("pause"); Signin_Menu(User_head); break;
	}
	return 1;
}
//�򵥵��������
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
//�����µ��˺� 
void Creat(struct User* User_head)
{
	char account[20] = { '\0' }, password1[20] = { '\0' }, password2[20] = { '\0' };
	printf("����������˺�:\n");
	scanf("%s", account);
	while (1)
	{
		printf("����������:\n");
		Secretword(password1);
		printf("��������һ�����룺\n");
		Secretword(password2);
		if (!(strcmp(password1, password2)))
		{
			printf("�˺Ŵ����ɹ���\n");
			break;
		}
		else
		{
			printf("�����������벻һ�£����������룡\n");
		}
	}
	User_N++;
	//д���ļ�
	FILE* fp;
	if ((fp = fopen("User_info.txt", "r")) == NULL)
	{
		fp = fopen("User_info.txt", "w");
		fclose(fp);
	}
	fp = fopen("User_info.txt", "a");
	fprintf(fp, "%s    %s\n", account, password1);
	fclose(fp);
	//д������
	struct User* p0, * p1, * p2;
	p1 = p2 = User_head;
	p0 = (struct User*)malloc(sizeof(struct User));
	strcpy(p0->account, account); strcpy(p0->password, password1);
	while (p1)
	{
		p2 = p1; p1 = p1->next;
	}
	p2->next = p0; p0->next = NULL; User_N++;
	SetColor(15, 12); printf("����ɹ����������������һ��!\n"); SetColor(7, 0); getch();
}
//ƥ�����ݿ��е��˺�����
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
//��½����Աϵͳ���� 
void Signin(struct User* User_head)
{
	if (User_N == 0)
	{
		SetColor(15, 12); printf("���ݿ�Ϊ�գ���ѡ������˺�!!\n"); SetColor(7, 0); system("pause"); Signin_Menu(User_head);
	}
	char Input_account[20] = { '\0' }, Input_password[20] = { '\0' };
	int flag; //�����ж��Ƿ��¼�ɹ� 
	printf("�������˺ţ�\n");
	scanf("%s", Input_account);
	printf("���������룺\n");
	Secretword(Input_password);
	flag = Match(Input_account, Input_password);
	if (flag == 1)
	{
		SetColor(15, 12); printf("��½�ɹ�!\n"); SetColor(7, 0);
		system("pause");
	}
	else
	{
		if (flag == -1)
		{
			printf("�������!\n");
			system("pause"); system("cls"); Signin_Menu_UI();
			Signin(User_head);
		}
		if (flag == 0)
		{
			printf("�����ڴ��û�,���½����˻���\n");
			system("pause"); system("cls");
			Signin_Menu(User_head);
		}
	}
}
//�������е����뱣�浽�ļ�
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
//�޸�����
void Change_Pass(struct User* User_head)
{
	if (User_N == 0)
	{
		SetColor(15, 12); printf("���ݿ�Ϊ�գ���ѡ������˺�!!\n"); SetColor(7, 0); system("pause"); Signin_Menu(User_head);
	}
	struct User* p1;
	struct User* p2;
	p2 = p1 = User_head;
	int i;
	char account1[20], password1[20], password2[20], password3[20];
	printf("����������˺�:\n");
	scanf("%s", account1);
	printf("������ԭ����:\n");
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
			SetColor(15, 12); printf("������ȷ!\n"); SetColor(7, 0);
			printf("�������µ�����:\n");
			Secretword(password2);
			printf("��������һ������:\n");
			Secretword(password3);
			if (strcmp(password2, password3) == 0)
			{
				strcpy(p1->password, password2);
				Print_PassWord(User_head);
				break;
			}
			else
			{
				SetColor(15, 12); printf("�����������벻һ�£�����������"); SetColor(7, 0);
			}
		}
		SetColor(15, 12); printf("�����޸ĳɹ���\n"); SetColor(7, 0); system("pause"); Signin_Menu(User_head);
	}
	else
	{
		if (i == -1)
		{
			SetColor(15, 12); printf("�������,����������!\n"); SetColor(7, 0);
			Change_Pass(User_head);
		}
		if (i == 0)
		{
			SetColor(15, 12); printf("�����ڴ��û�,�봴���˺�\n"); SetColor(7, 0);
			system("pause"); system("cls");
			Signin_Menu(User_head);
		}
	}
}
int main()
{
	/*���濪ʼ�����Ӱ���ܵ��к���*/
	int flag = 0;
	//��ͨ��λ 
	int Nor_Row, Nor_Col;
	/*�ж��Ƿ�Ҫ����*/
	if (Nor_MAX % Nor_Default)	flag = 1;
	else	flag = 0;
	Nor_Col = Nor_Default;
	Nor_Row = (Nor_MAX / Nor_Default) + flag;

	//3D��λ
	int TD_Row, TD_Col;
	/*�ж��Ƿ�Ҫ����*/
	if (ThreeD_MAX % TD_Default) flag = 1;
	else	flag = 0;
	TD_Col = TD_Default;
	TD_Row = (ThreeD_MAX / TD_Default) + flag;

	//VIP��λ 
	int VIP_Row, VIP_Col;
	/*�ж��Ƿ���Ҫ����*/
	if (VIP_MAX % VIP_Default) flag = 1;
	else	flag = 0;
	VIP_Col = VIP_Default;
	VIP_Row = (VIP_MAX / VIP_Default) + flag;
	/*Ӱ���ܵ��к��м������*/
	struct info* head;
	struct Movie_info* Movie_head;
	struct User* User_head;
	User_head = LoadUser();
	head = LoadData();
	/*��ʼ��*/
	Initi(head); Sort(head);
	Movie_head = LoadMovie(head);
	/*��������ļ��Ƿ��д���*/
	if(!Check_Input(head))	{printf("�������ݿ��ļ��Ƿ��д���\n");return 0;}
	/*���˵�ѡ��*/
	int i;
	while (1)
	{
		system("title ӰԺ����ϵͳV3.4");
		printf("===============================================\n");
		printf("=           ��ӭ����ӰԺ����ϵͳϵͳ          =\n");
		printf("===============================================\n");
		printf("=                                             =\n");
		printf("=              0.�˳�                         =\n");
		printf("=              1.�����û�ϵͳ                 =\n");
		printf("=              2.�������Աϵͳ               =\n");
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
