#pragma once

struct Vec2
{
    float x, y;

    Vec2 operator+(const Vec2& other) const { return { x + other.x, y + other.y }; }
    Vec2 operator-(const Vec2& other) const { return { x - other.x, y - other.y }; }
    Vec2 operator*(const Vec2& other) const { return { x * other.x, y * other.y }; }
    Vec2 operator/(const Vec2& other) const { return { x / other.x, y / other.y }; }

    bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vec2& other) const { return !(*this == other); }

    static float Distance(const Vec2& first, const Vec2& second)
    {
        return sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2));
    }

    static Vec2 Lerp(const Vec2& first, const Vec2& second, float time)
    {
        return Vec2(first.x + time * (second.x - first.x), first.y + time * (second.y - first.y));
    }

    Vec2(float _x, float _y) : x(_x), y(_y) { }
    Vec2() : x(0), y(0) { }
};

struct Vec3
{
    float x, y, z;

    Vec3 operator+(const Vec3& other) const { return { x + other.x, y + other.y, z + other.z }; }
    Vec3 operator-(const Vec3& other) const { return { x - other.x, y - other.y, z - other.z }; }
    Vec3 operator*(const Vec3& other) const { return { x * other.x, y * other.y, z * other.z }; }
    Vec3 operator/(const Vec3& other) const { return { x / other.x, y / other.y, z / other.z }; }

    Vec3 operator+(const Vec2& other) const { return { x + other.x, y + other.y, z }; }
    Vec3 operator-(const Vec2& other) const { return { x - other.x, y - other.y, z }; }
    Vec3 operator*(const Vec2& other) const { return { x * other.x, y * other.y, z }; }
    Vec3 operator/(const Vec2& other) const { return { x / other.x, y / other.y, z }; }

    bool operator==(const Vec3& other) const { return x == other.x && y == other.y && z == other.z; }
    bool operator!=(const Vec3& other) const { return !(*this == other); }

    static float Distance(const Vec3& first, const Vec3& second)
    {
        return sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2) + pow(first.z - second.z, 2));
    }

    static Vec3 Lerp(const Vec3& first, const Vec3& second, float time)
    {
        return Vec3(first.x + time * (second.x - first.x), first.y + time * (second.y - first.y), first.z + time * (second.z - first.z));
    }

    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
    Vec3() : x(0), y(0), z(0) { }
};
