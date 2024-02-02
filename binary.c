#include <stdio.h>
#include <stdlib.h>

#pragma warning (disable : 4996)

typedef struct treeNode
{
    int value; //값
    struct treeNode* left; //포인터(8바이트)
    struct treeNode* right; //포인터(8바이트)
}treeNode;

void displayTreeInorder(treeNode* t);
treeNode* memoryFree(treeNode* t);
int getTreeNodeCount(treeNode* t);
int getTreeNodeSum(treeNode* t);
treeNode* addNode(treeNode* t, int data);
treeNode* getMaxNode(treeNode* t);
treeNode* getMinNode(treeNode* t);
treeNode* searchTreeNode(treeNode* t, int target);
treeNode* removeTreeNode(treeNode* t, int target);

int main()
{
	treeNode* root = NULL;
	treeNode* findNode;
	int choice, data;
	while (1)
	{
		system("cls");
		printf("\n\n\t\t*** Binary Search Tree(BST) ***\n\n");
		printf("1. 원소 삽입\n");
		printf("2. 원소 제거\n");
		printf("3. 원소 검색\n");
		printf("4. 최댓값 구하기\n");
		printf("5. 최솟값 구하기\n");
		printf("6. 이진 검색 트리 출력\n");
		printf("7. 트리 노드의 합\n");
		printf("8. 트리 노드의 개수\n");
		printf("0. 프로그램 종료\n");
		printf("\n메뉴 선택 : ");
		scanf("%d", &choice);
		while (getchar() != '\n');

		switch (choice)
		{
		case 1:
			printf("\n\n트리 노드의 값 입력 : ");
			scanf("%d", &data);
			while (getchar() != '\n');
			//addNode함수 : 노드 생성 후 루트의 주소를 리턴
			root = addNode(root, data);
			break;
		case 2:
			printf("\n\n제거할 노드의 값 입력 : ");
			scanf("%d", &data);
			while (getchar() != '\n');
			root = removeTreeNode(root, data);
			break;
		case 3:
			printf("\n\n트리 노드의 값 검색 : ");
			scanf("%d", &data);
			while (getchar() != '\n');

			//searchNode 함수 : 검색한 노드가 존재하면 검색 노드의 주소를 리턴, 존재하지 않으면 NULL을 리턴
			findNode = searchTreeNode(root, data);
			if (findNode == NULL)
				printf("\n\n\t\t검색 노드는 존재 하지 않습니다.\n");
			else
				printf("\n\n\t\t검색한 노드의 값은 %p메모리에 저장되어 있습니다.\n", findNode);
			break;
		case 4:
			//getMaxNode함수 : 최댓값 노드를 구해 최댓값 노드의 주소를 리턴
			findNode = getMaxNode(root);
			printf("\n\n\t\t최댓값 노드의 값은 %d입니다.\n", findNode->value);
			break;
		case 5:
			findNode = getMinNode(root);
			printf("\n\n\t\t최솟값 노드의 값은 %d입니다.\n", findNode->value);
			break;
		case 6:
			printf("\n\nBST Display(Inorder) : ");
			displayTreeInorder(root);
			break;
		case 7:
			printf("\n\n트리 구조에 생성 된 노드의 합은 %d개 입니다.\n", getTreeNodeSum(root));			
			break;
		case 8:
			printf("\n\n트리 구조에 생성 된 노드의 개수는 %d개 입니다.\n", getTreeNodeCount(root));
			break;
		case 0:
			root = memoryFree(root);
			exit(0);
		}
		printf("\n\n\t\t");
		system("pause");
	}
	return 0;
}

void displayTreeInorder(treeNode* t)
{
    //중위순회
    if (t != NULL)
    {
        displayTreeInorder(t->left); //왼쪽 노드의 주소로 재귀호출
        printf("%d", t->value);
        displayTreeInorder(t->right); //오른쪽 노드의 주소로 재귀호출
    }
}

treeNode* addNode(treeNode* t, int data)
{
    if (t == NULL)
    {
        t = (treeNode*)malloc(sizeof(treeNode)); //노드생성
        t->value = data;
        t->left = NULL;
        t->right = NULL;
    }
    else if (data < t->value)
    {
        t->left = addNode(t->left, data); //왼쪽으로 재귀호출
    }
    else if (data > t->value)
    {
        t->right = addNode(t->right, data); //오른쪽으로 재귀호출
    }
    return t; //이전 노드와 연결
}

treeNode* searchNode(treeNode* t, int target)
{
    if (t != NULL)
    {
        if (target == t->value)
            return t; //찾는 값 노드의 주소를 리턴
        else if (target < t->value)
            return searchTreeNode(t->left, target);
        else
            return searchTreeNode(t->right, target);
    }
    return t;
}

treeNode* getMaxNode(treeNode* t)
{
    if (t != NULL)
    {
        if (t->right != NULL)
            return getMaxNode(t->right);
    }
    return t; //최댓값 노드
}

treeNode* getMinNode(treeNode* t)
{
    //반복문 이용
	if (t != NULL)
	{
		while (t->left != NULL)
		{
			t = t->left;
		}
	}
	return t;
}

treeNode* removeNode(treeNode* t, int target)
{
    if (t->value == target)
    {
        //노드를 삭제하는 코드
        if(t->left == NULL && t->right == NULL)//case1. 자식 노드가 없는 경우
        {
            free(t);//노드 제거
            printf("\n\n\t\t1. 자식 노드가 없는 경우\n");
            return NULL;
        }
        else if (t->left == NULL) //case2. 오른쪽 자식 노드만 있는 경우
        {
            temp = t->right;
            free(t);
            printf("\n\n\t\t3. 오른쪽 자식만 있는 경우\n");
            return temp; //부모에게 자식의 주소값을 리턴
        }
        //4. 자식이 둘 다 있는 경우
        else
        {
            temp = getMinNode(t->right); //오른쪽 자식 중에 최솟값을 구한다.
            t->value = temp->value; //최솟값을 삭제할 노드로 저장
            printf("\n\n\t\t4. 자식 노드가 둘 다 있는 경우(오른쪽 자식 중 최솟값을 삭제할 노드로 대입)\n");
			printf("\t\t오른쪽 자식 중 최솟값을 삭제하러 다시 출발!!");
            t->right = removeTreeNode(t->right, temp->value);//최솟값을 제거하러 다시 출발
        }
    }
    else if (t->value > target)
    {
        t->left = removeNode(t->left, target);//재귀호출
    }
    else
    {
        removeNode(t->right, target); //재귀호출
    }
    return nullptr;
}