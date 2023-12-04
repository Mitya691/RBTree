#pragma once
#include <stdlib.h>
#include <stdio.h>

typedef enum { black, red } clr; //������ ��� ����������, ����������� ������ ��� ��������, black � red

typedef struct node { //������ ���������
    clr color; //���� �����
    int key; //���� �����(��������)
    struct node* children[2]; //���� �����(left � right)
    struct node* parent; //���� ��������
} node;
/*������� ��� ������� ���������*/
int isDigit();
node* findPlace(node*, int*);
node* createChild(node*, int);
void balancingAdd(node**, node*);
void lefttRotate(node**, node*, node*);
void rightRotate(node**, node*, node*);
void doubleLeftRotate(node**, node*, node*, node*);
void doubleRightRotate(node**, node*, node*, node*);
void repainting(node**, node*, node*, node*);
void compareKeys(int*, node*, node**);
node* findNode(int*, node*);
node* findHeir(node*);
void balancingDelete(node**, node*);
int printRightBranch(node*, node**, int*);
node* findNearestLeftChild(node*, node*, int*);

int isDigit() { //������� ����� �����
    int n; //������ ���������� ���� int
    while (!scanf("%d", &n)) { //���� �� ������� �����
        printf("%s", "Incorrect input! Please try again!\n>> "); //��������� ������ ������ ���������� �����
        scanf("%*[^\n]"); //������� ����� �����
    }
    return n; //���������� ��������� ��������
}

node* findPlace(node* ptr, int* key) { //������� ��� ������ ����� ������� ��������
    if (*key < ptr->key) { //���� ��������� �������� ������ ��� ������� ��������
        if (!ptr->children[0]) return ptr; // � ������ �������� �� ����� �������(������), �� ��������� ����� ����� ��� �������
        else return findPlace(ptr->children[0], key); // ���� ���, �� ���������� �������� ������� ������ �� �������, �.�. ���������� �� ������� ����
    }
    else if (*key > ptr->key) { //���� ��������� �������� ������ ��� ������� ��������
        if (!ptr->children[1]) return ptr; // � ������ �������� �� ����� �������(�������), �� ��������� ����� ����� ��� �������
        else return findPlace(ptr->children[1], key); // ���� ���, �� ���������� �������� ������� ������ �� �������, �.�. ���������� �� ������� ����
    }
    else {
        return NULL; //���������� 0 ���������
    }
}

void rightRotate(node** root, node* papa, node* grpapa) { 
    grpapa->children[0] = papa->children[1]; 
    if (papa->children[1]) (papa->children[1])->parent = grpapa;
    papa->children[1] = grpapa;
    papa->parent = grpapa->parent;
    if (grpapa->parent) (grpapa->parent)->children[grpapa == (grpapa->parent)->children[0] ? 0 : 1] = papa;
    grpapa->parent = papa;
    if (*root == grpapa) *root = papa;
}

void leftRotate(node** root, node* papa, node* grpapa) {
    grpapa->children[1] = papa->children[0];
    if (papa->children[0]) (papa->children[0])->parent = grpapa;
    papa->children[0] = grpapa;
    papa->parent = grpapa->parent;
    if (grpapa->parent) (grpapa->parent)->children[grpapa == (grpapa->parent)->children[0] ? 0 : 1] = papa;
    grpapa->parent = papa;
    if (*root == grpapa) *root = papa;
}

void doubleLeftRotate(node** root, node* ptr, node* papa, node* grpapa) {
    ptr->parent = grpapa->parent;
    if (grpapa->parent) (grpapa->parent)->children[grpapa == (grpapa->parent)->children[0] ? 0 : 1] = ptr;
    papa->children[1] = ptr->children[0];
    if (ptr->children[0]) (ptr->children[0])->parent = papa;
    grpapa->children[0] = ptr->children[1];
    if (ptr->children[1]) (ptr->children[1])->parent = grpapa;
    papa->parent = ptr;
    grpapa->parent = ptr;
    ptr->children[0] = papa;
    ptr->children[1] = grpapa;
    if (*root == grpapa) *root = ptr;
}

