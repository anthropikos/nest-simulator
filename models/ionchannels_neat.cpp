#include "ionchannels_neat.h"


nest::FakeChannel::FakeChannel(double tau_r, double tau_d, double e_r, double g_step){
  set_params(tau_r, tau_d, e_r, g_step);
}

void nest::FakeChannel::init( const double dt ){
  m_p_r = std::exp(-dt / m_tau_r); m_p_d = std::exp(-dt / m_tau_d);
  m_g_r = 0.; m_g_d = 0.;
  m_g = 0.; m_g0 = 0.;
};


void nest::FakeChannel::set_params(double tau_r, double tau_d, double e_r, double g_step){
  m_tau_r = tau_r; m_tau_d = tau_d;
  m_e_r = e_r;
  m_g_step = g_step;
  // set the normalization
  double tp = (m_tau_r * m_tau_d) / (m_tau_d - m_tau_r) * std::log(m_tau_d / m_tau_r);
  m_norm = 1. / (-std::exp(-tp / m_tau_r) + std::exp(-tp / m_tau_d));
};


void nest::FakeChannel::add_spike(){
    m_g_r += m_g_step * m_norm;
    m_g_d += m_g_step * m_norm;
    std::cout << "spike added with amp =" << m_g_step * m_norm << std::endl;
};


void nest::FakeChannel::update(){
  // update conductance
  m_g_r *= m_p_r; m_g_d *= m_p_d;
  m_g0 = m_g;
  m_g = m_g_r + m_g_d;
};


std::pair< double, double > nest::FakeChannel::f_numstep(const double v_comp){
    // double g_val = m_g0 / 2.;
    // double i_val = (m_g0 + m_g) / 2. * (m_e_r - v_comp) + m_g0 * v_comp/2.;

    double g_val = m_g / 3.;
    double i_val = m_g * ((m_e_r - v_comp) + v_comp / 3.);

    return std::make_pair(g_val, i_val);
};