/*
************************INPUT**********************
8
5
1 2 4 5 3 
35 65 32 54 23
80 54 21 2 45
8 4 2 7 23
23 54 7 87 2
23 4 6 8 23
23 54 7 8 32
4 56 8 2 0
3
*/

#include<bits/stdc++.h>
using namespace std;

struct chromo{
	int *chr;
};

struct student_data{
	int *st;
	int group = -1;
};
int variance(int a[], int n) 
{ 
    // Compute mean (average of elements) 
    int sum = 0; 
    for (int i = 0; i < n; i++) 
        sum += a[i]; 
    double mean = (double)sum /  
                  (double)n; 
  
    // Compute sum squared  
    // differences with mean. 
    double sqDiff = 0; 
    for (int i = 0; i < n; i++)  
        sqDiff += (a[i] - mean) *  
                  (a[i] - mean); 
    return sqDiff / n; 
} 

int fitness(int g[], int n, int k, int s[]){
	int count = 0;
	int a[n], var = 0;
	for(int i = 0; i < k; ++i){
		for(int j = 0; j < n; ++j){
			if(g[j] == i){
				a[count] = s[j];
				count++;
			}
		}
		if(count != 0)
			var += variance(a, count);
		count = 0;
	}
	return 1000000 - var/k;
}

int pop_chromo(vector<chromo> population, int n, int k, int student[]){
	
	int idx = 0, max;
	int *c;
	
	max = fitness(population[0].chr, n, k, student);

	for(int i = 0; i < population.size(); ++i){
		int flag = 0;
		for(int j = 0; j < k; ++j){
				
				for(int w = 0; w < n; ++w){
					if(population[i].chr[w] == j){
						flag = 1;
						break;
					}
				}
				if(flag == 0){
					
					flag = -1;
					break;
				}
				flag = 0;
			}
		if(max < fitness(population[i].chr, n, k, student) && flag != -1){
			
			max = fitness(population[i].chr, n, k, student);
			idx = i;
		}
	}
	return idx;
}


int main(){
	int n;
	cout << "number of students..." << endl;
	cin >> n;
	int sno;
	cout << "number of subjects..." << endl;
	cin >> sno;
	int *student = new int[n];
	student_data ss[n];
	for(int i = 0; i < n; ++i){
		cout << "enter marks of " << i << " student \n";
		ss[i].st = new int [sno];
		int s_avg = 0;
		for(int j = 0; j < sno; ++j){
			cin >> ss[i].st[j];
			s_avg += ss[i].st[j];
		}
		student[i] = s_avg/sno;
	}
	
	
	int k;
	cout << "number of groups to form \n";
	cin >> k;

	int *chromosome;
	chromosome = new int [n];
	int w = 0;
	
	
	vector <chromo> population;

	//populating data in population
	
	chromo c1, c2, c3;
	c1.chr = new int [n];
	c2.chr = new int [n];
	c3.chr = new int [n];
	 
	for(int i = 0; i < n; ++i){
		c1.chr[i] = w++ % k; 
	}
	population.push_back(c1);


	for(int i = 0; i < n; ++i){
		c2.chr[i] = rand() % k;
	}
	population.push_back(c2);


	for(int i = 0; i < n; ++i){
		c3.chr[i] = rand() % k;
	}
	population.push_back(c3);
	

	int count = 0;
	while(count++ < 200){
		
		//parent selection
		//Using Stochastic Universal Sampling (SUS) for parent selection
		
		//Step 1 : Calculate S = the sum of a finesses.
		
		int s = 0;
		for(int i = 0; i < population.size(); ++i){
			s += fitness(population[i].chr, n, k, student);
		}
		
		//Step 2 : Generate a random number between 0 and S.
		
		int rnp1 = rand() % s;
		int rnp2 = (rnp1+(s/2)) % s;
		
		//Step 3 : Starting from the top of the population, keep adding the finesses to the partial sum P, till P<S.
		//Step 4 : The individual for which P exceeds S is the chosen individual.
		
		int itr1 = 0;
		while(rnp1 -  fitness(population[itr1].chr, n, k, student) > 0){
			rnp1 = rnp1 - fitness(population[itr1].chr, n, k, student);
			++itr1;
		}
		
	
		int itr2 = 0; 
		while(rnp2 -  fitness(population[itr2].chr, n, k, student) > 0){
			rnp2 = rnp2 - fitness(population[itr2].chr, n, k, student);
			++itr2;
		}
		if(itr1 == itr2 ) {
			if(itr1 != population.size()-1) ++itr2;
			else --itr2;
		}
		
		chromo cpw1, cpw2;
		cpw1.chr = new int [n];
		cpw2.chr = new int [n];
		for(int i = 0; i < n; ++i){
			cpw1.chr[i] = population[itr1].chr[i];
			cpw2.chr[i] = population[itr2].chr[i];
		}
		population.push_back(cpw1);
		population.push_back(cpw2);
		
		//If you wanna look at the parents
		/*	cout << " choosing parents as ... \n";
		
		for(int i = 0; i < n; ++i){
			cout << population[itr1].chr[i] << "  ";
		}
		cout << endl;
		for(int j = 0; j < n; ++j){
			cout << population[itr2].chr[j] << "  ";
		}
		cout << endl;
		*/
				
		//crossover
		//Using Uniform Crossover method
		
		for(int i = 0; i < n; ++i){
			int toss = rand() % 2; 			//will generate 1 or 0, if to swap or not
			if(toss){
				int swp = population[itr1].chr[i];
				population[itr1].chr[i] = population[itr2].chr[i];
				population[itr2].chr[i] = swp;
			}
		}
		
		
		//mutation
		//Using Swap Mutation method
		
		int mut1 = rand() % n, mut2 = rand() % n;
		if(mut1 == mut2){
			mut2 = rand() % n;
		}
		
		int swp = population[itr1].chr[mut1];
		population[itr1].chr[mut1] = population[itr1].chr[mut2];
		population[itr1].chr[mut2] = swp;
		
		mut1 = rand() % n;
		mut2 = rand() % n;
		if(mut1 == mut2){
			mut2 = rand() % n;
		}
		
		swp = population[itr2].chr[mut1];
		population[itr2].chr[mut1] = population[itr2].chr[mut2];
		population[itr2].chr[mut2] = swp;
		
	}
	//Incase if you want to print the population!!
	/*
	cout << "population ... starts ..\n";
		for(int i = 0; i < population.size(); ++i){
			for(int j = 0; j < n; ++j){
				cout << population[i].chr[j] << "  ";
			}
			cout << "    fitness :  " << fitness(population[i].chr, n, k, student);
			cout << endl;
		}
	cout << "population ends..... \n";
	*/
	
	//extracting chromosome with maximum fitness
	
	chromo cp3;
	int idx = pop_chromo(population, n, k, student);
	cp3.chr = population[idx].chr;
	/*
	for(int j = 0; j < n; ++j){
				cout << cp3.chr[j] << "  ";
	}
	cout << "    fitness :  " << fitness(cp3.chr, n, k, student);
	cout << endl;
	*/
	cout << "\n\n\n**********************************\n\n";
	for(int i = 0; i < k; ++i){
		cout << " group " << i << endl;
		for(int j= 0; j < n; ++j){
		 	if(cp3.chr[j] == i)
				cout << "s" << j+1 << " ,";
		}
		cout << " \n";
	}
	cout << "\n\n**********************************\n\n";
		
}











