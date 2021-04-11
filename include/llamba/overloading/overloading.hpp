#ifndef __OVERLOADING_HPP__
#define __OVERLOADING_HPP__


constexpr unsigned long long operator"" _THREADS(unsigned long long _threads)
{ 
    return  _threads; 
}

constexpr unsigned long long operator"" _PRIORITY(unsigned long long _priority)
{
    return _priority;
}


#endif