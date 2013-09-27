
#include<math.h>
#include<iostream>
#include<iomanip>

using namespace std;

void printJ(double **J);
void printx(double *x);

/*-- Test values -- */
double tu    = 0.0003;
double rho   = 1.022;
double mu    = 1e-5;
double U     = 1.;
double du_ds = 1e-2;

void f(double re_theta, double f_lambda, double *fk) {

  double re_theta0, f_lambda0, lambda;

  if (tu <= 1.3)
    re_theta0 = (1173.51 - 589.428*tu + 0.2196/pow(tu,2))*f_lambda;
  else
    re_theta0 = (331.5*pow(tu-0.5658,-0.671))*f_lambda;

  re_theta0 = max(re_theta0,20.);

  lambda = pow(re_theta,2) * mu/(rho*pow(U,2))*du_ds;
  cout << "lambda: " << lambda << " => ";
  //lambda = min(max(lambda,-0.1),0.1);
  cout << lambda << endl;

  if (lambda <= 0.0)
    f_lambda0 = 1 - (-12.986*lambda - 123.66*pow(lambda,2) - 405.689*pow(lambda,3))*exp(-pow(tu/1.5,1.5));
  else
    f_lambda0 = 1 + 0.275*(1-exp(-35.0*lambda))*exp(-tu/0.5);

  fk[0] = re_theta - re_theta0;
  fk[1] = f_lambda - f_lambda0;

}


void df(double re_theta, double f_lambda, double **J) {

  double re_theta0, f_lambda0, lambda;

  J[0][0] = 1.;
  if (tu <= 1.3)
    J[0][1] = (1173.51 - 589.428*tu + 0.2196/pow(tu,2));
  else
    J[0][1] = (331.5*pow(tu-0.5658,-0.671));
  J[0][1] *= -1;

  lambda = pow(re_theta,2) * mu/(rho*pow(U,2))*du_ds;
  //lambda = min(max(lambda,-0.1),0.1);
  double dre_dlamb = 2*re_theta * mu/(rho*pow(U,2))*du_ds;

  if (lambda <= 0) 
    J[1][0] = 0. - (-12.986 - 2*123.66*lambda - 3*405.689*pow(lambda,2))*dre_dlamb*exp(-pow(tu/1.5,1.5));
  else
    J[1][0] = 0. + 0.275*(0.0+35.0*exp(-35.0*lambda))*dre_dlamb*exp(-tu/0.5);
  J[1][0] *= -1;
  J[1][1] = 1.;

}

int main() {


  int k;
  int kmax = 1000;
  double *fk, dx[2], **J, Jt[2][2], *f2;
  double re_th,  f_lamb,  lamb;
  double re_th0, f_lamb0, lamb0;
  double newton_tol = 1e-10;
  double normres, factor;
  double eps = 1e-7;

  bool debug=false;

  J = new double*[2];
  for (int k=0; k<2; k++)
    J[k] = new double[2];
  fk = new double[2];
  f2 = new double[2];

  f_lamb = 1.;
  re_th  = (1173.51 - 589.428*tu + 0.2196/pow(tu,2))*f_lamb;
 
   f(re_th, f_lamb, fk);
  df(re_th, f_lamb, J);

  cout << "------" << endl;
  printx(fk);

  cout << "---------------" << endl;
  f(re_th+eps, f_lamb,f2);
  for (int j=0; j<2; j++)
    f2[j] = (f2[j]-fk[j])/eps;
  printx(f2);

  cout << "------" << endl;
  f(re_th, f_lamb+eps, f2);
  for (int j=0; j<2; j++)
    f2[j] = (f2[j]-fk[j])/eps;
  printx(f2);

  cout << "------" << endl;
  printJ(J);

}



void printx(double *x) {
  int k;

  for(k=0; k<2; k++)
    cout << x[k] << endl;
}

void printJ(double **J) {

  int j,k;

  cout << scientific;
  for(j=0; j<2; j++) {
    for(k=0; k<2; k++) {
      cout << setw(10) << J[j][k] << "  ";
    }
    cout << endl;
  }
}
