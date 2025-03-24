#pragma once

#include <stdio.h>
#include <initializer_list>
#include <cmath>

namespace lb
{
	template <typename _T, int length>
	class Vector;
	
	////////////////////////////////////
	////////////   MATRIX   ////////////
	////////////////////////////////////

	template <typename _T, int rows, int columns>
	class Matrix
	{
	public:
		static_assert(std::is_arithmetic<_T>::value > 0, "Expression must be arithmetic type");

		_T data[rows][columns];

		inline constexpr Matrix();
		inline constexpr Matrix(std::initializer_list<std::initializer_list<_T>> values);
		inline constexpr ~Matrix();

		static inline constexpr Matrix<_T, rows, columns> Identity();

		inline consteval int GetRows();
		inline constexpr Vector<_T, columns> GetRow(int v_index);
		inline consteval int GetColumns();

		inline constexpr void Fill(_T v_filler);

		inline constexpr Matrix<_T, columns, rows> Transpose();
		inline constexpr float Magnitude();
		inline constexpr Matrix<_T, rows, columns> Normalize();

		inline constexpr _T* operator[](int v_index);
		inline constexpr operator Vector<_T, columns>() const;
	};

	typedef Matrix<float, 4, 4> Matrix4f4;

	template<typename _T, int rows, int columns>
	inline constexpr Matrix<_T, rows, columns>::Matrix()
	{
		this->Fill(0);
		return;
	}

	template<typename _T, int rows, int columns>
	inline constexpr Matrix<_T, rows, columns>::Matrix(std::initializer_list<std::initializer_list<_T>> values)
	{
		int r = 0;
		for (const auto& row : values)
		{
			if (columns != row.size())
				throw 1;

			int c = 0;
			for (const auto& data : row)
			{
				this->data[r][c] = data;
				c++;
			}
			r++;
		}
	}

	template<typename _T, int rows, int columns>
	inline constexpr Matrix<_T, rows, columns>::~Matrix()
	{
		return;
	}

	template<typename _T, int rows, int columns>
	inline constexpr Matrix<_T, rows, columns> Matrix<_T, rows, columns>::Identity()
	{
		Matrix<_T, rows, columns> tmp;

		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < columns; c++)
			{
				if (r == c) tmp.data[r][c] = 1;
				else tmp.data[r][c] = 0;
			}
		}

		return tmp;
	}

	template<typename _T, int rows, int columns>
	inline consteval int Matrix<_T, rows, columns>::GetRows()
	{
		return rows;
	}

	template<typename _T, int rows, int columns>
	inline constexpr Vector<_T, columns> Matrix<_T, rows, columns>::GetRow(int v_index)
	{
		Vector<_T, columns> vVector;

		for (int c = 0; c < columns; c++)
			vVector[c] = this->data[v_index][c];

		return vVector;
	}

	template<typename _T, int rows, int columns>
	inline consteval int Matrix<_T, rows, columns>::GetColumns()
	{
		return columns;
	}

	template<typename _T, int rows, int columns>
	inline constexpr void Matrix<_T, rows, columns>::Fill(_T v_filler)
	{
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < columns; c++)
				this->data[r][c] = v_filler;
	}

	template<typename _T, int rows, int columns>
	inline constexpr Matrix<_T, columns, rows> Matrix<_T, rows, columns>::Transpose()
	{
		lb::Matrix<_T, columns, rows> vMx;

		for (int r = 0; r < rows; r++)
			for (int c = 0; c < columns; c++)
				vMx[c][r] = this->data[r][c];

		return vMx;
	}

	template<typename _T, int rows, int columns>
	inline constexpr float Matrix<_T, rows, columns>::Magnitude()
	{
		float vMagnitude = 0.f;

		for (int r = 0; r < rows; r++)
			for (int c = 0; c < columns; c++)
				vMagnitude += this->data[r][c] * this->data[r][c];

		vMagnitude = sqrt(vMagnitude);

		return vMagnitude;
	}

	template<typename _T, int rows, int columns>
	inline constexpr Matrix<_T, rows, columns> Matrix<_T, rows, columns>::Normalize()
	{
		Matrix<_T, rows, columns> vMx;

		float vMagnitude = this->Magnitude();

		for (int r = 0; r < rows; r++)
			for (int c = 0; c < columns; c++)
				vMx[r][c] = this->data[r][c] / vMagnitude;

		return vMx;
	}

	template<typename _T, int rows, int columns>
	inline constexpr _T* Matrix<_T, rows, columns>::operator[](int v_index)
	{
		return this->data[v_index];
	}

	template<typename _T, int rows, int columns>
	inline constexpr Matrix<_T, rows, columns>::operator Vector<_T, columns>() const
	{
		static_assert(rows == 1, "Matrix has too many rows to be cast to Vector");
		Vector<_T, columns> vVector;

		for (int c = 0; c < columns; c++)
			vVector[c] = this->data[0][c];

		return vVector;
	}

	////////////////////////////////////
	////////////   VECTOR   ////////////
	////////////////////////////////////

	template <typename _T, int length>
	class Vector : public Matrix<_T, 1, length> {
	public:
		using Matrix<_T, 1, length>::data;

		inline constexpr Vector() : Matrix<_T, 1, length>() {};
		inline constexpr Vector(std::initializer_list<_T> values);

		inline constexpr Vector<_T, 3> xyz();

		constexpr _T& operator[](int v_index);
		inline constexpr operator Matrix<_T, 1, length>() const;
	};

	typedef Vector<float, 3> Vector3f;
	typedef Vector<float, 4> Vector4f;

	template<typename _T, int length>
	inline constexpr Vector<_T, length>::Vector(std::initializer_list<_T> values)
		: Matrix<_T, 1, length>()
	{
		int c = 0;
		for (const auto& i : values)
		{
			this->data[0][c] = i;
			c++;
		}
	}

	template<typename _T, int length>
	inline constexpr Vector<_T, 3> Vector<_T, length>::xyz()
	{
		static_assert(length >= 3, "Not enough elements in vector for 'xyz'");
		return { this->data[0][0], this->data[0][1], this->data[0][2] };
	};			

	template<typename _T, int length>
	inline constexpr _T& Vector<_T, length>::operator[](int v_index)
	{
		return this->data[0][v_index];
	}

	template<typename _T, int length>
	inline constexpr Vector<_T, length>::operator Matrix<_T, 1, length>() const
	{
		Matrix<_T, 1, length> vMx;

		for (int c = 0; c < length; c++)
			vMx[0][c] = this->data[0][c];

		return vMx;
	}
}


