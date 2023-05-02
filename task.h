#ifndef TASK_H
#define TASK_H

#include <event.h>

class Task
{
public:
    Task() = default;
    virtual ~Task() = default;
    virtual void Init(event_base *base) = 0; 
};

#endif
