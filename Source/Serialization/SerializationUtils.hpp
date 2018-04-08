#pragma once

#define LOG_FILE "Serialization.log"

#include <Core/Utils/VA_ArgsUtils.hpp>
#include <array>
#include <tuple>
#include <string>
#include <sstream>
#include <vector>

#define DK_ADD_TAB(x)      \
for (int i = 0; i < x; ++i)    \
  ss << "\t";


#define DK_DATA_STRUCT()																		\
template<typename T>																			\
struct BaseType {																				\
	static size_t SetData(T& t, const char* c_str, size_t offset) {								\
		memcpy(&(t), c_str + offset, sizeof(T));												\
		return sizeof(T);																		\
	}																							\
	static std::tuple<void*, size_t> GetData(const T& t) {										\
		return std::make_tuple<void*, size_t>((void*)&(t), sizeof(T));							\
	}																							\
	static std::string ToJSON(const T& t, int indent) {											\
		return std::string("\"") + ValueToString<T>(t) + "\"";									\
	}																							\
};																								\
template<typename Type, size_t N>																\
struct BaseType<Type[N]> {																		\
	using T = Type[N];																			\
	static size_t SetData(T& t, const char* c_str, size_t offset) {								\
		memcpy((t), c_str + offset, sizeof(T));													\
		return sizeof(T);																		\
	}																							\
	static std::tuple<void*, size_t> GetData(const T& t) {										\
		return std::make_tuple<void*, size_t>((void*)(t), sizeof(T));							\
	}																							\
	static std::string ToJSON(const T& t, int indent) {											\
		std::string str("[\n");																	\
		for(size_t i = 0; i < N; ++i){															\
			for(int i2 = 0; i2 < indent; ++i2)													\
				str += '\t';																	\
			str += BaseType<Type>::ToJSON(t[i], indent + 1);									\
			str += ",\n";																		\
		}																						\
		str.erase(str.end() - 2);																\
		indent -= 1;																			\
		for(int i = 0; i < indent; ++i)															\
			str += '\t';																		\
		str += "]\n";																			\
		return str;																				\
	}																							\
};																								\
template<typename Type>																			\
struct BaseType<Type*> {																		\
	using T = Type*;																			\
	static size_t SetData(T& t, const char* c_str, size_t offset) {								\
		char isAllocated = *(c_str + offset);													\
		if(isAllocated){																		\
			t = new std::remove_pointer_t<T>();													\
			memcpy(t, c_str + offset + 1, sizeof(std::remove_pointer_t<T>));					\
			return sizeof(T) + 1;																\
		}																						\
		else{																					\
			t = nullptr;																		\
			return 1;																			\
		}																						\
	}																							\
	static std::tuple<void*, size_t> GetData(const T& t) {										\
		char* data = new char[sizeof(T) + 1], size;												\
		if (size = (bool)t) {																	\
			memcpy(data, &size, 1);																\
			memcpy(data + 1, t, sizeof(t));														\
			return std::make_tuple(data, sizeof(T) + 1);										\
		}																						\
		else{																					\
			memset(data, 0, 1);																	\
			return std::make_tuple(data, 1);													\
		}																						\
	}																							\
	static std::string ToJSON(const T& t, int indent) {											\
		if(t)																					\
			return std::string("\"") + ValueToString<Type>(*t) + "\"";							\
		else																					\
			return std::string("\"null\"");														\
	}																							\
};																								\
template<typename T>																			\
struct ComplexType {																			\
	static size_t SetData(T& t, const char* c_str, size_t offset) {								\
		MetaData<T>::SetBinary(t, c_str + offset);												\
		return MetaData<T>::ComputeTotalSize(t);												\
	}																							\
	static std::tuple<void*, size_t> GetData(const T& t) {										\
		return std::make_tuple((void*)std::get<0>(MetaData<T>::GetBinary(t)),					\
			MetaData<T>::ComputeTotalSize(t));													\
	}																							\
	static std::string ToJSON(const T& t, int indent) {											\
		return MetaData<T>::ToJSON(t, indent + 1);												\
	}																							\
};																								\
template<typename Type, size_t N>																\
struct ComplexType<Type[N]> {																	\
	using T = Type[N];																			\
	static size_t SetData(T& t, const char* c_str, size_t offset) {								\
		using MetaData = MetaData<std::remove_all_extents_t<T>>;								\
		size_t currentSize = 0;																	\
		for(int i = 0, size = drak::types::SizeOfArray_V<T>; i < size;							\
			currentSize += MetaData::ComputeTotalSize(t[i]), ++i)								\
			MetaData::SetBinary(t[i], c_str + (offset + currentSize));							\
		return currentSize;																		\
	}																							\
	static std::tuple<void*, size_t> GetData(const T& t) {										\
		using MetaData = MetaData<std::remove_all_extents_t<T>>;								\
		size_t totalArraySize = 0;																\
		for (auto& x : t)																		\
			totalArraySize += MetaData::ComputeTotalSize(x);									\
		char* c_str = new char[totalArraySize], *temp;											\
		for(int i = 0, size = SizeOfArray_V<T>, currentSize = 0; i < size;						\
			currentSize += (int)MetaData::ComputeTotalSize(t[i]), ++i) {						\
			temp = std::get<0>(MetaData::GetBinary(t[i]));										\
			memcpy(c_str + (currentSize), temp, MetaData::ComputeTotalSize(t[i]));				\
			delete[] temp;																		\
		}																						\
		return std::make_tuple(c_str, totalArraySize);											\
	}																							\
	static std::string ToJSON(const T& t, int indent) {											\
		std::string str("[\n");																	\
		for(size_t i = 0; i < N; ++i){															\
			for(int i2 = 0; i2 < indent; ++i2)													\
				str += '\t';																	\
			str += MetaData<Type>::ToJSON(t[i], indent + 1);									\
			str += ",\n";																		\
		}																						\
		str.erase(str.end() - 2);																\
		indent -= 1;																			\
		for(int i = 0; i < indent; ++i)															\
			str += '\t';																		\
		str += "]";																				\
		return str;																				\
	}																							\
};																								\
template<typename Type>																			\
struct ComplexType<Type*> {																		\
	using T = Type*;																			\
	static size_t SetData(T& t, const char* c_str, size_t offset) {								\
		char isAllocated = *(char*)(c_str + offset);											\
		if (isAllocated) {																		\
			t = new std::remove_pointer_t<T>();													\
			MetaData<std::remove_pointer_t<T>>::SetBinary(*t, c_str + offset + 1);				\
			return MetaData<std::remove_pointer_t<T>>::ComputeTotalSize(*t) + 1;				\
		}																						\
		else {																					\
			t = nullptr;																		\
			return 1;																			\
		}																						\
	}																							\
	static std::tuple<void*, size_t> GetData(const T& t) {										\
		using MetaData = MetaData<std::remove_pointer_t<T>>;									\
		if((bool)t) {																			\
			char* data = new char[MetaData::ComputeTotalSize(*t) + 1], size = 1;				\
			memcpy(data, &size, 1);																\
			memcpy(data + 1, std::get<0>(MetaData::GetBinary(*t)),								\
				MetaData::ComputeTotalSize(*t));												\
			return std::make_tuple(data, MetaData::ComputeTotalSize(*t) + 1);					\
		}																						\
		else {																					\
			char* data = new char[1];															\
			memset(data, 0, 1);																	\
			return std::make_tuple(data, 1);													\
		}																						\
	}																							\
	static std::string ToJSON(const T& t, int indent) {											\
		if(t)																					\
			return MetaData<Type>::ToJSON(*t, indent + 1);										\
		else																					\
			return std::string("\"null\"");														\
	}																							\
};

