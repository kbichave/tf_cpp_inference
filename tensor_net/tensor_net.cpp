#include <tensorflow/core/public/session.h>
#include <tensorflow/core/platform/env.h>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "tensor_net.hpp"


using namespace tensorflow;
using namespace boost::program_options;
using namespace std;

Session* session;
Status status = NewSession(SessionOptions(), &session);

void TensorNet::saveVectorFeatures(std::string filename, const std::vector<std::vector<float>> descriptor)
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

void TensorNet::init_network(std::string path_to_graph)
{
  
  
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
    //return 1;
  }

  // Read in the protobuf graph
  GraphDef graph_def;
  status = ReadBinaryProto(Env::Default(),path_to_graph, &graph_def);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
  }

  // Add the graph to the session
  status = session->Create(graph_def);
  if (!status.ok()) {
    std::cout << status.ToString() << "\n";
  }

}
void TensorNet::close_session()
{
  session->Close();
}
std::vector<std::vector<float>>  TensorNet::getFeatures(std::vector<std::vector<float>> DIMATCH_Descriptor, 
                                std::string saveFileName, 
                                std::vector<std::string>input_nodes,
                                std::vector<std::string>output_nodes,
                                std::vector<int> input_dtype,
                                std::vector<std::vector<int>> node_size,
                                int output_feature_dimension, 
                                std::string path_to_graph) 
{

  // Setup inputs and outputs:
  auto descriptor_size = DIMATCH_Descriptor.size();
  vector<vector<float>> output_vector; 
  int batch_size = 1000;
  
  int num_batches = int(descriptor_size/batch_size) + 1;
  int batch_itr = 0;

  for(int num=0; num<num_batches; num++)
  {
    vector<Tensor> feed_dict;
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
    printf("Creating feed dict \n");
    for(int i=0;i<input_dtype.size();i++)
    {
      if(input_dtype[i]==1)
      {
        printf("Data Type 1");
        std::vector<long long int> current_node_dimension;
        std::vector<int> node_dimension = node_size[i];
        current_node_dimension.push_back(current_batch_size);
        int num_elements_in_batch = current_batch_size;
        for(int i=0; i< node_dimension.size();i++)
        {
          num_elements_in_batch *= node_dimension[i];
          current_node_dimension.push_back(node_dimension[i]);
        }
        Tensor temp(DT_FLOAT, TensorShape(current_node_dimension));
        auto dst = temp.flat<float>().data();
        for (int i = 0; i < num_elements_in_batch; i++) {
          dst[i] = flat_discriptor[i];
        }
        feed_dict.push_back(temp);
      }
      else if(input_dtype[i]==0)
      {
        printf("Data Type 0");
        std::vector<int> node_dimension = node_size[i];
        Tensor temp(DT_FLOAT, TensorShape());
        temp.scalar<float>()() = float(node_dimension[0]);
        feed_dict.push_back(temp);
      }
    }
    printf("Feed dict created\n");
    std::cout<<feed_dict.size();
    std::vector<std::pair<std::string, tensorflow::Tensor>> inputs;
    for(int i=0; i<input_dtype.size(); i++)
    {
      std::pair<std::string, tensorflow::Tensor> temp = {input_nodes[i], feed_dict[i]};
      inputs.push_back(temp);
    }
    
    printf("Inputs created \n");
    std::cout<<inputs[0].first;
    // The session will initialize the outputs
    std::vector<tensorflow::Tensor> outputs;

    // Run the session, evaluating our "c" operation from the graph
    status = session->Run(inputs, output_nodes, {}, &outputs);
    if (!status.ok()) {
      std::cout << status.ToString() << "\n";
    }

    // Grab the output from Tensor to make ti <vector<vector<float>>
    auto output_c = outputs[0].flat<float>().data();

    auto idx = 0;
    for(auto row = 0;row < current_batch_size;row++){
      vector<float> tmp;
      for(auto col = 0;col < output_feature_dimension;col++){
        tmp.push_back(output_c[idx]);
        idx++;
      }
      output_vector.push_back(tmp);
    }
  }
  printf("Feature Computation Done!\n");
  saveVectorFeatures(saveFileName, output_vector);
  return output_vector;
}
