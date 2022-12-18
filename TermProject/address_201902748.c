#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE    1
#define FALSE   0

#define ADD 1
#define DELETE 2
#define FIND 3
#define PRINTALL 4
#define QUIT 5

#define NAMEMAX 10
#define PHONEMAX 15

typedef struct Address                  // book에 담길 내용 저장 구조체
{
    char name[NAMEMAX];                 // 이름 저장 배열
    char phone[PHONEMAX];               // 번호 저장 배열
} Address;

typedef struct AddressBlock            // AddressBook을 구성하는 구조체
{
    unsigned size;                     // 블록 내 book 개수
    unsigned max;                      // 최대 book 저장 가능량
    Address* book;                     // Address로 구성된 책들
    struct AddressBlock* next;         // 다음 block 주소
} AddressBlock;

typedef struct AddressBook             // 가장 큰 범위 AddressBook
{
    unsigned size;                     // Block의 개수
    unsigned booknum;                  // 전체 book의 개수
    AddressBlock* head;                // block의 시작 주소
} AddressBook;

void initializeAddressBook(AddressBook* addrbook)                 // 생성자
{
    addrbook->size=0;                                             // 크기를 0으로 초기화
    addrbook->booknum=0;                                          // 전체 book도 0권으로 시작
    addrbook->head=(AddressBlock*)malloc(sizeof(AddressBlock));   // 시작 주소에 AddressBlock만큼 동적 할당
}

Address* findAddressByName(char* name, AddressBook* addrbook)   // name이 addrbook에 있는지 찾는 함수
{
    if(addrbook->size==0) return NULL;                          // 전체 크기가 0이면 찾을 수 없으므로 NULL 반환
    AddressBlock *pre_block=addrbook->head;                     // 탐색하며 확인할 pre_block
    Address *result;                                            // 탐색 결과 위치를 저장할 result                                                     
    char found=FALSE;                                           // 탐색 결과 확인할 found 
    int i;
    do
    {
        for(i=0; i<pre_block->size; i++)                        // 한 블럭 내 권 수 만큼 탐색
        {
            if(strcmp(name, pre_block->book[i].name)==0)        // 이름이 일치하는 book을 발견하면
            {
                result=&(pre_block->book[i]);                   // result에 주소 저장
                found=TRUE;                                     // found를 TRUE로 설정
                break;                                          // 중복값 없고 찾았으니 for문 break
            }
        }
        if(found==TRUE) break;                                  // found가 TRUE이면 찾았으니 while문 break
        pre_block=pre_block->next;                              // 못 찾았다면 다음 블럭으로 이동
    } while (pre_block->next!=NULL);                            // 다음 블럭이 없을때까지 반복한다
    
    if(found==TRUE) return result;                              // 찾았다면 주소값을 반환
    return NULL;                                                // 못 찾았다면 NULL을 반환
}

int addAddress(Address* addr, AddressBook* addrbook)            // addr를 addrbook에 삽입하는 함수
{
    Address *check=findAddressByName(addr->name, addrbook);     // 값이 존재 여부 확인 할 check
    if(check != NULL) return FALSE;                             // check가 NULL이 아니면 값이 존재하므로 FALSE 반환
    
    if(addrbook->size==0 || addrbook->head->size==10)           // 첫 시작이나 head의 책 권 수가 10으로 꽉 찬 경우
    {
        AddressBlock *newBlock=malloc(sizeof(AddressBlock));    // 새로운 블럭을 AddressBlock 크기로 동적 할당
        newBlock->size=0;                                       // 개수는 0으로 설정
        newBlock->max=5;                                        // 용량은 5로 설정
        newBlock->book=(Address*)malloc(sizeof(Address)*5);     // 5권에 맞추어 Address 크기 동적 할당
        newBlock->next=addrbook->head;                          // 새 블럭의 다음 주소를 현재 head로 설정
        addrbook->head=newBlock;                                // head를 새 블럭으로 갱신
        (addrbook->size)++;                                     // 전체 블럭 개수 1 증가
    }
    else if(addrbook->head->size==5){                           // head의 권 수가 5로 꽉 찬 경우 
        addrbook->head->book=(Address*)realloc(addrbook->head->book, 10*sizeof(Address));   // Address의 10 크기로 동적 재할당
        addrbook->head->max=10;                                 // 용량 10으로 갱신
    } 
    addrbook->head->book[addrbook->head->size]=*addr;           // 인덱스에 맞추어 addr 저장
    (addrbook->head->size)++;                                   // head의 권 수 1 증가
    (addrbook->booknum)++;                                      // 전체 권 수 1 증가
    return TRUE;                                                // 삽입 완료
}

