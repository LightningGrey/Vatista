#pragma once
#include <memory>
#include <string>

#define NoCopy(TypeName) \
	TypeName(const TypeName& other) = delete; \
	TypeName& operator =(const TypeName& other) = delete;

#define NoMove(TypeName) \
	TypeName(const TypeName&& other) = delete; \
	TypeName& operator =(const TypeName&& other) = delete;

#define SharedPtr(TypeName)\
	typedef std::shared_ptr<TypeName> Sptr;\
	NoMove(TypeName);\
	NoCopy(TypeName); \
	 std::string DebugName;
