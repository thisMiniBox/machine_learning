#pragma once
#include<string>
#include<iostream>
namespace CMath {
#define PI       3.14159265358979323846   // pi
	// ����ת�Ƕ�
	static double radiansToDegrees(double radians)
	{
		return radians * 180.0 / PI;
	}

	// �Ƕ�ת����
	static double degreesToRadians(double degrees)
	{
		return degrees * PI / 180.0;
	}
	template<class T>
	struct _Vec3_Template_
	{
		T x;
		T y;
		T z;
		_Vec3_Template_(T val = 0) :x(val), y(val), z(val) {}
		_Vec3_Template_(T X, T Y, T Z) :x(X), y(Y), z(Z) {}

		_Vec3_Template_ normalize()const
		{
			auto Length = this->length();
			return _Vec3_Template_(x / Length, y / Length, z / Length);
		}
		static _Vec3_Template_ normalize(const _Vec3_Template_& vec)
		{
			// ���������ĳ��ȣ�ģ��
			double length = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

			// ���Գ��ȵõ���λ����
			_Vec3_Template_ result(vec.x / length, vec.y / length, vec.z / length);
			return result;
		}

		static T triple(const _Vec3_Template_& A, const _Vec3_Template_& B, const _Vec3_Template_& C)
		{
			return A.dot(B.cross(C));
		}
		double length()const
		{
			return sqrt(x * x + y * y + z * z);
		}

		static double length(const _Vec3_Template_& vec)
		{
			return vec.length();
		}

		T dot(const _Vec3_Template_& v2)const
		{
			return x * v2.x + y * v2.y + z * v2.z;
		}
		static double dot(const _Vec3_Template_& v1, const _Vec3_Template_& v2)
		{
			return v1.dot(v2);
		}
		_Vec3_Template_ cross(const _Vec3_Template_& other) const
		{
			// �����˵Ľ��
			T newX = y * other.z - z * other.y;
			T newY = z * other.x - x * other.z;
			T newZ = x * other.y - y * other.x;

			// �����µ�����
			_Vec3_Template_ result(newX, newY, newZ);
			return result;
		}
		static _Vec3_Template_ cross(const _Vec3_Template_& v1, const _Vec3_Template_& v2)
		{
			return v1.cross(v2);
		}
		_Vec3_Template_ project(const _Vec3_Template_& onto)const
		{
			if (isZeroVector() || onto.isZeroVector())
				return _Vec3_Template_();
			double ontoLengthSquared = onto.x * onto.x + onto.y * onto.y + onto.z * onto.z;
			// �������� v ��ͶӰ���� onto �ϵı���ͶӰ
			double scalarProjection = (x * onto.x + y * onto.y + z * onto.z) / ontoLengthSquared;

			// ����ͶӰ����
			_Vec3_Template_ projection;
			projection.x = onto.x * scalarProjection;
			projection.y = onto.y * scalarProjection;
			projection.z = onto.z * scalarProjection;

			return projection;
		}

		static _Vec3_Template_ project(const _Vec3_Template_& v, const _Vec3_Template_& onto)
		{
			return v.project(onto);
		}
		_Vec3_Template_ lerp(const _Vec3_Template_& end, T t)const
		{
			return Lerp(*this, end, t);
		}
		static _Vec3_Template_ lerp(const _Vec3_Template_& start, const _Vec3_Template_& end, T t)
		{
			T x = start.x + (end.x - start.x) * t;
			T y = start.y + (end.y - start.y) * t;
			T z = start.z + (end.z - start.z) * t;
			return _Vec3_Template_(x, y, z);
		}
		double projectionLength(const _Vec3_Template_& onto)const
		{
			// ����Ŀ�������ĳ��ȵ�ƽ��
			double ontoLengthSquared = onto.x * onto.x + onto.y * onto.y + onto.z * onto.z;

			// ���Ŀ���������ȵ�ƽ��Ϊ�㣬������
			if (ontoLengthSquared == 0)
			{
				return 0;
			}

			// �������� v ��Ŀ������ onto �ϵı���ͶӰ
			double scalarProjection = (x * onto.x + y * onto.y + z * onto.z) / ontoLengthSquared;

			// ����ͶӰ����
			double projectionLength = scalarProjection * sqrt(ontoLengthSquared);

			return projectionLength;
		}
		static double projectionLength(const _Vec3_Template_& v, const _Vec3_Template_& onto)
		{
			return v.projectionLength(onto);
		}

