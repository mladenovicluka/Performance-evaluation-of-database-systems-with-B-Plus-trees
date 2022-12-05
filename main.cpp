#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#define ll long long int
using namespace std;
struct podatak {
	ll  ca_id, ca_b_id, ca_c_id, ca_tax_st;
	double ca_bal;
	string ca_name;
	podatak(ll a, ll b, ll c, string d, ll f, double e) : ca_id(a), ca_b_id(b), ca_c_id(c), ca_name(d), ca_tax_st(f), ca_bal(e) {}
};
struct TreeNode {
	bool is_leaf;
	int m;
	vector<ll> keys;
	vector<podatak*> podaci;
	vector<TreeNode*> children;
	TreeNode* parent;
	TreeNode* right;
	TreeNode(bool leaf, int mm) {
		right = nullptr;
		parent = nullptr;
		m = mm;
		is_leaf = leaf;
		if (leaf) 
			right = nullptr;
	}
};
TreeNode* root=nullptr;

int height(TreeNode* r) {
	queue <TreeNode*> q;
	q.push(r);
	int cnt = 0;
	while (!q.empty()) {
		int n = q.size();
		for (int i = 0; i < n; i++)
		{
			TreeNode* curr = q.front();
			q.pop();
			for (auto x : curr->children)q.push(x);
		}
		cnt++;
	}
	return cnt;
}
void print_tree(TreeNode* r,int m) {
	if (!r)return;
	queue <TreeNode*> q;
	q.push(r);

	int h = height(r);
	int i=0, line_len = 62;
	int first_skip = line_len, in_between_skip;

	while (!q.empty()) {
		

		int n = q.size();
		int j = 1 << i, k, l; in_between_skip = first_skip; first_skip = (first_skip - m) / m;
		for (k = 0; k < first_skip; k++) putchar(' ');
		for (int i = 0; i < n; i++)
		{
			TreeNode* curr = q.front();
			q.pop();
			for (int i = 0; i < curr->keys.size(); i++) {
				if (i == curr->keys.size() - 1)printf("%lld ", curr->keys[i]);
				else printf("%lld|", curr->keys[i]);
			}

			printf("    ");
			for (l = 0; l < in_between_skip; l++) putchar('  ');
			for (auto x : curr->children)q.push(x);
		}
		putchar('\n');
		putchar('\n');
		i++;

	}
}

