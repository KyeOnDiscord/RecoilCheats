#pragma once


struct Vec2 {
	float x, y;
	Vec2()
	{
		x = y = 0;
	}

	Vec2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Vec2 operator+(Vec2 d)
	{
		return { x + d.x, y + d.y };
	}
	Vec2 operator-(Vec2 d)
	{
		return { x - d.x, y - d.y };
	}
	Vec2 operator*(Vec2 d)
	{
		return { x * d.x, y * d.y };
	}
	Vec2 operator*(float arg)
	{
		return { x * arg, y * arg };
	}

	float Distance(Vec2 d)
	{
		return sqrt(pow(d.x - x, 2) + pow(d.y - y, 2));
	}


	void Normalize()
	{
		while (y > 180) { y -= 360; }   // normalize angle
		while (y < -180) { y += 360; }  // ^
		if (x > 89.0f) { x = 89.0f; }   // ^
		if (x < -89.0f) { x = -89.0f; }
	}
};

struct Vec3 {
	float x, y, z;
	Vec3()
	{
		x = y = z = 0;
	}
	Vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vec3 operator+(Vec3 d) {
		return { x + d.x, y + d.y, z + d.z };
	}
	Vec3 operator-(Vec3 d) {
		return { x - d.x, y - d.y, z - d.z };
	}
	Vec3 operator*(float d) {
		return { x * d, y * d, z * d };
	}
	bool operator==(Vec3 d) {
		return (x == d.x && y == d.y && z == d.z);
	}

	float Distance(Vec3 d)
	{
		return hypot(hypot(this->x - d.x, this->y - d.y), this->z - d.z);
	}

	void Normalize() {
		while (y < -180) { y += 360; }
		while (y > 180) { y -= 360; }
		if (x > 89) { x = 89; }
		if (x < -89) { x = -89; }
	}

};

struct Vec4 {
	float x, y, z, w;
};