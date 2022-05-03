# rknn_project_cv

### 一、 软硬件环境

* 主板：RV1126/RV1109
* RKNN API Version: 1.7.1
* opencv:4.4.0

### 二、 更新日志

* 2022/5/3 增加DRM和RGA加速，加速比达到6倍左右，但仍有优化空间。
  
| 图像大小 | resize_opencv | resize_rga |
| :------: | :-----------: | :--------: |
| 640*480  |     22ms      |    3ms     |
| 1280*720 |     30ms      |    5ms     |