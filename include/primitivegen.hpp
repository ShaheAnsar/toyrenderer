#ifndef MY_PRIMITIVE_GEN
#define MY_PRIMITIVE_GEN

#include <vector>

namespace Rend{
  namespace Utils{
    namespace PrimitiveGen{
      enum class PrimType { UVSPERE, CUBE, QUAD };
      std::vector<float> gen_quad(std::size_t subdiv_x, std::size_t subdiv_y);
      std::vector<float> gen_cube(std::size_t subdiv);
      std::vector<float> gen_uvsphere(std::size_t subdiv);
      std::vector<float> gen_cylinder(std::size_t subdiv);
    }
  }
}
#endif
