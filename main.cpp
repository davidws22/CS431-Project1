#include <iostream>
#include <fstream> //for file I/O
#include <cstdlib> //for exit() and random number generator
//#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <sstream>  // for string streams
#include "Tuple.h"
using namespace std;
#define SWAP 3
#define QUANTA 50

void Batch_FirstComeFirstServe(ifstream& fin, string fileName);
void Batch_ShortestJobFirst(ifstream& fin, string fileName);
void RoundRobbin(ifstream& fin, string fileName, int quanta = QUANTA);
void Lottery(ifstream& fin, string fileName);


int main()
{
  ifstream fin;
  string myFiles[] = {"testdata1.txt", "testdata2.txt", "testdata3.txt", "testdata4.txt"};
  //LEN is the number of elements in the array of Files.
  int LEN = sizeof(myFiles)/sizeof(myFiles[0]); 

  int x;
  //convert string to c-string
  for(int i = 0; i < LEN; i++)
    {
      fin.open(myFiles[i].c_str());
      if(!fin)
	{
	  cerr << "Unable to open file. . .\n";
	  exit(1);
	}
      Batch_FirstComeFirstServe(fin, myFiles[i]);
      Batch_ShortestJobFirst(fin, myFiles[i]);
      RoundRobbin(fin, myFiles[i], 25);
      RoundRobbin(fin, myFiles[i]);
      Lottery(fin, myFiles[i]);
      fin.close();
    }
  return 0;
}


void Batch_FirstComeFirstServe(ifstream& fin, string fileName)
{
  int cputime = 0;
  int completiontime = 0;
  ofstream csv;
  int lastindex = fileName.find_last_of(".");
  string rawname = fileName.substr(0, lastindex);
  int pid = 0;
  int burstTime = 0;
  int temp;
  int counter = 0;
  
  rawname = "FirstComeFirstServe-" + rawname + ".csv";
  csv.open(rawname.c_str());
  csv << "CPUTime,PID,StartingBurstTime,EndingBurstTime,CompletionTime,\n";
  while(fin >> pid >> burstTime >> temp)
    {
      csv << cputime << ",";
      csv << pid << ",";
      csv << burstTime << ",";
      csv << "0,";
      cputime += burstTime;
      completiontime += cputime;
      csv << cputime << ",\n";
      cputime += SWAP;
      ++counter;
    }
  csv << "Average Completion Time: " << ((double)completiontime)/counter << "\n";
  csv.close();
  
  fin.clear(); //clearing eof bit
  fin.seekg(fin.beg); // seek to beginning of file
  
}

void Batch_ShortestJobFirst(ifstream& fin, string fileName)
{
  vector<Tuple> processes;
  Tuple* filler;
  
  int cputime = 0;
  int completiontime = 0;
  ofstream csv;
  int lastindex = fileName.find_last_of(".");
  string rawname = fileName.substr(0, lastindex);
  int pid = 0;
  int burstTime = 0;
  int temp;
  
  rawname = "ShortestJobFirst-" + rawname + ".csv";
  csv.open(rawname.c_str());
  //takes pid and burst time from file and creating tuple objects then storing them into
  // a vector of tuples
  while(fin >> pid >> burstTime >> temp)
    {
      filler = new Tuple(pid, burstTime, temp);
      processes.push_back(*filler);
    }
  csv << "CPUTime,PID,StartingBurstTime,EndingBurstTime,CompletionTime,\n";
  //sort the vector of Tuples in non-descending order by Burst Time
  sort(processes.begin(), processes.end());

  for(int i = 0; i < processes.size(); i++)
    {
      csv << cputime << ",";
      csv << (processes[i]).pid << ",";
      csv << (processes[i]).burst << ",";
      csv << "0,";
      cputime += (processes[i]).burst;
      completiontime += cputime;
      csv << cputime << ",\n";
      cputime += SWAP;
    }
  
  csv << "Average Completion Time: " << ((double)completiontime)/processes.size() << "\n";
  csv.close();
  
  fin.clear(); //clearing eof bit
  fin.seekg(fin.beg); // seek to beginning of file
  
}