void printAll(AddressBook* addrbook)                            // book 전체 내용을 출력하는 함수
{
    AddressBlock* pre_block = addrbook->head;                   // 탐색하며 확인할 pre_block 
    int i;
    while(pre_block!=NULL)                                      // 블럭이 비어 있지 않은 동안 반복
    {
        for(i=0; i<pre_block->size; i++){                       // 블럭의 권 수 만큼 반복
            printf("%s : %s\n", pre_block->book[i].name, pre_block->book[i].phone);     // book 마다 이름과 번호 출력
        }
        pre_block=pre_block->next;                              // 다음 블럭으로 이동
    }
}

AddressBlock* findMergingTarget(AddressBook* addrbook, AddressBlock* source)    // source를 병합할 블럭을 찾는 함수
{
    AddressBlock* pre_block = addrbook->head;                                   // 탐색하며 확인할 pre_block
    while (pre_block != NULL)                                                   // 블럭이 비어 있지 않은 동안 반복
    {
        if((pre_block->size)+(source->size)<=10 && pre_block->max==5)           // 블럭의 권 수와 source의 권 수가 10권을 이하에 블럭의 용량이 5인 경우
        {
            if(pre_block->next != source->next)                                 // 이 때 병합하려는 곳과 병합되는 곳이 같으면 안된다
            {
                pre_block->book=(Address*)realloc(pre_block->book, 10*sizeof(Address));     // 병합하려는 곳을 Address의 10 크기로 동적 재할당
                pre_block->max=10;                                              // 블럭의 용량 10으로 갱신
                break;                                                          // 위치를 찾았으니 break
            }
        }
        pre_block=pre_block->next;                                              // 다음 블럭으로 이동
    }
    return pre_block;                                                           // 병합하려는 곳 반환 없다면 NULL 반환
}

