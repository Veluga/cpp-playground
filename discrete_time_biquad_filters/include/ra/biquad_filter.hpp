#include "cexpr_math.hpp"

namespace ra::biquad {
using namespace ra::cexpr_math;
template <class Real> struct biquad_filter_coefs {
  using real = Real;

  constexpr biquad_filter_coefs(real a0_, real a1_, real a2_, real b0_,
                                real b1_, real b2_)
      : a0(a0_), a1(a1_), a2(a2_), b0(b0_), b1(b1_), b2(b2_){};

  template <class OtherReal>
  constexpr biquad_filter_coefs(const biquad_filter_coefs<OtherReal> &coefs)
      : a0(coefs.a0), a1(coefs.a1), a2(coefs.a2), b0(coefs.b0), b1(coefs.b1),
        b2(coefs.b2){};

  real a0;
  real a1;
  real a2;
  real b0;
  real b1;
  real b2;
};

template <class Real>
constexpr biquad_filter_coefs<Real> lowpass(Real f, Real q) {
  Real omega = tan<Real>(pi<Real> / 2 * f);
  Real omega_sqr = omega * omega;
  Real b0 = omega_sqr + omega / q + 1;
  return biquad_filter_coefs<Real>(omega_sqr / b0, 2 * omega_sqr / b0,
                                   omega_sqr / b0, 1, 2 * (omega_sqr - 1) / b0,
                                   (omega_sqr - omega / q + 1) / b0);
};

template <class Real>
constexpr biquad_filter_coefs<Real> highpass(Real f, Real q) {
  Real omega = tan<Real>(pi<Real> / 2 * f);
  Real omega_sqr = omega * omega;
  Real b0 = omega_sqr + omega / q + 1;
  return biquad_filter_coefs<Real>(1 / b0, -2 / b0, 1 / b0, 1,
                                   2 * (omega_sqr - 1) / b0,
                                   (omega_sqr - omega / q + 1) / b0);
};

template <class Real>
constexpr biquad_filter_coefs<Real> bandpass(Real f, Real q) {
  Real omega = tan<Real>(pi<Real> / 2 * f);
  Real omega_sqr = omega * omega;
  Real b0 = omega_sqr + omega / q + 1;
  return biquad_filter_coefs<Real>(omega / (q * b0), 0, -omega / (q * b0), 1,
                                   2 * (omega_sqr - 1) / b0,
                                   (omega_sqr - omega / q + 1) / b0);
};

template <class Real>
constexpr biquad_filter_coefs<Real> lowshelf_boost(Real f, Real a) {
  Real omega = tan<Real>(pi<Real> / 2 * f);
  Real omega_sqr = omega * omega;
  Real b0 = omega_sqr + sqrt<Real>(2) * omega + 1;
  return biquad_filter_coefs<Real>(
      (a * omega_sqr + sqrt<Real>(2 * a) * omega + 1) / b0,
      2 * (a * omega_sqr - 1) / b0,
      (a * omega_sqr - sqrt<Real>(2 * a) * omega + 1) / b0, 1,
      2 * (omega_sqr - 1) / b0, (omega_sqr - sqrt<Real>(2) * omega + 1) / b0);
};

template <class Real>
constexpr biquad_filter_coefs<Real> lowshelf_cut(Real f, Real a) {
  Real omega = tan<Real>(pi<Real> / 2 * f);
  Real omega_sqr = omega * omega;
  Real b0 = a * omega_sqr + sqrt<Real>(2 * a) * omega + 1;
  return biquad_filter_coefs<Real>(
      (omega_sqr + sqrt<Real>(2) * omega + 1) / b0, 2 * (omega_sqr - 1) / b0,
      (omega_sqr - sqrt<Real>(2) * omega + 1) / b0, 1,
      2 * (a * omega_sqr - 1) / b0,
      (a * omega_sqr - sqrt<Real>(2 * a) * omega + 1) / b0);
};
} // namespace ra::biquad
