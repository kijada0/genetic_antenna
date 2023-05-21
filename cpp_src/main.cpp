// ---------------------------------------- //
// created by: Kacper Kaczmarek
// created date: 20-05-2023
// ---------------------------------------- //

#include <iostream>

#include "c_geometry.h"
#include "nec_context.h"
#include "nec_exception.h"
#include "nec_radiation_pattern.h"

using namespace std;

int main(int argc, char **argv) {
    try {
        cout << "Nec2++ C++ example. Running (takes a few minutes...)" << endl;

        nec_context nec;
        nec.initialize();

        c_geometry* geo = nec.get_geometry();
        geo->wire(0, 70, -0.048, 0.021, -0.005, 0.035, 0.043, 0.014, 0.001, 1.0, 1.0);
        geo->wire(0, 66, 0.017, -0.015, 0.014, -0.027, 0.04, -0.031, 0.001, 1.0, 1.0);
        geo->wire(0, 47, 0.046, -0.01, 0.028, -0.013, -0.005, 0.031, 0.001, 1.0, 1.0);
        geo->wire(0, 77, -0.048, -0.038, -0.04, 0.049, -0.045, -0.04, 0.001, 1.0, 1.0);
        nec.geometry_complete(0);

        nec.gn_card(-1,0,0.0, 0.0, 0.0,0.0, 0.0, 0.0);
        nec.ld_card(5,0,0,0,3.72e7,0.0,0.0);
        nec.pt_card(-1, 0, 0, 0);
        nec.ex_card(EXCITATION_LINEAR, 1, 1, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        nec.fr_card(0, 2, 2400.0, 100.0);
        nec.rp_card(0, 10, 10, 0,5,0,0, 0.0, 0.0, 9.0, 9.0, 0.0, 0.0);

        // now get the radiation pattern data. The result index is 0 since
        // this is the first (and only) radiation pattern.
        nec_radiation_pattern* rp = nec.get_radiation_pattern(0);

        int nth = rp->get_ntheta();
        int nph = rp->get_nphi();

        cout << endl << "Theta \tPhi \tHorizontal \tVertical \tTotal" << endl;
        for (int j=0; j<nph; j++) {
            for (int i=0; i<nth; i++) {
                cout    << rp->get_theta(i) << "  \t"
                        << rp->get_phi(j) << "  \t"
                        << rp->get_power_gain_horiz(i,j) << "  \t"
                        << rp->get_power_gain_vert(i,j) << "  \t"
                        << rp->get_power_gain(i,j) << "  \t"
                        << rp->get_etheta_magnitude(i,j) << "  \t"
                        << rp->get_etheta_phase(i,j) << "  \t"
                        << rp->get_ephi_magnitude(i,j) << "  \t"
                        << rp->get_ephi_phase(i,j)
                        << endl;
            }
        }
    }
    catch (nec_exception* e) {
        cout << e->get_message() << endl;
    }
    return 0;
}