int deleteAddressByName(char* name, AddressBook* addrbook)                      // name을 찾아 삭제하는 함수
{   
    if(addrbook->size==0) return FALSE;                                         // 전체에 아무 것도 없으면 FALSE 반환
    if(findAddressByName(name, addrbook)==NULL) return FALSE;                   // 찾아 봐도 없으면 FALSE 반환
    
    AddressBlock *pre_block = addrbook->head;                                   // 탐색하며 확인할 pre_block
    int i,j;
    char deleted=FALSE;                                                         // 삭제 결과 확인할 deleted 
    while(pre_block != NULL)                                                    // 블럭이 비어 있지 않은 동안 반복
    {
        for(i=0; i<pre_block->size; i++)                                        // 블럭의 권 수 만큼 반복
        {
            if(strcmp(name, pre_block->book[i].name)==0)                        // 삭제하려는 책을 찾았다면
            {
                for(j=i; j<pre_block->size; j++) {pre_block->book[j]=pre_block->book[j+1];} // 책을 한 권 씩 앞으로 당겨 삭제
                pre_block->size--;                                              // 블럭의 권 수 1 감소
                addrbook->booknum--;                                            // 전체 권 수 1 감소
                deleted=TRUE;                                                   // deleted를 TRUE로 설정
                break;                                                          // 삭제 성공했으니 for문 break
            }
        }
        if(deleted==TRUE) break;                                                // 삭제 성공했다면 while break
        else pre_block=pre_block->next;                                         // 못 했다면 다음 블럭으로 이동
    }
 
    if (deleted==FALSE) return FALSE;                                           // 삭제 실패의 경우 FALSE 반환
    else{                                                                       // 삭제가 성공한 경우
        if(pre_block->size==0)                                                  // 삭제 후 블럭이 비게 되면
        {
            AddressBlock *tmp_block = addrbook->head;                           // 탐색하며 확인할 tmp_block
            if(tmp_block==pre_block)                                            // pre_block이 head인 경우
            {
                addrbook->head=addrbook->head->next;                            // head를 다음 블럭으로 설정
                addrbook->size--;                                               // 블럭 개수 1 감소
                return TRUE;                                                    // TRUE 반환하며 종료
            }
            while(tmp_block->next!=pre_block){tmp_block=tmp_block->next;}       // pre_block이 head가 아닌 경우 다음 주소가 pre_block일 때까지 이동
            tmp_block->next=pre_block->next;                                    // tmp_block의 다음 주소를 다다음 블럭으로 설정
            free(pre_block->book);                                              // pre_block의 book을 free
            free(pre_block);                                                    // pre_block 자체 free
            addrbook->size--;                                                   // 블럭 개수 1 감소
            return TRUE;                                                        // TRUE 반환하며 종료
        }
        else if(pre_block->size<5)                                              // 블럭 내 권 수가 5 미만이 된 경우
        {
            pre_block->max=5;                                                   // 용량을 5로 재설정
            AddressBlock *merge_block=findMergingTarget(addrbook, pre_block);   // 병합할 곳 merge_block
            if(merge_block==NULL) return TRUE;                                  // 병합할 곳이 없으면 삭제만 된 채 TRUE 반환
            else                                                                // 병합할 곳이 있으면
            {
                i=0;
                while(i<pre_block->size)                                        // pre_block의 권 수만큼 반복
                {
                    merge_block->book[(merge_block->size)+i]=pre_block->book[i];    // merge_block으로 book을 옮긴다
                    i++;                                                        // 옮긴 권 수 count
                }
                merge_block->size+=i;                                           // i만큼 merge_block 권 수 증가
                
                if(pre_block==addrbook->head){addrbook->head=addrbook->head->next;} // pre_block이 head라면 head를 다음 블럭으로 설정
                else                                                            // pre_block이 head가 아니라면
                {
                    AddressBlock *tmp=addrbook->head;                           // 탐색하며 확인할 tmp
                    while(tmp->next!=pre_block){tmp=tmp->next;}                 // 다음 주소가 pre_block일 때까지 이동
                    tmp->next=tmp->next->next;                                  // tmp의 다음 주소를 다다음 블럭으로 설정
                }
                addrbook->size--;                                               // 블럭 개수 1 감소
                free(pre_block->book);                                          // pre_block의 book을 free
                free(pre_block);                                                // pre_block 자체 free
                return TRUE;                                                    // TRUE 반환하며 종료
            }
        }
        return TRUE;                                                            // 특이사항 없어도 삭제되었다면 TRUE 반환
    }
}

void finalizeAddressBook(AddressBook* addrbook)         // 소멸자
{
    if(addrbook->booknum==0)                            // 전체가 비어 있다면
    {
        free(addrbook->head);                           // head만 free
        return;
    }   
    AddressBlock* pre_block = addrbook->head;           // 탐색하며 확인할 pre_block
    AddressBlock* tmp_block;                            // 임시 저장용인 tmp_block
    while (pre_block != NULL)                           // 블럭이 비어 있지 않은 동안 반복
    {
        free(pre_block->book);                          // pre_block의 book부터 free
        tmp_block = pre_block->next;                    // tmp_block에 다음 블럭 저장
        free(pre_block);                                // pre_block을 free
        pre_block = tmp_block;                          // pre_block에 다시 tmp_block 복구
    }
}

