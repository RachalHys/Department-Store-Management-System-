#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

struct Supplier {
    std::string name;
    std::string ID;
    std::string address;
    std::string phone;
    std::string email;

    bool operator<(const Supplier& other) const {
        return ID < other.ID;
    }

    bool operator>(const Supplier& other) const {
        return ID > other.ID;
    }

    bool operator==(const Supplier& other) const {
        return ID == other.ID;
    }
};

class SupplierManagement {
private:
    struct Node {
        Supplier supplier;
        Node *left, *right;
        int height;
        
        Node(Supplier s){
        	supplier = s;
        	left = nullptr;
        	right = nullptr;
        	height = 1;
		}
    };

    Node* root;

    int Height(Node *node) {
        if(node == nullptr) 
			return 0;
			
        return node->height;
    }

    Node* leftRotate(Node *node) {
        Node *x = node->right;
        node->right = x->left;
        x->left = node;

        node->height = max(Height(node->left), Height(node->right)) + 1;
        x->height = max(Height(x->left), Height(x->right)) + 1;

        return x;
    }

    Node* rightRotate(Node *node) {
        Node *x = node->left;
        node->left = x->right;
        x->right = node;

        node->height = max(Height(node->left), Height(node->right)) + 1;
        x->height = max(Height(x->left), Height(x->right)) + 1;

        return x;
    }

    int balancedFactor(Node *node) {
        if(node == nullptr) 
			return 0;
			
        return Height(node->left) - Height(node->right);
    }

    Node* Insert(Node *node, const Supplier& supplier) {
        if(node == nullptr) 
			return new Node(supplier);

        if(supplier == node->supplier) 
			return node;
        else if(supplier < node->supplier) 
			node->left = Insert(node->left, supplier);
        else 
			node->right = Insert(node->right, supplier);

        node->height = max(Height(node->left), Height(node->right)) + 1;
        int balance = balancedFactor(node);

        if(balance > 1 && supplier < node->left->supplier) 
			return rightRotate(node);
			
        if(balance < -1 && supplier > node->right->supplier) 
			return leftRotate(node);
			
        if(balance > 1 && supplier > node->left->supplier) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        
        if(balance < -1 && supplier < node->right->supplier) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        
        return node;
    }

