
/*******************************************
 * Author: Zhongqiang Richard Ren. 
 * All Rights Reserved. 
 *******************************************/


#ifndef ZHONGQIANGREN_SEARCH_API_H_
#define ZHONGQIANGREN_SEARCH_API_H_

#include "emoa.hpp"
#include "erca.hpp"

namespace rzq{
namespace search{


/**
 * @brief The entry point of EMOA*.
 * g - a pointer of graph, which can be either of class GridkConn or Roadmap. Polymorphism is used.
 * vo,vd - start and goal nodes.
 * time_limit - the run time limit for search.
 * res - the output argument.
 */
int RunEMOA(basic::Graph* g, long vo, long vd, double time_limit, rzq::search::EMOAResult* res) {

  size_t cdim = g->GetCostDim();
  std::cout << "[INFO] RunEMOA, M=" << cdim << " time_limit = " << time_limit << std::endl;
  int ret_flag = 0;

  // The following if-else is actually minor. Just use EMOAKd for all cases is totally fine...
  if (cdim == 3) {
    auto planner = rzq::search::EMOA();
    planner.SetGraphPtr(g) ; // set graph to the planner
    planner.InitHeu(vd); // this vd must be the same as the vd in Search().
    ret_flag = planner.Search(vo, vd, time_limit) ;
    *res = planner.GetResult(); // get result
  }else {
    auto planner = rzq::search::EMOAKd();
    planner.SetGraphPtr(g) ; // set graph to the planner
    planner.InitHeu(vd); // this vd must be the same as the vd in Search().
    ret_flag = planner.Search(vo, vd, time_limit) ;
    *res = planner.GetResult(); // get result
  }

  return ret_flag; // TODO, add more return flags.
};


/**
 * @brief The entry point of ERCA* (Enhanced Resource Constrained A*).
 * g - a pointer of graph, which can be either of class GridkConn or Roadmap. Polymorphism is used.
 * vo,vd - start and goal nodes.
 * time_limit - the run time limit for search.
 * resource_limits - limit for each type of resource.
 * res - the output argument.
 */
int RunERCA(basic::Graph* g, long vo, long vd, double time_limit, std::vector<long> resource_limits, rzq::search::EMOAResult* res, double w=1.0) {

  size_t cdim = g->GetCostDim();
  std::cout << "[INFO] RunERCA, M=" << cdim << " time_limit = " << time_limit << std::endl;
  int ret_flag = 0;

  if (w < 1.0) {
    std::cout << "[ERROR] heuristic inflation rate " << w << " is smaller than 1.0 !!" << std::endl;
    return 0;
  }

    auto planner = rzq::search::ERCAKd();
    planner.SetHeuInflateRate(w);
    planner.SetGraphPtr(g) ; // set graph to the planner
    planner.SetResourceLimits(resource_limits);
    planner.InitHeu(vd); // this vd must be the same as the vd in Search().
    ret_flag = planner.Search(vo, vd, time_limit) ;
    *res = planner.GetResult(); // get result

  // // The following if-else is actually minor. Just use EMOAKd for all cases is totally fine...
  // if (cdim == 3) {
  //   auto planner = rzq::search::ERCA3d();
  //   planner.SetGraphPtr(g) ; // set graph to the planner
  //   planner.InitHeu(vd); // this vd must be the same as the vd in Search().
  //   ret_flag = planner.Search(vo, vd, time_limit) ;
  //   *res = planner.GetResult(); // get result
  // }else {
  //   auto planner = rzq::search::ERCAKd();
  //   planner.SetGraphPtr(g) ; // set graph to the planner
  //   planner.InitHeu(vd); // this vd must be the same as the vd in Search().
  //   ret_flag = planner.Search(vo, vd, time_limit) ;
  //   *res = planner.GetResult(); // get result
  // }

  return ret_flag; // TODO, add more return flags.
};



} // end namespace search
} // end namespace rzq


#endif  // ZHONGQIANGREN_SEARCH_API_H_
