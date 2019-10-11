//#include <tensorflow/core/public/session.h>
//#include <tensorflow/core/platform/env.h>

std::vector<std::vector<float>>  getFeatures(std::vector <std::vector <float>> DIMATCH_Descriptor, std::string saveFileName);
void saveVector(std::string filename, const std::vector<std::vector<float>> descriptor);
