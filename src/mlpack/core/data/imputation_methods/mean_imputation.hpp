/**
 * @file mean_imputation.hpp
 * @author Keon Kim
 *
 * Definition and Implementation of the MeanImputation class.
 */
#ifndef MLPACK_CORE_DATA_IMPUTE_STRATEGIES_MEAN_IMPUTATION_HPP
#define MLPACK_CORE_DATA_IMPUTE_STRATEGIES_MEAN_IMPUTATION_HPP

#include <mlpack/core.hpp>

using namespace std;

namespace mlpack {
namespace data {

/**
 * A simple mean imputation
 */
template <typename T>
class MeanImputation
{
 public:
  void Apply (const arma::Mat<T>& input,
              arma::Mat<T>& output,
              const T& mappedValue,
              const size_t dimension,
              const bool transpose = true)
  {
    // initiate output
    output = input;

    double sum = 0;
    size_t elems = 0; // excluding nan or missing target

    using PairType = std::pair<size_t, size_t>;
    // dimensions and indexes are saved as pairs inside this vector.
    std::vector<PairType> targets;


    // calculate number of elements and sum of them excluding mapped value or
    // nan. while doing that, remember where mappedValue or NaN exists.
    if (transpose)
    {
      Log::Debug << "transpose mean imputation" << std::endl;
      for (size_t i = 0; i < input.n_cols; ++i)
      {
        if (input(dimension, i) == mappedValue)
        {
          targets.emplace_back(dimension, i);
        }
        else
        {
          elems++;
          sum += input(dimension, i);
        }
      }
    }
    else
    {
      Log::Debug << "un-transpose mean imputation" << std::endl;
      for (size_t i = 0; i < input.n_rows; ++i)
      {
        if (input(i, dimension) == mappedValue)
        {
          targets.emplace_back(i, dimension);
        }
        else
        {
          elems++;
          sum += input(i, dimension);
        }
      }
    }
    Log::Debug << "sum: " << sum << std::endl;
    Log::Debug << "elems: " << elems << std::endl;
    // calculate mean;
    const double mean = sum / elems;

    // Now replace the calculated mean to the missing variables
    // It only needs to loop through targets vector, not the whole matrix.
    for (const PairType& target : targets)
    {
      output(target.first, target.second) = mean;
    }
  }
}; // class MeanImputation

} // namespace data
} // namespace mlpack

#endif
