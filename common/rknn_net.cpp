#include "rknn_net.h"

RKNet::~RKNet() {
  if (_input_attrs) {
    delete[] _input_attrs;
    _input_attrs = NULL;
  }
  if (_output_attrs) {
    delete[] _output_attrs;
    _output_attrs = NULL;
  }
  if (_outputs) {
    delete[] _outputs;
    _outputs = NULL;
  }
  if (_ctx >= 0) {
    rknn_destroy(_ctx);
  }
}

int RKNet::loadModel(const char *fileName) {
  FILE *fp = fopen(fileName, "rb");
  if (fp == NULL) {
    printf("fopen %s fail!\n", fileName);
    return -1;
  }
  fseek(fp, 0, SEEK_END);
  size_t model_len = ftell(fp);
  unsigned char *model = (unsigned char *)malloc(model_len);
  fseek(fp, 0, SEEK_SET);
  if (model_len != fread(model, 1, model_len, fp)) {
    printf("fread %s fail!\n", fileName);
    free(model);
    model = NULL;
    return -2;
  }
  if (fp) {
    fclose(fp);
  }
  int ret = rknn_init(&_ctx, model, model_len, 0);
  if (ret < 0) {
    free(model);
    model = NULL;
    printf("rknn_init fail! ret=%d\n", ret);
    return -3;
  }
  free(model);
  model = NULL;
  query();
  mallocMemory();
}

int RKNet::query(void) {
  int ret = rknn_query(_ctx, RKNN_QUERY_IN_OUT_NUM, &_io_num, sizeof(_io_num));
  if (ret != RKNN_SUCC) {
    printf("rknn_query fail! ret=%d\n", ret);
  }
  return ret;
}

int RKNet::mallocMemory(void) {
  _input_attrs = new rknn_tensor_attr[_io_num.n_input];
  for (int i = 0; i < _io_num.n_input; i++) {
    _input_attrs[i].index = i;
    int ret = rknn_query(_ctx, RKNN_QUERY_INPUT_ATTR, &(_input_attrs[i]),
                         sizeof(rknn_tensor_attr));
    if (ret < 0) {
      printf("rknn_init error ret=%d\n", ret);
      return -1;
    }
  }
  _output_attrs = new rknn_tensor_attr[_io_num.n_output];
  for (size_t i = 0; i < _io_num.n_output; i++) {
    _output_attrs[i].index = i;
    int ret = rknn_query(_ctx, RKNN_QUERY_OUTPUT_ATTR, &(_output_attrs[i]),
                         sizeof(rknn_tensor_attr));
    if (ret != RKNN_SUCC) {
      printf("rknn_query fail! ret=%d\n", ret);
      return -2;
    }
  }
  _outputs = new rknn_output[_io_num.n_output];
  for (size_t i = 0; i < _io_num.n_output; i++) {
    _outputs[i].want_float = 1;
  }
}

RKExtractor RKNet::create() const { return RKExtractor(this); }

RKExtractor::RKExtractor(const RKNet *rknet) : _rknet(rknet) {
  _input_w = _rknet->_input_attrs[0].dims[1];
  _input_h = _rknet->_input_attrs[0].dims[2];
  _output_num = _rknet->_io_num.n_output;
}

RKExtractor::~RKExtractor() {
  rknn_outputs_release(_rknet->_ctx, _output_num, _rknet->_outputs);
}

int RKExtractor::input(const RKImage &input) {}

int RKExtractor::extract(std::vector<RKTensor> &outputs) {}