    Node* Delete(Node *node, const std::string& ID) {
        if (node == nullptr) 
			return node;
        
        if (ID < node->supplier.ID) 
			node->left = Delete(node->left, ID);
        else if (ID > node->supplier.ID) 
			node->right = Delete(node->right, ID);
        else {
            if (node->left == nullptr || node->right == nullptr) {
                Node *temp = node;
                node = (node->left ? node->left : node->right);
                delete temp;
            } 
			else {
                Node *parent = node;
                Node *successor = node->right;
                while (successor->left != nullptr) {
                    parent = successor;
                    successor = successor->left;
                }
                
                node->supplier = successor->supplier;
                
                if (parent->left == successor) 
					parent->left = successor->right;
                else 
					parent->right = successor->right;
                delete successor;
            }
        }

        if (node == nullptr) 
			return node;

        node->height = max(Height(node->left), Height(node->right)) + 1;
        int balance = balancedFactor(node);

        if (balance > 1 && balancedFactor(node->left) >= 0) 
			return rightRotate(node);
			
		if (balance < -1 && balancedFactor(node->right) <= 0) 
			return leftRotate(node);	
		
        if (balance > 1 && balancedFactor(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        
        if (balance < -1 && balancedFactor(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        
        return node;
    }

    Node* Search(Node *node, const std::string& ID) const {
        if (node == nullptr) 
			return nullptr;
        
        if (node->supplier.ID == ID) 
			return node;
        else if (node->supplier.ID > ID) 
			return Search(node->left, ID);
        else 
			return Search(node->right, ID);
    }
    
    void inorderTraversal(Node* node) const {
        if (node != nullptr) {
            inorderTraversal(node->left);
            printSupplier(node->supplier);
            inorderTraversal(node->right);
        }
    }

    void printSupplier(const Supplier& supplier) const {
        cout << "| " << setw(10) << left << supplier.ID
             << "| " << setw(20) << left << supplier.name
             << "| " << setw(30) << left << supplier.address
             << "| " << setw(15) << left << supplier.phone
             << "| " << setw(25) << left << supplier.email << " |" << endl;
    }
    
public:
    SupplierManagement() : root(nullptr) {}

    bool addSupplier(const Supplier& supplier) {
        if (Search(root, supplier.ID)) 
			return false;

        root = Insert(root, supplier);
        return true;
    }

    bool removeSupplier(const std::string& ID) {
        if (!Search(root, ID)) 
			return false;

        root = Delete(root, ID);
        return true;
    }

    bool updateSupplier(const Supplier& updatedSupplier) {
        Node* node = Search(root, updatedSupplier.ID);
        if (node != nullptr) {
            node->supplier = updatedSupplier;
            return true;
        }
        return false;
    }
    
    void listSupplier() const {
        cout << string(110, '-') << endl;
        cout << "| " << setw(10) << left << "ID"
             << "| " << setw(20) << left << "Name"
             << "| " << setw(30) << left << "Address"
             << "| " << setw(15) << left << "Phone"
             << "| " << setw(25) << left << "Email" << " |" << endl;
        cout << string(110, '-') << endl;
        
        inorderTraversal(root);
        
        cout << string(110, '-') << endl;
    }
};

struct Order {
    std::string ID;
    std::string productID;
    std::string productName;
    std::string supplierID;
    std::string supplierName;
    int quantity;
    int price;
    std::string currency;
    std::string date;
    std::string status;
};

class OrderManagement {
private:
    struct Element {
        Order order;
        bool isEmpty;
        
        Element(){
        	isEmpty = true;
		}
    };

    Element* Table;
    int N;
    int elements;
    double loadFactor;

    int Hash(const std::string& key) const {
        int hash = 0;
        for (char c : key)
            hash = (hash * 47 + c) % N;
        return hash;
    }

    int QuadraticProbing(int hashCode, int i) const {
        return (hashCode + i * i) % N;
    }

	void Insert(const Order& order) {
	    if ((double)elements / N > loadFactor)
	        Rehash();
	
	    int hashCode = Hash(order.ID);
	    
	    for (int i = 0; i < N; ++i) {
	        int index = QuadraticProbing(hashCode, i);
	        if (Table[index].isEmpty) {
	            Table[index].order = order;
	            Table[index].isEmpty = false;
	            ++elements;
	            return;
	        }
	    }
	}
	
	void Rehash() {
	    int oldSize = N;
	    Element* oldTable = Table;

	    N *= 2;
	    Table = new Element[N];
	    elements = 0;
	
	    for (int i = 0; i < oldSize; ++i)
	        if (!oldTable[i].isEmpty)
	            Insert(oldTable[i].order);
	
	    delete[] oldTable;
	}

    int Total() const {
        int total = 0;
        for (int i = 0; i < N; ++i)
            if (!Table[i].isEmpty)
                total += Table[i].order.quantity * Table[i].order.price;
                
        return total;
    }

    int TotalInRange(const std::string& start, const std::string& end) const {
        int total = 0;
        for (int i = 0; i < N; ++i)
            if (!Table[i].isEmpty && Table[i].order.date >= start && Table[i].order.date <= end)
                total += Table[i].order.quantity * Table[i].order.price;

        return total;
    }

public:
    OrderManagement() 
        : N(100), elements(0), loadFactor(0.75) {
        Table = new Element[100];
    }

    ~OrderManagement() {
        delete[] Table;
    }

    void addOrder(const Order& order){
        Insert(order);
    }

    void historyOrder() const {
        std::cout << "Order History:\n";
        for (int i = 0; i < N; ++i) {
            if (!Table[i].isEmpty) {
                const Order& element = Table[i].order;
                std::cout << "ID: " << element.ID << ", Product: " << element.productName 
                          << ", Quantity: " << element.quantity << ", Price: " << element.price 
                          << ", Date: " << element.date << ", Status: " << element.status << "\n";
            }
        }
    }

    void historyTransaction() const {
        std::cout << "Transaction History:\n";
        for (int i = 0; i < N; ++i) {
            if (!Table[i].isEmpty) {
                const Order& element = Table[i].order;
                std::cout << "ID: " << element.ID << ", Product: " << element.productName 
                          << ", Total: " << (element.quantity * element.price) << " " << element.currency 
                          << ", Date: " << element.date << "\n";
            }
        }
    }

    void calculateExpense() const {
        int expense = Total();
        std::cout << "Total Expense: " << expense << " USD\n";
    }

    void calculateRevenue() const {
        int revenue = Total();
        std::cout << "Total Revenue: " << revenue << " USD\n";
    }

    void reportExpenseDaily(const std::string& date) const {
        int dailyExpense = TotalInRange(date, date);
        std::cout << "Daily Expense (" << date << "): " << dailyExpense << " USD\n";
    }
    
    void reportExpenseMonthly(const std::string& month) const {
        std::string start = month + "-01";
        std::string end = month + "-31";
        int monthlyExpense = TotalInRange(start, end);
        std::cout << "Monthly Expense (" << month << "): " << monthlyExpense << " USD\n";
    }
    
    void reportExpenseYearly(const std::string& year) const {
        std::string start = year + "-01-01";
        std::string end = year + "-12-31";
        int yearlyExpense = TotalInRange(start, end);
        std::cout << "Yearly Expense (" << year << "): " << yearlyExpense << " USD\n";
    }

    void exportHistoryOrder() const {
        std::ofstream ofs("order_history.txt");
        ofs << "ID,ProductID,ProductName,SupplierID,SupplierName,Quantity,Price,Currency,Date,Status\n";
        for (int i = 0; i < N; ++i) {
            if (!Table[i].isEmpty) {
                const Order& order = Table[i].order;
                ofs << order.ID << "," << order.productID << "," << order.productName << ","
                        << order.supplierID << "," << order.supplierName << "," << order.quantity << ","
                        << order.price << "," << order.currency << "," << order.date << "," << order.status << "\n";
            }
        }
        ofs.close();
        std::cout << "Order history exported to order_history.txt\n";
    }

    void exportHistoryTransaction() const {
        std::ofstream ofs("transaction_history.txt");
        ofs << "ID,ProductName,Total,Currency,Date\n";
        for (int i = 0; i < N; ++i) {
            if (!Table[i].isEmpty) {
                const Order& order = Table[i].order;
                ofs << order.ID << "," << order.productName << ","
                        << (order.quantity * order.price) << "," << order.currency << "," << order.date << "\n";
            }
        }
        ofs.close();
        std::cout << "Transaction history exported to transaction_history.txt\n";
    }

    void importHistoryOrder() {
        std::ifstream ifs("order_history.txt");
        std::string line;
        std::getline(ifs, line);
        while (std::getline(ifs, line)) {
            std::istringstream iss(line);
            Order order;
            std::getline(iss, order.ID, ',');
            std::getline(iss, order.productID, ',');
            std::getline(iss, order.productName, ',');
            std::getline(iss, order.supplierID, ',');
            std::getline(iss, order.supplierName, ',');
            iss >> order.quantity;
            iss.ignore();
            iss >> order.price;
            iss.ignore();
            std::getline(iss, order.currency, ',');
            std::getline(iss, order.date, ',');
            std::getline(iss, order.status);
            Insert(order);
        }
        ifs.close();
        std::cout << "Order history imported from order_history.txt\n";
    }

    void importHistoryTransaction() {
        std::ifstream ifs("transaction_history.txt");
        std::string line;
        std::getline(ifs, line);
        while (std::getline(ifs, line)) {
            std::istringstream iss(line);
            Order order;
            std::getline(iss, order.ID, ',');
            std::getline(iss, order.productName, ',');
            int total;
            iss >> total;
            iss.ignore();
            std::getline(iss, order.currency, ',');
            std::getline(iss, order.date);
            order.quantity = 1;
            order.price = total;
            Insert(order);
        }
        ifs.close();
        std::cout << "Transaction history imported from transaction_history.txt\n";
    }
};