void RoundRobbin(ifstream& fin, string fileName, int quanta)
{ 
  vector<Tuple> processes;
  Tuple* filler;
  int lastindex = fileName.find_last_of(".");
  string rawname = fileName.substr(0, lastindex);
  int i = 0;
  int pid = 0;
  int burstTime = 0;
  int temp;
  int cputime = 0;
  int completiontime = 0;
  int length;
  int initial;
  long long  sum = 0;
  ofstream csv;
  ostringstream str;
  str << quanta;
  string s = str.str();
  bool flag = true;
  
  rawname = "RoundRobbin-" + s + "quanta-" + rawname + ".csv";
  csv.open(rawname.c_str());
  //takes pid and burst time from file and creating tuple objects then storing them into
  // a vector of tuples
  while(fin >> pid >> burstTime >> temp)
    {
      filler = new Tuple(pid, burstTime, temp);
      processes.push_back(*filler);
    }
  csv << "CPUTime,PID,StartingBurstTime,EndingBurstTime,CompletionTime,\n";
  length = processes.size();
  initial = processes.size();
  while(flag)
    {
      csv << cputime << ",";
      csv << (processes[i]).pid << ",";
      csv << (processes[i]).burst << ",";
      if(((processes[i]).burst - quanta) <  0)
	{
	  completiontime = cputime + processes[i].burst;
	  sum += completiontime;
	  csv << "0" << ",";
	  csv << completiontime << ",\n";
	  cputime = cputime + processes[i].burst + SWAP;
	  processes.erase(processes.begin() + i);
	  if(i >= 0)
	    --i;
	}
      else
	{
	  processes[i].burst -= quanta;
	  csv << processes[i].burst << ",";
	  csv << "-" << ",\n";
	  cputime = cputime + quanta + SWAP;
	}
      ++i;
      if(processes.size() == 0)
	flag = false;
      if(i == processes.size())
	i = 0;
    }
  csv << "Average Completion Time: " << sum/(double)initial << "\n";
  csv.close();

  fin.clear(); //clearing eof bit
  fin.seekg(fin.beg); // seek to beginning of file
}

void Lottery(ifstream& fin, string fileName)
{
  vector<Tuple> processes;
  Tuple* filler;
  int lastindex = fileName.find_last_of(".");
  string rawname = fileName.substr(0, lastindex);
  int i = 0;
  int pid = 0;
  int burstTime = 0;
  int priority;
  int cputime = 0;
  int completiontime = 0;
  int totalPrioritySum = 0;
  int sum;
  ofstream csv;
  int initial;
  bool flag = true;
  int ticket;
  srand(time(NULL));
  
  rawname = "Lottery-" + rawname + ".csv";
  csv.open(rawname.c_str());
  //takes pid and burst time from file and creating tuple objects then storing them into
  // a vector of tuples
  while(fin >> pid >> burstTime >> priority)
    {
      filler = new Tuple(pid, burstTime, priority);
      processes.push_back(*filler);
      totalPrioritySum += (*filler).priority;
    }
  ticket = rand() % totalPrioritySum + 1;
  //cout << "ticket = " << ticket << endl;
  csv << "CPUTime,PID,StartingBurstTime,EndingBurstTime,CompletionTime,\n";
  initial = processes.size();
  //cout << "size = " << initial << endl;
  while(flag)
    {
      ticket -= (processes[i]).priority;
      if(ticket > 0)
	{
	  ++i;
	}
      else
	{
	  ticket = rand() % totalPrioritySum + 1;
	  //cout << "ticket = " << ticket << endl;
	  csv << cputime << ",";
	  csv << (processes[i]).pid << ",";
	  csv << (processes[i]).burst << ",";

	  if(((processes[i]).burst - QUANTA) <  0)
	    {
	      completiontime = cputime + processes[i].burst;
	      sum += completiontime;
	      csv << "0" << ",";
	      csv << completiontime << ",\n";
	      cputime = cputime + processes[i].burst + SWAP;
	      totalPrioritySum -= processes[i].priority;
	      processes.erase(processes.begin() + i);
	    }
	  else
	    {
	      processes[i].burst -= QUANTA;
	      csv << processes[i].burst << ",";
	      csv << "-" << ",\n";
	      cputime = cputime + QUANTA + SWAP;
	    }
	  i = 0;
	}
      if(processes.size() == 0)
	flag = false;
      if(i == processes.size())
	i = 0;
      // cout << "processes size = " << processes.size() << endl;
    }
  csv << "Average Completion Time: " << ((double)sum)/initial << "\n";
  csv.close();

  fin.clear(); //clearing eof bit
  fin.seekg(fin.beg); // seek to beginning of file  
}