void doubleRightRotate(node** root, node* ptr, node* papa, node* grpapa) {
    ptr->parent = grpapa->parent;
    if (grpapa->parent) (grpapa->parent)->children[grpapa == (grpapa->parent)->children[0] ? 0 : 1] = ptr;
    papa->children[0] = ptr->children[1];
    if (ptr->children[1]) (ptr->children[1])->parent = papa;
    grpapa->children[1] = ptr->children[0];
    if (ptr->children[0]) (ptr->children[0])->parent = grpapa;
    papa->parent = ptr;
    grpapa->parent = ptr;
    ptr->children[0] = grpapa;
    ptr->children[1] = papa;
    if (*root == grpapa) *root = ptr;
}

void repainting(node** root, node* papa, node* uncle, node* grpapa) { //������� ����������
    grpapa->color = grpapa == *root ? black : red; //� ����������� �� 1 �������� ����������� ���� ����
    uncle->color = black; //���� ������� ������
    papa->color = black; //���� ������� ������
    balancingAdd(root, grpapa); //������� ������� ������������
}

node* createChild(node* ptr, int key) { //������� �������� �������
    node* someNode = (node*)malloc(sizeof(node));//������� ��������� ��������� � �������� ��� ��� ������
    someNode->color = red; //���� ���������� �������
    someNode->key = key; //��������� ��������
    someNode->children[0] = NULL; //������������� �����
    someNode->children[1] = NULL; //������������� �����
    someNode->parent = ptr; //������������� ��������

    /*�������� �������� (? :) �������� ��������� ���������� (�� ��������� ��� ��������).
    �������� �������� �������� ��������� �������.
    ������ ������� ������ ������������� � bool. �� �����������, � ��� �������� ������� ����������� ����� ������������.
    ���� ������ ������� ����� true �������� (1), ����������� ������ �������.
    ���� ������ ������� ����� false �������� (0), ����������� ������ �������� ��������.*/

    node** tmp = (key < ptr->key ? &(ptr->children[0]) : &(ptr->children[1]));
    *tmp = someNode;
    return someNode;  //���������� ��������� ���������
}

int addNode(node** root, int n) {
    if (!*root) { //������� ������ �������
        node* someNode = (node*)malloc(sizeof(node));
        someNode->color = black;
        someNode->key = n;
        someNode->children[0] = NULL;
        someNode->children[1] = NULL;
        someNode->parent = NULL;
        *root = someNode;
    }
    else { //����������� �������� ��������� � ������� ������� �������� �����
        node* parent = findPlace(*root, &n);
        if (!parent) {
            printf("Collision found! Please enter another element!\n");
            return 1;
        }
        balancingAdd(root, createChild(parent, n));
    }
    return 0;
}

