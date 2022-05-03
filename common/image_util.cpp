#include "image_util.h"

void ImageUtil::init(void) {
  memset(&rga_ctx, 0, sizeof(rga_context));
  memset(&drm_ctx, 0, sizeof(drm_context));
  drm_fd = drm_init(&drm_ctx);
  drm_buf = drm_buf_alloc(&drm_ctx, drm_fd, 1920, 1080, 24, &buf_fd, &handle,
                          &actual_size);
  RGA_init(&rga_ctx);
}
void ImageUtil::release(void) {
  drm_buf_destroy(&drm_ctx, drm_fd, buf_fd, handle, drm_buf, actual_size);
  drm_deinit(&drm_ctx, drm_fd);
  RGA_deinit(&rga_ctx);
}

void ImageUtil::resize(const cv::Mat& src, const cv::Size& size, void* dstPtr) {
  if (src.empty()) {
    printf("src is empty!\n");
    return;
  }
  int img_width = src.cols;
  int img_height = src.rows;
  memcpy(drm_buf, src.data, img_width * img_height * 3);
  img_resize_slow(&rga_ctx, drm_buf, img_width, img_height, dstPtr, size.width,
                  size.height);
}

ImageUtil& ImageUtil::getInstance() {
  static ImageUtil util;
  return util;
}
