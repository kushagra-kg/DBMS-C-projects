 #include <bits/stdc++.h>
using namespace std;

struct node{
	int maxSize;
	bool isLeaf;
	list<int> keys;
	list<node*> ptrs;
	node(int n, bool f){
		maxSize = n;
		isLeaf = f;
		ptrs.push_back(NULL);
	}
};

class BPlusTrees{
	private:
		int m,mf;
		node* root;
	public:
		BPlusTrees(int m , int mf){
			this->m = m;
			this->mf = mf;
			root = new node(mf, true);
		}
		
		pair<int, node*> insert(int key, node* cur){
			if(cur->isLeaf){
				auto keyit = cur->keys.begin();
				auto ptrit = cur->ptrs.begin();
				while(keyit != cur->keys.end() and *(keyit) < key){
					keyit++;
					ptrit++;
				}
					
				cur->keys.insert(keyit,key);
				cur->ptrs.insert(ptrit,NULL);
				
				keyit =cur->keys.begin();
				ptrit = cur->ptrs.begin();
				
				if(cur->keys.size() > mf){
					int cnt = 0;
					while(cnt < (mf + 2)/2){
						keyit++;
						ptrit++;
						cnt++;
					}
					
					node* split = new node(mf, true);
					
					auto spkeyit = split->keys.begin();
					auto spptrit = split->ptrs.begin();
					
					while(keyit != cur->keys.end()){
						spkeyit = split->keys.insert(spkeyit, *(keyit));
						spptrit = split->ptrs.insert(spptrit, *(ptrit));
						spkeyit++; spptrit++;
						keyit = cur->keys.erase(keyit);
						ptrit = cur->ptrs.erase(ptrit);
					}
					split->ptrs.back() = *(ptrit);
					*(ptrit) = split;
					node* nptr = split;
					return {cur->keys.back(), nptr};
				}
				return {-1, NULL};
			}
			else{
				
				auto keyit = cur->keys.begin();
				auto ptrit = cur->ptrs.begin();
				
				while(keyit != cur->keys.end() and *(keyit) < key){
					keyit++;
					ptrit++;
				}
				
				auto child = insert(key, *ptrit);
				
				if(child.second != NULL){
					keyit = cur->keys.begin();
					ptrit = cur->ptrs.begin();
					
					while(keyit != cur->keys.end() and *(keyit) < child.first){
						keyit++;
						ptrit++;
					}
					ptrit++;
					
					cur->keys.insert(keyit, child.first);
					cur->ptrs.insert(ptrit, child.second);
					
					
					if(cur->keys.size() > m-1){
						
						keyit = cur->keys.begin();
						ptrit = cur->ptrs.begin();
						
						int cnt = 1;
						while(cnt < ((m + 1)/2)){
							keyit++;
							ptrit++;
							cnt++;
						}
						
						int retkey = *keyit;	
						keyit = cur->keys.erase(keyit);
						ptrit++;
						
						node* split = new node(m, false);
						
						auto spkeyit = split->keys.begin();
						auto spptrit = split->ptrs.begin();
						
						while(keyit != cur->keys.end()){
							spkeyit = split->keys.insert(spkeyit, *(keyit));
							spptrit = split->ptrs.insert(spptrit, *(ptrit));
							spkeyit++; spptrit++;
							keyit = cur->keys.erase(keyit);
							ptrit = cur->ptrs.erase(ptrit);
						}
						
						split->ptrs.back() = *(ptrit);
						
						cur->ptrs.erase(ptrit);
						
						node* ptr = split;
						return {retkey, ptr};			
					}
					
				}
				return {-1, NULL};
			}
		}
		
