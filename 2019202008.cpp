#include <bits/stdc++.h>
#include <ctype.h>
#include <pthread.h> 
#include <unistd.h>
#include <time.h> 

using namespace std;
#define ll long long int

volatile int curr_running_threads = 0;
pthread_mutex_t curr_running_mutex = PTHREAD_MUTEX_INITIALIZER;

struct heapify
{
	ll filptr;
	vector<string> value;
};

struct threadstructure
{
	vector<vector<string>> a;
	ll low;
	ll high;
	string order;
	vector<string> columns;
	unordered_map<string, ll> ump;
};

//ll c[40];
vector<string> compareasc(vector<string> temp1, vector<string> temp2, vector<string> columns, unordered_map<string,ll> ump, ll &i, ll &j)
{
	vector<string> res;
	for(ll m =0; m<columns.size(); m++)
	{
		if(temp1.at(ump[columns.at(m)]) == temp2.at(ump[columns.at(m)]))
		{

		}
		else if(temp1.at(ump[columns.at(m)]) < temp2.at(ump[columns.at(m)]))
		{
//			cout<<"temp1 "<<i<<endl;
			res = temp1;
			i++;
			break;
		}
		else
		{
//			cout<<"temp2 "<<j<<endl;
			res = temp2;
			j++;
			break;
		}
//		cout<<"after comp "<<m<<endl;
	}
//	cout<<"before return"<<endl;
	return res;
}

vector<string> comparedesc(vector<string> temp1, vector<string> temp2, vector<string> columns, unordered_map<string,ll> ump, ll &i, ll &j)
{
	vector<string> res;
	for(ll m =0; m<columns.size(); m++)
	{
		if(temp1.at(ump[columns.at(m)]) == temp2.at(ump[columns.at(m)]))
		{

		}
		else if(temp1.at(ump[columns.at(m)]) > temp2.at(ump[columns.at(m)]))
		{
			res = temp1;
			i++;
			break;
		}
		else
		{
			res = temp2;
			j++;
			break;
		}
	}
	return res;
}

bool iscompareasc(vector<string> temp1, vector<string> temp2, vector<string> columns, unordered_map<string, ll> ump)
{
//	int count = 0;
	for(ll m =0; m<columns.size(); m++)
	{
//		cout<<"temp1 value "<<temp1.at(ump[columns.at(m)])<<endl;
//		cout<<"temp2 value "<<temp2.at(ump[columns.at(m)])<<endl;
		if(temp1.at(ump[columns.at(m)]) == temp2.at(ump[columns.at(m)]))
		{
//			count++;
			continue;
		}
		else if(temp1.at(ump[columns.at(m)]) < temp2.at(ump[columns.at(m)]))
		{
//			cout<<" returning true"<<endl;
			return true;
		}
		else
		{
//			cout<<"return false"<<endl;
			return false;
		}
	}
	// if(count == temp1.size())
// 	cout<<"return false "<<endl;
	return false;
}

bool iscomparedesc(vector<string> temp1, vector<string> temp2, vector<string> columns, unordered_map<string, ll> ump)
{
//	int count = 0;
	for(ll m =0; m<columns.size(); m++)
	{
		if(temp1.at(ump[columns.at(m)]) == temp2.at(ump[columns.at(m)]))
		{
	//		count++;
			continue;
		}
		else if(temp1.at(ump[columns.at(m)]) > temp2.at(ump[columns.at(m)]))
		{
			return true;
		}
		else
		{
			return false;
		}
		
	}
	// if(count == temp1.size())
	// 	return true;
	return false;
}

