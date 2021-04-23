#pragma once
#include "memory.hpp"

template<typename T>
union Container{
public:
    Container(): _{} {}
    Container(const T& value): _value(value) {_hasInit = true;}
    Container(T&& value): _value(value) {_hasInit = true;}
    
    Container& operator=(const T& value) {_value = value; return *this;}
    Container& operator=(T&& value) {_value = value; return *this;}
    operator T() {return _value;}

    template<typename ...ArgsT>
    void construct(ArgsT ...argsV) {
        if(_hasInit) _value.~T();
        new (&_value)T(argsV...);
        _hasInit = true;
    }

    ~Container() {if(_hasInit) _value.~T();}

    T* operator->() {
        return &_value;
    }

    bool hasInit() {return _hasInit;}
private:
    char _;
    struct {
        T _value;
        bool _hasInit;
    };
};