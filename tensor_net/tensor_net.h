#include <tensorflow/core/public/session.h>
#include <tensorflow/core/platform/env.h>

void saveVectorFeatures(std::string filename, const std::vector<std::vector<float>> descriptor);
std::vector<tensorflow::Tensor>  getFeatures(std::vector <std::vector <float>> DIMATCH_Descriptor);