template<typename _T, typename _S, int rows, int columns>
constexpr lb::Matrix<_T, rows, columns> operator*(lb::Matrix<_T, rows, columns> v_mx1, _S v_scalar)
{
	lb::Matrix<_T, rows, columns> vResult;

	for (int r = 0; r < rows; r++)
		for (int c = 0; c < columns; c++)
			vResult.data[r][c] = v_mx1.data[r][c] * v_scalar;

	return vResult;
}

template<typename _T, int rows1, int columns1, int rows2, int columns2>
constexpr lb::Matrix<_T, rows1, columns2> operator*(lb::Matrix<_T, rows1, columns1> v_mx1, lb::Matrix<_T, rows2, columns2> v_mx2)
{
	static_assert(columns1 == rows2, "columns of mx1 must = rows of mx2");
	
	lb::Matrix<_T, rows1, columns2> vResult;

	for (int i = 0; i < columns2; i++)
		for (int r = 0; r < rows1; r++)
			for (int c = 0; c < columns1; c++)
				vResult.data[r][i] += v_mx1.data[r][c] * v_mx2.data[c][i];

	return vResult;
}

template<typename _T, int rows, int columns>
constexpr lb::Matrix<_T, rows, columns> operator+(lb::Matrix<_T, rows, columns> v_mx1, lb::Matrix<_T, rows, columns> v_mx2)
{
	lb::Matrix<_T, rows, columns> vResult;

	for (int r = 0; r < rows; r++)
		for (int c = 0; c < columns; c++)
			vResult.data[r][c] = v_mx1.data[r][c] + v_mx2.data[r][c];

	return vResult;
}

template<typename _T, int rows, int columns>
constexpr lb::Matrix<_T, rows, columns> operator-(lb::Matrix<_T, rows, columns> v_mx1, lb::Matrix<_T, rows, columns> v_mx2)
{
	lb::Matrix<_T, rows, columns> vResult;

	for (int r = 0; r < rows; r++)
		for (int c = 0; c < columns; c++)
			vResult.data[r][c] = v_mx1.data[r][c] - v_mx2.data[r][c];

	return vResult;
}
