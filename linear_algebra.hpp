#include <string>
using namespace std;

// 2D Vector (2x1 Matrix)
struct Vec2 {
	double x,y;

	Vec2(){
		x = y = 0;
	}

	Vec2(double _x, double _y) {
		x = _x;
		y = _y;
	}

	Vec2& operator+=(const Vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2& operator*=(double a) {
		x *= a;
		y *= a;
		return *this;
	}

	string to_str() {
		char buffer[256];
		snprintf(buffer, sizeof(buffer), "( %lf , %lf )", x, y);
		return buffer;
	}
};

inline Vec2 operator*(const Vec2& v, double a) {
	return Vec2 {
		a * v.x, a * v.y
	};
}

inline Vec2 operator/(const Vec2& v, double a) {
	return Vec2 {
		v.x / a, v.y / a
	};
}

inline Vec2 operator+(const Vec2& v1, const Vec2& v2) {
	return Vec2 {
		v1.x + v2.x, v1.y + v2.y
	};
}

inline Vec2 operator-(const Vec2& v1, const Vec2& v2) {
	return Vec2 {
		v1.x - v2.x, v1.y - v2.y
	};
}

inline double dot(const Vec2& v1, const Vec2& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

inline double abs(const Vec2& v) {
	return sqrt(v.x * v.x + v.y * v.y);
}


// 3D Vector (3x1 Matrix)
struct Vec3 {
	double x,y,z;

	Vec3() {
		x = y = z = 0;
	}

	Vec3(double _x, double _y, double _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vec3& operator+=(const Vec3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	
	Vec3& operator-=(const Vec3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	
	Vec3& operator*=(double a) {
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}

	Vec3& operator/=(double a) {
		x /= a;
		y /= a;
		z /= a;
		return *this;
	}

	string to_str() {
		char buffer[256];
		snprintf(buffer, sizeof(buffer), "( %lf , %lf , %lf )", x, y, z);
		return buffer;
	}
};

inline Vec3 operator*(const Vec3& v, const double a) {
	return Vec3 {
		a * v.x, a * v.y, a * v.z
	};
}

inline Vec3 operator/(const Vec3& v, const double a) {
	return Vec3 {
		v.x / a, v.y / a, v.z / a
	};
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2) {
	return Vec3 {
		v1.x + v2.x, v1.y + v2.y, v1.z + v2.z
	};
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2) {
	return Vec3 {
		v1.x - v2.x, v1.y - v2.y, v1.z - v2.z
	};
}

inline double dot(const Vec3& v1, const Vec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline double abs(const Vec3& v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}


// 2x2 Matrix
struct Mat2x2 {
	double a,b,c,d;

	Mat2x2() {
		a = b = c = d = 0;
	}

	Mat2x2(double _a, double _b, double _c, double _d) {
		a = _a;
		b = _b;
		c = _c;
		d = _d;
	}

	Mat2x2& operator+=(const Mat2x2& other) {
		a += other.a;
		b += other.b;
		c += other.c;
		d += other.d;
		return *this;
	}

	Mat2x2& operator*=(double a) {
		a *= a;
		b *= a;
		c *= a;
		d *= a;
		return *this;
	}

	string to_str() {
		char buffer[512];
		snprintf(buffer, sizeof(buffer), "( %7lf %7lf\n  %7lf %7lf )", a, b, c, d);
		return buffer;
	}
};

inline Mat2x2 operator*(const Mat2x2& m, double a) {
	return Mat2x2 {
		a * m.a, a * m.b,
		a * m.c, a * m.d
	};
}

inline Mat2x2 operator+(const Mat2x2& m1, const Mat2x2& m2) {
	return Mat2x2 {
		m1.a + m2.a, m1.b + m2.b, 
		m1.c + m2.c, m1.d + m2.d
	};
}

inline Mat2x2 operator-(const Mat2x2& m1, const Mat2x2& m2) {
	return Mat2x2 {
		m1.a - m2.a, m1.b - m2.b, 
		m1.c - m2.c, m1.d - m2.d
	};
}

inline Vec2 operator*(const Mat2x2& m, const Vec2& v) {
	return Vec2 {
		m.a * v.x + m.b * v.y,
		m.c * v.x + m.d * v.y,
	};
}

inline Mat2x2 operator*(const Mat2x2 m1, const Mat2x2& m2) {
	return Mat2x2 {
		m1.a * m2.a + m1.b * m2.c, m1.a * m2.b + m1.b * m2.d, 
		m1.c * m2.a + m1.d * m2.c, m1.c * m2.b + m1.d * m2.d
	};
}


// 3x3 Matrix
struct Mat3x3 {
	double a1,a2,a3,b1,b2,b3,c1,c2,c3;

	Mat3x3() {
		a1 = a2 = a3 = b1 = b2 = b3 = c1 = c2 = c3 = 0; 
	}

	Mat3x3(
		double _a1, double _b1, double _c1,
		double _a2, double _b2, double _c2,
		double _a3, double _b3, double _c3
	) {
		a1 = _a1; b1 = _b1; c1 = _c1;
		a2 = _a2; b2 = _b2; c2 = _c2;
		a3 = _a3; b3 = _b3; c3 = _c3;
	}

	Mat3x3& operator+=(const Mat3x3& other) {
		a1 += other.a1; b1 += other.b1; c1 += other.c1;
		a2 += other.a2; b2 += other.b2; c2 += other.c2;
		a3 += other.a3; b3 += other.b3; c3 += other.c3;
		return *this;
	}

	Mat3x3& operator*=(double a) {
		a1 *= a; b1 *= a; c1 *= a;
		a2 *= a; b2 *= a; c2 *= a;
		a3 *= a; b3 *= a; c3 *= a;
		return *this;
	}

	string to_str() {
		char buffer[512];
		snprintf(
			buffer, sizeof(buffer), "( %7lf %7lf %7lf\n  %7lf %7lf %7lf\n %7lf %7lf %7lf )",
		 	a1, b1, c1, a2, b2, c2, a3, b3, c3
		);
		return buffer;
	}
};

inline Mat3x3 operator*(const Mat3x3& m, double a) {
	return Mat3x3 {
		m.a1 * a, m.b1 * a, m.c1 * a,
		m.a2 * a, m.b2 * a, m.c2 * a,
		m.a3 * a, m.b3 * a, m.c3 * a,
	};
}

inline Mat3x3 operator+(const Mat3x3& m1, const Mat3x3& m2) {
	return Mat3x3 {
		m1.a1 + m2.a1, m1.b1 + m2.b1, m1.c1 + m2.c1,
		m1.a2 + m2.a2, m1.b2 + m2.b2, m1.c2 + m2.c2,
		m1.a3 + m2.a3, m1.b3 + m2.b3, m1.c3 + m2.c3
	};
}

inline Mat3x3 operator-(const Mat3x3& m1, const Mat3x3& m2) {
	return Mat3x3 {
		m1.a1 - m2.a1, m1.b1 - m2.b1, m1.c1 - m2.c1,
		m1.a2 - m2.a2, m1.b2 - m2.b2, m1.c2 - m2.c2,
		m1.a3 - m2.a3, m1.b3 - m2.b3, m1.c3 - m2.c3
	};
}

inline Vec3 operator*(const Mat3x3& m, const Vec3& v) {
	return Vec3 {
		m.a1 * v.x + m.b1 * v.y + m.c1 * v.z,
		m.a2 * v.x + m.b2 * v.y + m.c2 * v.z,
		m.a3 * v.x + m.b3 * v.y + m.c3 * v.z,
	};
}

inline Mat3x3 operator*(const Mat3x3 m1, const Mat3x3& m2) {
	return Mat3x3 {
		m1.a1 * m2.a1 + m1.b1 * m2.a2 + m1.c1 * m2.a3, m1.a1 * m2.b1 + m1.b1 * m2.b2 + m1.c1 * m2.b3, m1.a1 * m2.c1 + m1.b1 * m2.c2 + m1.c1 * m2.c3,
		m1.a2 * m2.a1 + m1.b2 * m2.a2 + m1.c2 * m2.a3, m1.a2 * m2.b1 + m1.b2 * m2.b2 + m1.c2 * m2.b3, m1.a2 * m2.c1 + m1.b2 * m2.c2 + m1.c2 * m2.c3,
		m1.a3 * m2.a1 + m1.b3 * m2.a2 + m1.c3 * m2.a3, m1.a3 * m2.b1 + m1.b3 * m2.b2 + m1.c3 * m2.b3, m1.a3 * m2.c1 + m1.b3 * m2.c2 + m1.c3 * m2.c3,
	};
}