void balancingAdd(node** root, node* ptr) {//������� ������������
    if (ptr == *root) return; //���� ���������� ��������� ����� �����, �� ������� �� �������, �.�. ������������� ������
    node* papa = ptr->parent; //������ ����
    if (!papa->parent || papa->color != red) return; //���� ���� ���, ������� �� �������
    node* grpapa = papa->parent; //������� ����
    node* uncle = (grpapa->children[0] == papa ? grpapa->children[1] : grpapa->children[0]); //������� ���� ���� ������ ���� �������

    if (uncle && uncle->color == red) repainting(root, papa, uncle, grpapa); //���� ���� ���� � ���� ���� �������, �� ������ ����������
    else if (!uncle || uncle->color == black) { //���� ���� ��� ��� ��� ���� ������, ��
        if (papa->key > ptr->key && grpapa->key > papa->key) { //���� �������� ���� ������ ��� ������������ �������� � �������� ���� ��� ������
            papa->color = black;  //������ ���� � ������
            grpapa->color = red; //������ ���� � �������
            rightRotate(root, papa, grpapa); //� ������ ������ �������
        } 
        else if (papa->key < ptr->key && grpapa->key < papa->key) {//���� �������� ���� ������ ��� ������������ �������� � �������� ���� ��� ������
            papa->color = black;//������ ���� � ������
            grpapa->color = red;//������ ���� � �������
            leftRotate(root, papa, grpapa);//� ������ ����� �������
        }
        else if (papa->key < ptr->key && grpapa->key > ptr->key) {//���� �������� ���� ������ ��� ������������ �������� � �������� ���� ������
            ptr->color = black; //������ ������� ������� � �������
            grpapa->color = red; //������ ���� � �������
            doubleLeftRotate(root, ptr, papa, grpapa); //������ ������� ����� �������
        }
        else if (papa->key > ptr->key && grpapa->key < ptr->key) {//���� �������� ���� ������ ��� ������������ �������� � �������� ���� ������
            ptr->color = black;//������ ������� ������� � �������
            grpapa->color = red;//������ ���� � �������
            doubleRightRotate(root, ptr, papa, grpapa);//������ ������� ������ �������
        }
    }
}

void compareKeys(int* key, node* ptr, node** rez) { //������� ��������� ��������, ���������� ��� ���� ����� ����� ������� ����, � ret ����� ������ 
    if (*key == ptr->key) {  //���� ���� ����� ��������, ��
        *rez = ptr; // ������ � ret ��������� ���������� ���� 
        return; // � ��������� �������
    } 
    else if (*key < ptr->key) { //���� ������� ���� ������ ��������, �� 
        if (!ptr->children[0]) return; //���� �� ���������� ������ ������� � �������� ���� � ���������� ���� ������, �� ��������� �������
        compareKeys(key, ptr->children[0], rez); //���� �� ������� ����������, �� ���������� �������� ������� �� ������ ������� � ���������� ����� �� ������ �� ������ ����
    }
    else if (*key > ptr->key) { //���� ���� ������ ��������, ��
        if (!ptr->children[1]) return; //���� �� ���������� ������� ������� � �������� ���� � ���������� ���� ������, �� ��������� �������
        compareKeys(key, ptr->children[1], rez); //���� �� ������� ����������, �� ���������� �������� ������� �� ������� ������� � ���������� ����� �� ������ �� ������ ����
    }
}

node* findNode(int* key, node* root) { //������� ������ ����
    node* tmp = NULL; //������� ����
    compareKeys(key, root, &tmp); //�������� ��� ��� �������� � ������� ��������� �������� � ������� ����������� ����
    return tmp; //���������� ��������� ����
}

node* findHeir(node* ptr) { //������� ������ ����������
    node* tmp = ptr->children[1]; //���������� � ������� ���������
    while (tmp->children[0]) tmp = tmp->children[0]; // ���� ���� ����� �������, ���������� ���� � ������� ������ ������ ����
    return tmp; //���������� ��������� ���� � ����� ����� �����
}

