#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>  

using namespace std;

struct Student {
    string name;
    int id;
    string birthDate;
    string dormitory;
    string course;
};

struct Node {
    Student data;
    Node* left;
    Node* right;
    int height;
};

int height(Node* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

int getBalance(Node* node) {
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

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

//поиск по пропуску
Node* searchById(Node* root, int id) {
    if (root == nullptr || root->data.id == id)
        return root;

    if (id < root->data.id)
        return searchById(root->left, id);
    else
        return searchById(root->right, id);
}

void searchByName(Node* root, const string& name) {
    if (root == nullptr)
        return;

    searchByName(root->left, name);

    //поиск по имени
    if (root->data.name == name)
        cout << "ID: " << root->data.id << ", Имя: " << root->data.name << ", Дата рождения: " << root->data.birthDate
             << ", Общежитие: " << root->data.dormitory << ", Курс: " << root->data.course << endl;

    searchByName(root->right, name);
}

void inOrderTraversal(Node* root) {
    if (root != nullptr) {
        inOrderTraversal(root->left);
        cout << "ID: " << root->data.id << ", Имя: " << root->data.name << ", Дата рождения: " << root->data.birthDate
             << ", Общежитие: " << root->data.dormitory << ", Курс: " << root->data.course << endl;
        inOrderTraversal(root->right);
    }
}

//генераторы
string generateRandomName() {
    string names[] = {"Иван", "Денис", "Артём", "Тимофей", "Шамиль", "Александр", "Виктор", "Егор", "Григорий", "Татьяна", "Виктория", "Диана", "Елизавета", "Василиса", "Дарья", "Светлана"};
    return names[rand() % (sizeof(names) / sizeof(names[0]))];
}

string generateRandomBirthDate() {
    int day = rand() % 28 + 1;
    int month = rand() % 12 + 1;
    int year = rand() % 10 + 1995; 
    return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
}

Student generateRandomStudent(int id) {
    return {
        generateRandomName(),
        id,
        generateRandomBirthDate(),
        "Общежите " + to_string(rand() % 18 + 1),
        to_string(rand() % 5 + 1) + "th year, " +
        "Факультет " + to_string(rand() % 10 + 1) + ", " +
        "Университет " + to_string(rand() % 3 + 1)
    };
}

Node* generateAndInsertData(Node* root, int numRecords) {
    srand(static_cast<unsigned>(time(0)));

    for (int i = 1; i <= numRecords; ++i) {
        Student student = generateRandomStudent(i);
        root = insert(root, student);
    }

    return root;
}

int main() {
    setlocale(LC_ALL, "Russian");

    Node* root = nullptr;
    root = generateAndInsertData(root, 100000);

    // Пример поиска по ФИО
    string searchName = "Иван";
    cout << "Студенты, найденные по имени " << searchName << ":" << endl;
    searchByName(root, searchName);

    // Пример поиска по номеру пропуска
    int searchId = 2;
    Node* foundById = searchById(root, searchId);
    if (foundById != nullptr) {
        cout << "Студенты, найденные по ID " << searchId << ": " << foundById->data.name << endl;
    } else {
        cout << "Студенты с заданным ID " << searchId << " не найдены." << endl;
    }

   // cout << "All students in sorted order:" << endl;
   //  inOrderTraversal(root);

    return 0;
}