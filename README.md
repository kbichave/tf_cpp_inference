# Tensorflow C++ Inference Using Frozen Graph

This part of code is an example of how to use TensorNet, a TensorFlow C++ API for doing the inference of the model trained using TensorFlow Python and saved in .pb. The example uses a model [3DSmoothNet](https://github.com/zgojcic/3DSmoothNet) trained model for extracting features of dimension 32. 

Tu run demo:
```bash
$ ./run_inference.sh
```
The Python [code](https://github.com/path-robotics/tf_cpp_frozengraph_inference/blob/master/frozen_graoh/encrypt_decrypt.py) to encrypt and decrypt the frozen graph saved in .pb file is also provided. It depends on [pyAesCrypt](https://pypi.org/project/pyAesCrypt/).

## TensorNet Reference

| Members        | Description          | 
| :------------- |:-------------| 
| [TensorNet::saveVectorFeatures](https://github.com/path-robotics/tf_cpp_frozengraph_inference/blob/master/tensor_net/tensor_net.cpp#L16)     | Saves the values of arguments in the .csv format. |
| [TensorNet::init_network](https://github.com/path-robotics/tf_cpp_frozengraph_inference/blob/master/tensor_net/tensor_net.cpp#L36)| Initializes a session and loads a graph.|
| [TensorNet::close_session](https://github.com/path-robotics/tf_cpp_frozengraph_inference/blob/master/tensor_net/tensor_net.cpp#L59) | Closes the session and frees memory occupied by session.|
|[TensorNet::getFeatures](https://github.com/path-robotics/tf_cpp_frozengraph_inference/blob/master/tensor_net/tensor_net.cpp#L63) |Given arguments performs inference on the model using loaded graph and session created by [TensorNet::init_network](https://github.com/path-robotics/tf_cpp_frozengraph_inference/blob/master/tensor_net/tensor_net.cpp#L36) |


### TensorNet::saveVectorFeatures
Saves the values of arguments in the .csv format.

#### Summary

Arguments: <br />
> filename(std::string): Name of the csv file to save the data to.<br />
> descriptor (std::vector<std::vector<float>>): The data in the form of vector\<vector\>. <br />

Returns: <br />
> void


### TensorNet::init_network
Initializes a session and loads a graph.

#### Summary

Arguments: <br />
> path_to_graph(std::string): Absolute path to frozen graph. In current case the frozen graph is copied into the build folder. 

Returns: <br />
> void

### TensorNet::close_session
Closes the session and frees memory occupied by session.

### TensorNet::getFeatures
Given arguments performs inference on the model using loaded graph and session created by [TensorNet::init_network](https://github.com/path-robotics/tf_cpp_frozengraph_inference/blob/master/tensor_net/tensor_net.cpp#L36).

#### Summary

Arguments: <br />
> DIMATCH_Descriptor(std::vector\<std::vector\<float\>\>): Data to the input node. In this case it is assumed that one Tensor and one scalar is fed to the graph. The values for input placeholder with Tensor input are created based on this argument.<br />

> saveFileName(std::string): Name of the csv file to save the data to.<br />

> input_nodes(std::vector\<std::string\>): The names of all the input nodes. Again, it is assumed here that there are two input nodes, one taking Tensor and the other taking scalar. <br />

> output_nodes(std::vector\<std::string\>): Name of the output node.<br />

> input_dtype(std::vector\<int\>): The type of input corresponding to the names in input_nodes. 0 for scalar, 1 for Tensor.<br />

> node_size(std::vector\<std::vector<int\>\>): The dimension for input in reference to a Tensor input node. <br />

> output_feature_dimension(int): The dimension of output tensor for each datapoint. <br />



Returns: <br />
> output_vector(std::vector\<std::vector\<float\>\>): The output computed by inference converted to vector of vectors from vector of Tensors.