		bool isZeroVector()const
		{
			if (this->x == 0 && this->y == 0 && this->z == 0)
				return true;
			return false;
		}

		double calculateAngle(const _Vec3_Template_& other)
		{
			return calculateAngle(*this, other);
		}
		static double calculateAngle(const _Vec3_Template_& v1, const _Vec3_Template_& v2)
		{
			double dotProduct = v1.dot(v2);
			double lengthV1 = v1.length();
			double lengthV2 = v2.length();

			double angleInRadians = std::acos(dotProduct / (lengthV1 * lengthV2));
			return angleInRadians;
		}

		std::string toString()const
		{
			std::ostringstream os;
			os << x << ' ' << y << ' ' << z <<'\n';
			return os.str();
		}

		std::wstring toWstring()const
		{
			std::wostringstream os;
			os << x << L' ' << y << L' ' << z << L'\n';
			return os.str();
		}

		_Vec3_Template_ operator+(_Vec3_Template_ other)const
		{
			return _Vec3_Template_(x + other.x, y + other.y, z + other.z);
		}
		_Vec3_Template_ operator-(_Vec3_Template_ other)const
		{
			return _Vec3_Template_(x - other.x, y - other.y, z - other.z);
		}
		_Vec3_Template_ operator+(double size)const
		{
			// ���������ĳ���
			double length = sqrt(x * x + y * y + z * z);

			// ��������ĳ���Ϊ�㣬����ԭ����
			if (length == 0) {
				return *this;
			}

			// ������������
			double factor = (size + length) / length;

			// ������������ÿ������������������
			_Vec3_Template_ result;
			result.x = x * factor;
			result.y = y * factor;
			result.z = z * factor;

			return result;
		}
		_Vec3_Template_ operator-(double size)const
		{
			return *this + -size;
		}
		_Vec3_Template_ operator+=(double size)const
		{
			// ���������ĳ���
			double length = sqrt(x * x + y * y + z * z);

			// ��������ĳ���Ϊ�㣬����ԭ����
			if (length == 0) {
				return *this;
			}

			// ������������
			double factor = (size + length) / length;

			// ������������ÿ������������������
			_Vec3_Template_ result;
			result.x = x * factor;
			result.y = y * factor;
			result.z = z * factor;

			*this = result;
			return result;
		}

