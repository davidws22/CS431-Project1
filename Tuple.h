//struct definition to store objects that contain pid and bursttime
#ifndef TUPLE_H
#define TUPLE_H

struct Tuple {
  int pid;
  int burst;
  int priority;

  //default constructor
  Tuple();
  Tuple(int pid, int burst, int priority);

  //destructor
  ~Tuple();

  //Accessors
  int GetBurst() const;
  int GetPID() const;
  int GetPriority() const;
  
  //Mutators
  void SetBurst(int burst);
  void SetPID(int pid);
  void SetPriority(int priority);

  //overloaded operator to sort Tuples
  bool operator< (const Tuple& T) const
  {
    return (burst < T.burst);
  }
};
  
#endif
