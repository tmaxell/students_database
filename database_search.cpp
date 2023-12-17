#include <iostream>
#include <string>
using namespace std;

// Структура для представления данных о студенте
struct Student {
    string name;
    int id;
    string birthDate;
    string dormitory;
    string course;
};

// Структура узла AVL-дерева
struct Node {
    Student data;
    Node* left;
    Node* right;
    int height;
};

// Функция для получения высоты узла
int height(Node* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// Функция для вычисления баланса узла
int getBalance(Node* node) {
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

// Функция для поворота вправо поддерева, корень которого равен y
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Выполнение поворота
    x->right = y;
    y->left = T2;

    // Обновление высот узлов
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Функция для поворота влево поддерева, корень которого равен x
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Выполнение поворота
    y->left = x;
    x->right = T2;

    // Обновление высот узлов
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Функция для вставки узла с данными о студенте в AVL-дерево
Node* insert(Node* root, Student data) {
    if (root == nullptr) {
        Node* newNode = new Node();
        newNode->data = data;
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }

    if (data.id < root->data.id)
        root->left = insert(root->left, data);
    else if (data.id > root->data.id)
        root->right = insert(root->right, data);
    else
        return root; // Дубликаты не допускаются

    // Обновление высоты текущего узла
    root->height = 1 + max(height(root->left), height(root->right));

    // Получение фактора баланса у текущего узла
    int balance = getBalance(root);

    // Балансировка узла

    // Лево-левая
    if (balance > 1 && data.id < root->left->data.id)
        return rightRotate(root);

    // Право-правая
    if (balance < -1 && data.id > root->right->data.id)
        return leftRotate(root);

    // Лево-правая
    if (balance > 1 && data.id > root->left->data.id) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Право-левая
    if (balance < -1 && data.id < root->right->data.id) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Функция для поиска студента по номеру пропуска
Node* searchById(Node* root, int id) {
    if (root == nullptr || root->data.id == id)
        return root;

    if (id < root->data.id)
        return searchById(root->left, id);
    else
        return searchById(root->right, id);
}

// Функция для поиска студента по ФИО
void searchByName(Node* root, const string& name) {
    if (root == nullptr)
        return;

    searchByName(root->left, name);

    // Поиск по ФИО, можно использовать любое подходящее условие
    if (root->data.name == name)
        cout << "ID: " << root->data.id << ", Name: " << root->data.name << ", BirthDate: " << root->data.birthDate
             << ", Dormitory: " << root->data.dormitory << ", Course: " << root->data.course << endl;

    searchByName(root->right, name);
}

// Вспомогательная функция для вывода дерева в виде отсортированного списка
void inOrderTraversal(Node* root) {
    if (root != nullptr) {
        inOrderTraversal(root->left);
        cout << "ID: " << root->data.id << ", Name: " << root->data.name << ", BirthDate: " << root->data.birthDate
             << ", Dormitory: " << root->data.dormitory << ", Course: " << root->data.course << endl;
        inOrderTraversal(root->right);
    }
}

int main() {
    Node* root = nullptr;

    // Пример вставки данных о студентах
     root = insert(root, {"John Doe", 1, "01/01/1990", "Dorm 101", "1st year, Computer Science, University A"});
    root = insert(root, {"Jane Doe", 2, "02/02/1991", "Dorm 102", "2nd year, Electrical Engineering, University B"});
    root = insert(root, {"Bob Smith", 3, "03/03/1992", "Dorm 103", "3rd year, Mechanical Engineering, University C"});

    // Пример поиска по номеру пропуска
    int searchId = 2;
    Node* foundById = searchById(root, searchId);
    if (foundById != nullptr) {
        cout << "Student found by ID " << searchId << ": " << foundById->data.name << endl;
    } else {
        cout << "Student with ID " << searchId << " not found." << endl;
    }

    // Пример поиска по ФИО
    string searchName = "Jane Doe";
    cout << "Students found by name " << searchName << ":" << endl;
    searchByName(root, searchName);

    // Пример вывода всего дерева
    cout << "All students in sorted order:" << endl;
    inOrderTraversal(root);

    return 0;
}