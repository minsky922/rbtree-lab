#include <stdio.h>
#include <stdlib.h>

#pragma warning (disable : 4996)

typedef struct node
{
    int value; //값
    struct node* next; // 다음 노드의 주소를 저장하는 포인터 편수(8byte)
}node; //연결 리스트의 저장 단위

node* head = NULL;

//O(1) : 상수 시간의 복잡도 : 생성된 노드의 개수와 상관 없이 일정한 횟수의 연산으로 문제 해결
void insertNodeFront(int data)
{
    //newNode포인터 : 새로 생성된 구조체 변수(노드)의 주소를 저장(8byte)
    node* newNode = (node*)malloc(sizeof(node)); //노드 생성
    newNode->value = data;
    newNode->next = NULL;

    if (head == NULL) //아직 연결 된 노드가 없다면?
    {
        head = newNode;//새로 생성된 노드의 주소를 head에 저장
        printf("\n\n\t\t노드 맨 앞 삽입 : 첫 노드로 연결\n");
        return; //함수 종료
    }

    //맨 앞에 노드를 추가
    newNode->next = head;
    head = newNode;
    printf("\n\n\t\t노드 맨 앞 삽입 : 일반적인 경우\n");
}

void displayNode() //O(N) : 노드의 개수만큼 반복 : 순회 코드는 무조건 O(N)
{
    node* curNode; //방문 노드의 주소를 저장하는 포인터(8바이트)

    if (head == NULL) //아직 연결 된 노드가 없다면?
    {
        print("\n\n\t\t연결리스트가 구성 되지 않아 출력 할 노드가 없습니다.\n");
        return;
    }
    printf("\n\nSingly Linked List Display : ");

    //head포인터 : 첫 노드의 주소를 저장하는 포인터
    curNode = head; // 첫 노드의 주소를 저장
    while (curNode->next != NULL) //방문한 노드의 다음 노드가 있다면? 반복! // 마지막 노드는 포함되지 않음
    {
        printf("%d => ", curNode->value);
        curNode = curNode->next;
    }
    printf("%d\n", curNode->value); //마지막 노드를 출력
}

void insertNodeTail(int data) //O(N)
{
    node* curNode;
    node* newNode = (node*)malloc(sizeof(node)); //구조체 변수 선언(노드생성)
    newNode->value = data;
    newNode->next = NULL;

    if (head == NULL) //연결리스트가 구성 되지 않은 상태?
    {
        head = newNode; //새로 생성된 노드를 head로 설정
        printf("\n\n\t\t1. 노드 맨 뒤 삽입 : 첫 노드로 연결");
        return;
    }
    curNode = head;
    while (curNode->next != NULL) //방문한 노드의 다음 노드가 있다면?
    {
        curNode = curNode->next; //다음 노드로 이동
    }
    curNode->next =newNode; //마지막 노드에 새로 생성된 노드를 연결
    printf("\n\n\t\t2. 노드 맨 뒤 삽입 : 일반적인 경우 순회한 뒤 삽입");
}

void removeNode()
{
    node* delNode;
    delNode = head;
    while (head != NULL)
    {
        head = head->next;
        free(delNode);
        delNode = head;
    }
}

void insertNodeSort(int data)
{
    node* curNode, *prevNode, *newNode;
    newNode = (node*)malloc(sizeof(node));//구조체 변수(노드) 생성
    newNode->value = data;
    newNode->next = NULL;

    //case1. head가 NULL인 경우
    if (head == NULL)
    {
        head = newNode;
        printf("\n\n\t\tcase 1. 오름차순 정렬 삽입 : head == NULL\n");
        return 1; //삽입 성공
    }

    //case2. 가장 작은 값 삽입
    if (head->value > newNode->value) //첫 노드의 값 > 새 노드의 값
    {
        newNode->next = head;
        head = newNode;
        printf("\n\n\t\tcase 2. 오름차순 정렬 삽입 : 가장 작은 값 삽입 => head변경\n");
		return 1;
    }

    //case3. 일반적인 경우
    prevNode = curNode = head;
    while (curNode->next != NULL)
    {
        curNode = curNode->next;
        if (curNode->value > newNode->value)
        {
            //연결
            newNode->next = curNode;
            prevNode->next = newNode;
            printf("\n\n\t\tcase 3. 오름차순 정렬 삽입 : 일반적인 경우\n");
			return 1;
        }
        prevNode = prevNode->next;
    }
    //case4. 가장 큰 값을 삽입한 경우'
    curNode->next = newNode;
    printf("\n\n\t\tcase 4. 오름차순 정렬 삽입 : 가장 큰 값 삽입\n");
	return 1;
}