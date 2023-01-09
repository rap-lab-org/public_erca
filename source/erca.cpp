
/*******************************************
 * Author: Zhongqiang Richard Ren. 
 * All Rights Reserved. 
 *******************************************/


#include "erca.hpp"
#include <set>
#include <memory>
#include <chrono>

namespace rzq{
namespace search{

ERCAKd::ERCAKd() {};

ERCAKd::~ERCAKd() {};

int ERCAKd::SetResourceLimits(std::vector<long> in) {
  _resource_limits = basic::CostVector(in);
  return 1;
};

int ERCAKd::Search(long vo, long vd, double time_limit) {
  // ### init heu ###
  InitHeu(vd);

  // ### init ###
  auto tstart = std::chrono::steady_clock::now();
  _vo = vo;
  _vd = vd;
  basic::CostVector zero_vec;
  zero_vec.resize(_graph->GetCostDim(), 0);
  Label lo(_GenLabelId(), vo, zero_vec, _Heuristic(_vo));
  _label[lo.id] = lo;
  _res.n_generated++;
  // _UpdateFrontier(lo);
  _open.insert( std::make_pair(lo.f, lo.id) );

  if (DEBUG_ERCA > 0) {
    std::cout << "[DEBUG] ERCAKd::Search Init, lo = " << lo << std::endl;
  }

  // ### main search loop ###
  while ( !_open.empty() ) {

    // check timeout
    auto tnow = std::chrono::steady_clock::now();
    if (std::chrono::duration<double>(tnow-tstart).count() > time_limit) {
      std::cout << "[INFO] ERCAKd::Search timeout !" << std::endl;
      _res.timeout = true;
      break;
    }

    // ## select label l, lexicographic order ##
    Label l = _label[ _open.begin()->second ];
    _open.erase(_open.begin());

    if (DEBUG_ERCA > 0) {
      std::cout << "[DEBUG] ### Pop l = " << l << std::endl;
    }

    // ## lazy dominance check ##
    if ( _FrontierCheck(l) || _ResourceCheck(l) ) { // [DIFF FROM EMOA]
      if (DEBUG_ERCA > 1) {
        std::cout << "[DEBUG] F- AND S-check, dom, cont..." << std::endl;
      }
      continue;
    }
    _UpdateFrontier(l);
    if (l.v == vd) {
      break; // [DIFF FROM EMOA]
    }
    if (DEBUG_ERCA > 1) {
      std::cout << "[DEBUG] ### Exp. " << l << std::endl;
    }

    // ## expand label l ##
    _res.n_expanded++;
    auto succs = _graph->GetSuccs(l.v);
    for (auto u : succs) {
      basic::CostVector gu = l.g + _graph->GetCost(l.v, u);
      Label l2(_GenLabelId(), u, gu, gu + _Heuristic(u));
      _label[l2.id] = l2;
      _parent[l2.id] = l.id;
      if (DEBUG_ERCA > 0) {
        std::cout << "[DEBUG] >>>> Loop v= " << u << " gen l' = " << l2 << std::endl;
      }
      if (_FrontierCheck(l2) || _ResourceCheck(l2) ) {
        if (DEBUG_ERCA > 1) {
          std::cout << "[DEBUG] ----- F-Check, dom, cont..." << std::endl;
        }
        continue;
      }
      if (DEBUG_ERCA > 0) {
        std::cout << "[DEBUG] ----- Add to open..." << std::endl;
      }
      _res.n_generated++;
      _open.insert( std::make_pair(l2.f, l2.id) );
    }
  };

  // ### post-process the results ###
  std::cout << "[INFO] ERCAKd::_PostProcRes..." << std::endl;
  _PostProcRes();

  auto tend = std::chrono::steady_clock::now();
  _res.n_domCheck = G_DOM_CHECK_COUNT;
  _res.rt_search = std::chrono::duration<double>(tend-tstart).count();

  // count nondom labels
  double total = 0;
  double den = 0;
  for (auto& iter : _alpha){
    double this_size = iter.second.label_ids.size();
    total += this_size;
    den += 1;
    if (this_size > _res.num_nondom_labels_max) {
      _res.num_nondom_labels_max = this_size;
    }
  }
  _res.num_nondom_labels_avg = total/den;

  std::cout << "[INFO] ERCAKd::Search exit." << std::endl;
  return 1;
};

void ERCAKd::SetHeuInflateRate(double w) {
  _w = w;
  return ;
};

bool ERCAKd::_ResourceCheck(Label l)
{
  for (size_t i = 0; i < _resource_limits.size(); i++){
    if (l.f[i+1] > _resource_limits[i]) {
      return true; // at least one type of resource violates the budget.
    }
  }
  return false; 
};

basic::CostVector ERCAKd::_Heuristic(long v) {
  auto out = basic::CostVector(0, _graph->GetCostDim());
  for (size_t cdim = 0; cdim < out.size(); cdim++) {
    out[cdim] = _dijks[cdim].GetCost(v);
    // out[cdim] = 0;
    if (out[cdim] < 0) {
      throw std::runtime_error( "[ERROR], unavailable heuristic !?" );
    }
  }
  out[0] *= _w; // heuristic inflation
  return out;
};

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////



} // end namespace search
} // end namespace rzq
