
/*******************************************
 * Author: Zhongqiang Richard Ren.
 * All Rights Reserved.
 *******************************************/

#include "api.hpp"
#include "graph_io.hpp"
#include "debug.hpp"
#include <iostream>
#include <string>

void print_help_message();

int main( int argc, char *argv[] ) {

  // help
  if (argc >= 2) {
    std::string arg1 = argv[1];
    if (arg1 == "-h" || arg1 == "--help") {
      print_help_message();
      return 0;
    }
  }else{
    std::cout << " wrong input ... " << std::endl;
    print_help_message();
    return 0;
  }

  // get args
  for (int i = 0; i < argc; i++){
    std::cout << "[INFO] input arg[" << i << "]=" << argv[i] << std::endl;
  }

  long vo = std::stol(argv[1]);
  long vd = std::stol(argv[2]);
  double time_limit = std::stod(argv[3]);
  int M = std::stoi(argv[4]);

  int expected_args = M + 6 + (M-1);
  if (argc != expected_args) {
    std::cout << "M=" << M << ", expected 6+M+M-1 (" << expected_args << ") arguments, received " << argc << std::endl;
    return -1;
  }

  // get filenames
  std::string result_fname = argv[argc - 1];
  std::vector<std::string> input_fnames;
  for (int i = 0; i < M; i++) {
    input_fnames.push_back(argv[i + 5]);
  }

  // resource limits
  std::vector<long> resource_limits;
  for (int i = 0; i < M-1; i++) {
    resource_limits.push_back(std::stoi(argv[i + M + 5]));
    std::cout << " get resource limit " << resource_limits.back() << std::endl;
  }

  // timer for loading graph files
  rzq::basic::SimpleTimer timer;
  timer.Start();

  rzq::basic::Roadmap g;
  {
    int status = rzq::basic::ReadRoadmapFromFile(input_fnames, false, &g);
    if (status < 0) {
      // if return value is less than 0, then error
      std::cout << "Error: ReadRoadmapFromFile()" << std::endl;
      return -1;
    }
  }

  double load_graph_time = timer.GetDurationSecond();

  // do some print and verification to make sure the generated graph is correct.
  std::cout << "num_nodes: " << g.GetNumberOfNodes() << std::endl;
  std::cout << "num_edges: " << g.GetNumberOfEdges() << std::endl;
  std::cout << "cdims: " << g.GetCostDim() << std::endl;

  // ######################################### //
  // ####### Test 2 - run planner ######### //
  // ######################################### //

  rzq::search::EMOAResult res;
  rzq::search::RunERCA(&g, vo, vd, time_limit, resource_limits, &res);

  rzq::basic::SaveResultToFile(result_fname, load_graph_time, &res);

  return 1;
};

void print_help_message () {
  // std::cout << "./run_erca (arg1 v_start) (arg2 v_dest) (arg3 time_limit) (arg4 M) (arg5 graph1_path) (arg6 graph2_path) ... ((arg(M+4) graphM_path)) (arg(M+5) result_path)" << std::endl;

  std::cout << "./run_erca (arg1 v_start) (arg2 v_dest) (arg3 time_limit) (arg4 M) \n"
            << "\t(arg5 graph1_file_path) (arg6 graph2_file_path) ... ((arg(M+4) graphM_file_path)) \n"
            << "\t(arg(M+4+1) resource limit 1) (arg(M+4+2) resource limit 2) ... (arg(M+4+M-1) resource limit M-1) \n"
            << "\t(arg(M+4+M) result_path)" << std::endl;
  std::cout << "Note that here M is the number of graph files, which is equal to the number of resources + 1." << std::endl;

}
