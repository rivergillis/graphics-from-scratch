#ifndef MAT_H_
#define MAT_H_

#include "common.h"

// Mat (basically a monochrome image format but it can technically fit T)

template <typename T>
class Mat {
  public:
    // Allocs and de-allocs in ctor and dtor.
    // Inits to zero.
    Mat(int cols, int rows);
    ~Mat();

    T* Row(int r);
    const T* Row(int r) const;

    T& At(int c, int r);
    const T At(int c, int r) const;
    T& operator()(int c, int r) { return At(c, r); }
    const T operator()(int c, int r) const { return At(c, r); }

    int Cols() const { return cols_; }
    int Rows() const { return rows_; }

    // Mat<T> operator+(const float amt) const;

  private:
    int cols_;
    int rows_;

    T* data_;
};


/// Implementation

template <typename T>
Mat<T>::Mat(int cols, int rows) {
  data_ = static_cast<T*>(std::calloc(cols * rows, sizeof(T)));
  cols_ = cols;
  rows_ = rows;
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
  free(data_);
}

// TODO: this should be const& but I'm dumb
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