void print_tree_v2(TreeNode* r) {
	if (!r)return;
	queue <TreeNode*> q;
	q.push(r);
	int cnt = 0;
	int h = height(root);
	int i, line_len = 62;
	int first_skip = line_len, in_between_skip;
	while (!q.empty()) {
		cout << "NIVO: " << cnt<<"\n";
		int n = q.size();
		for (int i = 0; i < n; i++)
		{
			TreeNode* curr = q.front();
			q.pop();
			if (curr->parent)cout << "OTAC:"<<curr->parent->keys[0]<<"|";
			for (auto x : curr->keys)cout << " " << x << " ";
			cout << endl;
			for (auto x : curr->children)q.push(x);
		}
		cnt++;
		
	}

}
void find(TreeNode* root,stack<TreeNode*>& s, ll key) {
	TreeNode* curr = root;
	s.push(curr);
	while (!curr->is_leaf) {
		int i = 0;
		while (i < curr->keys.size() && curr->keys[i] < key) i++;
		curr = curr->children[i];
		s.push(curr);
	}
}
void insertuj(TreeNode* p, ll k,podatak* x) {
	int i = 0;
	while (i < p->keys.size() && p->keys[i] < k)i++;
	p->podaci.insert(p->podaci.begin() + i, x);
	p->keys.insert(p->keys.begin() + i, k);
}
void razlomi(TreeNode* p, TreeNode* l, TreeNode* d, ll key, int m,podatak* pod) {
		int i = 0;
		while (i < p->keys.size() && p->keys[i] < key)i++;
		p->keys.insert(p->keys.begin() + i, key);
		p->podaci.insert(p->podaci.begin() + i, pod);
		p->children.insert(p->children.begin() + i +1, d);
		d->parent = p;
		l->parent = p;
}
void roditelj(TreeNode* l, TreeNode*d,int m, podatak* pod) {
		ll k = l->keys[l->keys.size() - 1];
		podatak* p = l->podaci[l->podaci.size() - 1];
		TreeNode* pom1 = new TreeNode(false, m);
		if (l->children.size() + d->children.size() > 3) {

			l->keys.pop_back();
			l->podaci.pop_back();
		}
		pom1->keys.push_back(k);
		pom1->podaci.push_back(p);
		pom1->children.push_back(l);
		pom1->children.push_back(d);
		l->parent = pom1;
		d->parent = pom1;
		d->right = l->right;
		l->right = d;
		root = pom1;
}
void dodaj(podatak* p, int m) {
	int flag = 0;
	TreeNode* d = new TreeNode(true, m);
	TreeNode* l=nullptr ;
	int key = -1;
	if (!root) {
		d->is_leaf = true;
		root = d;
		root->keys.push_back(p->ca_id);
		root->parent = nullptr;
		root->podaci.push_back(p);
	}
	else {
			stack<TreeNode*> s;
			find(root, s, p->ca_id);
			insertuj(s.top(), p->ca_id,p);
			TreeNode* dno = s.top(); 
			while (!s.empty() && s.top()->keys.size() == m ) {
				l = s.top();
				s.pop(); 
				d= new TreeNode(false, m);
				if (dno ==l)d->is_leaf = true;
				d->parent = l->parent;
				int mid = l->keys.size() / 2;
				int mw = l->children.size() / 2;

				for (int i = mid + 1; i < l->keys.size(); i++) {
					d->keys.push_back(l->keys[i]);
					d->podaci.push_back(l->podaci[i]);
				}
				for (int i = mid + 1; i < l->keys.size(); i++) {
					l->keys.pop_back();
					l->podaci.pop_back();
				}
				int additive = (l->children.size() % 2);
				for (int i = mw+ additive; i < l->children.size(); i++) {
					TreeNode* n = l->children[i];
					d->children.push_back(n);
					n->parent = d;
				}
				for (int i = mw+ additive-1; i < l->children.size(); i++) l->children.pop_back();
				d->right = l->right;
				l->right = d;
				if (s.empty()) {
					roditelj(l, d, m,p);
					break;
				}
				else
					razlomi(s.top(), l,d,l->keys[l->keys.size()-1], m,p);
			}

				
	}
}
void setuj(const char* ime_fajla,int m) {
	FILE* fin = fopen(ime_fajla, "r");
	ll p, d, t;
	char ime[400];
	double balance;
	int id;
	while (fscanf(fin, "%lld|%lld|%lld|%[^|]|%d|%lf", &p, &d, &t,ime,&id,&balance)==6) {
		podatak* temp = new podatak(p, d, t, ime, id, balance);
		dodaj(temp,m);		
	//	print_tree(root, m);
	}


	fclose(fin);
}
void add_to_file(const char* ime_fajla, podatak* p) {
	FILE* f = fopen(ime_fajla, "a");
	fprintf(f, "%lld|%lld|%lld|%s|%lld|%lf\n", p->ca_id, p->ca_b_id, p->ca_c_id, p->ca_name.c_str(), p->ca_tax_st, p->ca_bal);
	fclose(f);
}
void ispis_podatka(podatak* p) {
	printf("%lld|%lld|%lld|%s|%lld|%lf\n", p->ca_id, p->ca_b_id, p->ca_c_id, p->ca_name.c_str(), p->ca_tax_st, p->ca_bal);
}