int deleteNode(node** root, int n) {
    node* ptr = findNode(&n, *root), * deletable = NULL, * son = NULL;
    if (!ptr) {
        printf("Node with key %d doesn't exist. Please try again!\n", n);
        return 1;
    }

    int heirs = 0; //���������� �������
    for (int i = 0; i < 2; i++) if (ptr->children[i]) heirs++; //��������� ���������� �������
    if (heirs < 2) deletable = ptr; //���� � ���� ������ 2 �������, �� ��������� ���������� ���� � ��������� ���������
    else deletable = findHeir(ptr); //���� � ���� 2 ����, �� ��������� ���������� ���������(���� � ��� ������ � ������ ��������� ���������� ����)
    if (deletable->children[0]) son = deletable->children[0];//���� � ���������� �������� ���� ����� ���, �� ������� ���
    else son = deletable->children[1]; //����� ������
    if (son) son->parent = deletable->parent; //������ ������� ���� ���� � ���� ���������� ��������
    if (!deletable->parent) *root = son; //���� �� ���������� �������� � ���������� ��������, �.�. �� �������� ������, �� ������ ���� ������
    else (deletable->parent)->children[deletable == (deletable->parent)->children[0] ? 0 : 1] = son; //� ��������� ������� ����������� ���� � ����
    if (deletable != ptr) ptr->key = deletable->key; //������ �������� ���������� � ��������� ����� � ������ ��� � �������� �����

    if (!*root) {
        free(deletable); //����������� ������
        return 0;
    }

    if (deletable->color == black) { //���� ���� ���������� �������� ������, ��
        if (son && son->color == red) son->color = black; //� ���� ���� ���� �������, �� ������ ���� ���� �� ������
        else if (son && son->color == black) balancingDelete(root, son); //� ��������� ������ ���������� ������������ ����� �������� ����
    }
    free(deletable); //����������� ������

    return 0;
}