int main_menu()                                             // 동작 선택 입력을 받는 함수
{
    char str[50];                                           // 입력 받을 배열
    int menu = 0, i;                                        // 입력을 반환할 menu
    do {
        printf("1. Add an address\n");
        printf("2. Delete an address\n");
        printf("3. Find an address\n");
        printf("4. Print all\n");
        printf("5. Quit\n");
        printf("Select: ");
        scanf("%s", str);
        
        for(i=0; str[i]; i++){                              // 정상 입력 확인
            if(str[i]>='0' && str[i]<='9') menu=menu*10+str[i]-'0';     // 한 자리 정수 확인
            else{                                           // 아닐 경우 재입력 요구
                printf("Wrong Input. Try Again Please.\n\n");
                break;                                      // 확인 중지
            } 
        }
    } while (menu<1 || menu>5);                             // 1~5 사이의 값이 들어오도록

    return menu;                                            // menu 반환
}

void name_menu(char* name)                                  // 이름 입력 함수
{
    printf("Type a name: ");
    scanf("%s", name);
}

void add_menu(Address* addr)                                // 이름과 번호 추가 시 입력 함수
{
    printf("Type a name: ");
    scanf("%s", addr->name);                                // 이름 입력
    printf("Type a phone number: ");
    scanf("%s", addr->phone);                               // 번호 입력
}

int main()                                                  // main 문
{
    int menu = 0;
    AddressBook addrbook;
    Address addr;
    char name[10];

    initializeAddressBook(&addrbook);                       // 생성자
    do {
        menu = main_menu();                                 // menu 선택
        printf("\n");

        switch (menu) {
        case ADD:                                           // 삽입하려는 경우(1번)
            add_menu(&addr);                                // addr에 이름과 번호 입력 받아 저장
            if (addAddress(&addr, &addrbook)) printf("%s is added successfully!\n", addr.name); // 삽입 성공 시
            else printf("%s already exists in the book\n", addr.name);  // 삽입 실패 시
            break;
        case DELETE:                                        // 삭제하려는 경우(2번)
            name_menu(name);                                // name에 삭제하려는 이름 입력 받아 저장
            if (deleteAddressByName(name, &addrbook)) printf("%s is deleted\n", name);  // 삭제 성공 시
            else printf("%s does not exist in the book\n", name);   // 삭제 실패 시
            break;
        case FIND:                                              // 탐색하려는 경우(3번)
            name_menu(name);                                    // name에 탐색하려는 이름 입력 받아 저장
            Address* p = findAddressByName(name, &addrbook);    // findAddressByName으로 탐색한 주소 p 
            if (p == NULL) printf("%s is not found\n", name);   // p가 NULL이면 못 찾은 경우
            else printf("%s : %s\n", name, p->phone);           // 그렇지 않으면 탐색 결과 출력
            break;
        case PRINTALL:                                          // 전체 출력의 경우(4번)
            printAll(&addrbook);                                // addrbook 내 전체 출력
            break;
        }

        if (menu != QUIT) {                                     // 종료하지 않는 경우(1~4번)
            printf("\n");
            printf("---------------------------Providing My AddressBook---------------------------------------\n");
            printf("(Address Book Size: %d)\n\n", addrbook.booknum);                // 전체 책 권 수 출력
            for (AddressBlock* p = addrbook.head; p->next != NULL; p = p->next)     
                printf("block size : %d\n", p->size);                               // 각 블럭 속 책 권 수  출력
            printf("------------------------------------------------------------------------------------------\n");
            printf("\n");
        }         
           
    } while (menu != QUIT);     // 종료(5번)하지 않는 동안 반복
    finalizeAddressBook(&addrbook); // 소멸자
    return 0;
}