#define DK_SET_DATA()																			\
template<typename T>																			\
static size_t SetVector(T& t, const char* c_str, size_t offset) {								\
	size_t size = *(size_t*)(c_str + offset);													\
	const char* temp = c_str + offset + sizeof(size_t);											\
	t.reserve(size);																			\
	for(int i = 0; i < size; ++i) {																\
		drak::types::VectorType_T<T> data;														\
		temp += SetData<drak::types::VectorType_T<T>>(data, temp, 0);							\
		t.emplace_back(data);																	\
	}																							\
	return temp - (c_str + offset);																\
}																								\
static size_t SetString(std::string& t, const char* c_str, size_t offset) {						\
	size_t size = *(size_t*)(c_str + offset);													\
	t.insert(0, c_str + offset + sizeof(size_t), size);											\
	return size + sizeof(size_t);																\
}																								\
template<typename T>																			\
static size_t SetData(T& t, const char* c_str, size_t offset) 	{								\
	if constexpr (drak::types::IsBaseType_V<T>) {												\
		return BaseType<T>::SetData(t, c_str, offset);											\
	}																							\
	else if constexpr (!std::is_same_v<T, drak::types::VectorType_T<T>>) {						\
		return SetVector<T>(t, c_str, offset);													\
	}																							\
	else if constexpr (std::is_same_v<T, std::string>) {										\
		return SetString(t, c_str, offset);														\
	}																							\
	else if constexpr(!drak::types::IsBaseType_V<T>) {											\
		return ComplexType<T>::SetData(t, c_str, offset);										\
	}																							\
}

