#include "Vec2.hpp"
#include <cassert>

namespace drak {
namespace math {

template<typename T>
constexpr bool Vec2<T>::isLarger;

template<typename T>
constexpr bool Vec2<T>::isIntegral;

template<typename T>
Vec2<T>::Vec2() : m_vec{ 0, 0 } {
}

template<typename T>
Vec2<T>::Vec2(const Type X,
	const Type Y) : m_vec{ X, Y } {

}


template<typename T>
Vec2<T>::Vec2(ENABLE_IF_ELSE_T(isLarger, T&&, NOT_A_TYPE)X,
	ENABLE_IF_ELSE_T(isLarger, T&&, NOT_A_TYPE)Y)
	: m_vec{ std::forward<T>(X), std::forward<T>(Y) } {
}

template<typename T>
Vec2<T>::Vec2(const Vec2<T>& v) : m_vec{ v.x, v.y } {
}

template<typename T> 
Vec2<T>::Vec2(Vec2<T>&& v) : m_vec{ std::forward<Vec2<T>>(v).x, std::forward<Vec2<T>>(v).y } {
}


template<typename T>
bool Vec2<T>::operator==(const Vec2<T>& v) const {
	return drakTypes::IsEqual_V<T>(v.x, x) && drakTypes::IsEqual_V<T>(v.y, y);
}
template<typename T>
bool Vec2<T>::operator!=(const Vec2<T>& v) const {
	return drakTypes::IsNotEqual_V<T>(v.x, x) && drakTypes::IsNotEqual_V<T>(v.y, y);
}
template<typename T>
bool Vec2<T>::operator>(const Vec2<T>& v) const {
	return x > v.x && y > v.y;
}
template<typename T>
bool Vec2<T>::operator<(const Vec2<T>& v) const {
	return x < v.x && y < v.y;
}
template<typename T>
bool Vec2<T>::operator>=(const Vec2<T>& v) const {
	return x >= v.x && y >= v.y;;
}
template<typename T>
bool Vec2<T>::operator<=(const Vec2<T>& v) const {
	return  x <= v.x && y <= v.y;
}
template<typename T>
bool Vec2<T>::isNormalized() const {
	return drakTypes::IsEqual_V<T>(magnitude(), 1.0f);
}
template<typename T>
bool Vec2<T>::isNull() const {
	return drakTypes::IsEqual_V<T>(x, 0.0f) && drakTypes::IsEqual_V<T>(y, 0.0f);
}
template<typename T>
f32 Vec2<T>::magnitude() const {
	return std::sqrt<f32>(x * x + y * y);
}


template<typename T>
template<MathUtils::AngleUnit au>
f32 Vec2<T>::rotation() const {
	if (y) {
		if (y > static_cast<T>(0))
			if constexpr (au == MathUtils::AngleUnit::DEGREE)
				return acos<f32>(x / magnitude()) * MathUtils::ToDegF;
			else
				return acos<f32>(x / magnitude());
		else if (y < static_cast<T>(0))
			if constexpr (au == MathUtils::AngleUnit::DEGREE)
				return -(acos<f32>(x / magnitude()) * MathUtils::ToDegF) + 360.f;
			else
				return -acos<f32>(x / magnitude());
	}
	else
		return 0.f;
	return 0.f;
}

template<typename T>
Vec2<T>& Vec2<T>::operator=(const Vec2<T>& v) {
	x = v.x;
	y = v.y;
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator=(Vec2<T>&& v) {
	x = std::forward<Vec2<T>>(v).x;
	y = std::forward<Vec2<T>>(v).y;
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator+=(const Type n) {
	x += n;
	y += n;
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator-=(const Type n) {
	x -= n;
	y -= n;
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator*=(const Type n) {
	x *= n;
	y *= n;
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator/=(const Type n) {

	assert(drakTypes::IsEqual_V<T>(n, static_cast<T>(0)));
	if (drakTypes::IsNotEqual_V<T>(n, static_cast<T>(0))) {
		x /= n;
		y /= n;
	}
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator+=(ENABLE_IF_ELSE_T(isLarger, T &&, NOT_A_TYPE)n) {
	x += std::forward<T>(n);
	y += std::forward<T>(n);
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator-=(ENABLE_IF_ELSE_T(isLarger, T &&, NOT_A_TYPE)n) {
	x -= std::forward<T>(n);
	y -= std::forward<T>(n);
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator*=(ENABLE_IF_ELSE_T(isLarger, T &&, NOT_A_TYPE)n) {
	x *= std::forward<T>(n);
	y *= std::forward<T>(n);
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator/=(ENABLE_IF_ELSE_T(isLarger, T &&, NOT_A_TYPE)n) {
	assert(drakTypes::IsEqual_V<T>(n, static_cast<T>(0)));
	if (drakTypes::IsNotEqual_V<T>(n, static_cast<T>(0))) {
		x /= std::forward<T>(n);
		y /= std::forward<T>(n);
	}
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator>>=(const ENABLE_IF_ELSE_T(isIntegral, int32, NOT_A_TYPE) n) {
	x >>= n;
	y >>= n;
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator<<=(const ENABLE_IF_ELSE_T(isIntegral, int32, NOT_A_TYPE) n) {
	x <<= n;
	y <<= n;
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator&=(const ENABLE_IF_ELSE_T(isIntegral, int32, NOT_A_TYPE) n) {
	x &= n;
	y &= n;
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator^=(const ENABLE_IF_ELSE_T(isIntegral, int32, NOT_A_TYPE) n) {
	x ^= n;
	y ^= n;
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator|=(const ENABLE_IF_ELSE_T(isIntegral, int32, NOT_A_TYPE) n) {
	x |= n;
	y |= n;
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator++() {
	++x;
	++y;
	return *this;
}
template<typename T>
Vec2<T>& Vec2<T>::operator--() {
	--x;
	--y;
	return *this;
}

template<typename T>
Vec2<T> Vec2<T>::operator+(const Type n) const {
	return Vec2<T>(x + n, y + n);
}
template<typename T>
Vec2<T> Vec2<T>::operator-(const Type n) const {
	return Vec2<T>(x - n, y - n);
}
template<typename T>
Vec2<T> Vec2<T>::operator*(const Type n) const {
	return Vec2<T>(x * n, y * n);
}
template<typename T>
Vec2<T> Vec2<T>::operator/(const Type n) const {
	assert(drakTypes::IsEqual_V<T>(n, static_cast<T>(0)));
	if(drakTypes::IsNotEqual_V<T>(n, static_cast<T>(0)))
		return Vec2<T>(x / n, y / n);
	return Vec2<T>(x, y);
}

template<typename T>
Vec2<T> Vec2<T>::operator+(ENABLE_IF_ELSE_T(isLarger, T &&, NOT_A_TYPE)n) const {
	return Vec2<T>(x + std::forward<T>(n), y + std::forward<T>(n));
}
template<typename T>
Vec2<T> Vec2<T>::operator-(ENABLE_IF_ELSE_T(isLarger, T &&, NOT_A_TYPE)n) const {
	return Vec2<T>(x - std::forward<T>(n), y - std::forward<T>(n));
}
template<typename T>
Vec2<T> Vec2<T>::operator*(ENABLE_IF_ELSE_T(isLarger, T &&, NOT_A_TYPE)n) const {
	return Vec2<T>(x * std::forward<T>(n), y * std::forward<T>(n));
}
template<typename T>
Vec2<T> Vec2<T>::operator/(ENABLE_IF_ELSE_T(isLarger, T &&, NOT_A_TYPE)n) const {
	assert(drakTypes::IsEqual_V<T>(std::forward<T>(n), static_cast<T>(0)));
	if (drakTypes::IsNotEqual_V<T>(std::forward<T>(n), static_cast<T>(0)))
		return Vec2<T>(x / std::forward<T>(n), y / std::forward<T>(n));
	return Vec2<T>(x, y);
}

template<typename T>
Vec2<T> Vec2<T>::operator>>(const ENABLE_IF_ELSE_T(isIntegral, int32, NOT_A_TYPE) n) const {
	return Vec2<T>(x >> n, y >> n);
}
template<typename T>
Vec2<T> Vec2<T>::operator<<(const ENABLE_IF_ELSE_T(isIntegral, int32, NOT_A_TYPE) n) const {
	return Vec2<T>(x << n, y << n);
}
template<typename T>
Vec2<T> Vec2<T>::operator&(const ENABLE_IF_ELSE_T(isIntegral, int32, NOT_A_TYPE) n) const {
	return Vec2<T>(x & n, y & n);
}
template<typename T>
Vec2<T> Vec2<T>::operator^(const ENABLE_IF_ELSE_T(isIntegral, int32, NOT_A_TYPE) n) const {
	return Vec2<T>(x ^ n, y ^ n);
}
template<typename T>
Vec2<T> Vec2<T>::operator|(const ENABLE_IF_ELSE_T(isIntegral, int32, NOT_A_TYPE) n) const {
	return Vec2<T>(x | n, y | n);
}

template<typename T>
Vec2<T> Vec2<T>::operator++(const int32) const {
	return Vec2<T>(++x, ++y);
}
template<typename T>
Vec2<T> Vec2<T>::operator--(const int32) const {
	return Vec2<T>(++x, ++y);
}
template<typename T>
Vec2<T> Vec2<T>::operator-() const {
	return Vec2<T>(-x, -y);
}

template<typename T>
Vec2<T> Vec2<T>::conjugate() const{
	return -*this;
}

template<typename T>
Vec2<T> Vec2<T>::normalize() const{
	assert(drakTypes::IsEqual_V(size, static_cast<T>(0)))
	f32 size = magnitude();
	if(drakTypes::IsNotEqual_V(size, static_cast<T>(0)))
		return *this / size;
	return *this;
}

template<typename T>
Vec2<T> Vec2<T>::rotate(const f32 angle)const {
	f32 angleRad = angle * MathUtils::ToRadF;
	T angleCos = cos<f32>(angleRad);
	T angleSin = sin<f32>(angleRad);
	return Vec2<T>(x * angleCos - y * angleSin, x * angleSin + y * angleCos);
}

template<typename T>
Vec2<T> Vec2<T>::perpendicularVector() const {
	return Vec2<T>(x, -y);
}

template<typename T>
T* Vec2<T>::vec() const {
	return m_vec;
}

template<typename T>
template<typename U>
Vec2<U> Vec2<T>::cast() const {
	static_assert(std::is_scalar_v<U>, "\"U\" must be a scalar Type");
	return Vec2<U>(static_cast<U>(x), static_cast<U>(y));
}

template<typename T>
Vec2<T> operator+(const Vec2<T>& v1, const Vec2<T>& v2) {
	return Vec2<T>(v1.x + v2.x, v1.y + v2.y);
}

template<typename T>
Vec2<T> operator-(const Vec2<T>& v1, const Vec2<T>& v2) {
	return Vec2<T>(v1.x - v2.x, v1.y - v2.y);
}

template<typename T>
Vec2<T> operator*(const Vec2<T>& v1, const Vec2<T>& v2) {
	return Vec2<T>(v1.x * v2.x, v1.y * v2.y);
}

template<typename T>
Vec2<T> operator/(const Vec2<T>& v1, const Vec2<T>& v2) {
	assert(drakTypes::IsEqual_V<T>(v2.x, static_cast<T>(0)));
	assert(drakTypes::IsEqual_V<T>(v2.y, static_cast<T>(0)));

	if (drakTypes::IsNotEqual_V<T>(v2.x, static_cast<T>(0)) &&
		drakTypes::IsNotEqual_V<T>(v2.y, static_cast<T>(0)))
		return Vec2<T>(v1.x / v2.x, v1.y / v2.y);
	else if (drakTypes::IsNotEqual_V<T>(v2.x, static_cast<T>(0)))
		return Vec2<T>(v1.x, v1.y / v2.y);
	else
		return Vec2<T>(v1.x / v2.x, v1.y);

}

template<typename T>
Vec2<T> operator>>(const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)& v1,
	const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)& v2) {
	return Vec2<T>(v1.x >> v2.x, v1.y >> v2.y);
}

template<typename T>
Vec2<T> operator<<(const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)& v1,
	const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)& v2) {
	return Vec2<T>(v1.x << v2.x, v1.y << v2.y);
}

template<typename T>
Vec2<T> operator&(const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)& v1,
	const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)& v2) {
	return Vec2<T>(v1.x & v2.x, v1.y & v2.y);
}

template<typename T>
Vec2<T> operator^(const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)& v1,
	const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)& v2) {
	return Vec2<T>(v1.x ^ v2.x, v1.y ^ v2.y);
}

template<typename T>
Vec2<T> operator|(const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)& v1,
	const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)& v2) {
	return Vec2<T>(v1.x | v2.x, v1.y | v2.y);
}

template<typename T>
Vec2<T>& operator+=(Vec2<T>& v1, const Vec2<T>& v2) {
	v1.x += v2.x;
	v1.y += v2.y;
	return *this;
}

template<typename T>
Vec2<T>& operator-=(Vec2<T>& v1, const Vec2<T>& v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	return *this;
}

template<typename T>
Vec2<T>& operator*=(Vec2<T>& v1, const Vec2<T>& v2) {
	v1.x *= v2.x;
	v1.y *= v2.y;
	return *this;
}

template<typename T>
Vec2<T>& operator/=(Vec2<T>& v1, const Vec2<T>& v2) {
	assert(drakTypes::IsEqual_V<T>(v2.x, static_cast<T>(0)));
	assert(drakTypes::IsEqual_V<T>(v2.y, static_cast<T>(0)));

	if (drakTypes::IsNotEqual_V<T>(v2.x, static_cast<T>(0)) &&
		drakTypes::IsNotEqual_V<T>(v2.y, static_cast<T>(0))) {

		v1.x /= std::forward<Vec2<T>>(v2).x;
		v1.y /= std::forward<Vec2<T>>(v2).y;
	}
	else if (drakTypes::IsNotEqual_V<T>(std::forward<Vec2<T>>(v).x, static_cast<T>(0))) {
		v1.y /= std::forward<Vec2<T>>(v2).y;
	}
	else {
		v1.x /= std::forward<Vec2<T>>(v2).x;
	}
	return *this;
}

template<typename T>
Vec2<T>& operator>>=(ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)&v1, const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)&v2) {
	v1.x >>= v2.x;
	v1.y >>= v2.y;
	return *this;
}

template<typename T>
Vec2<T>& operator<<=(ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)&v1, const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)&v2) {
	v1.x <<= v2.x;
	v1.y <<= v2.y;
	return *this;
}

template<typename T>
Vec2<T>& operator&=(ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)&v1, const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)&v2) {
	v1.x &= v2.x;
	v1.y &= v2.y;
	return *this;
}

template<typename T>
Vec2<T>& operator^=(ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)&v1, const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)&v2) {
	v1.x ^= v2.x;
	v1.y ^= v2.y;
	return *this;
}