		_Vec3_Template_ operator-=(double size)const
		{
			return *this + -size;
		}
		_Vec3_Template_ operator*(double multiple)const
		{
			return _Vec3_Template_(x * multiple, y * multiple, z * multiple);
		}
		_Vec3_Template_ operator/(double multiple)const
		{
			return _Vec3_Template_(x / multiple, y / multiple, z / multiple);
		}
		_Vec3_Template_ operator+=(_Vec3_Template_ other)
		{
			return *this = _Vec3_Template_(x + other.x, y + other.y, z + other.z);
		}
		_Vec3_Template_ operator-=(_Vec3_Template_ other)
		{
			return *this = _Vec3_Template_(x - other.x, y - other.y, z - other.z);
		}
		_Vec3_Template_ operator*=(double multiple)
		{
			return *this = _Vec3_Template_(x * multiple, y * multiple, z * multiple);
		}
		_Vec3_Template_ operator/=(double multiple)
		{
			return *this = _Vec3_Template_(x / multiple, y / multiple, z / multiple);
		}
		//��ȡԪ��
		const T& operator[](char number)const
		{
			switch (number)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case '0':
				return x;
			case '1':
				return y;
			case '2':
				return z;
			case 'x':
				return x;
			case 'y':
				return y;
			case 'z':
				return z;
			default:
				return x;
				break;
			}
		}
		T& operator[](char number)
		{
			switch (number)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case '0':
				return x;
			case '1':
				return y;
			case '2':
				return z;
			case 'x':
				return x;
			case 'y':
				return y;
			case 'z':
				return z;
			default:
				return x;
				break;
			}
		}
		friend std::ostream& operator<<(std::ostream& os, const _Vec3_Template_& vec)
		{
			os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
			return os;
		}

		// ����>>�������ʵ�ִӱ�׼������������
		friend std::istream& operator>>(std::istream& is, _Vec3_Template_& vec)
		{
			is >> vec.x >> vec.y >> vec.z;
			return is;
		}
	};

	typedef _Vec3_Template_<float> Vec3;
	typedef _Vec3_Template_<double> Vec3d;
	typedef _Vec3_Template_<float> Color3f;
	typedef _Vec3_Template_<int> Color3;

	template<class T>
	struct _Vec4_Template_
	{
		T x;
		T y;
		T z;
		T w;
		_Vec4_Template_(T val = 0) :x(val), y(val), z(val), w(val) {}
		_Vec4_Template_(T X, T Y, T Z, T W) :x(X), y(Y), z(Z), w(W) {}

		_Vec4_Template_ normalize()const
		{
			auto Length = this->length();
			return _Vec4_Template_(x / Length, y / Length, z / Length, w / Length);
		}
		static _Vec4_Template_ normalize(const _Vec4_Template_& vec)
		{
			// ���������ĳ��ȣ�ģ��
			double length = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);

			// ���Գ��ȵõ���λ����
			_Vec4_Template_ result(vec.x / length, vec.y / length, vec.z / length, vec.w / length);
			return result;
		}
		double length()const
		{
			return sqrt(x * x + y * y + z * z + w * w);
		}

		static double length(const _Vec4_Template_& vec)
		{
			return vec.length();
		}

		double dot(const _Vec4_Template_& v2)const
		{
			return x * v2.x + y * v2.y + z * v2.z, w* v2.w;
		}
		static double dot(const _Vec4_Template_& v1, const _Vec4_Template_& v2)
		{
			return v1.dot(v2);
		}
		_Vec4_Template_ lerp(const _Vec4_Template_& end, T t)const
		{
			T x = x + (end.x - x) * t;
			T y = y + (end.y - y) * t;
			T z = z + (end.z - z) * t;
			T w = w + (end.w - w) * t;
			return _Vec4_Template_(x, y, z);
		}
		static _Vec4_Template_ lerp(const _Vec4_Template_& start,const _Vec4_Template_& end, T t)
		{
			return start.Lerp(end, t);
		}
		_Vec4_Template_ project(const _Vec4_Template_& target)const
		{
			T dotProduct = dot(*this, target);  // �������������ĵ��
			T targetLengthSquared = dot(target, target);  // ����Ŀ�������ĳ��ȵ�ƽ��

			if (targetLengthSquared == 0) {
				// Ŀ������Ϊ���������޷�����ͶӰ
				return _Vec4_Template_(0, 0, 0, 0);
			}

			T scaleFactor = dotProduct / targetLengthSquared;  // ����ͶӰ����

			_Vec4_Template_ projection = target * scaleFactor;  // ����ͶӰ����

			return projection;
		}

		static _Vec4_Template_ project(const _Vec4_Template_& v, const _Vec4_Template_& onto)
		{
			return v.project(onto);
		}

		T projectionLength(const _Vec4_Template_& onto)const
		{
			// ����Ŀ�������ĳ��ȵ�ƽ��
			T ontoLengthSquared = onto.dot(onto);

			// ���Ŀ���������ȵ�ƽ��Ϊ�㣬������
			if (ontoLengthSquared == 0)
			{
				return 0;
			}

			// �������� v ��Ŀ������ onto �ϵı���ͶӰ
			T scalarProjection = dot(*this,onto) / ontoLengthSquared;

			// ����ͶӰ����
			T projectionLength = scalarProjection * sqrt(ontoLengthSquared);

			return projectionLength;
		}
		static double projectionLength(const _Vec4_Template_& v, const _Vec4_Template_& onto)
		{
			return v.projectionLength(onto);
		}

		bool isZeroVector()const
		{
			if (this->x == 0 && this->y == 0 && this->z == 0 && this->w == 0)
				return true;
			return false;
		}

		std::string toString()const
		{
			std::ostringstream os;
			os << x << ' ' << y << ' ' << z << ' ' << w << '\n';
			return os.str();
		}

		std::wstring toWstring()const
		{
			std::wostringstream os;
			os << x << L' ' << y << L' ' << z << L' ' << w << L'\n';
			return os.str();
		}

		_Vec4_Template_ operator+(_Vec4_Template_ other)const
		{
			return _Vec4_Template_(x + other.x, y + other.y, z + other.z, w + other.w);
		}
		_Vec4_Template_ operator-(_Vec4_Template_ other)const
		{
			return _Vec4_Template_(x - other.x, y - other.y, z - other.z, w - other.w);
		}
		_Vec4_Template_ operator+(double size)const
		{
			// ���������ĳ���
			double Length = Length();

			// ��������ĳ���Ϊ�㣬����ԭ����
			if (Length == 0) {
				return *this;
			}

			// ������������
			double factor = (size + Length) / Length;

			// ������������ÿ������������������
			_Vec4_Template_ result;
			result.x = x * factor;
			result.y = y * factor;
			result.z = z * factor;
			result.w = w * factor;

			return result;
		}
		_Vec4_Template_ operator-(double size)const
		{
			return *this + -size;
		}
		_Vec4_Template_ operator+=(double size)const
		{
			// ���������ĳ���
			double length = sqrt(x * x + y * y + z * z);

			// ��������ĳ���Ϊ�㣬����ԭ����
			if (length == 0) {
				return *this;
			}

			// ������������
			double factor = (size + length) / length;

			// ������������ÿ������������������
			_Vec4_Template_ result;
			result.x = x * factor;
			result.y = y * factor;
			result.z = z * factor;

			*this = result;
			return result;
		}

		_Vec4_Template_ operator-=(double size)const
		{
			return *this + -size;
		}
		_Vec4_Template_ operator*(double multiple)const
		{
			return _Vec4_Template_(x * multiple, y * multiple, z * multiple, w * multiple);
		}
		_Vec4_Template_ operator/(double multiple)const
		{
			return _Vec4_Template_(x / multiple, y / multiple, z / multiple, w / multiple);
		}
		_Vec4_Template_ operator+=(_Vec4_Template_ other)
		{
			return *this = _Vec4_Template_(x + other.x, y + other.y, z + other.z, w + other.w);
		}
		_Vec4_Template_ operator-=(_Vec4_Template_ other)
		{
			return *this = _Vec4_Template_(x - other.x, y - other.y, z - other.z, w - other.w);
		}
		_Vec4_Template_ operator*=(double multiple)
		{
			return *this = _Vec4_Template_(x * multiple, y * multiple, z * multiple, w * multiple);
		}
		_Vec4_Template_ operator/=(double multiple)
		{
			return *this = _Vec4_Template_(x / multiple, y / multiple, z / multiple, w / multiple);
		}
		//��ȡԪ��
		const T& operator[](char number)const
		{
			switch (number)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			case '0':
				return x;
			case '1':
				return y;
			case '2':
				return z;
			case '3':
				return w;
			case 'x':
				return x;
			case 'y':
				return y;
			case 'z':
				return z;
			case 'w':
				return w;
			default:
				return x;
				break;
			}
		}
		T& operator[](char number)
		{
			switch (number)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			case '0':
				return x;
			case '1':
				return y;
			case '2':
				return z;
			case '3':
				return w;
			case 'x':
				return x;
			case 'y':
				return y;
			case 'z':
				return z;
			case 'w':
				return w;
			default:
				return x;
				break;
			}
		}
		friend std::ostream& operator<<(std::ostream& os, const _Vec4_Template_& vec)
		{
			os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
			return os;
		}

		// ����>>�������ʵ�ִӱ�׼������������
		friend std::istream& operator>>(std::istream& is, _Vec4_Template_& vec)
		{
			is >> vec.x >> vec.y >> vec.z >> vec.w;
			return is;
		}
	};
	typedef _Vec4_Template_<int> Color;
}