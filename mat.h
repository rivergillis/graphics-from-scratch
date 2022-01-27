#ifndef MAT_H_
#define MAT_H_

#include "common.h"

// Mat (basically a monochrome image format but it can technically fit T)
// TODO: Make r,c part of the template.

template <typename T>
class Mat {
  public:
    Mat() {}

    // Allocs and de-allocs in ctor and dtor.
    // Inits to zero.
    Mat(int cols, int rows);
    Mat(const Mat<T>& other);
    Mat(Mat<T>&& other);
    Mat<T>& operator=(Mat<T>&& other);
    ~Mat();

    static Mat<T> Identity(int size);
    static Mat<T> RotateX(float degrees);
    static Mat<T> RotateY(float degrees);
    static Mat<T> RotateZ(float degrees);

    T* Row(int r);
    const T* Row(int r) const;

    T& At(int c, int r);
    const T At(int c, int r) const;
    T& operator()(int c, int r) { return At(c, r); }
    const T operator()(int c, int r) const { return At(c, r); }

    int Cols() const { return cols_; }
    int Rows() const { return rows_; }
    const T* Data() const { return data_; }

    Mat<T> operator+(const Mat<T>& other) const;
    Mat<T> operator-(const Mat<T>& other) const;
    Mat<T> operator*(const Mat<T>& other) const;
    // Treat the other vector as a column vector.
    // The result is a column vector as a CxR = 1 x Rows() matrix.
    // So if we multiply a 4x4 matrix by a 1x4 column vector, we get a 1x4 column vector.
    Mat<T> operator*(const Vec3<T>& other) const;

    // Treats this is a column vector.
    Vec3<T> ToVec3() const;

  private:
    int cols_;
    int rows_;

    T* data_ = nullptr;
};


/// Implementation

template <typename T>
Mat<T>::Mat(int cols, int rows) {
  data_ = static_cast<T*>(std::calloc(cols * rows, sizeof(T)));
  cols_ = cols;
  rows_ = rows;
}

template <typename T>
Mat<T>::Mat(const Mat<T>& other) {
  data_ = static_cast<T*>(std::calloc(other.Cols() * other.Rows(), sizeof(T)));
  cols_ = other.Cols();
  rows_ = other.Rows();
  memcpy(data_, other.Data(), cols_ * rows_ * sizeof(T));
}

template <typename T>
Mat<T>::Mat(Mat<T>&& other) {
  data_ = other.data_;
  cols_ = other.cols_;
  rows_ = other.rows_;
  other.data_ = nullptr;
}

template <typename T>
Mat<T>& Mat<T>::operator=(Mat<T>&& other) {
  if (this != &other) {
    if (data_) free(data_);
    data_ = other.data_;
    cols_ = other.cols_;
    rows_ = other.rows_;
    other.data_ = nullptr;
  }
  return *this;
}

template <typename T>
T* Mat<T>::Row(int r) {
  if (r < 0 || r >= rows_) {
    throw std::runtime_error("Row out of bounds.");
  }
  return &data_[r * cols_];
}

template <typename T>
const T* Mat<T>::Row(int r) const {
  if (r < 0 || r >= rows_) {
    throw std::runtime_error("Row out of bounds.");
  }
  return &data_[r * cols_];
}

template <typename T>
T& Mat<T>::At(int c, int r) {
  if (c < 0 || c >= cols_) {
    throw std::runtime_error("Column out of bounds.");
  }
  return Row(r)[c];
}

template <typename T>
const T Mat<T>::At(int c, int r) const {
  if (c < 0 || c >= cols_) {
    throw std::runtime_error("Column out of bounds.");
  }
  return Row(r)[c];
}

template <typename T>
Mat<T>::~Mat() {
  if (data_ != nullptr) {
    free(data_);
    data_ = nullptr;
  }
}

// Arithematic

