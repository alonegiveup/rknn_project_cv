#ifndef _RKNN_NET_H
#define _RKNN_NET_H

#include <memory>
#include <vector>

#include "rknn_api.h"
class RKExtractor;
typedef struct {
  int width;    // tensor width
  int height;   // tensor height
  int channel;  // tensor channel
  float* data;  // tensor data
} RKTensor;

typedef struct {
  int width;
  int height;
  uint8_t* data;
} RKImage;

class RKNet {
 public:
  RKNet()
      : _ctx(-1),
        _io_num({0}),
        _input_attrs(nullptr),
        _output_attrs(nullptr),
        _outputs(nullptr) {}
  rknn_context _ctx;
  rknn_input_output_num _io_num;
  rknn_tensor_attr* _input_attrs;
  rknn_tensor_attr* _output_attrs;
  rknn_output* _outputs;

  ~RKNet();
  // load model from file
  int loadModel(const char* fileName);
  // create extractor
  RKExtractor create() const;

 private:
  int query(void);
  int mallocMemory(void);
};

class RKExtractor {
 public:
  ~RKExtractor();

  int input(const RKImage& input);

  int extract(std::vector<RKTensor>& outputs);

 protected:
  friend RKExtractor RKNet::create() const;
  RKExtractor(const RKNet* rknet);

 private:
  const RKNet* _rknet;
  int _input_w;
  int _input_h;
  int _output_num;
};

#endif