#define DK_GET_DATA()														\
static std::tuple<void*, size_t> GetStringData(const std::string& s) {		\
	size_t size = s.size();													\
	std::string temp = s;													\
	temp.insert(0, (const char*)&size, sizeof(size_t));						\
	char* c_ptr = new char[temp.size()];									\
	memcpy(c_ptr, temp.c_str(), temp.size());								\
	return std::make_tuple<void*, size_t>((void*)c_ptr, temp.size());		\
}																			\
static std::string StringToJSON(const std::string& s) {						\
	return std::string("\"") + s + "\"";									\
}																			\
template<typename T>														\
static std::tuple<void*, size_t> GetVectorData(const T& t) {				\
	using VecType = drak::types::VectorType_T<T>;							\
	size_t size = t.size(), size2 = 0;										\
	std::vector<std::tuple<void*, size_t>> data;							\
	data.reserve(t.size());													\
	for(auto& x : t) {														\
		data.emplace_back(GetData<TYPEOF(x)>(x));							\
		size2 += std::get<1>(data.back());									\
	}																		\
	char* binarydata = new char[size2 + sizeof(size_t)];					\
	memcpy(binarydata, &size, sizeof(size_t));								\
	char* temp = binarydata + sizeof(size_t);								\
	for(auto& x : data) {													\
		memcpy(temp, std::get<0>(x), std::get<1>(x));						\
		temp += std::get<1>(x);												\
		if constexpr (!drak::types::IsBaseType_V<VecType> &&				\
			!std::is_array_v<VecType>)										\
		delete[] std::get<0>(x);											\
	}																		\
	return std::make_tuple(binarydata, temp - binarydata);					\
}																			\
template<typename T>														\
static std::string VectorToJSON(const T& t, int indent)	{					\
	using VecType = drak::types::VectorType_T<T>;							\
	std::string str("[\n");													\
	str.reserve(SizeOfDynamiclyAllocatedType<T>(t));						\
	for(auto& x : t) {														\
		for (int i = 0; i < indent; ++i)									\
			str += '\t';													\
		str += GetJSON<VecType>(x, indent + 1);								\
		str += ",\n";														\
	}																		\
	str.erase(str.end() - 2);												\
	indent -= 1;															\
	for(int i = 0; i < indent; ++i)											\
		str += '\t';														\
	str += "]";																\
	return str;																\
}																			\
template<typename T, typename U>											\
static std::string IntrinToJSON(const T& t, int indent) {					\
	std::string str("[\n");													\
	for (int i = 0, size = sizeof(T) / 4; i < size; ++i) {					\
		for (int i2 = 0; i2 < indent; ++i2)									\
			str += '\t';													\
		str += BaseType<U>::ToJSON(*((U*)(&t) + i), indent + 1);			\
		str += ",\n";														\
	}																		\
	str.erase(str.end() - 2);												\
	indent -= 1;															\
	for (int i = 0; i < indent; ++i)										\
		str += '\t';														\
	str += "]\n";															\
	return str;																\
}																			\
template<typename T>														\
static std::tuple<void*, size_t> GetData(const T& t) {						\
	if constexpr (drak::types::IsBaseType_V<T>) {							\
		return BaseType<T>::GetData(t);										\
	}																		\
	else if constexpr(std::is_same_v<T, std::string>) {						\
		return GetStringData(t);											\
	}																		\
	else if constexpr (!std::is_same_v<T, drak::types::VectorType_T<T>>) {	\
		return GetVectorData<T>(t);											\
	}																		\
	else if constexpr(!drak::types::IsBaseType_V<T>) {						\
		return ComplexType<T>::GetData(t);									\
	}																		\
}																			\
template<typename T>														\
static std::string GetJSON(const T& t, int indent) {						\
	if constexpr (drak::types::IsBaseType_V<T> &&							\
		!drak::types::IsIntrinType_T<T>) {									\
		return BaseType<T>::ToJSON(t, indent);								\
	}																		\
	else if constexpr (drak::types::IsIntrinType_T<T>) {					\
		if constexpr (std::is_same_v<T, __m64> ||							\
			std::is_same_v<T, __m128> || std::is_same_v<T, __m256>)			\
			return IntrinToJSON<T, F32>(t, indent);							\
		else if constexpr (std::is_same_v<T, __m128i> ||					\
			std::is_same_v<T, __m256i>)										\
			return IntrinToJSON<T, I32>(t, indent);							\
	}																		\
	else if constexpr(std::is_same_v<T, std::string>) {						\
		return StringToJSON(t);												\
	}																		\
	else if constexpr (!std::is_same_v<T, drak::types::VectorType_T<T>>) {	\
		return VectorToJSON<T>(t, indent);									\
	}																		\
	else if constexpr(!drak::types::IsBaseType_V<T>) {						\
		return ComplexType<T>::ToJSON(t, indent);							\
	}																		\
}

