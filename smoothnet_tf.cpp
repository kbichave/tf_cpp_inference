#include"cnpy.h"
#include <tensorflow/core/public/session.h>
#include <tensorflow/core/platform/env.h>

using namespace tensor

int main(int argc, char* argv[]) {
  // Initialize a tensorflow session
  Session* session;
  Status status = NewSession(SessionOptions(), &session);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
    return 1;
  }

  // Read in the protobuf graph we exported
  // (The path seems to be relative to the cwd. Keep this in mind
  // when using `bazel run` since the cwd isn't where you call
  // `bazel run` but from inside a temp folder.)
  GraphDef graph_def;
  status = ReadBinaryProto(Env::Default(), "graph.pb", &graph_def);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
    return 1;
  }

  // Add the graph to the session
  status = session->Create(graph_def);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
    return 1;
  }

  // Setup inputs and outputs:

  // Our graph doesn't require any inputs, since it specifies default values,
  // but we'll change an input to demonstrate.
  printf("AQUIIII\n");
  cnpy::NpyArray arr = cnpy::npy_load("data.npy");
  float* loaded_data = arr.data<float>();
  int nx,ny,nz;
  nx = ny = nz = 16;
  auto batch_size = 1000;
  auto stride = nx * ny * nz;
  Tensor X_reference(DT_FLOAT, TensorShape({1000,16,16,16,1}));

  auto dst = X_reference.flat<float>().data();
  for (int i = 0; i < batch_size*nx*ny*nz; i++) {
    dst[i] = loaded_data[i];
    //auto img = mnist.testData.at(i).pixelData;
    //std::copy_n(loaded_data[i], 1, dst);
    //dst += imageDim;
    //dst++;
  }
  //for(auto k1 = 0;k1<arr.shape[0];k1++)
  /*
  for(auto k1 = 0;k1<1000*16*16*16;k1++)
    if(loaded_data[k1]>0)
      printf("%d %f\n",k1,loaded_data[k1]);
      */
  //X_reference.tensor<float>()() = loaded_data;

  Tensor keep_probability(DT_FLOAT, TensorShape());
  keep_probability.scalar<float>()() = 1.0;

  std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
    { "X_reference", X_reference },
    { "keep_probability", keep_probability },
  };

  // The session will initialize the outputs
  std::vector<tensorflow::Tensor> outputs;

  // Run the session, evaluating our "c" operation from the graph
  status = session->Run(inputs, {"3DIM_cnn_1/copy"}, {}, &outputs);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
    return 1;
  }

  // Grab the first output (we only evaluated one graph node: "c")
  // and convert the node to a scalar representation.
  // auto output_c = outputs[0].scalar<float>();

  // (There are similar methods for vectors and matrices here:
  // https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/public/tensor.h)

  // Print the results
  std::cout << outputs[0].DebugString() << "\n"; // Tensor<type: float shape: [] values: 30>
  // std::cout << output_c() << "\n"; // 30

  // Free any resources used by the session
  session->Close();
  return 0;
}
