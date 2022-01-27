#include "image.h"
#include "common.h"
#include "vec.h"

Image::Image(int cols, int rows) {
  data_ = static_cast<uint8_t*>(std::malloc(cols * rows * 3 * sizeof(uint8_t)));
  cols_ = cols;
  rows_ = rows;
}

uint8_t* Image::Row(int row) {
  if (row < 0 || row >= rows_) {
    throw std::runtime_error("Row out of bounds " + std::to_string(row));
  }
  return &data_[row * RowWidth()];
}

const uint8_t* Image::Row(int row) const {
  if (row < 0 || row >= rows_) {
    throw std::runtime_error("Row out of bounds " + std::to_string(row));
  }
  return &data_[row * RowWidth()];
}

uint8_t& Image::At(int col, int row, int channel) {
  if (col < 0 || col >= cols_) {
    throw std::runtime_error("Column out of bounds " + std::to_string(col));
  } else if (channel < 0 || channel > 2) {
    throw std::runtime_error("Channel out of bounds " + std::to_string(channel));
  }
  return Row(row)[(col * 3) + channel];
}

const uint8_t Image::At(int col, int row, int channel) const {
  if (col < 0 || col >= cols_) {
    throw std::runtime_error("Column out of bounds " + std::to_string(col));
  } else if (channel < 0 || channel > 2) {
    throw std::runtime_error("Channel out of bounds " + std::to_string(channel));
  }
  return Row(row)[(col * 3) + channel];
}

void Image::SetPixel(const Vec2<int>& xy, const Color& pix) {
  At(xy.x, xy.y,  0) = pix.x;
  At(xy.x, xy.y,  1) = pix.y;
  At(xy.x, xy.y,  2) = pix.z;
}

void Image::SetAll(uint8_t value) {
  std::memset(data_, value, rows_ * RowWidth());
}

void Image::SetAll(const Color& pix) {
  for (int r = 0; r < Rows(); r++) {
    for (int c = 0; c < Cols(); c++) {
      SetPixel({c,r}, pix);
    }
  }
}

Image::CropInfo Image::GetCroppedView(int first_row, int num_rows) {
  if (first_row < 0 || first_row >= rows_ || (first_row + num_rows) > rows_ || num_rows < 0) {
    throw std::runtime_error("Bad bound on GetCroppedView");
  }
  return {Row(first_row), num_rows * RowWidth(), RowWidth()};
}

std::string Image::ToPPM() const {
  std::stringstream ss;
  ss << "P3\n" << Cols() << " " << Rows() << "\n255\n";
  for (int r = 0; r < Rows(); r++) {
    for (int c = 0; c < Cols() ;c++) {
      ss << (int)At(c, r, 0) << " " << (int)At(c, r, 1) << " " << (int)At(c, r, 2) << "\n";
    }
  }
  return ss.str();
}

Image::~Image() {
  free(data_);
}