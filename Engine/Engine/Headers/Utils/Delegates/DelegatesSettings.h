#pragma once
#include "MultiCastDelegate.h"

#define DECLARE_DELEGATE(DelegateName, ...) typedef delegate<void(__VA_ARGS__)> DelegateName;
#define DECLARE_MULTICAST_DELEGATE(DelName, ...) typedef multicast_delegate<typename void(__VA_ARGS__)> DelName; DECLARE_DELEGATE(DelName ## _delegate, __VA_ARGS__);
#define ADD_MULTICAST_DELEGATE(DelType, DelRef, Class, Method, Instance) DelType = DelType.create<Class, Method>(Instance); DelRef += DelType;
#define ADD_DELEGATE(DelRef, Class, Method, Instance) DelRef = DelRef.create<Class, Method>(Instance)
#define REMOVE_DELEGATE(DelRef); DelRef = NULL