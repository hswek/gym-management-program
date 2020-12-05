#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//구조체 선언
typedef struct node
{
	char name[20]; //이름
	char number[20]; //연락처
	int remain; //남은 기간
	int locker; //개인 사물함번호
	struct node* next; //다음 노드의 정보
} node;

//구조체 포인터 선언
node* new = NULL;
node* buf=NULL; 
node* head = NULL;
node* last = NULL;
node* previous=NULL;
node* nextnode=NULL;

//임시 버퍼 선언
int remainbuf;
char namebuf[20];
char numberbuf[20];
char charbuf[20];
int lockerbuf;

// 함수 선언
void withdrawal(void (*func_delete)(char*));
void join(void (*func_add)(char*,char*,int,int,node*(*func)(char*)),node*(*func_searchlocker)(int));
void extend(node* (*func_searchname)(char*));
void save(node *head);
void clear();
void daygo(node *head);
void printone(node*(*func)(char*));
void modify(node*(*func_searchname)(char*));
void add(char *name, char *number,int remain,int lockerbuf,node *(*func_searchname)(char*));
void printall(node *head);
void delete(char *charbuf);
void update(node *head,void(*func_add)(char*,char*,int,int,node*(char*)));
node *searchname();
node *searchlocker(int lokcerbuf);