#define DK_FIELD_SERIALIZATION																	\
static std::stringstream& SerializeToBinary(const type& t, std::stringstream& ss) {				\
	char* data = BinaryData(t);																	\
	for (int i = 0, size = (int)ComputeTotalSize(t); i < size; ++i)								\
		ss << data[i];																			\
	delete[] data;																				\
	return ss;																					\
}																								\
static std::stringstream& SerializeToJson(const type& t, std::stringstream& ss) {				\
	ss << ToJSON(t, 1);																			\
	return ss;																					\
}																								\
static type& DeserializeJSON(type& t, std::stringstream& ss) {									\
																								\
}																								\
template<EExtension ext>																		\
static std::stringstream& Serialize(const type& t, std::stringstream& ss) {						\
	switch (ext) {																				\
		case EExtension::BINARY :																\
			return SerializeToBinary(t, ss);													\
			break;																				\
		case EExtension::JSON :																	\
			return SerializeToJson(t, ss);														\
			break;																				\
		case EExtension::INI :																	\
			/*return SerializeToIni(t, ss);	*/													\
			break;																				\
		default:																				\
			return ss;																			\
			break;																				\
	}																							\
}																								\
template<EExtension ext>																		\
static type& Deserialize(std::stringstream& ss, type& t) {										\
switch (ext) {																					\
		case EExtension::BINARY :																\
			return SetFieldBinary(t, ss.str().c_str());											\
			break;																				\
		case EExtension::JSON :																	\
			return DeserializeJSON(t, ss);														\
			break;																				\
		case EExtension::INI :																	\
			/*return SerializeToIni(t, ss);	*/													\
			break;																				\
		default:																				\
			return ss;																			\
			break;																				\
	}																							\
}																								\

#define DK_METADATA_FIELD_SERIALIZATION(...)	\
DK_METADATA_SET_FIELD_BINARY(__VA_ARGS__)		\
DK_METADATA_GET_FIELD_BINARY(__VA_ARGS__)		\
DK_METADATA_FIELD_TO_JSON(__VA_ARGS__)			\
DK_METADATA_SERIALIZE_FIELDS(__VA_ARGS__)

#define DK_METADATA_FIELD_TO_JSON(...)													\
static std::string ToJSON(const type& t, int indent) {									\
std::string str(TypeName());															\
str += ": {\n";																			\
DK_EXPAND(DK_CONCAT(DK_METADATA_FIELD_TO_JSON, DK_ARGS_N(__VA_ARGS__))(__VA_ARGS__))	\
for(int i = 0; i < indent - 1; ++i)														\
		str += '\t';																	\
str += '}';																				\
return str;																				\
}

#define DK_METADATA_SET_FIELD_BINARY(...)				\
static void SetBinary(type& t, const char* data) {		\
	DK_EXPAND(DK_CONCAT(DK_METADATA_SET_FIELD_BINARY, DK_ARGS_N(__VA_ARGS__))(__VA_ARGS__))	\
}

#define DK_METADATA_GET_FIELD_BINARY(...)													\
static std::tuple<char*, size_t> GetBinary(const type& t) {									\
	size_t size = ComputeTotalSize(t);														\
	char* data = new char[size], *temp = data;															\
	DK_EXPAND(DK_CONCAT(DK_METADATA_GET_FIELD_BINARY, DK_ARGS_N(__VA_ARGS__))(__VA_ARGS__))	\
	return std::make_tuple(temp, size);												\
}

#define DK_METADATA_SERIALIZE_FIELDS(...)															\
template<EExtension OutType>																		\
static std::stringstream& Serialize(std::stringstream& ss, const type& t, int indent = 0) {			\
if constexpr(OutType != EExtension::JSON) {														\
	DK_EXPAND(DK_CONCAT(DK_METADATA_SERIALIZE_FIELD, DK_ARGS_N(__VA_ARGS__))(__VA_ARGS__))			\
}																									\
else																								\
	ss << ToJSON(t, 1);																				\
return ss;																							\
}																									\
DK_DESERIALIZE_FUNC1																				\
DK_DESERIALIZE_FUNC2

#define DK_SERIALIZED_OBJECT(type)												\
friend drak::serialization::MetaData<type>;										\
static constexpr bool s_serialized = true;										\
public:																			\
using MetaData = drak::serialization::MetaData<type>;							\
template<drak::serialization::EExtension OutType>								\
std::stringstream& Serialize(std::stringstream& ss) {							\
return MetaData::Serialize<OutType>(ss, *this, 0);								\
}																				\
template<drak::serialization::EExtension OutType>								\
std::stringstream Serialize() {													\
return MetaData::Serialize<OutType>(std::stringstream(), *this, 0);				\
}

