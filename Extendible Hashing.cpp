#include <bits/stdc++.h>
using namespace std;

struct bucket_list{
	int Local_Depth;
	list<int> ls;
	bucket_list(int n){
		this -> Local_Depth = n;
	}	
};

class Extendible_Hashing{
	private:
		int Bucket_Size;
		int Global_Depth;
		vector<bucket_list*> Directory;
	public:
		Extendible_Hashing(int Bucket_Size){
			Global_Depth = 1;
			Directory.resize((1<<Global_Depth));
			this -> Bucket_Size = Bucket_Size;
			for(int i = 0; i < (1 << Global_Depth); i++){
				Directory[i] = new bucket_list(Bucket_Size);
				Directory[i] -> Local_Depth = 1;
			}
		}
		
		int Hash_Function(int element, int k){
			int res = 0;
			for(int i = 0; i < k; i++){
				if((element >> i)&1){
					res += (1 << i);
				}
			}
			return res;
		}
		
		void insert(int element){
			int index = Hash_Function(element, Global_Depth);
			int local_depth = Directory[index]->Local_Depth;
			int nxt = (element | (1 << local_depth));
			
			int split = Hash_Function(element, local_depth);
			
			if(Directory[index]->ls.size() == Bucket_Size){
				
				auto it = Directory[index] -> ls.begin();
				
				list<int> temp_list;
				
				while(it != Directory[index]->ls.end()){
					if(((*it)&(1 << local_depth)) == (1 << local_depth)){
						temp_list.push_back(*it);
						it = Directory[index]->ls.erase(it);		
					}
					else{
						it++;
					}
				}
				
				bucket_list *tmp = new bucket_list(Bucket_Size);
				tmp->Local_Depth = local_depth + 1;
				tmp->ls = temp_list;
				
				if(local_depth < Global_Depth){
					for(int i = 0; i < (1 << (Global_Depth)); i++){
						int mask = ((i^index) & ((1 << (local_depth + 1))  - 1));
						if(mask == (1 << local_depth)){
							Directory[i] = tmp;	
						}
						else if(mask == 0){
							Directory[i] = Directory[index];
						}
					}
					Directory[index]->Local_Depth = local_depth+1;
					insert(element);
				}
				else{
					for(int i = 0; i < (1 << Global_Depth); i++){
						if(i == index){
							Directory.push_back(tmp);
						}
						else{
							Directory.push_back(Directory[i]);
						}
					}
					
					this->Global_Depth = this->Global_Depth+1;
					Directory[index]->Local_Depth = local_depth+1;
					insert(element);
				}
			}
			else{
				Directory[index]->ls.push_back(element);
			}
		}
		
		void Delete(int element,bool done){
			int index = Hash_Function(element, Global_Depth);
			if(!done){
				auto it = find(Directory[index]->ls.begin(), Directory[index]->ls.end(), element);
				if(it == Directory[index]->ls.end()){
					cout << "No Such Element Present\n";
					return;
				}
				Directory[index]->ls.erase(it);
			}
			
			bool ok = true;
			
			for(int i = 0; i < (1 << Global_Depth); i++){
				if(Directory[i] -> Local_Depth == Global_Depth){
					ok = false;
				}
			}
			
			if(ok){
				Directory.erase(Directory.begin() + (1 << Global_Depth - 1), Directory.end());
				Directory.resize((1 << (Global_Depth-1)));
				this->Global_Depth = this->Global_Depth - 1;
			}
			
			int hes = Hash_Function(element, Directory[index]->Local_Depth);
			int mask = ((1 << (Directory[index]->Local_Depth-1))^(hes));
			
			if(Directory[index] -> ls.size() + Directory[mask] -> ls.size() <= Bucket_Size){
				for(auto c: Directory[mask] -> ls){
					Directory[index] -> ls.push_back(c);
				}
				Directory[index] -> Local_Depth = Directory[index] -> Local_Depth - 1;
				delete Directory[mask];
				Directory[mask] = Directory[index];
				Delete(element, true);
			}
			else{
				if(!ok){
					return;
				}
				else{
					Delete(element, true);
				}
			}			
		}
		
		void Display(){
			
			cout << "Global_Depth: " << Global_Depth << "\n";
			
			for(int i = 0; i < (1 << Global_Depth); i++){
				
				for(int j = Global_Depth; j >= 0; j--){
					cout << ((i >> j)&1);
				}
				
				cout << " ";
				
				cout << "Local_Depth: " << Directory[i] -> Local_Depth << ", ";
				
				cout << "[";
				
				auto it = Directory[i]->ls.begin();
				while(it != Directory[i]->ls.end()){
					cout << (*it) << ",";
					it++;
				}
				
				cout << "]";
				
				cout << endl;
			}
			cout << endl;
		}
		
		
		~Extendible_Hashing(){
			Directory.clear();
		}
	
};

int main(){
	Extendible_Hashing EH(2);
		
	vector<int> v1 = {45,56,60,72,12,31,29,57,11};
	vector<int> v2 = {45, 56, 12, 31, 11};
	
	for(auto c: v1){
		cout << "Inserting: " << c << endl;
		EH.insert(c);
		EH.Display();
	}
	
	cout << "DELETION\n\n";
	
	for(auto c: v2){
		cout << "Deleting: " << c << endl;
		EH.Delete(c, false);
		EH.Display();
	}
}