//메인함수 시작
void main()
{
	int num;
	//메뉴 반복 시작
	while(num!=11)
	{
		printf("--------------------------------\n");
		printf("헬스장 회원 관리 프로그램임니다.\n");
		printf("기능을 선택하여 주세요\n\n");
		printf("1. 회원 가입\n");
		printf("2. 회원 탈퇴\n");
		printf("3. 회원 연락처 변경\n");
		printf("4. 회원  기간 연장\n");
		printf("5. 회원  검색\n");
		printf("6. 전체 회원 정보 출력\n");
		printf("7. 24시간 경과\n");
		printf("8. 초기화\n");
		printf("9. 파일에 저장\n");
		printf("10. 파일 불러오기\n");
		printf("11. 종료\n");
		printf("--------------------------------\n");
		printf("입력: ");
		scanf("%d", &num);
		switch (num)
		{
			case 1:
				join(add,searchlocker);
				break;
			case 2:
				withdrawal(delete);
				break;
			case 3:
				modify(searchname);
				break;
			case 4:
				extend(searchname);
				break;
			case 5:
				printone(searchname);
				break;
			case 6:
				printall(head);
				break;
			case 7:
				daygo(head);
				break;
			case 8:
				clear();
				break;
			case 9:
				save(head);
				break;
			case 10:
				update(head,add);
				break;
			default:
				printf("잘못된 입력입니다.\n\n");
				break;
		}
		//메뉴종료	
		if (num==11)
			printf("프로그램을 종료합니다");
	}
}	
//회원가입기능 문자열입출력 함수
void join(void(*func_add)(char*,char*,int,int,node* (*func_searchname)(char*)),node *(*func_searchlocker)(int))
{
	//구조체의 정보를 입력받음
	printf("이름:");
    scanf("%s",namebuf);
    printf("번호:");
    scanf("%s",numberbuf);
    printf("등록할 기간(day):");
    scanf("%d",&remainbuf);
	//등록 기간은 양수여야한다
    while (remainbuf <=0)
    {
   		printf("등록할 기간(day):");
        scanf("%d",&remainbuf);
        if(remainbuf<=0)
        printf("1 이상의 정수를 입력해주세요");
     }
     printf("개인 사물함 번호를 입력하십시오( 1~100)");
     scanf("%d",&lockerbuf);
	//개인 사물함은 중복불가능이다
     buf=func_searchlocker(lockerbuf);
     while (buf!=NULL && lockerbuf<=100&&lockerbuf>=1)
     { 
		if (buf!=NULL)
        	printf("이미 사용중인 사물함 입니다. 다시 입력해주세요");
        if (lockerbuf>100 || lockerbuf<0)
			printf("1에서 100 사이의 숫자를 입력해 주세요");
		scanf("%d",&lockerbuf);
		//중복되는 사물함이 있는지 체크한다
        buf=func_searchlocker(lockerbuf); 
     }
	//입력받은 문자열을 구조체 수정 함수로 넣어준다
     func_add(namebuf,numberbuf,remainbuf,lockerbuf,searchname);
}
//회원 가입 구조체 추가 함수
void add(char *namebuf, char *numberbuf,int remainbuf,int lockerbuf,node*(*func_searchname)(char*)) 
{	
	int addbuf=1;
	buf=func_searchname(namebuf);
	//만약 이미 추가된 회원이라면 추가하지 않고 넘어간다
	if (buf!=NULL)
	{
		if (strcmp(buf->number,numberbuf)==0)
		{
			printf("이미 존재하는 회원입니다\n");
			addbuf=0;
		}
	}
	//실질적인 추가 부분
	if (addbuf==1)
	{
		new=(node*)malloc(sizeof(node));
		new->next = NULL;
		strcpy(new->name,namebuf);
		strcpy(new->number, numberbuf);
		new->remain=remainbuf;
		new->locker=lockerbuf;
		printf("%s씨가 추가되었습니다\n",namebuf);
		//만약 구조체의 개수가 0이라면
		if (head == NULL)
			head=new;
		//구조체가 이미 존재한다면
		else
			last->next=new;
		last=new;
	}
}
//회원 삭제 문자열 입출력함수
void withdrawal(void (*func_delete)(char*))
{	
	printf("삭제하실 이름을 입력하여 주세요.\n\n");
    scanf("%s",charbuf);
	//문자열을 입력받아 구조체수정 함수로 넘겨준다
    func_delete(charbuf);
}
//회원 삭제 
void delete(char *charbuf)
{
	//오류 방ㄹ생을 최소하하기 위해 경우의 수를 나누어 처리한다
	if (head==NULL)
		printf("잘못된 입력입니다.\n\n");
	else if (strcmp(head->name,charbuf)==0)
		{
		node *remove=head;
		head=head->next;
		free(remove);
		printf("삭제 완료되었습니다.\n");
		}
	else
	{
		buf=head;
		//전체노드를 순환하며, 정해진 노드를 찾는다
		while (buf!=NULL)
		{
			if (strcmp(buf->name,charbuf)==0)
			{	
				previous->next=buf->next;
				free(buf);
				printf("삭제 완료되었습니다.\n");
				break;
			}
			previous=buf;
			buf=buf->next;
		}
		//찾는 노드가 없을시
		if (buf==NULL)
	        printf("잘못된 입력입니다.\n\n");
	return;
	}
}		 
//입력받은 이름과 같은 노드를 리턴한다
node *searchname(char *name)
{
	buf=head;
	//반복문을 통해 노드를 찾는다
	while (buf!=NULL)
		{
		if (strcmp(buf->name,name)==0) break;
        buf=buf->next;
		}     
	return buf; //노드를 리턴한다
}
//입력받은 이름과 같은 사물함 노드를 찾는다
node *searchlocker(int lokcerbuf)
{
	buf=head;
	//반복문을 통해 노드를 찾는다
	while (buf!=NULL)
		{
		if (lockerbuf==buf->locker) break;
		buf=buf->next;
		}
	return buf; //노드를 리턴한다
}
//입력받은 이름과 같은 노드 정보 프린트
void printone(node* (*func_searchname)(char*))
{
	//출력할 노드의 이름을 입력받는다
	printf("이름을 입력하여 주십시오:");
	scanf("%s",charbuf);
	//노드를 찾는다
	buf=func_searchname(charbuf);
	if (buf==NULL)
		printf("검색하신 회원은 없는 사람입니다.\n\n");
	else
		{
		//노드의 정보를 출력한다
		printf("검색된 번호는 %s입니다.\n",buf->number);
		printf("남은 기간은 %d일입니다.\n",buf->remain);
		printf("개인 사물함 번호는 %d입니다.\n",buf->locker);
		}
}
//노드의 연락처 정보  수정
void modify(node*(*func_searchname)(char*))
{
	//변경할 노드의 이름을 입력받는다
	printf("변경하실 회원 성함을 입력해주세요:");
	scanf("%s",charbuf);
	//노드를 검색 및 리턴한다
	buf=func_searchname(charbuf);
	if (buf==NULL)
	{
		printf("존재하지 않는 회원입니다\n");
		return;
	}
	printf("\n변경 전  번호는 %s입니다.\n",buf->number);
	printf("변경 후 번호를 입력하여 주세요:"); 
	scanf("%s",charbuf);
	//노드의 데이터를 수정한다
	strcpy(buf->number,charbuf);
	printf("변경 완료되었습니다.\n");
	printf("변경 후 번호:%s.\n",buf->number);
}
//구조체의 remain요소 수정
void extend(node *(*func_searchname)(char*))
{
	//수정할 노드의 정보를 입력한다
	 printf("변경하실 회원 성함을 입력해주세요:");
     scanf("%s",charbuf);
	 //전체 노드를 순환하며, 노드를 리턴한다
     buf=func_searchname(charbuf);
 	 if (buf==NULL)
     {
         printf("존재하지 않는 회원입니다\n");
         return;
     }
     printf("\n변경 전 기간은 % d입니다.\n\n",buf->remain);
     printf("연장하실 기간을  입력하여 주세요:");
     scanf("%d",&remainbuf);
	 //노드의 정보를 수정한다
     buf->remain+=remainbuf;
     printf("변경 완료되었습니다.\n");
     printf("변경 후 기간:%d.\n",buf->remain);
}
	
