#include "Tuple.h"

Tuple::Tuple()
{
  pid = 0;
  burst = 0;
  priority = 0;
}

Tuple::Tuple(int pid, int burst, int priority)
{
  this->pid = pid;
  this->burst = burst;
  this->priority = priority;
}

Tuple::~Tuple()
{
  pid = 0;
  burst = 0;
}
int Tuple::GetPID() const
{
  return pid;
}
int Tuple::GetBurst() const
{
  return burst;
}
int Tuple::GetPriority() const
{
  return priority;
}
void Tuple::SetPID(int pid)
{
  this->pid = pid;
}
void Tuple::SetBurst(int burst)
{
  this->burst = burst;
}
void Tuple::SetPriority(int priority)
{
  this->priority = priority;
}

