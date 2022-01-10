#ifndef SIECI_TYPES_HPP
#define SIECI_TYPES_HPP

#include <cstdlib>
#include <functional>


//TODO: sprawdzić typy tych danych (poniżej)
using ElementID = std::size_t;

using TimeOffset = int;

using Time = int;

using ProbabilityGenerator = std::function<double()>;

#endif
