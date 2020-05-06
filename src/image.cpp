#include "roversight/roversight.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <experimental/filesystem>

namespace roversight {

ImageRGB::ImageRGB(const int r, const int c) : rows(r), cols(c) {
    pixels.resize(rows*cols);
}

Vector3f& ImageRGB::at(const int ir, const int ic) {
    const int r = std::clamp<int>(ir, 0, rows-1);
    const int c = std::clamp<int>(ic, 0, cols-1);
    return pixels[r * cols + c];
}

Vector3f ImageRGB::at(const int ir, const int ic) const {
    const int r = std::clamp<int>(ir, 0, rows-1);
    const int c = std::clamp<int>(ic, 0, cols-1);
    return pixels[r * cols + c];
}

Vector3f ImageRGB::at(const float fr, const float fc) const {
    // bilinear interpolation
    const int rm = (int)fr;  // prior row index
    const int rp = rm+1;     //  next row index
    const int cm = (int)fc;  // prior col index
    const int cp = cm+1;     //  next col index

    const float lerp_r = fr-rm; // row interpolant [0, 1)
    const float lerp_c = fc-cm; // col interpolant [0, 1)

    const Vector3f tl = at(rm, cm);   // top left
    const Vector3f tr = at(rm, cp);   // top right
    const Vector3f bl = at(rp, cm);   // bottom left
    const Vector3f br = at(rp, cp);   // bottom right

    // lerp the top row colors
    const Vector3f top_color = tl*(1-lerp_c) + tr*lerp_c;
    // lerp the bottom row colors
    const Vector3f bot_color = bl*(1-lerp_c) + br*lerp_c;
    // lerp the top and bottom row colors
    return top_color*(1-lerp_r) + bot_color*lerp_r;
}

void ImageRGB::from_file(const std::string& filename) {
    int x, y, n;

    uint8_t* data = stbi_load(filename.c_str(), &x, &y, &n, STBI_rgb);

    if(data == NULL) {
        throw std::runtime_error("Failed to load " + filename);
    }

    cols = x;
    rows = y;

    pixels.resize(x*y);
    for(int i=0; i<x*y; ++i) {
        pixels[i] = Vector3f { data[3*i]/255.0f, data[3*i+1]/255.0f, data[3*i+2]/255.0f };
    }
}

void ImageRGB::to_file(const std::string& filename) const {
    namespace fs = std::experimental::filesystem;
    auto path = fs::path(filename);
    auto ext = path.extension();

    char data[rows*cols*3];
    for(int i=0; i<rows*cols; ++i) {
        data[3*i+0] = static_cast<uint8_t>(pixels[i][0] * 255.0f);
        data[3*i+1] = static_cast<uint8_t>(pixels[i][1] * 255.0f);
        data[3*i+2] = static_cast<uint8_t>(pixels[i][2] * 255.0f);
    }

    if( ext == ".bmp" ) {
        stbi_write_bmp(filename.c_str(), cols, rows, 3, (void*)data);
    } else if( ext == ".jpg" ) {
        stbi_write_jpg(filename.c_str(), cols, rows, 3, (void*)data, 100);
    }
}

ImageRGB ImageRGB::undistort(const Camera& cam) const {
    assert(cam.width == cols);
    assert(cam.height == rows);

    ImageRGB new_img(rows, cols);

    Matrix3f K;
    K << cam.fx, cam.skew, cam.cx,
           0.0f,   cam.fy, cam.cy,
           0.0f,     0.0f,   1.0f;

    float f_rows = (float)rows;
    float f_cols = (float)cols;
    float S = std::max(f_rows, f_cols);

    for(int iy=0; iy<rows; ++iy) {
        for(int ix=0; ix<cols; ++ix) {
            float x = (ix-f_cols/2.0) / S;   // [-1, 1)
            float y = (iy-f_rows/2.0) / S;   // [-1, 1)

            const float k1 = cam.r1;
            const float k2 = cam.r2;
            const float k3 = cam.r3;
            const float p1 = cam.t1;
            const float p2 = cam.t2;

            const float r2 = x*x + y*y;
            const float r4 = r2*r2;
            const float r6 = r4*r2;
            const float kr = (1.0 + k1*r2 + k2*r4 + k3*r6);

            float xp = x*kr + 2.0f*p1*x*y + p2*(r2 + 2.0*x*x);
            float yp = y*kr + 2.0f*p2*x*y + p1*(r2 + 2.0*y*y);

            Vector3f xy1 {xp, yp, 1.0f};
            xy1 = K*xy1;
            xp = xy1[0]/xy1[2];
            yp = xy1[1]/xy1[2];

            if(0 <= yp && yp < rows && 0 <= xp && xp < cols) {
                new_img.at(iy,ix) = this->at(yp,xp);
            } else {
                new_img.at(iy,ix) = Vector3f {1.0f, 0.0f, 0.0f};
            }
        }
    }
    return new_img;
}

}
