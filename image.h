#ifndef IMAGE_H_
#define IMAGE_H_

#include "common.h"
#include "vec.h"

// RGB24 Image Format
// TODO: Make this thread-safe

class Image {
  public:
    // Allocs and de-allocs in ctor and dtor.
    Image(int cols, int rows);
    ~Image();

    uint8_t* Row(int row);
    const uint8_t* Row(int row) const;

    // Returns a pixel channel value that can be changed.
    uint8_t& At(int col, int row, int channel);
    const uint8_t At(int c, int r, int channel) const;
    uint8_t& operator()(int col, int row, int channel) { return At(col, row, channel); }
    const uint8_t operator()(int col, int row, int channel) const { return At(col, row, channel); }

    void SetPixel(const Vec2<int>& xy, const Color& pix);
    void SetAll(uint8_t value);
    void SetAll(const Color& pix);

    // Gets a cropped set of rows. Useful for overscan.
    //  GetCroppedView(0, 240) will give you data for 240 rows.
    struct CropInfo {
      uint8_t* data;
      size_t length;     // length of data
      size_t row_width;  // unchanged from Image::RowWidth
    };
    CropInfo GetCroppedView(int first_row, int num_rows);

    int Cols() const { return cols_; }
    int Rows() const { return rows_; }
    // Row width in bytes.
    size_t RowWidth() const { return cols_ * 3 * sizeof(uint8_t); }

    // The size of the output buffer is exactly Rows() * RowWidth().
    uint8_t* Data() { return data_; }
    const uint8_t* Data() const { return data_; }

    std::string ToPPM() const;

  private:
    int cols_;
    int rows_;

    uint8_t* data_;
};

#endif 