//모든 노드를 순환하며 노드의 모든 요소 출력 
void printall(node *head)
{	
	node *print=head;
	printf("\n");
	//만약 노드가 NULL이라면
	if (head=NULL)
		printf("리스트가 비어있습니다.\n\n");
	//전체 노드를 순환하며, 노드정보를 출력한다
	while (print!=NULL)
	{
		printf("이름 : %s\n",print->name);
		printf("전화번호 : %s\n",print->number);
		printf("남은 기간: %d\n",print->remain);
		printf("사물함 번호:%d\n\n",print->locker);
		print=print->next;
	}
}
//노드 초기화
void clear()
{
    buf=head;
	//전체 노드를 순환하며, 메모리 할당을 해제한다
    while(buf!=NULL)
    {
        previous=buf;
        buf=buf->next;
        free(previous);
    }
	//구조체 포인터도 초기화시킨다
    new = NULL;
    buf=NULL;
    head = NULL;
    last = NULL;
    previous=NULL;
    printf("초기화가 완료되었습니다\n");
}
//노드를 파일 안에 저장
void save(node *head)
{
	int write=1;
	char filebuf[20];
	node* tmp;
	//파일이름을 입력받는다
	printf("저장 하실 파일 명을 입력해주세요:");
	scanf("%s",charbuf);
	FILE* fp=fopen(charbuf,"a+");
	//오류 발생시
	if (fp==NULL)
		{
		printf("파일을 여는데 실패하였습니다.\n");
		return;
		}
	//파일이 이미 존재할경우, 덮어쓸지 이어쓸지 입력받는다
	if (fgets(filebuf,20,fp)!=NULL)
	{
		printf("파일이 이미 존재합니다.\n");
		printf("덮어쓰기:1\n이어쓰기:2\n입력:");
		scanf("%d",&write);
	}
	//덮어쓸경우
	if (write==1)	
	{
		fclose(fp);
		fp=fopen(charbuf,"w+");	
		//구조체를 순환하며, 노드의 정보를 파일에 입력한다
		while(head)
		{
			tmp=head;
			fprintf(fp,"%s#%s#%d#%d\n",tmp->name,tmp->number,tmp->remain,tmp->locker);
			head=head->next;
		}
		//파일을 닫는다
		fclose(fp);
	}
	//이어쓸경우
	if (write==2)
	{
		fclose(fp);
		fp=fopen(charbuf,"a+");
		//전체 노드를 순환하며, 노드의 데이터를 입력한다
        while(head)
        {
            tmp=head;
            fprintf(fp,"%s#%s#%d#%d\n",tmp->name,tmp->number,tmp->remain,tmp->locker);
            head=head->next;
        }
        fclose(fp);
    }
	printf("파일이 저장되었습니다.\n");
}
//파일안의 데이터를 구조체로 변환
void update(node *head,void (*func_add)(char*,char*,int,int,node*(char*)))
{
	//임시 버퍼 선언 
	char charbuf[80];
	char* token;
	char namebuf[20];
	char numberbuf[20];
	int remainbuf;
	int lockerbuf;
	//불러올 파일의 정보를 입력받는다
	printf("불러오실 파일 이름을 입력해주세요:");
	scanf("%s",charbuf);
	FILE *fp=fopen(charbuf,"r");
	if (fp==NULL)
	{
		printf(	"존재하지 않는 파일 명 입니다.\n");
		return;
	}
	//ㅇㅇ
	while(fgets(charbuf,80,fp)!=NULL)
	{
		//이름 복사 밑 삽입
		token=strtok(charbuf,"#");
		strcpy(namebuf,token);
		//연락처 복사 밑 삽입
		token=strtok(NULL,"#");
		strcpy(numberbuf,token);
		//남은 기간 복사 및 삽입
		token=strtok(NULL,"#");
		remainbuf=atoi(token);
		// 사물함번호 복사 및 삽입
		token=strtok(NULL,"#");
		lockerbuf=atoi(token);
		//구조체 추가
		func_add(namebuf,numberbuf,remainbuf,lockerbuf, searchname);
	}
	fclose(fp);
}
//전체 구조체의 remain요소를 1씩 감소
void daygo(node *head)
{
	int i=0;
	char deletelist[99][20];
	node *go=head;
    printf("\n");
    if (head=NULL)
        printf("리스트가 비어있습니다.\n\n");
	//만약 기간이 다 소진되었을경우
    while (go!=NULL)
    {
       	go->remain-=1;
		if (go->remain==0)
			{
			printf("%s씨의 회원 기간이 만료되었습니다.\n",go->name);
			strcpy(deletelist[i],go->name);
			i+=1;
			}
		go=go->next;
    }
	//소진된 회원을 삭제한다
	for(int n=0; n<i; n++)
		{
			printf("%s",deletelist[n]);
			delete(deletelist[n]);
		}
	printf("\n회원 정보가 새로고침 되었습니다.\n");
}