		void remove(int key, node* cur){
			if(cur->isLeaf){
				auto keyit = cur->keys.begin();
				auto ptrit = cur->ptrs.begin();
				while(keyit != cur->keys.end() and *(keyit) < key){
					keyit++;
					ptrit++;
				}

				if(*keyit == key){
					cur->keys.erase(keyit);
					cur->ptrs.erase(ptrit);
				}
			}
			else{
				auto keyit = cur->keys.begin();
				auto ptrit = cur->ptrs.begin();
				
				while(keyit != cur->keys.end() and *(keyit) < key){
					keyit++;
					ptrit++;
				}
				
				remove(key, *ptrit);
				
				if((*ptrit)->isLeaf){
					if((*ptrit)->keys.size() < (mf+1)/2){
						auto rightptr = ptrit;
						++rightptr;
						auto leftptr = ptrit;
						if( rightptr != cur->ptrs.end() and (*rightptr)->keys.size() > (mf+1)/2 ){
							(*ptrit)->keys.push_back(*((*rightptr)->keys.begin()));
							(*ptrit)->ptrs.insert(--((*ptrit)->ptrs.end()), *((*rightptr)->ptrs.begin()));
							(*rightptr)->keys.erase((*rightptr)->keys.begin());
							(*rightptr)->ptrs.erase((*rightptr)->ptrs.begin());
							
							*(keyit) = ((*ptrit)->keys.back());
						}
						else if(ptrit != cur->ptrs.begin() and (*(--leftptr))->keys.size() > (mf + 1)/2){
							(*ptrit)->keys.insert((*ptrit)->keys.begin(), (*leftptr)->keys.back());
							(*ptrit)->ptrs.insert((*ptrit)->ptrs.begin(), *(----((*leftptr)->ptrs.end())));
							(*leftptr)->keys.erase(--(*leftptr)->keys.end());
							(*leftptr)->ptrs.erase(----(*leftptr)->ptrs.end());
							
							auto leftkey = keyit;
							--leftkey;
							
							*(leftkey) = *((*leftptr)->keys.begin());
						}
						else if(rightptr != cur->ptrs.end()){
							(*ptrit)->ptrs.pop_back();
							(*ptrit)->ptrs.splice((*ptrit)->ptrs.end(), (*rightptr)->ptrs);
							(*ptrit)->keys.splice((*ptrit)->keys.end(), (*rightptr)->keys);
							
							cur->keys.erase(keyit);
							delete *(++ptrit);
							cur->ptrs.erase(ptrit);
						}
						else if(ptrit != cur->ptrs.begin()){
							(*leftptr)->ptrs.pop_back();
							(*leftptr)->keys.splice((*leftptr)->keys.end(), (*ptrit)->keys);
							(*leftptr)->ptrs.splice((*leftptr)->ptrs.end(), (*ptrit)->ptrs);
							
							cur->keys.erase(--keyit);
							delete *(ptrit);
							cur->ptrs.erase(ptrit);
						}
					}
				}
				else{
					if((*ptrit)->ptrs.size() < (m+1)/2){
						auto rightptr = ptrit;
						++rightptr;
						auto leftptr = ptrit;
						
						if( rightptr != cur->ptrs.end() and (*rightptr)->ptrs.size() > (m + 1)/2 ){
							(*ptrit)->keys.push_back(*(keyit));
							(*ptrit)->ptrs.insert(((*ptrit)->ptrs.end()), *((*rightptr)->ptrs.begin()));
							
							*(keyit) = *((*rightptr)->keys.begin());
							
							(*rightptr)->keys.erase((*rightptr)->keys.begin());
							(*rightptr)->ptrs.erase((*rightptr)->ptrs.begin());
							
						}
						else if(ptrit != cur->ptrs.begin() and (*(--leftptr))->ptrs.size() > (m + 1)/2){
							
							auto leftkey = keyit;
							--leftkey;
							
							(*ptrit)->keys.insert((*ptrit)->keys.begin(), (*leftkey));
							(*ptrit)->ptrs.insert((*ptrit)->ptrs.begin(), (*leftptr)->ptrs.back());
							
							*(leftkey) = (*leftptr)->keys.back();
							
							(*leftptr)->keys.erase(--(*leftptr)->keys.end());
							(*leftptr)->ptrs.erase(--(*leftptr)->ptrs.end());
						
						}
						else if(rightptr != cur->ptrs.end()){
							(*ptrit)->keys.push_back(*keyit);
							(*ptrit)->ptrs.splice((*ptrit)->ptrs.end(), (*rightptr)->ptrs);
							(*ptrit)->keys.splice((*ptrit)->keys.end(), (*rightptr)->keys);
							
							cur->keys.erase(keyit);
							delete *(++ptrit);
							cur->ptrs.erase(ptrit);
						}
						else if(ptrit != cur->ptrs.begin()){
							(*leftptr)->keys.push_back(*(--keyit));
							(*leftptr)->keys.splice((*leftptr)->keys.end(), (*ptrit)->keys);
							(*leftptr)->ptrs.splice((*leftptr)->ptrs.end(), (*ptrit)->ptrs);
							
							cur->keys.erase(keyit);
							delete *(ptrit);
							cur->ptrs.erase(ptrit);
						}
						
					}
				}
			}
		}
		
		void display(node* cur){
			if(cur == NULL) return;
			
			for(auto c: cur->keys){
				cout << c << ' ';
			}
			
			if(cur->isLeaf) cout << "leaf";
			cout << endl << endl;
			for(auto c: cur->ptrs){
				display(c);
			}
		}
		
		void displays(){
			display(root);
		}
		 
		void inserts(int key){
			auto x = insert(key, root);
			if(x.second != NULL){
				node* nroot = new node(m, false);
				nroot->keys.push_front(x.first);
				nroot->ptrs.push_front(root);
				nroot->ptrs.back() = x.second;
				root = nroot;
			}
		}
		
		void removes(int key){
			remove(key, root);
			if(root->keys.size() == 0){
				node* nroot = *(root->ptrs.begin());
				delete root;
				root = nroot;
			}
		}
};

int main(){
	BPlusTrees bpt(3, 2);
	
	bpt.inserts(1);
	// bpt.display();
	bpt.inserts(2);
	// bpt.display();
	bpt.inserts(3);
	// bpt.display();
	bpt.inserts(4);
	// bpt.display();
	bpt.inserts(5);
	// bpt.display();
	bpt.inserts(6);
	// bpt.displays();
	bpt.inserts(7);
	// bpt.displays();
	bpt.inserts(8);
	
	bpt.inserts(9);
	
	// bpt.removes(20);
	
	// bpt.removes(22);
	
	// bpt.removes(24);
	
	// bpt.removes(14);
	
	// bpt.removes(25);
	
	// bpt.removes(23);
	
	// vector<int> v = {1,4,7,10,17,21,31,25,19,20,28,42};
	
	// for(auto c: v){
	// 	bpt.inserts(c);
	// }
	
	// bpt.removes(21);
	// bpt.removes(31);
	// bpt.removes(20);
	// bpt.removes(10);
	// bpt.removes(7);
	// bpt.removes(25);
	// bpt.removes(42);
	// bpt.removes(4);
	bpt.displays();	
}