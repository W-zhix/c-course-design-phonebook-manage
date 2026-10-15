#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NAMESIZE 20

typedef struct PhoneBook
{
	int userid;             //联系人编号
	char name[NAMESIZE];	//姓名
	char firstphone[20];	//手机号1
	char secondphone[20];	//手机号2
}PhoneBook;

typedef struct Node
{
	PhoneBook phonebook;	//值域
	struct Node* next;		//后继
}Node;

Node* head; //设计头指针

int maxuserid = 0;  //联系人编号最大值

//1.初始化链表、文件
void Init(const char* filename)
{
	head = (Node*)malloc(sizeof(Node));
	if (head == NULL) return;
	head->next = NULL;
	//读取文件信息 初始化 头插法存储到单链表
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) return;
	while (fgetc(fp) != EOF)
	{
		fseek(fp, -1, SEEK_CUR);
		Node* p = (Node*)malloc(sizeof(Node));
		if (p == NULL) return;
		fscanf(fp, "%d %s %s %s\n", &p->phonebook.userid, p->phonebook.name, p->phonebook.firstphone,
			p->phonebook.secondphone);
		if (p->phonebook.userid > maxuserid)
			maxuserid = p->phonebook.userid;
		p->next = head->next;
		head->next = p;
	}
	fclose(fp);
}
//2.插入信息
void Insert()
{
	Node* p = (Node*)malloc(sizeof(Node));
	if (p == NULL)
	{
		printf("内存申请失败\n");
		return;
	}
	p->phonebook.userid = ++maxuserid; //联系人编号
	printf("请输入联系人姓名:");
	scanf("%s", p->phonebook.name);
	printf("请输入手机号1:");
	scanf("%s", p->phonebook.firstphone);
	printf("请输入手机号2(若无则填null):");
	scanf("%s", p->phonebook.secondphone);
	p->next = head->next;
	head->next = p;
	printf("插入成功,该联系人的编号为%d\n", p->phonebook.userid);
}
//3.删除
void Delete(const char* name)
{
	Node* q = NULL;
	for (Node* p = head;p->next != NULL;p = p->next)
	{
		if (strcmp(p->next->phonebook.name, name) == 0)
		{
			char flag[4] = { 0 };
			printf("确定删除该联系人吗?(yes/no): ");
			scanf("%s", flag);
			if (strcmp(flag, "yes") == 0 || strcmp(flag, "是") == 0)
			{
				q = p->next;
				p->next = p->next->next;
				free(q);
				printf("删除成功\n");
			}
			else
			{
				printf("取消删除\n");
			}
			return;
		}
	}
	printf("不存在该联系人\n");
}
//4.展示所有联系人信息
void Show()
{
	int Total = 0, total = 0;   //Total：联系人数量，total：存入的手机号数量
	int onePhone = 0;  //仅有一个手机号的联系人
	printf("\n================ 电话簿信息 ================\n");
	printf("%-12s %-10s %-18s %-18s\n", "联系人编号", "姓名", "手机号1", "手机号2");
	for (Node* p = head->next;p != NULL;p = p->next)
	{
		printf("%-12d %-10s %-18s %-18s\n", p->phonebook.userid, p->phonebook.name,
			p->phonebook.firstphone, p->phonebook.secondphone);
		Total++;
		total++;
		if (strcmp(p->phonebook.secondphone, "null") != 0)
		{
			total++;
		}
		if (strcmp(p->phonebook.firstphone, "null") != 0
			&& strcmp(p->phonebook.secondphone, "null") == 0)
		{
			onePhone++;
		}
	}
	printf("\n================ 统计信息 ================\n");
	printf("     联系人总数: %d 人\n", Total);
	printf("     手机号总数: %d 个\n", total);
	printf("     已输出信息: %d 条\n", Total);
	printf("     只含一个号码的人数: %d 人\n", onePhone);
	printf("============================================\n");
}
//5.按姓名查询联系人信息
void ShowByName(const char* name)
{
	int flag = 0;
	for (Node* p = head->next;p != NULL;p = p->next)
	{
		if (strcmp(p->phonebook.name, name) == 0)
		{
			printf("%-12s %-10s %-18s %-18s\n", "联系人编号", "姓名", "手机号1", "手机号2");
			printf("%-12d %-10s %-18s %-18s\n", p->phonebook.userid, p->phonebook.name, p->phonebook.firstphone,
				p->phonebook.secondphone);
			flag = 1;
		}
	}
	if (flag == 0)
		printf("未找到此联系人\n");
}
//6.按手机号查询联系人信息
void ShowByNumber(const char* phone)
{
	int flag = 0;
	for (Node* p = head->next;p != NULL;p = p->next)
	{
		if (strcmp(p->phonebook.firstphone, phone) == 0 || strcmp(p->phonebook.secondphone, phone) == 0)
		{
			printf("%-12s %-10s %-18s %-18s\n", "联系人编号", "姓名", "手机号1", "手机号2");
			printf("%-12d %-10s %-18s %-18s\n", p->phonebook.userid, p->phonebook.name,
				p->phonebook.firstphone, p->phonebook.secondphone);
			flag = 1;
		}
	}
	if (flag == 0)
		printf("未找到该手机号对应的联系人\n");
}
//7.修改（按联系人编号修改）
void Change(int userid)
{
	int choice, flag = 0;
	char newphone[20] = { 0 };
	char newname[20] = { 0 };
	for (Node* p = head->next;p != NULL;p = p->next)
	{
		if (p->phonebook.userid == userid)
		{
			flag = 1;
			printf("\n========== 当前联系人信息 ==========\n");
			printf("  姓名: %s\n", p->phonebook.name);
			printf("  当前手机号1：%s\n", p->phonebook.firstphone);
			printf("  当前手机号2：%s\n", p->phonebook.secondphone);
			printf("--------------------------------\n");
			printf("  1. 修改手机号1\n");
			printf("  2. 修改手机号2\n");
			printf("  3. 修改姓名\n");
			printf("--------------------------------\n");
			printf("请选择（1-3）：");
			scanf("%d", &choice);
			if (choice == 0)
			{
				break;
			}
			switch (choice)
			{
			case 1:
				printf("请输入新的手机号:");
				scanf("%19s", newphone);
				strcpy(p->phonebook.firstphone, newphone);
				break;
			case 2:
				printf("请输入新的手机号:");
				scanf("%19s", newphone);
				strcpy(p->phonebook.secondphone, newphone);
				break;
			case 3:
				printf("请输入新的姓名:");
				scanf("%s", newname);
				strcpy(p->phonebook.name, newname);
				break;
			default:
				break;
			}
			break;
		}
	}
	if (flag)
		printf("修改成功\n");
	else
		printf("不存在编号为%d的联系人\n", userid);
}
//8.保存文件
void SaveFile(const char* filename)
{
	FILE* fp = fopen(filename, "w");
	if (fp == NULL)
	{
		printf("文件操作失败\n");
		return;
	}
	for (Node* p = head->next;p != NULL;p = p->next)
	{
		fprintf(fp, "%d %s %s %s\n", p->phonebook.userid, p->phonebook.name,
			p->phonebook.firstphone, p->phonebook.secondphone);
	}
	printf("文件操作成功\n");
	fclose(fp);
}
//9.释放堆区内存空间 free
void Destroy()
{
	while (head->next != NULL)
	{
		Node* p = head->next;
		head->next = head->next->next;
		free(p);
	}
	free(head);
}
//10.菜单函数Menu
int Menu()
{
	int choice;
	printf("\n================================================\n");
	printf("                 电话簿管理系统\n");
	printf("================================================\n");
	printf("  1. 插入联系人信息        2. 删除联系人信息\n");
	printf("  3. 修改联系人信息        4. 按姓名查找联系人\n");
	printf("  5. 按手机号查找联系人    6. 保存文件\n");
	printf("  7. 展示全部联系人信息\n");
	printf("------------------------------------------------\n");
	printf("  0. 退出系统\n");
	printf("================================================\n");
	printf("\n请输入您的选择: ");
	scanf("%d", &choice);
	return choice;
}
//主函数调用
int main()
{
	char name[NAMESIZE];
	int userid;
	char phone[20] = { 0 };
	Init("telephone.txt");
	while (1)
	{
		int choice = Menu();
		if (choice == 0)
		{
			printf("\n感谢使用电话簿管理系统\n");
			break;
		}
		switch (choice)
		{
		case 1:
			printf("请按要求输入....\n");
			Insert();
			break;
		case 2:
			printf("请输入待删除的联系人姓名:");
			scanf("%s", name);
			Delete(name);
			break;
		case 3:
			printf("请输入联系人编号:");
			scanf("%d", &userid);
			Change(userid);
			break;
		case 4:
			printf("请输入要查询的联系人姓名:");
			scanf("%s", name);
			ShowByName(name);
			break;
		case 5:
			printf("请输入要查询的手机号:");
			scanf("%19s", phone);
			ShowByNumber(phone);
			break;
		case 6:
			SaveFile("telephone.txt");
			break;
		case 7:
			printf("\n");
			Show();
			break;
		default:
			break;
		}
		system("pause");
		system("cls");
	}
	Destroy();
	return 0;
}