void balancingDelete(node** root, node* son) {
    node* papa, * bro, * leftCousin, * rightCousin; //������ ����, �����, ������� ������ � ������ ������
    papa = son->parent; //�������������� ����
    if (papa) bro = papa->children[son == papa->children[0] ? 1 : 0]; //���� ���� ����, �� ���� ����� ���� ������ ���� ���� �������
    else return; //� ��������� ������ �������, ��� ��� ������������� ������
    leftCousin = bro->children[0]; //����� ����� ��� ����� ������� �����
    rightCousin = bro->children[1]; //������ ����� ��� ������ ������� �����

    if (papa->children[0] == son) { //���� ��� �����, �� ������������� ��������
        if (papa->color == red && bro->color == black && (leftCousin && leftCousin->color == black) && (rightCousin && rightCousin->color == black)) {
            /*���� ���� �������, ���� ������ � ���� ����� ������, ��*/
            bro->color = red; //������ ����� � �������
            papa->color = black; //������ ���� � ������
        }
        else if (bro->color == black && (rightCousin && rightCousin->color == red)) {
            /*���� ���� ������ � ��� ������ ������� �������, ��*/
            leftRotate(root, bro, papa); //������ ����� �������
            rightCousin->color = black; //������� ������ ������ � ������
            bro->color = papa->color; //���� �������� ���� ����
            papa->color = black;//������ ���� � ������
        }
        else if (bro->color == black && (rightCousin && rightCousin->color == black) && (leftCousin && leftCousin->color == red)) {
            /*���� ������, ��� ������ ��� ������, � ����� �������*/
            rightRotate(root, leftCousin, bro); //������ ������ �������
            leftCousin->color = black; //������ ������ ������ � ������
            bro->color = red; //������ ����� � �������
            balancingDelete(root, son); //���������� �������� ������������, �.�. �� ����� ������ ������ � �����������
        }
        else if (bro->color == red) {
            /*���� �������*/
            leftRotate(root, bro, papa); //������ ����� �������
            bro->color = black; //������ ����� � ������
            papa->color = red; //������ ���� � �������
            balancingDelete(root, son); //���������� �������� ������������, �.�. �� ����� ������ ������ � ������ �� ������ ����
        }
        else if (papa->color == black && bro->color == black && (leftCousin && leftCousin->color == black) && (rightCousin && rightCousin->color == black)) {
            /*��� ������*/
            bro->color = red; //������ ����� � �������
            balancingDelete(root, papa); //�������� ���������� ������������, ��� ��� ���������� ������ ����� �� ���� ����������� � ����� ��������� ��������� 
            // �������, ���������� ��������� ��������� ����
        }
    }
    else if (papa->children[1] == son) { //����������� ������ �� ��� ������� ����
        if (papa->color == red && bro->color == black && (leftCousin && leftCousin->color == black) && (rightCousin && rightCousin->color == black)) {
            bro->color = red;
            papa->color = black;
        }
        else if (bro->color == black && (leftCousin && leftCousin->color == red)) {
            rightRotate(root, bro, papa);
            leftCousin->color = black;
            bro->color = papa->color;
            papa->color = black;
        }
        else if (bro->color == black && (rightCousin && rightCousin->color == red) && (leftCousin && leftCousin->color == black)) {
            leftRotate(root, rightCousin, bro);
            rightCousin->color = black;
            bro->color = red;
            balancingDelete(root, son);
        }
        else if (bro->color == red) {
            rightRotate(root, bro, papa);
            bro->color = black;
            papa->color = red;
            balancingDelete(root, son);
        }
        else if (papa->color == black && bro->color == black && (leftCousin && leftCousin->color == black) && (rightCousin && rightCousin->color == black)) {
            bro->color = red;
            balancingDelete(root, papa);
        }
    }
}
//�������� �� ������ � ��������� ��������� ���������� ������������� ��������
node* findNearestLeftChild(node* ptr, node* root, int* padding) { //������� ������ ���������� ������ �������
    while (1) { //���� �� �� ������ �� 1 �������
        /*1*/if (ptr->children[0]) {
            printf("\n");
            for (int i = 0; i < *padding; i++) printf("%c", ' ');
            return ptr->children[0];
        }
        /*2*/else {
            while (ptr->parent && (ptr->parent)->children[0] == ptr) { //���� � �������� �������� ���� ����, � ����� ��� ���� ��������� � ������� ��������� 
                ptr = ptr->parent; //�������� ������� ������ �� ��� ���� ����
                *padding -= 7; //��������� ����������
            }
            if (ptr->parent && (ptr->parent)->children[1] == ptr) { //���� 
                ptr = ptr->parent;
                *padding -= 7; //����� ����������� ������������ �������
            }
            else if (!ptr->parent) return NULL;
        }
    }
}
//������� ���������� ������ �����
int printRightBranch(node* ptr, node** lastPoint, int* depth) {
    if (!ptr) return 1; //���� ������� ������� ������� ������� �� �������
    for (node* tmp = ptr; tmp != NULL; tmp = tmp->children[1]) { //�������� ������ ������ �����
        printf("%4d(%c)", tmp->key, tmp->color == red ? 'R' : 'B');
        *depth += 7; //����������� ������� �� 7 ��� "���������" ������
        *lastPoint = tmp; //��������� ������������ ������� � ������ �����
    }
    return 0;
}
//������� ������ ������
int outputTree(node* root) { 
    if (!root) return 1; //���� ��� �����, �� ������� �� �������
    node* lastPoint;
    int depth = 0;
    printRightBranch(root, &lastPoint, &depth);
    while (!printRightBranch(findNearestLeftChild(lastPoint, root, &depth), &lastPoint, &depth)) {}
    /*���� ������� printRightBranch �� ������ ��� 0 �� ����� ��� ������� �������� �� ����������
    ������������� ��������*/
    return 0;
}

int main() {
    char* menu[4] = { "\n0. Exit", "1. Add node", "2. Delete node", "3. Print tree" };
    int choice;
    node* root = NULL;

    while (1) {
        for (int i = 0; i < 4; i++) printf("%s\n", menu[i]);
        printf("%s", ">> ");
        choice = isDigit();
        switch (choice) {
        case 0: return 0;
        case 1: {
            printf("%s", "Please enter the key of new node: ");
            addNode(&root, isDigit());
            break;
        }
        case 2: {
            printf("%s", "Please enter the key of the node: ");
            deleteNode(&root, isDigit());
            break;
        }
        case 3: {
            outputTree(root);
            break;
        }
        default:
            printf("%s", "There is no such option! Please try again.\n");
        }
    }
}