template<typename T>
Vec2<T>& operator|=(ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)&v1, const ENABLE_IF_ELSE_T(Vec2<T>::isIntegral, Vec2<T>, NOT_A_TYPE)&v2) {
	v1.x |= v2.x;
	v1.y |= v2.y;
	return *this;
}

template<typename T>
f32 Dot(const Vec2<T>& v1, const Vec2<T>& v2) {
	return (v1.x * v2.x + v1.y * v2.y);
}
template<typename T>
Vec2<T> Cross(const Vec2<T>& v1, const Vec2<T>& v2) {
	return Vec2<T>((v1.x * v2.y) - (v1.y * v2.x));
}
template<typename T>
f32 Distance(const Vec2<T>& v1, const Vec2<T>& v2) {
	return Direction(v1, v2).magnitude();
}
template<typename T>
Vec2<T> Direction(const Vec2<T>& origin, const Vec2<T>& destination) {
	return destination - origin;
}
template<typename T>
void RotateAround(Vec2<T>& v1, const Vec2<T>& point, const f32 angle) {
	Vec2<T> dir = Direction(point, v1);
	f32 dist = dir.magnitude();
	v1 = v1 - dir;
	v1 = v1.rotate(angle);
	v1 *= dist;
}
template<typename T>
bool ArePerpendicular(const Vec2<T>& v1, const Vec2<T>& v2) {
	return drakTypes::IsEqual_V<f32>(Dot(v1, v2), static_cast<T>(0));
}
template<typename T>
bool AreColinear(const Vec2<T>& v1, const Vec2<T>& v2) {
	return drakTypes::IsEqual_V<f32>(Dot(v1, v2), 1.f);
}
template<typename T>
bool AreOpposed(const Vec2<T>& v1, const Vec2<T>& v2) {
	return drakTypes::IsEqual_V<f32>(Dot(v1, v2), -1.f);
}
template<typename T>
bool AreSameDirection(const Vec2<T>& v1, const Vec2<T>& v2) {
	return Dot(v1, v2) > 0.f;
}
template<typename T>
bool AreOpposedDirection(const Vec2<T>& v1, const Vec2<T>& v2) {
	return Dot(v1, v2) < 0.f;
}

} //namespace math
} //namespace drak