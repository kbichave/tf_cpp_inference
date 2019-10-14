
class TensorNet
{
    public:
        std::vector<std::vector<float>> getFeatures(std::vector <std::vector <float>> DIMATCH_Descriptor, 
                                std::string saveFileName, 
                                std::vector<std::string>input_nodes,
                                std::vector<std::string>output_nodes,
                                std::vector<int> input_dtype,
                                std::vector<std::vector<int>> node_size,
                                int output_feature_dimension,
                                std::string path_to_graph);
        void saveVectorFeatures(std::string filename, const std::vector<std::vector<float>> descriptor);
        void init_network(std::string path_to_graph);
        void close_session();
};