void mergesort(vector<vector<string>>& a,ll low, ll mid, ll high, string order, vector<string> columns, unordered_map<string,ll> ump)
{
	vector<vector<string>> c;
	ll i = low;
	ll j = mid+1;
	ll k = low;
//	cout<<"low "<<low<<" high "<<high<<" size of a"<<a.size()<<endl;
	if(order == "asc")
	{
		while(i<=mid && j<=high)
		{
			vector<string> temp1 = a.at(i);
			vector<string> temp2 = a.at(j);
//			cout<<"i "<<i<<" j "<<j<<endl;
			c.push_back(compareasc(temp1, temp2, columns, ump, i,j));
//			cout<<"after compare"<<endl;
		}

		while(i <= mid)
		{
//			cout<<"i "<<i<<endl;
			c.push_back(a.at(i));
			i++;
		}
		while(j <= high)
		{
//			cout<<"j "<<j<<endl;
			c.push_back(a.at(j));
			j++;
		}
//		cout<<"in asc "<<i<<" "<<j<<"size of c "<<c.size()<<endl;
		for(ll p=low; p<=high; p++)
		{
//			cout<<"p "<<p<<" c.at "<<c.at(p-low).at(0)<<" "<<c.at(p-low).at(1)<<" "<<c.at(p-low).at(2)<<endl;
			a.at(p) = c.at(p-low);
		}
	}
	else if(order == "desc")
	{
		while(i<=mid && j<=high)
		{
			vector<string> temp1 = a.at(i);
			vector<string> temp2 = a.at(j);
			c.push_back(comparedesc(temp1, temp2, columns, ump, i,j));
		}

		while(i <= mid)
		{
			c.push_back(a.at(i));
			i++;
		}
		while(j <= high)
		{
			c.push_back(a.at(j));
			j++;
		}
		for(ll p=low; p<=high; p++)
		{
			a.at(p) = c.at(p-low);
		}
	}
}

void merge(vector<vector<string>> &a,ll low, ll high,string order, vector<string> columns, unordered_map<string, ll> ump)
{
	ll mid;
	if(low != high)
	{
		mid = (low + high) / 2;
		merge(a,low,mid, order, columns, ump);
		merge(a,mid+1,high,order, columns, ump);
		mergesort(a,low, mid, high,order, columns, ump);
	}
}

void *merge_thread(void *pst)
{
	struct threadstructure *ps = (struct threadstructure *)pst;
	ll mid;
	if(ps->low != ps->high)
	{
		mid = (ps->low + ps->high) /2;
		merge(ps->a, ps->low, mid, ps->order, ps->columns, ps->ump);
		merge(ps->a,mid+1, ps->high, ps->order, ps->columns, ps->ump);
		mergesort(ps->a, ps->low, mid, ps->high, ps->order, ps->columns, ps->ump);
	}
}



void heapifyele(ll i, struct heapify heapelements[], ll outrptrcount, string order, vector<string> columns, unordered_map<string, ll> ump)
{
    ll smallest;
    ll leftchild = 2*i+1;
    ll rightchild = 2*i+2;
	if(order == "asc")
	{
		if(rightchild < outrptrcount && iscompareasc(heapelements[rightchild].value, heapelements[i].value, columns, ump))// karray[rightchild].num < karray[i].num)
		{
        	smallest = rightchild;
    	}
    	else
		{
        	smallest = i;
    	}           
    	if(leftchild < outrptrcount && iscompareasc(heapelements[leftchild].value, heapelements[smallest].value, columns, ump)) //karray[leftchild].num < karray[small].num)
		{
        	smallest = leftchild;
    	}            
    	if(smallest != i)
		{
        	heapify temp = heapelements[i];
        	heapelements[i] = heapelements[smallest];
        	heapelements[smallest] = temp;
        	heapifyele(smallest, heapelements, outrptrcount, order, columns, ump);
    	}
	}
	else if(order == "desc")
	{
		if(rightchild < outrptrcount && iscomparedesc(heapelements[rightchild].value, heapelements[i].value, columns, ump))// karray[rightchild].num < karray[i].num)
		{
        	smallest = rightchild;
    	}
    	else
		{
        	smallest = i;
    	}           
    	if(leftchild < outrptrcount && iscomparedesc(heapelements[leftchild].value, heapelements[smallest].value, columns, ump)) //karray[leftchild].num < karray[small].num)
		{
        	smallest = leftchild;
    	}            
    	if(smallest != i)
		{
        	heapify temp = heapelements[i];
        	heapelements[i] = heapelements[smallest];
        	heapelements[smallest] = temp;
        	heapifyele(smallest, heapelements, outrptrcount, order, columns, ump);
    	}
	}
}

void buildheap(struct heapify heapelements[], ll outrptrcount, string order, vector<string> columns, unordered_map<string, ll> ump)
{
    //heapsize = outrptrcount;            
    ll i;
    //karray = b;
    for(i=(outrptrcount-1)/2; i>=0; i--)
	{
        heapifyele(i, heapelements, outrptrcount, order, columns, ump);
    }            
}

