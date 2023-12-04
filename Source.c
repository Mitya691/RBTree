#pragma once
#include <stdlib.h>
#include <stdio.h>

typedef enum { black, red } clr; //завели тип переменной, принимающий только два значения, black и red

typedef struct node { //завели структуру
    clr color; //поле цвета
    int key; //поле ключа(значения)
    struct node* children[2]; //поле детей(left и right)
    struct node* parent; //поле родителя
} node;
/*Описали все функции программы*/
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

int isDigit() { //функция ввода числа
    int n; //завели переменную типа int
    while (!scanf("%d", &n)) { //пока не введено число
        printf("%s", "Incorrect input! Please try again!\n>> "); //программа просит ввести корректное число
        scanf("%*[^\n]"); //функция ввода числа
    }
    return n; //возвращает введенное значение
}

node* findPlace(node* ptr, int* key) { //функция для поиска метса вставки элемента
    if (*key < ptr->key) { //если введенное значение меньше чем текущее значение
        if (!ptr->children[0]) return ptr; // и данное значение не равно ребенку(левому), то завершаем поикс места для вставки
        else return findPlace(ptr->children[0], key); // если нет, то рекурсивно вызываем функцию поиска от ребенка, т.е. спускаемся на уровень ниже
    }
    else if (*key > ptr->key) { //если введенное значение больше чем текущее значение
        if (!ptr->children[1]) return ptr; // и данное значение не равно ребенку(правому), то завершаем поикс места для вставки
        else return findPlace(ptr->children[1], key); // если нет, то рекурсивно вызываем функцию поиска от ребенка, т.е. спускаемся на уровень ниже
    }
    else {
        return NULL; //возвращаем 0 значениие
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

void repainting(node** root, node* papa, node* uncle, node* grpapa) { //функция перекраски
    grpapa->color = grpapa == *root ? black : red; //в зависимости от 1 операнда определится цвет деда
    uncle->color = black; //дядю сделали черным
    papa->color = black; //папу сделали черным
    balancingAdd(root, grpapa); //вызвали функцию балансировки
}

node* createChild(node* ptr, int key) { //функция создания ребенка
    node* someNode = (node*)malloc(sizeof(node));//создали некоторую структуру и выделили под нее память
    someNode->color = red; //цвет изначально красный
    someNode->key = key; //некоторое значение
    someNode->children[0] = NULL; //инициализация детей
    someNode->children[1] = NULL; //инициализация детей
    someNode->parent = ptr; //инициализация родителя

    /*Условный оператор (? :) является тернарным оператором (он принимает три операнда).
    Условный оператор работает следующим образом.
    Первый операнд неявно преобразуется в bool. Он вычисляется, и все побочные эффекты завершаются перед продолжением.
    Если первый операнд имеет true значение (1), вычисляется второй операнд.
    Если первый операнд имеет false значение (0), выполняется оценка третьего операнда.*/

    node** tmp = (key < ptr->key ? &(ptr->children[0]) : &(ptr->children[1]));
    *tmp = someNode;
    return someNode;  //возвращаем созданную структуру
}

int addNode(node** root, int n) {
    if (!*root) { //создаем первый элемент
        node* someNode = (node*)malloc(sizeof(node));
        someNode->color = black;
        someNode->key = n;
        someNode->children[0] = NULL;
        someNode->children[1] = NULL;
        someNode->parent = NULL;
        *root = someNode;
    }
    else { //последующие элементы заполняем с помощью функции создания детей
        node* parent = findPlace(*root, &n);
        if (!parent) {
            printf("Collision found! Please enter another element!\n");
            return 1;
        }
        balancingAdd(root, createChild(parent, n));
    }
    return 0;
}

void balancingAdd(node** root, node* ptr) {//функция балансировки
    if (ptr == *root) return; //если переданная структура равна корню, то выходим из функции, т.к. балансировать нечего
    node* papa = ptr->parent; //завели папу
    if (!papa->parent || papa->color != red) return; //если деда нет, выходим из функции
    node* grpapa = papa->parent; //заводим деда
    node* uncle = (grpapa->children[0] == papa ? grpapa->children[1] : grpapa->children[0]); //заводим дядю либо левого либо правого

    if (uncle && uncle->color == red) repainting(root, papa, uncle, grpapa); //если дядя есть и цвет дяди красный, то делаем перекраску
    else if (!uncle || uncle->color == black) { //если дяди нет или его цвет черный, то
        if (papa->key > ptr->key && grpapa->key > papa->key) { //если значение папы больше чем вставляемого элемента и значение деда еще больше
            papa->color = black;  //красим папу в черный
            grpapa->color = red; //красим деда в красный
            rightRotate(root, papa, grpapa); //и делаем правый поворот
        } 
        else if (papa->key < ptr->key && grpapa->key < papa->key) {//если значение папы меньше чем вставляемого элемента и значение деда еще меньше
            papa->color = black;//красим папу в черный
            grpapa->color = red;//красим деда в красный
            leftRotate(root, papa, grpapa);//и делаем левый поворот
        }
        else if (papa->key < ptr->key && grpapa->key > ptr->key) {//если значение папы меньше чем вставляемого элемента и значение деда больше
            ptr->color = black; //красим текущий элемент в красный
            grpapa->color = red; //красим деда в красный
            doubleLeftRotate(root, ptr, papa, grpapa); //делаем двойной левый поворот
        }
        else if (papa->key > ptr->key && grpapa->key < ptr->key) {//если значение папы больше чем вставляемого элемента и значение деда меньше
            ptr->color = black;//красим текущий элемент в красный
            grpapa->color = red;//красим деда в красный
            doubleRightRotate(root, ptr, papa, grpapa);//делаем двойной правый поворот
        }
    }
}

void compareKeys(int* key, node* ptr, node** rez) { //функция сравнения значений, необходима для того чтобы найти искомый узел, в ret будет лежать 
    if (*key == ptr->key) {  //если ключ равен искомому, то
        *rez = ptr; // кладем в ret найденный подходящий узел 
        return; // и завершаем функцию
    } 
    else if (*key < ptr->key) { //если искомый ключ меньше текущего, то 
        if (!ptr->children[0]) return; //если не существует левого ребенка у текущего узла и спускаться ниже некуда, то завершаем функцию
        compareKeys(key, ptr->children[0], rez); //если же ребенок существует, то рекурсивно вызываем функцию от левого ребенка и продолжаем спуск по дереву до поиска узла
    }
    else if (*key > ptr->key) { //если ключ больше текущего, то
        if (!ptr->children[1]) return; //если не существует правого ребенка у текущего узла и спускаться ниже некуда, то завершаем функцию
        compareKeys(key, ptr->children[1], rez); //если же ребенок существует, то рекурсивно вызываем функцию от правого ребенка и продолжаем спуск по дереву до поиска узла
    }
}

node* findNode(int* key, node* root) { //функция поиска узла
    node* tmp = NULL; //создали узел
    compareKeys(key, root, &tmp); //передаем его как параметр в функцию сравнения значений и находим необходимый узел
    return tmp; //возвращаем найденный узел
}

node* findHeir(node* ptr) { //функция поиска наследника
    node* tmp = ptr->children[1]; //обратились к правому поддереву
    while (tmp->children[0]) tmp = tmp->children[0]; // пока есть левый ребенок, спускаемся вниз и находим самого левого сына
    return tmp; //возвращаем найденный узел с самым левым сыном
}

int deleteNode(node** root, int n) {
    node* ptr = findNode(&n, *root), * deletable = NULL, * son = NULL;
    if (!ptr) {
        printf("Node with key %d doesn't exist. Please try again!\n", n);
        return 1;
    }

    int heirs = 0; //количество сыновей
    for (int i = 0; i < 2; i++) if (ptr->children[i]) heirs++; //посчитали количество сыновей
    if (heirs < 2) deletable = ptr; //если у узла меньше 2 сыновей, то удаляемым становится узел с указанным значением
    else deletable = findHeir(ptr); //если у него 2 сына, то удаляемым становится наследник(узел с мин ключлм в правом поддереве удаляемого узла)
    if (deletable->children[0]) son = deletable->children[0];//если у удаляемого элемента есть левый сын, то созадем его
    else son = deletable->children[1]; //иначе правый
    if (son) son->parent = deletable->parent; //меняем местами отца сына и отца удаляемого элемента
    if (!deletable->parent) *root = son; //если не существует родителя у удоляемого элемента, т.е. он является корнем, то делаем сына корнем
    else (deletable->parent)->children[deletable == (deletable->parent)->children[0] ? 0 : 1] = son; //в противном случаем прикрепляем сына к отцу
    if (deletable != ptr) ptr->key = deletable->key; //кладем значение наследника в удаляемый уезел и сводим все к удалению листа

    if (!*root) {
        free(deletable); //освобождаем память
        return 0;
    }

    if (deletable->color == black) { //если цвет удаляемого элемента черный, то
        if (son && son->color == red) son->color = black; //и если цвет сына красный, то меняем цвет сына на черный
        else if (son && son->color == black) balancingDelete(root, son); //в противном случае производим балансировку после удаления узла
    }
    free(deletable); //освобождаем память

    return 0;
}

void balancingDelete(node** root, node* son) {
    node* papa, * bro, * leftCousin, * rightCousin; //завели папу, брата, правого кузена и левого кузена
    papa = son->parent; //инициализируем папу
    if (papa) bro = papa->children[son == papa->children[0] ? 1 : 0]; //если папа есть, то брат равен либо левому сыну либо правому
    else return; //в противном случае выходим, так как балансировать нечего
    leftCousin = bro->children[0]; //левый кузен это левый ребенок брата
    rightCousin = bro->children[1]; //правый кузен это правый ребенок брата

    if (papa->children[0] == son) { //если сын левый, то рассматриваем варианты
        if (papa->color == red && bro->color == black && (leftCousin && leftCousin->color == black) && (rightCousin && rightCousin->color == black)) {
            /*если папа красный, брат черный и дети брата черные, то*/
            bro->color = red; //красим брата в красный
            papa->color = black; //красим папу в черный
        }
        else if (bro->color == black && (rightCousin && rightCousin->color == red)) {
            /*если брат черный и его правый ребенок красный, то*/
            leftRotate(root, bro, papa); //делаем левый поворот
            rightCousin->color = black; //правого кузена красим в черный
            bro->color = papa->color; //брат получает цвет отца
            papa->color = black;//красим отца в черный
        }
        else if (bro->color == black && (rightCousin && rightCousin->color == black) && (leftCousin && leftCousin->color == red)) {
            /*брат черный, его правый сын черный, а левый красный*/
            rightRotate(root, leftCousin, bro); //делаем правый поворот
            leftCousin->color = black; //красим левого кузена в черный
            bro->color = red; //красим брата в красный
            balancingDelete(root, son); //рекурсивно вызываем балансировку, т.к. мы свели данный случай к предыдущему
        }
        else if (bro->color == red) {
            /*брат красный*/
            leftRotate(root, bro, papa); //делаем левый поворот
            bro->color = black; //красим брата в черный
            papa->color = red; //красим отца в красный
            balancingDelete(root, son); //рекурсивно вызываем балансировку, т.к. мы свели данный случай к одному из первых трех
        }
        else if (papa->color == black && bro->color == black && (leftCousin && leftCousin->color == black) && (rightCousin && rightCousin->color == black)) {
            /*все черные*/
            bro->color = red; //красим брата в красный
            balancingDelete(root, papa); //вызываем рекурсивно балансировку, так как количество черных узлов на пути уменьшилось и могло произойти нарушение 
            // свойств, необходимо исправить нарушения выше
        }
    }
    else if (papa->children[1] == son) { //аналогичные случаи но для правого сына
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
//отвечает за подъем и изменение положения последнего напечатанного элемента
node* findNearestLeftChild(node* ptr, node* root, int* padding) { //функция поиска ближайшего левого ребенка
    while (1) { //пока мы не выйдем по 1 условию
        /*1*/if (ptr->children[0]) {
            printf("\n");
            for (int i = 0; i < *padding; i++) printf("%c", ' ');
            return ptr->children[0];
        }
        /*2*/else {
            while (ptr->parent && (ptr->parent)->children[0] == ptr) { //пока у текущего элемента есть отец, и левый сын отца совпадает с текущим элементом 
                ptr = ptr->parent; //изменяем текущий элмент на его отца отца
                *padding -= 7; //уменьшаем расстояние
            }
            if (ptr->parent && (ptr->parent)->children[1] == ptr) { //если 
                ptr = ptr->parent;
                *padding -= 7; //коэфф учитывающий накопившуюся глубину
            }
            else if (!ptr->parent) return NULL;
        }
    }
}
//функция печатающая правую ветку
int printRightBranch(node* ptr, node** lastPoint, int* depth) {
    if (!ptr) return 1; //если текущий элемент нулевой выходим из функции
    for (node* tmp = ptr; tmp != NULL; tmp = tmp->children[1]) { //печатаем только правую ветку
        printf("%4d(%c)", tmp->key, tmp->color == red ? 'R' : 'B');
        *depth += 7; //увеличиваем глубину на 7 для "красивого" вывова
        *lastPoint = tmp; //последний напечатанный элемент в правой ветке
    }
    return 0;
}
//функция вывода дерева
int outputTree(node* root) { 
    if (!root) return 1; //если нет корня, то выходим из функции
    node* lastPoint;
    int depth = 0;
    printRightBranch(root, &lastPoint, &depth);
    while (!printRightBranch(findNearestLeftChild(lastPoint, root, &depth), &lastPoint, &depth)) {}
    /*Пока функция printRightBranch не вернет нам 0 мы будем эту функцию вызывать от последнего
    напечатанного элемента*/
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
