#pragma once
#include "MultiCastDelegate.h"

#define DEFINE_DELEGATE(DelName, ...) delegate<__VA_ARGS__> DelName
#define DEFINE_MULTICAST_DELEGATE(DelName, ...) multicast_delegate<__VA_ARGS__> DelName; DEFINE_DELEGATE(DelName ## _delegate, __VA_ARGS__)
#define CREATE_DELEGATE(DelRef, Class, Method, Instance); DelRef = DelRef.create<Class, Method>(Instance)
#define CREATE_MULTICAST_DELEGATE(DelRef, Class, Method, Instance) DelRef ## _delegate.create<Class, Method>(Instance)
#define REMOVE_DELEGATE(DelRef); DelRef = NULL