int main(int argc, char** argv)
{
	char input[100];
	char output[100];
	ll size;
	ll numofthreads = 0;
	bool isthread = false;
	unordered_map<string,ll> ump;
	vector<string> columns;
	vector<int> columnssize;
	vector<string> largest_string;
	vector<string> smallest_string;
	FILE* metap = fopen("/media/indranil/New Volume1/second sem/DB/Assignment 2/Sample_test/metadata.txt","r");
	ifstream metafile("/media/indranil/New Volume1/second sem/DB/Assignment 2/Sample_test/metadata.txt");
	string mline;
	ll index = 0;
	ll rowsize = 0;
	string order;

	cout<<"Start execution"<<endl;

	if(atoi(argv[4]) <= 0)
	{
		cout<<"Not thread"<<endl;
		isthread = false;
		if(argc <= 5)
		{
			cout<<"Please provide proper argument"<<endl;
			return 0;
		}
	}
	else
	{
		cout<<"thread"<<endl;
		isthread = true;
		if(argc <= 6)
		{
			cout<<"Please provide proper argument"<<endl;
			return 0;
		}
	}

	clock_t starttime;
	
	if(isthread == false)
	{
		strcpy(input,argv[1]);
		strcpy(output, argv[2]);
		size = atoi(argv[3]);
		string temp(argv[4]);
		order = temp;
	}
	else
	{
		strcpy(input,argv[1]);
		strcpy(output, argv[2]);
		size = atoi(argv[3]);
		numofthreads = atoi(argv[4]);
		string temp(argv[5]);
		order = temp;
	}

	pthread_t threads[numofthreads];

	while(getline(metafile,mline))
	{
		char tempchar[mline.size()];
		for(int p = 0; p<mline.size();p++)
		{
			tempchar[p] = mline[p];
		}
		char* columnname = strtok(tempchar,",");
		string temp(columnname);
		ump[columnname] = index;
		char* colsize = strtok(NULL,",");
		string tempstr = "";
		string tempstr1 = "";
		for(int i =0; i< atoi(colsize); i++)
		{
			tempstr = tempstr+"~";
		}
		for(int i =0; i< atoi(colsize); i++)
		{
			tempstr1 = tempstr1+" ";
		}
		largest_string.push_back(tempstr);
		smallest_string.push_back(tempstr1);
		rowsize += atoi(colsize);
		columnssize.push_back(atoi(colsize));
		index++;
	}
//	columnssize.at(0) = columnssize.at(0)-2;
	if(isthread == false)
	{
		for(int i=5; i<argc; i++)
		{
			string temp(argv[i]);
			if(ump.find(temp) == ump.end())
			{
				cout<<"Column not found"<<endl;
				return 0;
			}
			columns.push_back(temp);
		}
	}
	else
	{
		for(int i=6; i<argc; i++)
		{
			string temp(argv[i]);
			if(ump.find(temp) == ump.end())
			{
				cout<<"Column not found"<<endl;
				return 0;
			}
			columns.push_back(temp);
		}
	}
	
	ll chunksize =0;
	if(isthread == false)
		chunksize = floor((size*1000000)/rowsize);
	else
		chunksize = floor((size*1000000)/(rowsize*numofthreads));
	
	ll count =0;
	ll outptrcount = 0;
//	FILE* infp = fopen(input, "r");
	ifstream infp(input);
	ifstream file(input);
	string line;
    while (getline(file, line))
        count++;
    file.close();

    ll loopcount = count / chunksize;
    ll rem = count % chunksize;

    if(rem == 0)
    	outptrcount = loopcount;
    else
    	outptrcount = loopcount + 1;

    FILE* outptrfile[outptrcount];
  //  cout<<"outptrcount "<<outptrcount<<endl;
    char outfile[100000]; 
    for (ll i = 0; i < outptrcount; i++) 
    {  
        snprintf(outfile, sizeof(outfile), "%lld", i);  
        outptrfile[i] = fopen(outfile, "w"); 
    } 
    memset(outfile,0, sizeof(outfile));
    //vector<vector<string>> sortarr;
    heapify heap[outptrcount];
//    cout<<"loopcpunt "<<loopcount<<" chunksize "<<chunksize<<endl;
    ll currptr=0;
    ll l=0;
    while(loopcount > 0)
    {
		cout<<"Splitting sublist "<<currptr<<endl;
		vector<vector<string>> sortarr;
    	for(ll i=0; i<chunksize; i++)
    	{
		//	char temp[rowsize+200];
		//	fscanf(infp,"%s", temp);
			string line;
    	//	char temp[rowsize+200];
			//fscanf(infp,"%s", temp);
			getline(infp,line);
			char temp[line.size()+1];
			strcpy(temp, line.c_str());
			string s2(temp);
		//	cout<<"s2 "<<s2<<endl;
			vector<string> tempstr;
			int start =0;
			for(int s =0; s<columnssize.size(); s++)
			{
		//		cout<<"columnsize "<<columnssize.at(s)<<" start "<<start<<endl;
				string newstring = s2.substr(start,columnssize.at(s));
		//		cout<<" newstring "<<newstring<<endl;
				tempstr.push_back(newstring);
				start= start+columnssize.at(s)+2;
			}
			//cout<<"before sort "<<temp<<" i "<<i<<endl;
			memset(temp,0, sizeof(temp));
			sortarr.push_back(tempstr);
    	}
    // 	cout<<"here"<<endl;
		cout<<"Sorting sublist "<<currptr<<endl;
		if(isthread == false)
    		merge(sortarr,0,chunksize-1,order,columns,ump);
		else
		{
			pthread_t threadid;
			pthread_mutex_lock(&curr_running_mutex);
     		curr_running_threads++;
			threadid++;
     		pthread_mutex_unlock(&curr_running_mutex);

			threadstructure ps;
			ps.a = sortarr;
			ps.columns = columns;
			ps.low = 0;
			ps.high = chunksize-1;
			ps.order = order;
			ps.ump = ump;
			
			while(curr_running_threads >= numofthreads)
			{
				usleep(1);
			}

			pthread_create(&threadid, NULL, &merge_thread, (void *)&ps);
			pthread_join(threadid, NULL);

			pthread_mutex_lock(&curr_running_mutex);
			curr_running_threads--;
			pthread_mutex_unlock(&curr_running_mutex);

			sortarr = ps.a;
		}
		
    //	cout<<"here 2"<<endl;
		int count =0;
    	// for(ll i = 0; i<chunksize ; i++)
    	// {
    //		cout<<currptr<<" "<<sortarr[i]<<endl;
			cout<<"Writing to disk sublist "<<currptr<<endl;
			for(ll m =0; m < sortarr.size(); m++)
			{
				for(ll n = 0; n<sortarr[0].size(); n++)
				{
					fprintf(outptrfile[currptr],"%s", sortarr[m].at(n).c_str());
					if(n != sortarr[0].size() -1)
					{
						fprintf(outptrfile[currptr],"%s","  ");
					}
				}
				fprintf(outptrfile[currptr],"\n");
			}
    //	}
   // 	cout<<"currptr "<<currptr<<endl;
    	currptr++;
    	loopcount--;
    //	memset(sortarr,0, sizeof(sortarr));
    }
//    cout<<"rowsieze "<<rowsize<<endl;
    if(rem  != 0)
    {
		cout<<"Splitting sublist "<<currptr<<endl;
    	vector<vector<string>> nsortarr;
  //  	cout<<"rem "<<rem;
    	for(ll i=0; i<rem; i++)
    	{
			string line;
    	//	char temp[rowsize+200];
			//fscanf(infp,"%s", temp);
			getline(infp,line);
			char temp[line.size()+1];
			strcpy(temp, line.c_str());
	//		cout<<"temp "<<temp;
			string s2(temp);
	//		cout<<"s2 "<<s2<<endl;
			vector<string> tempstr;
			int start =0;
			for(int s =0; s<columnssize.size(); s++)
			{
		//		cout<<"columnsize "<<columnssize.at(s)<<" start "<<start<<endl;
				string newstring = s2.substr(start,columnssize.at(s));
		//		cout<<" newstring "<<newstring<<endl;
				tempstr.push_back(newstring);
				start= start+columnssize.at(s)+2;
			}
			//}
		//	cout<<endl;
			memset(temp,0, sizeof(temp));
			nsortarr.push_back(tempstr);
    	}
	//	cout<<"before size"<<nsortarr.size()<<endl;
		cout<<"Sorting sublist "<<currptr<<endl;
    	merge(nsortarr,0,rem-1,order,columns,ump);
	//	cout<<"after merge size"<<nsortarr.size()<<endl;
	//	int count =1;
    	// for(ll i = 0; i<rem ; i++)
    	// {
			cout<<"Writing to disk sublist "<<currptr<<endl;
  	 		for(ll m =0; m < nsortarr.size(); m++)
			{
			//	cout<<count<<endl;
			//	count++;
				for(ll n = 0; n < nsortarr[0].size(); n++)
				{
					fprintf(outptrfile[currptr],"%s", nsortarr[m].at(n).c_str());
					if(n != nsortarr[0].size() -1)
					{
						fprintf(outptrfile[currptr],"%s","  ");
					}
				}
				fprintf(outptrfile[currptr],"\n");
			}
    	//}
    } 

    for (ll i = 0; i < outptrcount; i++) 
    {  
        fclose(outptrfile[i]); 
    }  

    for (ll i = 0; i < outptrcount; i++) 
    {  
        snprintf(outfile, sizeof(outfile), "%lld", i);  
        outptrfile[i] = fopen(outfile, "r"); 
    } 

    ll element =0;
//	struct heapify heapelements[outptrcount];
	cout<<"Merging"<<endl;
	for(ll i=0; i<outptrcount; i++)
    {
    //	fscanf(outptrfile[i],"%lld", &element);
		char temp[rowsize+100];
		fgets(temp,rowsize+100,outptrfile[i]);
		string s2(temp);
		vector<string> element;
    	// char *token = strtok(temp, ","); 
    	// while (token != NULL) 
    	// { 
		// 	string s3(token);
    	// 	element.push_back(s3);
    	// 	token = strtok(NULL, ","); 
    	// }
//    	cout<<"element "<<s2<<endl;
		int start =0;
		for(int s =0; s<columnssize.size(); s++)
		{
		//	cout<<"columnsize "<<columnssize.at(s)<<" start "<<start<<endl;
			string newstring = s2.substr(start,columnssize.at(s));
//			cout<<" newstring "<<newstring<<endl;
			element.push_back(newstring);
			start= start+columnssize.at(s)+2;
		}
    	heapify newheap;
    	newheap.value = element;
    	newheap.filptr = i;
    //	insertheap(heap,newheap,i, order, columns, ump);
		heap[i] = newheap;
    } 

	buildheap(heap,outptrcount, order, columns, ump);

    FILE* outfp = fopen(output, "w");
    ll filecount = outptrcount;

    while(count > 0)
    {

		heapify retheap = heap[0];
		//	cout<<"popped "<<retheap.value<<endl;
	//	fprintf(outfp, "%lld\n", retheap.value);
	//	cout<<"retvalue size "<<retheap.value.size()<<endl;
		for(ll n = 0; n<retheap.value.size(); n++)
		{
			fprintf(outfp,"%s", retheap.value.at(n).c_str());
			if(n != retheap.value.size() -1)
			{
				fprintf(outfp,"%s","  ");
			}
		}
	//	cout<<endl;
		fprintf(outfp,"\n");
		char ele[rowsize+100];
		heapify newheap;
		if(fgets(ele,rowsize+100,outptrfile[retheap.filptr]) == NULL)
		{
			if(order == "asc")
				heap[0].value = largest_string;
			else if(order == "desc")
				heap[0].value = smallest_string;
		//	cout<<"new ele "<<ele<<endl;
		}
		else
		{
		//	newheap.filptr = retheap.filptr;
			vector<string> element;
			string s2(ele);
    		int start =0;
			for(int s =0; s<columnssize.size(); s++)
			{
			//	cout<<"columnsize "<<columnssize.at(s)<<" start "<<start<<endl;
				string newstring = s2.substr(start,columnssize.at(s));
			//	cout<<" newstring "<<newstring<<endl;
				element.push_back(newstring);
				start= start+columnssize.at(s)+2;
			}
		//	newheap.value = element;
//			heap[0] = heap[filecount-1];
//		if(filecount > count)
//			filecount = count;
//			heap[filecount-1] = newheap;
			heap[0].value = element;
		}
		heapifyele(0, heap, outptrcount, order, columns, ump);
		count--;
		//insertheap(heap,newheap,0);
		//count--;
		//if(filecount > count)
		//	filecount = count;
    }

    for (ll i = 0; i < outptrcount; i++) 
    {  
        fclose(outptrfile[i]); 
    }  
    fclose(outfp);
    infp.close();

	clock_t endtime; 
    endtime = clock()-starttime;
	double timetaken = (double)endtime/1000000;
	cout<<"Time taken to sort "<<timetaken<<endl;
	return 0;
}