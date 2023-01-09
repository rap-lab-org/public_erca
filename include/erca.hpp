
/*******************************************
 * Author: Zhongqiang Richard Ren. 
 * All Rights Reserved. 
 *******************************************/


#ifndef ZHONGQIANGREN_SEARCH_ERCA_H_
#define ZHONGQIANGREN_SEARCH_ERCA_H_

#include "emoa.hpp"

namespace rzq{
namespace search{

#define DEBUG_ERCA 0

/**
 * Enhanced Resource-Constrained A* (E-RC-A*) for any number of resource constraints.
 * This is inherited from EMOAKd.
 */
class ERCAKd : public EMOAKd
{
public:
  ERCAKd();
  virtual ~ERCAKd();
  virtual int SetResourceLimits(std::vector<long> resource_limits) ; // New API
  virtual int Search(long vo, long vd, double time_limit) override ;
  virtual void SetHeuInflateRate(double w) ;
protected:
  virtual bool _ResourceCheck(Label l); // New procedure
  virtual basic::CostVector _Heuristic(long v) override ;
  basic::CostVector _resource_limits;
  double _w = 1.0;
};

} // end namespace search
} // end namespace rzq


#endif  // ZHONGQIANGREN_SEARCH_EMOA_H_
