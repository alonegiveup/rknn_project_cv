#ifndef _IMAGE_UTIL_H
#define _IMAGE_UTIL_H

#include "drm_func.h"
#include "opencv2/opencv.hpp"
#include "rga_func.h"

class ImageUtil {
 private:
  ImageUtil() { init(); };
  virtual ~ImageUtil() { release(); };
  ImageUtil(const ImageUtil &) = delete;
  ImageUtil(ImageUtil &&) = delete;
  ImageUtil &operator=(const ImageUtil &) = delete;
  ImageUtil &operator=(ImageUtil &&) = delete;
  void *drm_buf = NULL;
  int drm_fd = -1;
  int buf_fd = -1;  // converted from buffer handle
  unsigned int handle;
  size_t actual_size = 0;
  rga_context rga_ctx;
  drm_context drm_ctx;
  void init(void);
  void release(void);

 public:
  static ImageUtil &getInstance();

  void resize(const cv::Mat &src, const cv::Size &size, void *dstPtr);
};

#endif