#define DK_DESERIALIZE_FUNC1							\
static type& Deserialize(type& t, const char* c_str) {	\
SetBinary(t, c_str);									\
DK_DESERIALIZE_FUNC_END

#define DK_DESERIALIZE_FUNC2					\
static type Deserialize(const char* c_str) {	\
type t = Create(c_str);							\
DK_DESERIALIZE_FUNC_END


#define DK_DESERIALIZE_FUNC_END	\
return t;						\
}

#define DK_SET_EVERY_DATA_FUNC(...)	\
DK_SET_EVERY_DATA_IMPL_FUNC				\
static void SetFieldBinary(type& t, const char* c_str) {	\
	size_t offset = 0;										\
DK_EXPAND(DK_CONCAT(DK_SET_EVERY_DATA, DK_ARGS_N(__VA_ARGS__))(__VA_ARGS__))	\
}

#define DK_SET_EVERY_DATA_IMPL_FUNC \
template<typename T>\
static void SetFieldBinaryImpl(T& t, const char* c_str, size_t& offset) {\
	offset += SetData<T>(t, c_str, offset); \
}
#define DK_SET_EVERY_DATA0
#define DK_SET_EVERY_DATA1(ty)			\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);

#define DK_SET_EVERY_DATA2(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA1(__VA_ARGS__))

#define DK_SET_EVERY_DATA3(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA2(__VA_ARGS__))

#define DK_SET_EVERY_DATA4(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA3(__VA_ARGS__))

#define DK_SET_EVERY_DATA5(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA4(__VA_ARGS__))

#define DK_SET_EVERY_DATA6(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA5(__VA_ARGS__))

#define DK_SET_EVERY_DATA7(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA6(__VA_ARGS__))

#define DK_SET_EVERY_DATA8(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA7(__VA_ARGS__))

#define DK_SET_EVERY_DATA9(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA8(__VA_ARGS__))

#define DK_SET_EVERY_DATA10(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA9(__VA_ARGS__))

#define DK_SET_EVERY_DATA11(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA10(__VA_ARGS__))

#define DK_SET_EVERY_DATA12(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA11(__VA_ARGS__))

#define DK_SET_EVERY_DATA13(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA12(__VA_ARGS__))

#define DK_SET_EVERY_DATA14(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA13(__VA_ARGS__))

#define DK_SET_EVERY_DATA15(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA14(__VA_ARGS__))

#define DK_SET_EVERY_DATA16(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA15(__VA_ARGS__))

#define DK_SET_EVERY_DATA17(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA16(__VA_ARGS__))

#define DK_SET_EVERY_DATA18(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA17(__VA_ARGS__))

#define DK_SET_EVERY_DATA19(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA18(__VA_ARGS__))

#define DK_SET_EVERY_DATA20(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA19(__VA_ARGS__))

#define DK_SET_EVERY_DATA21(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA20(__VA_ARGS__))

#define DK_SET_EVERY_DATA22(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA21(__VA_ARGS__))

#define DK_SET_EVERY_DATA23(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA22(__VA_ARGS__))

#define DK_SET_EVERY_DATA24(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA23(__VA_ARGS__))

#define DK_SET_EVERY_DATA25(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA24(__VA_ARGS__))

#define DK_SET_EVERY_DATA26(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA25(__VA_ARGS__))

#define DK_SET_EVERY_DATA27(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA26(__VA_ARGS__))

#define DK_SET_EVERY_DATA28(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA27(__VA_ARGS__))

#define DK_SET_EVERY_DATA29(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA28(__VA_ARGS__))

#define DK_SET_EVERY_DATA30(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA29(__VA_ARGS__))

#define DK_SET_EVERY_DATA31(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA30(__VA_ARGS__))

#define DK_SET_EVERY_DATA32(ty, ...)	\
SetFieldBinaryImpl<TYPEOF(t.ty)>(t.ty, c_str, offset);	\
DK_EXPAND(DK_SET_EVERY_DATA31(__VA_ARGS__))

#define DK_FIELD_BINARY_FUNC(...)													\
static char* BinaryData(const type& t) {											\
	char* data = new char[ComputeTotalSize(t)], *data2;							\
	int offset = 0;																	\
	std::tuple<void*, size_t> binary;											\
DK_EXPAND(DK_CONCAT(DK_FIELD_BINARY_IMPL, DK_ARGS_N(__VA_ARGS__))(__VA_ARGS__))	\
	return data;																	\
}

#define DK_FIELD_BINARY_IMPL(ty)								\
binary = GetData<TYPEOF(t.ty)>(t.ty);							\
data2 = (char*)std::get<0>(binary);								\
memcpy((data + offset), data2, std::get<1>(binary));			\
if constexpr (!drak::types::IsBaseType_V<TYPEOF(t.ty)> &&		\
	!std::is_array_v<TYPEOF(t.ty)>)								\
	delete[] data2;												\
offset += (int)std::get<1>(binary);

#define DK_FIELD_BINARY_IMPL0

#define DK_FIELD_BINARY_IMPL1(t)\
DK_FIELD_BINARY_IMPL(t)

#define DK_FIELD_BINARY_IMPL2(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL1(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL3(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL2(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL4(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL3(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL5(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL4(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL6(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL5(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL7(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL6(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL8(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL7(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL9(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL8(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL10(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL9(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL11(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL10(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL12(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL11(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL13(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL12(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL14(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL13(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL15(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL14(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL16(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL15(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL17(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL16(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL18(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL17(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL19(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL18(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL20(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL19(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL21(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL20(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL22(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL21(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL23(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL22(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL24(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL23(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL25(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL24(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL26(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL25(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL27(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL26(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL28(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL27(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL29(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL28(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL30(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL29(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL31(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL30(__VA_ARGS__))

#define DK_FIELD_BINARY_IMPL32(t, ...)\
DK_FIELD_BINARY_IMPL(t)	\
DK_EXPAND(DK_FIELD_BINARY_IMPL31(__VA_ARGS__))

#define DK_FIELD_TO_JSON_FUNC(...)													\
static std::string ToJSON(const type& t, int indent) {								\
std::string str;																	\
DK_EXPAND(DK_CONCAT(DK_FIELD_TO_JSON, DK_ARGS_N(__VA_ARGS__))(__VA_ARGS__))			\
return str;																			\
}

#define DK_FIELD_TO_JSON_IMPL(ty)					\
for(int i = 0; i < indent; ++i)						\
		str += '\t';								\
str += "\""#ty"\": ";								\
str += GetJSON<TYPEOF(t.ty)>(t.ty, indent + 1) + ",\n";	\

#define DK_FIELD_TO_JSON0

#define DK_FIELD_TO_JSON1(ty)				\
for(int i = 0; i < indent; ++i)				\
		str += '\t';						\
str += "\""#ty"\": ";						\
str += GetJSON<TYPEOF(t.ty)>(t.ty, indent + 1) + '\n';

#define DK_FIELD_TO_JSON2(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON1(__VA_ARGS__))

#define DK_FIELD_TO_JSON3(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON2(__VA_ARGS__))

#define DK_FIELD_TO_JSON4(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON3(__VA_ARGS__))

#define DK_FIELD_TO_JSON5(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON4(__VA_ARGS__))

#define DK_FIELD_TO_JSON6(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON5(__VA_ARGS__))

#define DK_FIELD_TO_JSON7(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON6(__VA_ARGS__))

#define DK_FIELD_TO_JSON8(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON7(__VA_ARGS__))

#define DK_FIELD_TO_JSON9(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON8(__VA_ARGS__))

#define DK_FIELD_TO_JSON10(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON9(__VA_ARGS__))

#define DK_FIELD_TO_JSON11(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON10(__VA_ARGS__))

#define DK_FIELD_TO_JSON12(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON11(__VA_ARGS__))

#define DK_FIELD_TO_JSON13(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON12(__VA_ARGS__))

#define DK_FIELD_TO_JSON14(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON13(__VA_ARGS__))

#define DK_FIELD_TO_JSON15(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON14(__VA_ARGS__))

#define DK_FIELD_TO_JSON16(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON15(__VA_ARGS__))

#define DK_FIELD_TO_JSON17(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON16(__VA_ARGS__))

#define DK_FIELD_TO_JSON18(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON17(__VA_ARGS__))

#define DK_FIELD_TO_JSON19(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON18(__VA_ARGS__))

#define DK_FIELD_TO_JSON20(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON19(__VA_ARGS__))

#define DK_FIELD_TO_JSON21(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON20(__VA_ARGS__))

#define DK_FIELD_TO_JSON22(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON21(__VA_ARGS__))

#define DK_FIELD_TO_JSON23(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON22(__VA_ARGS__))

#define DK_FIELD_TO_JSON24(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON23(__VA_ARGS__))

#define DK_FIELD_TO_JSON25(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON24(__VA_ARGS__))

#define DK_FIELD_TO_JSON26(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON25(__VA_ARGS__))

#define DK_FIELD_TO_JSON27(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON26(__VA_ARGS__))

#define DK_FIELD_TO_JSON28(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON27(__VA_ARGS__))

#define DK_FIELD_TO_JSON29(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON28(__VA_ARGS__))

#define DK_FIELD_TO_JSON30(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON29(__VA_ARGS__))

#define DK_FIELD_TO_JSON31(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON30(__VA_ARGS__))

#define DK_FIELD_TO_JSON32(t, ...)\
DK_FIELD_TO_JSON_IMPL(t)	\
DK_EXPAND(DK_FIELD_TO_JSON31(__VA_ARGS__))

#define DK_JSON_TO_FIELD_IMPL(ty)					\
for(int i = 0; i < indent; ++i)						\
		str += '\t';								\
str += "\""#ty"\": ";								\
str += GetJSON<TYPEOF(t.ty)>(t.ty, indent + 1) + ",\n";	\

#define DK_JSON_TO_FIELD0

#define DK_JSON_TO_FIELD1(ty)				\
for(int i = 0; i < indent; ++i)				\
		str += '\t';						\
str += "\""#ty"\": ";						\
str += GetJSON<TYPEOF(t.ty)>(t.ty, indent + 1) + '\n';

#define DK_JSON_TO_FIELD2(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD1(__VA_ARGS__))

#define DK_JSON_TO_FIELD3(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD2(__VA_ARGS__))

#define DK_JSON_TO_FIELD4(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD3(__VA_ARGS__))

#define DK_JSON_TO_FIELD5(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD4(__VA_ARGS__))

#define DK_JSON_TO_FIELD6(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD5(__VA_ARGS__))

#define DK_JSON_TO_FIELD7(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD6(__VA_ARGS__))

#define DK_JSON_TO_FIELD8(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD7(__VA_ARGS__))

#define DK_JSON_TO_FIELD9(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD8(__VA_ARGS__))

#define DK_JSON_TO_FIELD10(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD9(__VA_ARGS__))

#define DK_JSON_TO_FIELD11(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD10(__VA_ARGS__))

#define DK_JSON_TO_FIELD12(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD11(__VA_ARGS__))

#define DK_JSON_TO_FIELD13(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD12(__VA_ARGS__))

#define DK_JSON_TO_FIELD14(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD13(__VA_ARGS__))

#define DK_JSON_TO_FIELD15(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD14(__VA_ARGS__))

#define DK_JSON_TO_FIELD16(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD15(__VA_ARGS__))

#define DK_JSON_TO_FIELD17(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD16(__VA_ARGS__))

#define DK_JSON_TO_FIELD18(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD17(__VA_ARGS__))

#define DK_JSON_TO_FIELD19(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD18(__VA_ARGS__))

#define DK_JSON_TO_FIELD20(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD19(__VA_ARGS__))

#define DK_JSON_TO_FIELD21(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD20(__VA_ARGS__))

#define DK_JSON_TO_FIELD22(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD21(__VA_ARGS__))

#define DK_JSON_TO_FIELD23(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD22(__VA_ARGS__))

#define DK_JSON_TO_FIELD24(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD23(__VA_ARGS__))

#define DK_JSON_TO_FIELD25(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD24(__VA_ARGS__))

#define DK_JSON_TO_FIELD26(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD25(__VA_ARGS__))

#define DK_JSON_TO_FIELD27(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD26(__VA_ARGS__))

#define DK_JSON_TO_FIELD28(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD27(__VA_ARGS__))

#define DK_JSON_TO_FIELD29(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD28(__VA_ARGS__))

#define DK_JSON_TO_FIELD30(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD29(__VA_ARGS__))

#define DK_JSON_TO_FIELD31(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD30(__VA_ARGS__))

#define DK_JSON_TO_FIELD32(t, ...)\
DK_JSON_TO_FIELD_IMPL(t)	\
DK_EXPAND(DK_JSON_TO_FIELD31(__VA_ARGS__))


#define DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
fields::SetFieldBinary(t, data);					\
data += fields::ComputeTotalSize(t);

#define DK_METADATA_SET_FIELD_BINARY0

#define DK_METADATA_SET_FIELD_BINARY1(fields)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)

#define DK_METADATA_SET_FIELD_BINARY2(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY1(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY3(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY2(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY4(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY3(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY5(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY4(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY6(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY5(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY7(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY6(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY8(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY7(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY9(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY8(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY10(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY9(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY11(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY10(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY12(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY11(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY13(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY12(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY14(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY13(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY15(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY14(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY16(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY15(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY17(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY16(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY18(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY17(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY19(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY18(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY20(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY19(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY21(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY20(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY22(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY21(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY23(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY22(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY24(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY23(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY25(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY24(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY26(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY25(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY27(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY26(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY28(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY27(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY29(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY28(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY30(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY29(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY31(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY30(__VA_ARGS__))

#define DK_METADATA_SET_FIELD_BINARY32(fields, ...)\
DK_METADATA_SET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SET_FIELD_BINARY31(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY_IMPL(fields)					\
size_t size##fields = fields::ComputeTotalSize(t);					\
memcpy(data, fields::BinaryData(t), fields::ComputeTotalSize(t));	\
data += size##fields;

#define DK_METADATA_GET_FIELD_BINARY0

#define DK_METADATA_GET_FIELD_BINARY1(fields)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)

#define DK_METADATA_GET_FIELD_BINARY2(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY1(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY3(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY2(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY4(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY3(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY5(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY4(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY6(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY5(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY7(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY6(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY8(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY7(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY9(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY8(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY10(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY9(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY11(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY10(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY12(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY11(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY13(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY12(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY14(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY13(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY15(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY14(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY16(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY15(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY17(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY16(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY18(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY17(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY19(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY18(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY20(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY19(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY21(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY20(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY22(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY21(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY23(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY22(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY24(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY23(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY25(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY24(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY26(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY25(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY27(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY26(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY28(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY27(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY29(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY28(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY30(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY29(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY31(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY30(__VA_ARGS__))

#define DK_METADATA_GET_FIELD_BINARY32(fields, ...)\
DK_METADATA_GET_FIELD_BINARY_IMPL(fields)	\
DK_EXPAND(DK_METADATA_GET_FIELD_BINARY31(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
str += fields::ToJSON(t, indent);

#define DK_METADATA_FIELD_TO_JSON0

#define DK_METADATA_FIELD_TO_JSON1(fields)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)

#define DK_METADATA_FIELD_TO_JSON2(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON1(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON3(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON2(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON4(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON3(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON5(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON4(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON6(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON5(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON7(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON6(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON8(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON7(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON9(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON8(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON10(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON9(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON11(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON10(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON12(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON11(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON13(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON12(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON14(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON13(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON15(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON14(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON16(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON15(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON17(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON16(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON18(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON17(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON19(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON18(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON20(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON19(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON21(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON20(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON22(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON21(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON23(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON22(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON24(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON23(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON25(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON24(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON26(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON25(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON27(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON26(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON28(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON27(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON29(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON28(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON30(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON29(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON31(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON30(__VA_ARGS__))

#define DK_METADATA_FIELD_TO_JSON32(fields, ...)\
DK_METADATA_FIELD_TO_JSON_IMPL(fields)	\
DK_EXPAND(DK_METADATA_FIELD_TO_JSON31(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
fields::Serialize<OutType>(t, ss);

#define DK_METADATA_SERIALIZE_FIELD0

#define DK_METADATA_SERIALIZE_FIELD1(fields)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)

#define DK_METADATA_SERIALIZE_FIELD2(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD1(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD3(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD2(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD4(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD3(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD5(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD4(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD6(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD5(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD7(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD6(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD8(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD7(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD9(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD8(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD10(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD9(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD11(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD10(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD12(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD11(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD13(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD12(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD14(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD13(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD15(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD14(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD16(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD15(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD17(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD16(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD18(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD17(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD19(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD18(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD20(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD19(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD21(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD20(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD22(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD21(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD23(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD22(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD24(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD23(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD25(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD24(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD26(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD25(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD27(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD26(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD28(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD27(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD29(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD28(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD30(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD29(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD31(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD30(__VA_ARGS__))

#define DK_METADATA_SERIALIZE_FIELD32(fields, ...)\
DK_METADATA_SERIALIZE_FIELD_IMPL(fields)	\
DK_EXPAND(DK_METADATA_SERIALIZE_FIELD31(__VA_ARGS__))
/*
#define DK_PUBLIC_METHODE(type, ...)														\
static constexpr std::array<const char*, DK_ARGS_N(__VA_ARGS__)> m_publicMethodeName =		\
{ DK_STRINGIZE_VA_ARGS(__VA_ARGS__) };
#define DK_PRIVATE_METHODE(type, ...)															\
static constexpr std::array<const char*, DK_ARGS_N(__VA_ARGS__)> m_privateMethodeName =			\
{ DK_STRINGIZE_VA_ARGS(__VA_ARGS__) };
#define DK_PUBLIC_STATIC_METHODE(type, ...)														\
static constexpr std::array<const char*, DK_ARGS_N(__VA_ARGS__)> m_staticPublicMethodeName =	\
{ DK_STRINGIZE_VA_ARGS(__VA_ARGS__) };
#define DK_PRIVATE_STATIC_METHODE(type, ...)													\
static constexpr std::array<const char*, DK_ARGS_N(__VA_ARGS__)> m_staticPrivateMethodeName =	\
{ DK_STRINGIZE_VA_ARGS(__VA_ARGS__) };*/