void leaves(TreeNode* r) {
	if (!r)return;
	queue <TreeNode*> q;
	q.push(r);
	int h = height(r);
	int cnt = 0;
	while (!q.empty()) {
		if (h == cnt)break;
		int n = q.size();
		for (int i = 0; i < n; i++)
		{
			TreeNode* curr = q.front();
			q.pop();
			if (curr->parent != nullptr && curr->parent->keys.size() > 1) {
				if (curr->parent->keys[curr->parent->keys.size() - 1] == curr->keys[curr->keys.size() - 1])
				{
				
					curr->parent->keys.pop_back();
				}
			}

			for (auto x : curr->children)q.push(x);
		}
		cnt++;
	}
	int n = q.size();
	for (int i = 0; i < n - 1; i++) {
		TreeNode* curr= q.front();
		q.pop();
		curr->right = q.front();
	}
}
bool search(TreeNode* root, ll key, int& steps) {
	TreeNode* curr = root;
	int i;
	while (!curr->is_leaf) {
		steps++;
		 i= 0;
		while (i < curr->keys.size() && curr->keys[i] < key) i++;
		curr = curr->children[i];
	}
	for(int i=0;i<curr->keys.size();i++)
		if(curr->keys[i]==key) {
			ispis_podatka(curr->podaci[i]);
			add_to_file("output.txt", curr->podaci[i]);
			return true;
	}
	return false;
}
void find_sukc(TreeNode* root, ll key, int k,int &steps) {
	TreeNode* curr = root;
	int i;
	while (!curr->is_leaf) {
		steps++;
		i = 0;
		while (i < curr->keys.size() && curr->keys[i] < key) i++;
		curr = curr->children[i];
	}
	i = 0;
	while (i < curr->keys.size() && curr->keys[i] < key)i++;
	TreeNode* t = curr;
	for (int j = 0; j < k; j++) {
		if (!curr)return;
		if (i < curr->keys.size()) {
			ispis_podatka(curr->podaci[i]);
			add_to_file("output1.txt", curr->podaci[i]);
			i++;
		}
		else {
			curr = curr->right;
			i = 0;
			j--;
		}
	}
}
void uzmi_desnog(TreeNode* l, TreeNode* d,ll key) {
	ll aaa = d->keys[0];
	l->keys.push_back(d->keys[0]);
	l->podaci.push_back(d->podaci[0]);
	d->keys.erase(d->keys.begin());
	d->podaci.erase(d->podaci.begin());
	TreeNode* curr = l;
	while (curr) {
		for (int i = 0; i < curr->keys.size(); i++) {
			if (curr->keys[i] == key)curr->keys[i] = aaa;
		}
		curr = curr->parent;
	}
}
void uzmi_levog(TreeNode* l, TreeNode* d,ll key) {
	ll aaa = d->keys[d->keys.size() - 1];
	cout << aaa;
	l->keys.push_back(d->keys[d->keys.size()-1]);
	l->podaci.push_back(d->podaci[d->podaci.size() - 1]);
	d->keys.pop_back();
	d->podaci.pop_back();
	TreeNode* curr = l;
	while (curr) {
		for (int i = 0; i < curr->keys.size(); i++) {
			if (curr->keys[i] == key)curr->keys[i] = aaa;
		}
		curr = curr->parent;
	}
}
void merge(TreeNode* c,ll key) {
	TreeNode* otac = c->parent;
}
void pozajmi_od_brata(TreeNode* r,TreeNode* c,ll key,int m) {
	
	queue<TreeNode*> q;
	q.push(r);
	int flag = 0;
	int level = 0;
	while (!q.empty()) {
		int n = q.size();
		for (int i = 0; i < n; i++) {
			TreeNode* t = q.front();
			q.pop();
			for (auto x : t->children) 
				q.push(x);
			if (t == c) {
				flag = 1;
				break;
			}
		}
		if (flag == 1)break;
		level++;
	}
	while (!q.empty())q.pop();
	q.push(r);
	for (int i = 0; i < level; i++) {
		int n = q.size();
		for (int j = 0; j < n; j++) {
			TreeNode* t = q.front();
			q.pop();
			for (auto x : t->children)
				q.push(x);
		}
	}
	vector<TreeNode*> v;

		
	while (!q.empty()) {
		v.push_back(q.front());
		q.pop();
	}
	
	for (int i = 0; i < v.size(); i++) {
		
		if (v[i]->is_leaf && v[i]->keys.size() < m / 2) {
			if (i + 1 < v.size() && v[i+1]->keys.size()>m/2) {
				
				uzmi_desnog(c, v[i + 1], key);
			}
			else if (i - 1 > 0 && v[i - 1]->keys.size() > m / 2) {
				
				uzmi_levog(c, v[i - 1], key);
			}
			else
			{
				merge(c, key);
			}
			return;
		}
	

	}
	

}
void delete_val(TreeNode* root, vector<ll> vp,int m) {
	if (!root)return;
	TreeNode* curr = root;
	stack<TreeNode*> s;
	ll key = vp[vp.size() - 1];
	s.push(curr);
	while (!curr->is_leaf) {
		int i = 0;
		while (i < curr->keys.size() && curr->keys[i] < key) i++;
		curr = curr->children[i];
		s.push(curr);
	}
	while (!s.empty()) {
		int i = 0;
		curr = s.top();
		s.pop();
		while (i < curr->keys.size() && curr->keys[i] < key)i++;
		if (i == curr->keys.size())continue;
		if (curr->keys[i] == key) {
			curr->keys.erase(curr->keys.begin() + i);
			curr->podaci.erase(curr->podaci.begin() + i);
			if (curr->keys.size() < m / 2) {
				
				pozajmi_od_brata(root, curr, key, m);
				break;
			}
			else
			{
				ll aaa = curr->keys[curr->keys.size() - 1];
				TreeNode* curr1 = curr;
				while (curr1) {
					for (int i = 0; i < curr1->keys.size(); i++) {
						if (curr1->keys[i] == key)curr1->keys[i] = aaa;
					}
					curr1 = curr1->parent;
				}
			}
		}
		
	}
}
int main()
{
	int izbor;
	char* s = new char(100);
	vector<ll> bez;
	while (1) {
		

		cout << "1-Stvaranje i unistavanje indeksa nad tabelom iz zadatog imena fajla" << endl;//Done
		cout << "2-Ispis indeksa na standarni izlaz" << endl;//Done
		cout << "3-Dodavanje novog zapisa u tabelu" << endl;//Done
		cout << "4-Brisanje zapisa iz tabela" << endl;//Done
		cout << "5-Pretrazivanje podataka" << endl;//Done
		cout << "6-Pretrazivanje k-podataka" << endl;//Done
		cout << "7-Pretrazivanje k sukcesivnih podataka" << endl;//Done
		cout << "8-Izlaz" << endl;//Done
		cout << "Vas izbor: ";
		cin >> izbor;
		cout << endl << endl;
		cout << endl << endl;
		switch (izbor)
		{
		case 1: {
			
			//cout << "Ime fajla: ";
			//cin >> s;
			setuj("CustomerAccount20.txt", 3);
			leaves(root);
			cout << "Uspesno kreiranje B+ stabla!" << endl<<endl;
			break;
		}
		case 2: {

			print_tree_v2(root);
			cout << endl;
		    print_tree(root, 3);

			break;
		}
		case 3: {
			
			ll p, d, t;
			char ime[400];
			int id;
			double balance;
			cout << "CA_ID: "; cin >> p;
			cout << "CA_B_ID: "; cin >> d;
			cout << "CA_C_ID: "; cin >> t;
			cout << "CA_NAME: "; cin >> ime;
			cout << "CA_TAX_ST: "; cin >> id;
			cout << "CA_BAL: "; cin >> balance;
			podatak* temp = new podatak(p, d, t, ime, id, balance);
			dodaj(temp, 3);
			//add_to_file(s,temp);
			break;
			
		}
		case 4: {
			ll key;
			cout << "Kljuc: ";
			cin >> key;
			bez.push_back(key);
			delete_val(root, bez,3);
			cout << "Brisanje uspesno" << endl;
			break;
		}
		case 5: {
			ll key;
			cout << "Kljuc: ";
			cin >> key;
			bool flag;
			int steps = 0;
			flag = search(root, key, steps);
			if (flag)cout<<"Uspesna pretraga u " << steps << " koraka!" << endl;
			else cout << "Neuspesna pretraga u " << steps << " koraka!" << endl;
			break;
		}
		case 6: {
			FILE* f = fopen("output.txt", "w");
			fclose(f);
			int k;
			vector<ll> k_data;
			cout << "Unesite k: ";
			cin >> k;
			cout << "Unesite podatke: " << endl;
			for (int i = 0; i < k; i++) {
				ll x;
				cin >> x;
				k_data.push_back(x);
			}
			int suma = 0;
			int cnt = 0;
			bool flag;
			for (auto x : k_data) {
				cnt = 0;
				flag = search(root, x, cnt);
				if (flag)cout << x<<" - " << "Uspesna pretraga u " << cnt << " koraka!" << endl;
				else cout << x << " - " << "Neuspesna pretraga u " << cnt << " koraka!" << endl;
				suma += cnt;
			}
			cout << "Ukupno broj koraka: " << suma << endl;
			break;
		}
		case 7: {
			FILE* f = fopen("output1.txt", "w");
			fclose(f);
			int k;
			cout << "Unesite k: ";
			cin >> k;
			ll key;
			cout << "Kljuc: ";
			cin >> key;
			int steps = 0;
			find_sukc(root,key,k,steps);
			break;
		}
		case 8: {
			exit(0);
			break;
		}

		default: {
			cout << "Nepostojeci izbor"<<endl;
			break;
		}
			
		}

	}

}
/* 

40|4300000001|4300000001|Joshua Fowle Savings Account|1|6334235.05
20|4300000003|4300000001|Joshua Fowle Play Money|2|2731646.43
30|4300000004|4300000001|Joshua Fowle Emergency Expenses|2|7625683.59
25|4300000005|4300000001|Joshua Fowle Vacation Account|2|79823.44
35|4300000010|4300000001|Joshua Fowle Healthcare Fund|2|1104436.03
22|4300000008|4300000001|Joshua Fowle New Car Account|1|1594842.08
45|4300000004|4300000002|Willie Swigert Joint Account|1|-9093725.95
37|4300000003|4300000002|Willie Swigert Business Account|1|9288893.44
10|4300000007|4300000002|Willie Swigert College Fund|1|8096128.80
18|4300000006|4300000002|Willie Swigert Flexible Spending|0|9638415.62
24|4300000001|4300000002|Willie Swigert Play Money|1|-1877283.56




*/