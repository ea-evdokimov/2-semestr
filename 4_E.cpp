/*Реализуйте структуру данных “массив строк” на основе декартового дерева по неявному ключу со следующими методами: 
    // Добавление строки в позицию position.\\  
    // Все последующие строки сдвигаются на одну позицию вперед.\\  
    void InsertAt( int position, const std::string& value );\\  
    // Удаление строки из позиции position.\\  
    // Все последующие строки сдвигаются на одну позицию назад.\\  
    void DeleteAt( int position );\\  
    // Получение строки из позиции position.\\  
    std::string GetAt( int position );
Все методы должны работать за O(log n) в среднем, где n – текущее количество строк в массиве.*/
#include <iostream>
#include <queue>
#include <string>
using namespace std;

template<typename T>
class DECART {
private:
	struct node {
		T data;
		int prior;
		int c = 0;
		node* left = nullptr;
		node* right = nullptr;

		node(T const& new_data) :data(new_data), prior(rand()) {}

		void print(int level) const {
			if (this == nullptr) return;
			right->print(level + 1);
			for (int i = 0; i < level; i++) cout << "__";
			cout << data << "-" << c << "-" << prior << endl;
			left->print(level + 1);
		}

	} *root = nullptr;

public:
	~DECART() {
		queue <node*> q;
		q.push(root);

		while (!q.empty()) {
			auto curr = q.front();
			q.pop();

			if (curr->left != nullptr)
				q.push(curr->left);

			if (curr->right != nullptr)
				q.push(curr->right);

			delete curr;
		}
	}

	bool empty() const {
		return root == nullptr;
	}

    int get_c(node* cur){
        return cur ? cur->c : 0;
    }

    void update_c(node* cur){
        if(cur) {
            cur->c = get_c(cur->left) + get_c(cur->right) + 1;
        }
    }

    pair<node*, node*> split(node* root_, int position) {
		//root_ - текущий
	    if (root_ == nullptr)
			return { nullptr , nullptr };

		int index = get_c(root_->left);

		if (index < position) {
			pair<node*, node*> spl = split(root_->right, position - index - 1);
			root_->right = spl.first;
			update_c(root_);
			return { root_, spl.second };
		} else {
			pair<node*, node*> spl = split(root_->left, position);
			root_->left = spl.second;
            update_c(root_);
			return { spl.first, root_ };
		}
	}

	node* merge(node* left, node* right) {

	    if (left == nullptr) {
	        update_c(right);
	        return right;
        }
        if (right == nullptr) {
            update_c(left);
            return left;
        }

		if (left->prior > right->prior) {
			left->right = merge(left->right, right);
			update_c(left);
			return left;
		} else {
			right->left = merge(left, right->left);
			update_c(right);
			return right;
		}
	}

    T get_at(int position){
        node* curr = root;
        //потомки левого сына
        int index = get_c(curr->left);

        while (index != position) {
            //если левее, чем нужно
            if (position >= index) {
                position -= index + 1;
                curr = curr->right;
            } else {
                curr = curr->left;
            }
            index = get_c(curr->left);
        }

        return curr -> data;
	}

    void insert_at(int pos, T const& value) {
        node *new_node = new node(value);
        //разрезаем по индексу
        pair<node*, node*> splited = split(root, pos);
        //у правой части приклеиваем новое значение
        node* new_left_part = merge(splited.first, new_node);
        //склеиваем с левой частью
        root = merge(new_left_part, splited.second);

    }

	void delete_at(int position){
	    //разрезаем по индексу
        pair<node*, node*> splited = split(root, position);
        //отделяем удаляемый элемент
        pair<node*, node*> new_parts = split(splited.second, 1);
        //склеиваем с правой частью(без отрезанного удаляемого элемента)
        root = merge(splited.first, new_parts.second);
	}

	int height() {
		if (empty())
			return 0;
		return find_height(root);
	}

	int find_height(node* root_) {
		int h_left = 0, h_right = 0;

		if (root_->left != nullptr)
			h_left = find_height(root_->left);

		if (root_->right != nullptr)
			h_right = find_height(root_->right);

		return h_left > h_right ? h_left + 1 : h_right + 1;
	}

	void print() {
		root->print(0);
	}
};

int main() {
	DECART<string> d;
	char c;
	int pos = 0;
	size_t k = 0; cin >> k;
//    srand(1590510280);
	srand(time(0));
    string data;

    for(int i = 0; i < k; ++i){
//    while(true){
        cin >> c;

	    if(c == '-'){
	        int pos_end = 0;
	        cin >> pos >> pos_end;
            for(int j = pos; j <= pos_end; ++j)
	            d.delete_at(j);
	    }
        else if(c == '?'){
            cin >> pos;
            cout << d.get_at(pos) << endl;
        }
        else if(c == '+'){
            cin >> pos >> data;
            d.insert_at(pos, data);
        } else
            d.print();
	}
	return 0;
}

/*
6
+ 0 myau
+ 0 krya
? 0
+ 2 gav
- 1 1
? 1
 */

/*
 6
+ 0 myau
+ 0 krya
? 0
 */