template <typename T>
Mat<T> Mat<T>::operator+(const Mat<T>& other) const {
  if (cols_ != other.cols_ || rows_ != other.rows_) {
    throw std::runtime_error("Mat sizes don't match.");
  }
  Mat<T> result(cols_, rows_);
  for (int r = 0; r < rows_; ++r) {
    for (int c = 0; c < cols_; ++c) {
      result.At(c, r) = At(c, r) + other.At(c, r);
    }
  }
  return result;
}

template <typename T>
Mat<T> Mat<T>::operator-(const Mat<T>& other) const {
  if (cols_ != other.cols_ || rows_ != other.rows_) {
    throw std::runtime_error("Mat sizes don't match.");
  }
  Mat<T> result(cols_, rows_);
  for (int r = 0; r < rows_; ++r) {
    for (int c = 0; c < cols_; ++c) {
      result.At(c, r) = At(c, r) - other.At(c, r);
    }
  }
  return result;
}

template <typename T>
Mat<T> Mat<T>::operator*(const Mat<T>& other) const {
  if (cols_ != other.rows_) {
    throw std::runtime_error("Mat sizes don't match.");
  }
  Mat<T> result(other.cols_, rows_);
  for (int r = 0; r < result.Rows(); ++r) {
    for (int c = 0; c < result.Cols(); ++c) {
      T sum = 0;
      for (int i = 0; i < cols_; ++i) {
        sum += At(i, r) * other.At(c, i);
      }
      result.At(c, r) = sum;
    }
  }
  return result;
}

template <typename T>
Mat<T> Mat<T>::operator*(const Vec3<T>& other) const {
  if (cols_ != 3) {
    throw std::runtime_error("Mat must be col_size 3.");
  }
  Mat<T> result(1, rows_);
  for (int r = 0; r < result.Rows(); ++r) {
    const T sum = At(0, r) * other.x + At(1, r) * other.y + At(2, r) * other.z;
    result.At(0, r) = sum;
  }
  return result;
}

template <typename T>
Vec3<T> Mat<T>::ToVec3() const {
  if (rows_ != 3) {
    throw std::runtime_error("Mat must be row_size 3.");
  }
  return {At(0, 0), At(0, 1), At(0, 2)};
}

template <typename T>
Mat<T> Mat<T>::Identity(int size) {
  Mat<T> result(size, size);
  for (int i = 0; i < size; ++i) {
    result(i, i) = 1;
  }
  return result;
}

template <typename T>
Mat<T> Mat<T>::RotateX(float degrees) {
  const float radians = degrees * M_PI / 180.0f;
  const float c = cos(radians);
  const float s = sin(radians);
  Mat<T> result(3, 3);
  result(0, 0) = 1;
  result(1, 1) = c;
  result(2, 1) = -s;
  result(1, 2) = s;
  result(2, 2) = c;
  return result;
}

template <typename T>
Mat<T> Mat<T>::RotateY(float degrees) {
  const float radians = degrees * M_PI / 180.0f;
  const float c = cos(radians);
  const float s = sin(radians);
  Mat<T> result(3, 3);
  result(0, 0) = c;
  result(2, 0) = s;
  result(1, 1) = 1;
  result(0, 2) = -s;
  result(2, 2) = c;
  return result;
}

template <typename T>
Mat<T> Mat<T>::RotateZ(float degrees) {
  const float radians = degrees * M_PI / 180.0f;
  const float c = cos(radians);
  const float s = sin(radians);
  Mat<T> result(3, 3);
  result(0, 0) = c;
  result(1, 0) = -s;
  result(0, 1) = s;
  result(1, 1) = c;
  result(2, 2) = 1;
  return result;
}

template <typename T>
std::ostream& operator<< (std::ostream &out, const Mat<T>& mat) {
  for (int r = 0; r < mat.Rows(); ++r) {
    for (int c = 0; c < mat.Cols(); ++c) {
      out << mat(c, r) << "\t";
    }
    out << std::endl;
  }
  return out;
}

#endif // MAT_H_