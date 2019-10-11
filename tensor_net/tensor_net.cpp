#include <tensorflow/core/public/session.h>
#include <tensorflow/core/platform/env.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "tensor_net.hpp"


using namespace tensorflow;
using namespace std;

void saveVectorFeatures(std::string filename, const std::vector<std::vector<float>> descriptor)
{
    std::cout << "Saving Features to a CSV file:" << std::endl;
    std::cout << filename << std::endl;

    std::ofstream outFile;
    outFile.open(filename, std::ios::binary);

    float writerTemp;
    for (int i = 0; i < descriptor.size(); i++)
    {
        for (int j = 0; j < descriptor[i].size(); j++)
        {
            writerTemp = descriptor[i][j];
            outFile.write(reinterpret_cast<const char*>(&writerTemp), sizeof(float));
        }
    }
    outFile.close();
}

std::vector<std::vector<float>>  getFeatures(std::vector <std::vector <float>> DIMATCH_Descriptor, std::string saveFileName) {
  // Initialize a tensorflow session
  Session* session;
  Status status = NewSession(SessionOptions(), &session);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
    //return 1;
  }

  // Read in the protobuf graph we exported
  // (The path seems to be relative to the cwd. Keep this in mind
  // when using `bazel run` since the cwd isn't where you call
  // `bazel run` but from inside a temp folder.)

  GraphDef graph_def;
  status = ReadBinaryProto(Env::Default(), "graph.pb", &graph_def);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
  }

  // Add the graph to the session
  status = session->Create(graph_def);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
  }

  // Setup inputs and outputs:

  // Our graph doesn't require any inputs, since it specifies default values,
  // but we'll change an input to demonstrate.

  int nx,ny,nz;
  nx = ny = nz = 16;
  auto descriptor_size = DIMATCH_Descriptor.size();
  auto stride = nx * ny * nz;
  int feature_dimension = 32;
  vector<vector<float>> output_vector; //(descriptor_size, vector<float> (feature_dimension));
  int output_vector_pointer = 0;
  // output_vector.resize(descriptor_size);
  int batch_size = 1000;
  // Convert vector of vectors to vector
  int num_batches = int(descriptor_size/batch_size) + 1;
  int batch_itr = 0;
  for(int num=0; num<num_batches; num++)
  {
    printf("Batch no.: %d\n", num);
    int current_batch_size = 0;
    vector<vector<float>> discriptor_batch;
    if(num+1 == num_batches)
    {
      for(int val=batch_itr; val<descriptor_size; val++)
      {
        current_batch_size++;
        discriptor_batch.push_back(DIMATCH_Descriptor[val]);
      }
    }
    else
    {
      current_batch_size = batch_size;
      for(int val=batch_itr; val<batch_itr + batch_size; val++)
      {
        discriptor_batch.push_back(DIMATCH_Descriptor[val]);
      }
      batch_itr = batch_itr + batch_size;
    }
    
    std::vector<double> flat_discriptor;
    for(auto && value : discriptor_batch){
      flat_discriptor.insert(flat_discriptor.end(), value.begin(), value.end());
    }  
    Tensor X_reference(DT_FLOAT, TensorShape({current_batch_size,nx,ny,nz,1}));

    auto dst = X_reference.flat<float>().data();
    for (int i = 0; i < current_batch_size*nx*ny*nz; i++) {
      dst[i] = flat_discriptor[i];
    }
    Tensor keep_probability(DT_FLOAT, TensorShape());
    keep_probability.scalar<float>()() = 1.0;

    std::vector<std::pair<std::string, tensorflow::Tensor>> inputs = {
      { "X_reference", X_reference },
      { "keep_probability", keep_probability }
    };

    // The session will initialize the outputs
    std::vector<tensorflow::Tensor> outputs;

    // Run the session, evaluating our "c" operation from the graph
    status = session->Run(inputs, {"3DIM_cnn_1/copy"}, {}, &outputs);
    if (!status.ok()) {
      std::cout << status.ToString() << "\n";
    }




    // Grab the output from Tensor to make ti <vector<vector<float>>
    auto output_c = outputs[0].flat<float>().data();
    /*
    vector<float> temp;
    bool flag = true;
    for(int i=0; i< current_batch_size*feature_dimension; i++)
    {
      temp.push_back(output_c[i]);
      if(i%feature_dimension-1==0 && !flag)
      {
        flag = false;
        output_vector.push_back(temp);
        vector<float> temp;
      }
    }
    */
    auto idx = 0;
    for(auto row = 0;row < current_batch_size;row++){
      vector<float> tmp;
      for(auto col = 0;col < feature_dimension;col++){
        tmp.push_back(output_c[idx]);
        idx++;
      }
      output_vector.push_back(tmp);
    }


  }
  printf("Feature Computation Done!\n");
  // Free any resources used by the session
  session->Close();
  saveVector(saveFileName, output_vector);
  return output_vector;
}
