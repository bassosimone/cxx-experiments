namespace MeasurementKit {
namespace common {
namespace constraints {

struct NonCopyable {
  NonCopyable() {}
  NonCopyable(NonCopyable&) = delete;
  NonCopyable& operator=(NonCopyable&) = delete;

};

}}}

namespace MeasurementKit {
namespace protocols {
namespace traceroute {

namespace MKC = MeasurementKit::common::constraints;

struct Tracerouter : public MKC::NonCopyable {};

}}}

int main() {
  MeasurementKit::common::constraints::NonCopyable x;
  MeasurementKit::protocols::traceroute::Tracerouter y;
  //MKC::NonCopyable z;  /* With this line in, it does not compile */
  MeasurementKit::protocols::traceroute::MKC::NonCopyable w;  // <-- OOPS
}
