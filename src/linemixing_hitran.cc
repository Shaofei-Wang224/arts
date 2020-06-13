#include <fstream>
#include <Faddeeva/Faddeeva.hh>
#include "linemixing_hitran.h"


namespace lm_hitran_2017 {
namespace parameters {
  static constexpr Index nSigmx=5000001;  // Max number of spectral points
  static constexpr Index iFile=3;  // Unit Number for File access
  static constexpr Index nBmx=7000;  // Max Number of Bands
  static constexpr Index nLmx=700;  // Max Number of Lines per Band
  static constexpr Index nIsotp=10;  // Number of CO2 Isotopomers
  static constexpr Index Nlifmax=10;  // Max number of l values
  static constexpr Index Jmax=131;  // Max number of j values
  static constexpr Index Nwmax=100000;  // Max Number of W coupling
  
  static constexpr Numeric Ct=1.4387686e0;  // Constant
  static constexpr Numeric T0=296;  // Constant
  static constexpr Numeric CtGamD=1.1325e-08;  // Constant
  static constexpr Numeric aMolAtm=7.33889e+21;  // Constant
  static constexpr Numeric Pi=3.141592654e0;  // Constant
  
  static constexpr auto aMass = stdarrayify(44.e-3,45.e-3,46.e-3,45.e-3,47.e-3,46.e-3,48.e-3,47.e-3,46.e-3,49.e-3);  // Constant
  
};

struct CommonBlock {
struct Bands {
  Index nBand;
  std::array<Index, parameters::nBmx> Isot;
  std::array<Index, parameters::nBmx> nLines;
  std::array<Index, parameters::nBmx> li;
  std::array<Index, parameters::nBmx> lf;
  std::array<String, parameters::nBmx> BandFile;
} Bands;

struct LineSg {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>  Sig;
  LineSg() : Sig(parameters::nLmx, parameters::nBmx) {};
} LineSg;

struct DipoRigid {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> Dipo0;
  DipoRigid() : Dipo0(parameters::nLmx, parameters::nBmx) {};
} DipoRigid;

struct Energy {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>  E;
  Energy() : E(parameters::nLmx, parameters::nBmx) {};
} Energy;

struct GamVT0AIR {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> HWVT0AIR;
  GamVT0AIR() : HWVT0AIR(parameters::nLmx, parameters::nBmx) {};
} GamVT0AIR;

struct GamSDVT0AIR {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> HWSDVT0AIR;
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> rHWT0AIR;
  GamSDVT0AIR() : HWSDVT0AIR(parameters::nLmx, parameters::nBmx), rHWT0AIR(parameters::nLmx, parameters::nBmx) {};
} GamSDVT0AIR;

struct DTGAMAIR {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> BHWAIR;
  DTGAMAIR() : BHWAIR(parameters::nLmx, parameters::nBmx) {};
} DTGAMAIR;

struct GamVT0CO2 {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> HWVT0SELF;
  GamVT0CO2() : HWVT0SELF(parameters::nLmx, parameters::nBmx) {};
} GamVT0CO2;

struct GamSDVT0CO2 {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> HWSDVT0SELF;
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> rHWT0SELF;
  GamSDVT0CO2() : HWSDVT0SELF(parameters::nLmx, parameters::nBmx), rHWT0SELF(parameters::nLmx, parameters::nBmx) {};
} GamSDVT0CO2;

struct DTGAMCO2 {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> BHWSELF;
  DTGAMCO2() : BHWSELF(parameters::nLmx, parameters::nBmx) {};
} DTGAMCO2;

struct GamVT0H2O {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> HWVT0H2O;
  GamVT0H2O() : HWVT0H2O(parameters::nLmx, parameters::nBmx) {};
} GamVT0H2O;

struct GamSDVT0H2O {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> HWSDVT0H2O;
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> rHWT0H2O;
  GamSDVT0H2O() : HWSDVT0H2O(parameters::nLmx, parameters::nBmx), rHWT0H2O(parameters::nLmx, parameters::nBmx) {};
} GamSDVT0H2O;

struct DTGAMH2O {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> BHWH2O;
  DTGAMH2O() : BHWH2O(parameters::nLmx, parameters::nBmx) {};
} DTGAMH2O;

struct GamT {
  std::array<Numeric, parameters::nLmx> HWT;
  std::array<Numeric, parameters::nLmx> HWSDV2T;
} GamT;

struct SHIFT {
  std::array<Numeric, parameters::nLmx> shft;
} SHIFT;

struct SHIFT0 {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> shft0;
  SHIFT0() : shft0(parameters::nLmx, parameters::nBmx) {};
} SHIFT0;

struct PopuT {
  std::array<Numeric, parameters::nLmx> PopuT;
} PopuT;

struct PopTrf {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> PopuT0;
  PopTrf() : PopuT0(parameters::nLmx, parameters::nBmx) {};
} PopTrf;

struct DipoTcm {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> DipoT;
  DipoTcm() : DipoT(parameters::nLmx, parameters::nBmx) {};
} DipoTcm;

struct Jiln {
  Eigen::Matrix<Index, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> Ji;
  Jiln() : Ji(parameters::nLmx, parameters::nBmx) {};
} Jiln;

struct Jfln {
  Eigen::Matrix<Index, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> Jf;
  Jfln() : Jf(parameters::nLmx, parameters::nBmx) {};
} Jfln;

struct FicLSR {
  std::array<Numeric, parameters::nLmx> SSR;
} FicLSR;

struct FicLSI {
  std::array<Numeric, parameters::nLmx> SSI;
} FicLSI;

struct FicLPR {
  std::array<Numeric, parameters::nLmx> AlphR;
} FicLPR;

struct FicLPI {
  std::array<Numeric, parameters::nLmx> AlphI;
} FicLPI;

struct Zss {
  std::array<Complex, parameters::nLmx> ZS;
} Zss;

struct Zaa {
  std::array<Complex, parameters::nLmx> ZA;
} Zaa;

struct Wmatrix {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> W;
  Wmatrix() : W(parameters::nLmx, parameters::nLmx) {};
} Wmatrix;

struct Wfittedp {
  Tensor4 W0pp;
  Tensor4 W0pq;
  Tensor4 W0pr;
  Wfittedp() :
  W0pp(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax),
  W0pq(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax),
  W0pr(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax) {}
} Wfittedp;

struct Wfittedq {
  Tensor4 W0qp;
  Tensor4 W0qq;
  Tensor4 W0qr;
  Wfittedq() :
  W0qp(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax),
  W0qq(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax),
  W0qr(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax) {}
} Wfittedq;

struct Wfittedr {
  Tensor4 W0rp;
  Tensor4 W0rq;
  Tensor4 W0rr;
  Wfittedr() :
  W0rp(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax),
  W0rq(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax),
  W0rr(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax) {}
} Wfittedr;

struct Bfittedp {
  Tensor4 B0pp;
  Tensor4 B0pq;
  Tensor4 B0pr;
  Bfittedp() :
  B0pp(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax),
  B0pq(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax),
  B0pr(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax) {}
} Bfittedp;

struct Bfittedq {
  Tensor4 B0qp;
  Tensor4 B0qq;
  Tensor4 B0qr;
  Bfittedq() :
  B0qp(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax),
  B0qq(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax),
  B0qr(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax) {}
} Bfittedq;

struct Bfittedr {
  Tensor4 B0rp;
  Tensor4 B0rq;
  Tensor4 B0rr;
  Bfittedr() :
  B0rp(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax),
  B0rq(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax),
  B0rr(parameters::Nlifmax, parameters::Nlifmax, parameters::Jmax, parameters::Jmax) {}
} Bfittedr;

struct DiagnR {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> OpR;
  DiagnR() : OpR(parameters::nLmx, parameters::nLmx) {};
} DiagnR;

struct DiagnI {
  Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> OpI;
  DiagnI() : OpI(parameters::nLmx, parameters::nLmx) {};
} DiagnI;

struct YLT {
  std::array<Numeric, parameters::nLmx> YT;
} YLT;
};  // CommonBlock

void readlines(
  CommonBlock& cmn,
  const bool /*mixsdv*/)
{
  if (cmn.Bands.nBand > parameters::nBmx)
    throw std::runtime_error("Too many bands");
  
  for (Index iband=0; iband<cmn.Bands.nBand; iband++) {
    std::ifstream fortranfile;
    const String fname = String("data_new/") + cmn.Bands.BandFile[iband] + String(".dat");
    fortranfile.open(fname.c_str());
    
    if (fortranfile.is_open()) {
      String line;
      getline(fortranfile, line);
      
      Index nliner=0;
      while (fortranfile.good()) {
        if (nliner >= parameters::nLmx)
          throw std::runtime_error("Too many lines");
        
        char tpline, x;
        Numeric intens, eina;
        char sDipoRigid[21], sPopTrf[21];
        sscanf(line.c_str(), 
               "%c%c" "%1ld" "%12lf" "%10lf"
               "%10lf" "%5lf" "%5lf" "%4lf"
               "%5lf" "%5lf" "%4lf" "%10lf"
               "%4lf" "%4lf" "%8lf" "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
               "%c" "%3ld" "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c" "%5lf"
               "%5lf" "%4lf" "%5lf"
               "%20s" "%20s",
               &x,&x,
               &cmn.Bands.Isot[iband],
               &cmn.LineSg.Sig(nliner, iband),
               &intens,
               &eina,
               &cmn.GamVT0AIR.HWVT0AIR(nliner, iband),
               &cmn.GamSDVT0AIR.HWSDVT0AIR(nliner, iband),
               &cmn.GamSDVT0AIR.rHWT0AIR(nliner, iband),
               &cmn.GamVT0CO2.HWVT0SELF(nliner, iband),
               &cmn.GamSDVT0CO2.HWSDVT0SELF(nliner, iband),
               &cmn.GamSDVT0CO2.rHWT0SELF(nliner, iband),
               &cmn.Energy.E(nliner, iband),
               &cmn.DTGAMAIR.BHWAIR(nliner, iband),
               &cmn.DTGAMCO2.BHWSELF(nliner, iband),
               &cmn.SHIFT0.shft0(nliner, iband),
               &x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,
               &tpline,
               &cmn.Jiln.Ji(nliner, iband),
               &x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,&x,
               &cmn.GamVT0H2O.HWVT0H2O(nliner, iband),
               &cmn.GamSDVT0H2O.HWSDVT0H2O(nliner, iband),
               &cmn.GamSDVT0H2O.rHWT0H2O(nliner, iband),
               &cmn.DTGAMH2O.BHWH2O(nliner, iband),
               sDipoRigid,
               sPopTrf);
        getline(fortranfile, line);
        
        // Fix...
        String ssDipoRigid = sDipoRigid;
        String ssPopTrf = sPopTrf;
        std::replace(ssDipoRigid.begin(), ssDipoRigid.end(), 'D', 'E');
        std::replace(ssPopTrf.begin(), ssPopTrf.end(), 'D', 'E');
        cmn.DipoRigid.Dipo0(nliner, iband) = std::stod(ssDipoRigid);
        cmn.PopTrf.PopuT0(nliner, iband) = std::stod(ssPopTrf);
        
        // Dipole at temperature
        cmn.DipoTcm.DipoT(nliner, iband) = std::sqrt(intens/(cmn.PopTrf.PopuT0(nliner,iband) * cmn.LineSg.Sig(nliner,iband) * (1-std::exp(-1.4388*cmn.LineSg.Sig(nliner,iband)/296))));
        
//         std::cout <<
//         cmn.Bands.Isot[iband]<< " " <<
//         cmn.LineSg.Sig(nliner, iband)<< " " <<
//         intens<< " " <<
//         eina<< " " <<
//         cmn.GamVT0AIR.HWVT0AIR(nliner, iband)<< " " <<
//         cmn.GamSDVT0AIR.HWSDVT0AIR(nliner, iband)<< " " <<
//         cmn.GamSDVT0AIR.rHWT0AIR(nliner, iband)<< " " <<
//         cmn.GamVT0CO2.HWVT0SELF(nliner, iband)<< " " <<
//         cmn.GamSDVT0CO2.HWSDVT0SELF(nliner, iband)<< " " <<
//         cmn.GamSDVT0CO2.rHWT0SELF(nliner, iband)<< " " <<
//         cmn.Energy.E(nliner, iband)<< " " <<
//         cmn.DTGAMAIR.BHWAIR(nliner, iband)<< " " <<
//         cmn.DTGAMCO2.BHWSELF(nliner, iband)<< " " <<
//         cmn.SHIFT0.shft0(nliner, iband)<< " " <<
//         tpline<< " " <<
//         cmn.Jiln.Ji(nliner, iband)<< " " <<
//         cmn.GamVT0H2O.HWVT0H2O(nliner, iband)<< " " <<
//         cmn.GamSDVT0H2O.HWSDVT0H2O(nliner, iband)<< " " <<
//         cmn.GamSDVT0H2O.rHWT0H2O(nliner, iband)<< " " <<
//         cmn.DTGAMH2O.BHWH2O(nliner, iband)<< " " <<
//         sDipoRigid<< " " <<
//         sPopTrf<< " " <<
//         cmn.DipoTcm.DipoT(nliner, iband)<< "\n";
        
        // Fix Js
        if (tpline == 'P')
          cmn.Jfln.Jf(nliner, iband) = cmn.Jiln.Ji(nliner, iband) - 1;
        else if (tpline == 'Q')
          cmn.Jfln.Jf(nliner, iband) = cmn.Jiln.Ji(nliner, iband);
        else
          cmn.Jfln.Jf(nliner, iband) = cmn.Jiln.Ji(nliner, iband) + 1;
        
        // Fix isotologue
        if (cmn.Bands.Isot[iband] == 0)
          cmn.Bands.Isot[iband] = 10;
        
        nliner++;
      }
      cmn.Bands.nLines[iband] = nliner;
    }
    
    fortranfile.close();
  }
}

template<size_t NT>
Numeric atob(const Numeric& aa,
             const std::array<Numeric, NT>& a,
             const std::array<Numeric, NT>& b)
{ 
  for (size_t i=1; i<NT; i++) {
    if (a[i] >= aa) {
      if (i < 2 or i == NT) {
        size_t j=i;
        
        if (i < 2)
          j = 2;
        if (i == NT-1)
          j = NT-1;
        
        Numeric A0D1=a[j-2]-a[j-1]; if (A0D1 == 0) A0D1=0.0001;
        Numeric A0D2=a[j-2]-a[j];   if (A0D2 == 0) A0D2=0.0001;
        Numeric A1D1=a[j-1]-a[j-2]; if (A1D1 == 0) A1D1=0.0001;
        Numeric A1D2=a[j-1]-a[j];   if (A1D2 == 0) A1D2=0.0001;
        Numeric A2D1=a[j]-a[j-2];   if (A2D1 == 0) A2D1=0.0001;
        Numeric A2D2=a[j]-a[j-1];   if (A2D2 == 0) A2D2=0.0001;
        
        const Numeric A0=(aa-a[j-1])*(aa-a[j])/(A0D1*A0D2);
        const Numeric A1=(aa-a[j-2])*(aa-a[j])/(A1D1*A1D2);
        const Numeric A2=(aa-a[j-2])*(aa-a[j-1])/(A2D1*A2D2);
        
        return A0*b[j-2] + A1*b[j-1] + A2*b[j];
      } else {
        size_t j = i;
        
        Numeric A0D1=a[j-2]-a[j-1]; if (A0D1 == 0) A0D1=0.0001;
        Numeric A0D2=a[j-2]-a[j];   if (A0D2 == 0) A0D2=0.0001;
        Numeric A0D3=a[j-2]-a[j+1]; if (A0D3 == 0) A0D3=0.0001;
        Numeric A1D1=a[j-1]-a[j-2]; if (A1D1 == 0) A1D1=0.0001;
        Numeric A1D2=a[j-1]-a[j];   if (A1D2 == 0) A1D2=0.0001;
        Numeric A1D3=a[j-1]-a[j+1]; if (A1D3 == 0) A1D3=0.0001;
        Numeric A2D1=a[j]-a[j-2];   if (A2D1 == 0) A2D1=0.0001;
        Numeric A2D2=a[j]-a[j-1];   if (A2D2 == 0) A2D2=0.0001;
        Numeric A2D3=a[j]-a[j+1];   if (A2D3 == 0) A2D3=0.0001;
        Numeric A3D1=a[j+1]-a[j-2]; if (A3D1 == 0) A3D1=0.0001;
        Numeric A3D2=a[j+1]-a[j-1]; if (A3D2 == 0) A3D2=0.0001;
        Numeric A3D3=a[j+1]-a[j];   if (A3D3 == 0) A3D3=0.0001;
        
        
        Numeric A0=(aa-a[j-1])*(aa-a[j])*(aa-a[j+1]);
        A0=A0/(A0D1*A0D2*A0D3);
        Numeric A1=(aa-a[j-2])*(aa-a[j])*(aa-a[j+1]);
        A1=A1/(A1D1*A1D2*A1D3);
        Numeric A2=(aa-a[j-2])*(aa-a[j-1])*(aa-a[j+1]);
        A2=A2/(A2D1*A2D2*A2D3);
        Numeric A3=(aa-a[j-2])*(aa-a[j-1])*(aa-a[j]);
        A3=A3/(A3D1*A3D2*A3D3);
        
        return A0*b[j-2] + A1*b[j-1] + A2*b[j] + A3*b[j+1];
      }
    }
  }
  
  return std::numeric_limits<Numeric>::quiet_NaN();
}

void qt_co2(const Numeric& t,
            const Index& iso,
            Numeric& gsi,
            Numeric& qt)
{
  constexpr auto xgj = stdarrayify(1.,2.,1.,6.,2.,12.,1.,6.,1.,2.,12.);
  constexpr auto tdat = stdarrayify(60.,  85., 110., 135., 160., 185., 210., 235.,
                                    260., 285., 310., 335., 360., 385., 410., 435., 460., 485.,
                                    510., 535., 560., 585., 610., 635., 660., 685., 710., 735.,
                                    760., 785., 810., 835., 860., 885., 910., 935., 960., 985.,
                                    1010.,1035.,1060.,1085.,1110.,1135.,1160.,1185.,1210.,1235.,
                                    1260.,1285.,1310.,1335.,1360.,1385.,1410.,1435.,1460.,1485.,
                                    1510.,1535.,1560.,1585.,1610.,1635.,1660.,1685.,1710.,1735.,
                                    1760.,1785.,1810.,1835.,1860.,1885.,1910.,1935.,1960.,1985.,
                                    2010.,2035.,2060.,2085.,2110.,2135.,2160.,2185.,2210.,2235.,
                                    2260.,2285.,2310.,2335.,2360.,2385.,2410.,2435.,2460.,2485.,
                                    2510.,2535.,2560.,2585.,2610.,2635.,2660.,2685.,2710.,2735.,
                                    2760.,2785.,2810.,2835.,2860.,2885.,2910.,2935.,2960.,2985.,
                                    3010.);
  constexpr auto qoft = stdarrayify(
// c...        --       626
      stdarrayify( 0.53642E+02, 0.75947E+02, 0.98292E+02,
     + 0.12078E+03, 0.14364E+03, 0.16714E+03, 0.19160E+03, 0.21731E+03,
     + 0.24454E+03, 0.27355E+03, 0.30456E+03, 0.33778E+03, 0.37343E+03,
     + 0.41170E+03, 0.45280E+03, 0.49692E+03, 0.54427E+03, 0.59505E+03,
     + 0.64948E+03, 0.70779E+03, 0.77019E+03, 0.83693E+03, 0.90825E+03,
     + 0.98440E+03, 0.10656E+04, 0.11522E+04, 0.12445E+04, 0.13427E+04,
     + 0.14471E+04, 0.15580E+04, 0.16759E+04, 0.18009E+04, 0.19334E+04,
     + 0.20739E+04, 0.22225E+04, 0.23798E+04, 0.25462E+04, 0.27219E+04,
     + 0.29074E+04, 0.31032E+04, 0.33097E+04, 0.35272E+04, 0.37564E+04,
     + 0.39976E+04, 0.42514E+04, 0.45181E+04, 0.47985E+04, 0.50929E+04,
     + 0.54019E+04, 0.57260E+04, 0.60659E+04, 0.64221E+04, 0.67952E+04,
     + 0.71859E+04, 0.75946E+04, 0.80222E+04, 0.84691E+04, 0.89362E+04,
     + 0.94241E+04, 0.99335E+04, 0.10465E+05, 0.11020E+05, 0.11598E+05,
     + 0.12201E+05, 0.12828E+05, 0.13482E+05, 0.14163E+05, 0.14872E+05,
     + 0.15609E+05, 0.16376E+05, 0.17173E+05, 0.18001E+05, 0.18861E+05,
     + 0.19754E+05, 0.20682E+05, 0.21644E+05, 0.22643E+05, 0.23678E+05,
     + 0.24752E+05, 0.25865E+05, 0.27018E+05, 0.28212E+05, 0.29449E+05,
     + 0.30730E+05, 0.32055E+05, 0.33426E+05, 0.34845E+05, 0.36312E+05,
     + 0.37828E+05, 0.39395E+05, 0.41015E+05, 0.42688E+05, 0.44416E+05,
     + 0.46199E+05, 0.48041E+05, 0.49942E+05, 0.51902E+05, 0.53925E+05,
     + 0.56011E+05, 0.58162E+05, 0.60379E+05, 0.62664E+05, 0.65019E+05,
     + 0.67444E+05, 0.69942E+05, 0.72515E+05, 0.75163E+05, 0.77890E+05,
     + 0.80695E+05, 0.83582E+05, 0.86551E+05, 0.89605E+05, 0.92746E+05,
     + 0.95975E+05, 0.99294E+05, 0.10271E+06, 0.10621E+06, 0.10981E+06,
     + 0.11351E+06),
// c...        --       636
      stdarrayify( 0.10728E+03, 0.15189E+03, 0.19659E+03,
     + 0.24164E+03, 0.28753E+03, 0.33486E+03, 0.38429E+03, 0.43643E+03,
     + 0.49184E+03, 0.55104E+03, 0.61449E+03, 0.68263E+03, 0.75589E+03,
     + 0.83468E+03, 0.91943E+03, 0.10106E+04, 0.11085E+04, 0.12137E+04,
     + 0.13266E+04, 0.14477E+04, 0.15774E+04, 0.17163E+04, 0.18649E+04,
     + 0.20237E+04, 0.21933E+04, 0.23743E+04, 0.25673E+04, 0.27729E+04,
     + 0.29917E+04, 0.32245E+04, 0.34718E+04, 0.37345E+04, 0.40132E+04,
     + 0.43087E+04, 0.46218E+04, 0.49533E+04, 0.53041E+04, 0.56749E+04,
     + 0.60668E+04, 0.64805E+04, 0.69171E+04, 0.73774E+04, 0.78626E+04,
     + 0.83736E+04, 0.89114E+04, 0.94772E+04, 0.10072E+05, 0.10697E+05,
     + 0.11353E+05, 0.12042E+05, 0.12765E+05, 0.13523E+05, 0.14317E+05,
     + 0.15148E+05, 0.16019E+05, 0.16930E+05, 0.17883E+05, 0.18879E+05,
     + 0.19920E+05, 0.21008E+05, 0.22143E+05, 0.23328E+05, 0.24563E+05,
     + 0.25852E+05, 0.27195E+05, 0.28594E+05, 0.30051E+05, 0.31568E+05,
     + 0.33146E+05, 0.34788E+05, 0.36496E+05, 0.38271E+05, 0.40115E+05,
     + 0.42031E+05, 0.44021E+05, 0.46086E+05, 0.48230E+05, 0.50453E+05,
     + 0.52759E+05, 0.55150E+05, 0.57628E+05, 0.60195E+05, 0.62854E+05,
     + 0.65608E+05, 0.68459E+05, 0.71409E+05, 0.74461E+05, 0.77618E+05,
     + 0.80883E+05, 0.84258E+05, 0.87746E+05, 0.91350E+05, 0.95073E+05,
     + 0.98918E+05, 0.10289E+06, 0.10698E+06, 0.11121E+06, 0.11558E+06,
     + 0.12008E+06, 0.12472E+06, 0.12950E+06, 0.13443E+06, 0.13952E+06,
     + 0.14475E+06, 0.15015E+06, 0.15571E+06, 0.16143E+06, 0.16732E+06,
     + 0.17338E+06, 0.17962E+06, 0.18604E+06, 0.19264E+06, 0.19943E+06,
     + 0.20642E+06, 0.21360E+06, 0.22098E+06, 0.22856E+06, 0.23636E+06,
     + 0.24436E+06),
// c...        --       628
      stdarrayify( 0.11368E+03, 0.16096E+03, 0.20833E+03,
     + 0.25603E+03, 0.30452E+03, 0.35442E+03, 0.40640E+03, 0.46110E+03,
     + 0.51910E+03, 0.58093E+03, 0.64709E+03, 0.71804E+03, 0.79422E+03,
     + 0.87607E+03, 0.96402E+03, 0.10585E+04, 0.11600E+04, 0.12689E+04,
     + 0.13857E+04, 0.15108E+04, 0.16449E+04, 0.17883E+04, 0.19416E+04,
     + 0.21054E+04, 0.22803E+04, 0.24668E+04, 0.26655E+04, 0.28770E+04,
     + 0.31021E+04, 0.33414E+04, 0.35956E+04, 0.38654E+04, 0.41516E+04,
     + 0.44549E+04, 0.47761E+04, 0.51160E+04, 0.54755E+04, 0.58555E+04,
     + 0.62568E+04, 0.66804E+04, 0.71273E+04, 0.75982E+04, 0.80944E+04,
     + 0.86169E+04, 0.91666E+04, 0.97446E+04, 0.10352E+05, 0.10990E+05,
     + 0.11660E+05, 0.12363E+05, 0.13101E+05, 0.13874E+05, 0.14683E+05,
     + 0.15531E+05, 0.16418E+05, 0.17347E+05, 0.18317E+05, 0.19332E+05,
     + 0.20392E+05, 0.21499E+05, 0.22654E+05, 0.23859E+05, 0.25116E+05,
     + 0.26426E+05, 0.27792E+05, 0.29214E+05, 0.30695E+05, 0.32236E+05,
     + 0.33840E+05, 0.35508E+05, 0.37242E+05, 0.39045E+05, 0.40917E+05,
     + 0.42862E+05, 0.44881E+05, 0.46977E+05, 0.49152E+05, 0.51407E+05,
     + 0.53746E+05, 0.56171E+05, 0.58683E+05, 0.61286E+05, 0.63981E+05,
     + 0.66772E+05, 0.69661E+05, 0.72650E+05, 0.75742E+05, 0.78940E+05,
     + 0.82246E+05, 0.85664E+05, 0.89196E+05, 0.92845E+05, 0.96613E+05,
     + 0.10050E+06, 0.10452E+06, 0.10867E+06, 0.11295E+06, 0.11736E+06,
     + 0.12191E+06, 0.12661E+06, 0.13145E+06, 0.13643E+06, 0.14157E+06,
     + 0.14687E+06, 0.15232E+06, 0.15794E+06, 0.16372E+06, 0.16968E+06,
     + 0.17580E+06, 0.18211E+06, 0.18859E+06, 0.19526E+06, 0.20213E+06,
     + 0.20918E+06, 0.21643E+06, 0.22388E+06, 0.23154E+06, 0.23941E+06,
     + 0.24750E+06),
// c...        --       627
      stdarrayify( 0.66338E+03, 0.93923E+03, 0.12156E+04,
     + 0.14938E+04, 0.17766E+04, 0.20676E+04, 0.23705E+04, 0.26891E+04,
     + 0.30267E+04, 0.33866E+04, 0.37714E+04, 0.41839E+04, 0.46267E+04,
     + 0.51023E+04, 0.56132E+04, 0.61618E+04, 0.67508E+04, 0.73827E+04,
     + 0.80603E+04, 0.87863E+04, 0.95636E+04, 0.10395E+05, 0.11284E+05,
     + 0.12233E+05, 0.13246E+05, 0.14326E+05, 0.15477E+05, 0.16702E+05,
     + 0.18005E+05, 0.19390E+05, 0.20861E+05, 0.22422E+05, 0.24077E+05,
     + 0.25832E+05, 0.27689E+05, 0.29655E+05, 0.31734E+05, 0.33931E+05,
     + 0.36250E+05, 0.38698E+05, 0.41280E+05, 0.44002E+05, 0.46869E+05,
     + 0.49886E+05, 0.53062E+05, 0.56400E+05, 0.59909E+05, 0.63594E+05,
     + 0.67462E+05, 0.71521E+05, 0.75777E+05, 0.80238E+05, 0.84911E+05,
     + 0.89804E+05, 0.94925E+05, 0.10028E+06, 0.10588E+06, 0.11173E+06,
     + 0.11785E+06, 0.12423E+06, 0.13090E+06, 0.13785E+06, 0.14510E+06,
     + 0.15265E+06, 0.16053E+06, 0.16873E+06, 0.17727E+06, 0.18615E+06,
     + 0.19540E+06, 0.20501E+06, 0.21501E+06, 0.22540E+06, 0.23619E+06,
     + 0.24740E+06, 0.25904E+06, 0.27112E+06, 0.28365E+06, 0.29664E+06,
     + 0.31012E+06, 0.32409E+06, 0.33856E+06, 0.35356E+06, 0.36908E+06,
     + 0.38516E+06, 0.40180E+06, 0.41902E+06, 0.43683E+06, 0.45525E+06,
     + 0.47429E+06, 0.49397E+06, 0.51431E+06, 0.53532E+06, 0.55702E+06,
     + 0.57943E+06, 0.60256E+06, 0.62644E+06, 0.65107E+06, 0.67648E+06,
     + 0.70269E+06, 0.72972E+06, 0.75758E+06, 0.78629E+06, 0.81588E+06,
     + 0.84636E+06, 0.87775E+06, 0.91008E+06, 0.94337E+06, 0.97763E+06,
     + 0.10129E+07, 0.10492E+07, 0.10865E+07, 0.11249E+07, 0.11644E+07,
     + 0.12050E+07, 0.12467E+07, 0.12896E+07, 0.13337E+07, 0.13789E+07,
     + 0.14255E+07),
// c...        --       638
      stdarrayify( 0.22737E+03, 0.32194E+03, 0.41671E+03,
     + 0.51226E+03, 0.60963E+03, 0.71017E+03, 0.81528E+03, 0.92628E+03,
     + 0.10444E+04, 0.11707E+04, 0.13061E+04, 0.14518E+04, 0.16085E+04,
     + 0.17772E+04, 0.19588E+04, 0.21542E+04, 0.23644E+04, 0.25903E+04,
     + 0.28330E+04, 0.30934E+04, 0.33726E+04, 0.36717E+04, 0.39918E+04,
     + 0.43342E+04, 0.47001E+04, 0.50907E+04, 0.55074E+04, 0.59515E+04,
     + 0.64244E+04, 0.69276E+04, 0.74626E+04, 0.80310E+04, 0.86344E+04,
     + 0.92744E+04, 0.99528E+04, 0.10671E+05, 0.11432E+05, 0.12236E+05,
     + 0.13086E+05, 0.13984E+05, 0.14932E+05, 0.15932E+05, 0.16985E+05,
     + 0.18096E+05, 0.19265E+05, 0.20495E+05, 0.21788E+05, 0.23148E+05,
     + 0.24576E+05, 0.26075E+05, 0.27648E+05, 0.29298E+05, 0.31027E+05,
     + 0.32839E+05, 0.34736E+05, 0.36721E+05, 0.38798E+05, 0.40970E+05,
     + 0.43240E+05, 0.45611E+05, 0.48087E+05, 0.50671E+05, 0.53368E+05,
     + 0.56180E+05, 0.59111E+05, 0.62165E+05, 0.65347E+05, 0.68659E+05,
     + 0.72107E+05, 0.75694E+05, 0.79425E+05, 0.83303E+05, 0.87334E+05,
     + 0.91522E+05, 0.95872E+05, 0.10039E+06, 0.10507E+06, 0.10994E+06,
     + 0.11498E+06, 0.12021E+06, 0.12563E+06, 0.13125E+06, 0.13707E+06,
     + 0.14309E+06, 0.14933E+06, 0.15579E+06, 0.16247E+06, 0.16938E+06,
     + 0.17653E+06, 0.18392E+06, 0.19156E+06, 0.19946E+06, 0.20761E+06,
     + 0.21604E+06, 0.22473E+06, 0.23371E+06, 0.24298E+06, 0.25254E+06,
     + 0.26240E+06, 0.27258E+06, 0.28307E+06, 0.29388E+06, 0.30502E+06,
     + 0.31651E+06, 0.32834E+06, 0.34052E+06, 0.35307E+06, 0.36599E+06,
     + 0.37929E+06, 0.39298E+06, 0.40706E+06, 0.42155E+06, 0.43645E+06,
     + 0.45178E+06, 0.46753E+06, 0.48373E+06, 0.50038E+06, 0.51748E+06,
     + 0.53506E+06),
// c...        --       637
      stdarrayify( 0.13267E+04, 0.18785E+04, 0.24314E+04,
     + 0.29888E+04, 0.35566E+04, 0.41426E+04, 0.47550E+04, 0.54013E+04,
     + 0.60886E+04, 0.68232E+04, 0.76109E+04, 0.84574E+04, 0.93678E+04,
     + 0.10348E+05, 0.11402E+05, 0.12536E+05, 0.13755E+05, 0.15065E+05,
     + 0.16471E+05, 0.17980E+05, 0.19598E+05, 0.21330E+05, 0.23184E+05,
     + 0.25166E+05, 0.27283E+05, 0.29543E+05, 0.31953E+05, 0.34521E+05,
     + 0.37256E+05, 0.40164E+05, 0.43256E+05, 0.46541E+05, 0.50026E+05,
     + 0.53723E+05, 0.57641E+05, 0.61790E+05, 0.66180E+05, 0.70823E+05,
     + 0.75729E+05, 0.80910E+05, 0.86378E+05, 0.92145E+05, 0.98224E+05,
     + 0.10463E+06, 0.11137E+06, 0.11846E+06, 0.12592E+06, 0.13375E+06,
     + 0.14198E+06, 0.15062E+06, 0.15969E+06, 0.16920E+06, 0.17916E+06,
     + 0.18959E+06, 0.20052E+06, 0.21196E+06, 0.22392E+06, 0.23642E+06,
     + 0.24949E+06, 0.26314E+06, 0.27740E+06, 0.29227E+06, 0.30779E+06,
     + 0.32398E+06, 0.34085E+06, 0.35842E+06, 0.37673E+06, 0.39579E+06,
     + 0.41563E+06, 0.43626E+06, 0.45772E+06, 0.48003E+06, 0.50322E+06,
     + 0.52730E+06, 0.55232E+06, 0.57829E+06, 0.60524E+06, 0.63320E+06,
     + 0.66219E+06, 0.69226E+06, 0.72342E+06, 0.75571E+06, 0.78916E+06,
     + 0.82380E+06, 0.85966E+06, 0.89678E+06, 0.93518E+06, 0.97490E+06,
     + 0.10160E+07, 0.10585E+07, 0.11023E+07, 0.11477E+07, 0.11946E+07,
     + 0.12430E+07, 0.12929E+07, 0.13445E+07, 0.13977E+07, 0.14526E+07,
     + 0.15093E+07, 0.15677E+07, 0.16280E+07, 0.16901E+07, 0.17541E+07,
     + 0.18200E+07, 0.18880E+07, 0.19579E+07, 0.20300E+07, 0.21042E+07,
     + 0.21805E+07, 0.22591E+07, 0.23400E+07, 0.24232E+07, 0.25087E+07,
     + 0.25967E+07, 0.26871E+07, 0.27801E+07, 0.28757E+07, 0.29739E+07,
     + 0.30747E+07),
// c...        --       828
      stdarrayify( 0.60334E+02, 0.85430E+02, 0.11058E+03,
     + 0.13590E+03, 0.16167E+03, 0.18821E+03, 0.21588E+03, 0.24502E+03,
     + 0.27595E+03, 0.30896E+03, 0.34431E+03, 0.38225E+03, 0.42301E+03,
     + 0.46684E+03, 0.51397E+03, 0.56464E+03, 0.61907E+03, 0.67753E+03,
     + 0.74027E+03, 0.80753E+03, 0.87961E+03, 0.95676E+03, 0.10393E+04,
     + 0.11275E+04, 0.12217E+04, 0.13222E+04, 0.14293E+04, 0.15434E+04,
     + 0.16648E+04, 0.17940E+04, 0.19312E+04, 0.20769E+04, 0.22315E+04,
     + 0.23954E+04, 0.25691E+04, 0.27529E+04, 0.29474E+04, 0.31530E+04,
     + 0.33702E+04, 0.35995E+04, 0.38414E+04, 0.40965E+04, 0.43654E+04,
     + 0.46484E+04, 0.49464E+04, 0.52598E+04, 0.55892E+04, 0.59353E+04,
     + 0.62988E+04, 0.66803E+04, 0.70804E+04, 0.74998E+04, 0.79394E+04,
     + 0.83998E+04, 0.88817E+04, 0.93859E+04, 0.99132E+04, 0.10464E+05,
     + 0.11040E+05, 0.11642E+05, 0.12270E+05, 0.12925E+05, 0.13609E+05,
     + 0.14321E+05, 0.15064E+05, 0.15838E+05, 0.16643E+05, 0.17482E+05,
     + 0.18355E+05, 0.19263E+05, 0.20207E+05, 0.21188E+05, 0.22208E+05,
     + 0.23267E+05, 0.24366E+05, 0.25508E+05, 0.26692E+05, 0.27921E+05,
     + 0.29195E+05, 0.30516E+05, 0.31886E+05, 0.33304E+05, 0.34773E+05,
     + 0.36294E+05, 0.37869E+05, 0.39499E+05, 0.41185E+05, 0.42929E+05,
     + 0.44732E+05, 0.46596E+05, 0.48522E+05, 0.50513E+05, 0.52569E+05,
     + 0.54692E+05, 0.56884E+05, 0.59146E+05, 0.61481E+05, 0.63890E+05,
     + 0.66375E+05, 0.68937E+05, 0.71578E+05, 0.74301E+05, 0.77107E+05,
     + 0.79998E+05, 0.82976E+05, 0.86043E+05, 0.89201E+05, 0.92452E+05,
     + 0.95799E+05, 0.99242E+05, 0.10278E+06, 0.10643E+06, 0.11018E+06,
     + 0.11403E+06, 0.11799E+06, 0.12206E+06, 0.12625E+06, 0.13055E+06,
     + 0.13497E+06),
// c...        --       728
      stdarrayify( 0.70354E+03, 0.99615E+03, 0.12893E+04,
     + 0.15846E+04, 0.18848E+04, 0.21940E+04, 0.25162E+04, 0.28554E+04,
     + 0.32152E+04, 0.35991E+04, 0.40099E+04, 0.44507E+04, 0.49242E+04,
     + 0.54332E+04, 0.59802E+04, 0.65681E+04, 0.71996E+04, 0.78776E+04,
     + 0.86050E+04, 0.93847E+04, 0.10220E+05, 0.11114E+05, 0.12070E+05,
     + 0.13091E+05, 0.14182E+05, 0.15345E+05, 0.16585E+05, 0.17906E+05,
     + 0.19311E+05, 0.20805E+05, 0.22393E+05, 0.24078E+05, 0.25865E+05,
     + 0.27760E+05, 0.29768E+05, 0.31893E+05, 0.34140E+05, 0.36516E+05,
     + 0.39025E+05, 0.41674E+05, 0.44469E+05, 0.47416E+05, 0.50520E+05,
     + 0.53789E+05, 0.57229E+05, 0.60847E+05, 0.64650E+05, 0.68645E+05,
     + 0.72840E+05, 0.77242E+05, 0.81859E+05, 0.86699E+05, 0.91770E+05,
     + 0.97081E+05, 0.10264E+06, 0.10846E+06, 0.11454E+06, 0.12090E+06,
     + 0.12754E+06, 0.13447E+06, 0.14171E+06, 0.14927E+06, 0.15715E+06,
     + 0.16536E+06, 0.17392E+06, 0.18284E+06, 0.19213E+06, 0.20179E+06,
     + 0.21185E+06, 0.22231E+06, 0.23319E+06, 0.24450E+06, 0.25625E+06,
     + 0.26845E+06, 0.28112E+06, 0.29427E+06, 0.30791E+06, 0.32206E+06,
     + 0.33674E+06, 0.35196E+06, 0.36772E+06, 0.38406E+06, 0.40098E+06,
     + 0.41850E+06, 0.43663E+06, 0.45539E+06, 0.47480E+06, 0.49488E+06,
     + 0.51564E+06, 0.53710E+06, 0.55928E+06, 0.58219E+06, 0.60586E+06,
     + 0.63029E+06, 0.65553E+06, 0.68157E+06, 0.70844E+06, 0.73616E+06,
     + 0.76476E+06, 0.79424E+06, 0.82464E+06, 0.85597E+06, 0.88826E+06,
     + 0.92153E+06, 0.95580E+06, 0.99108E+06, 0.10274E+07, 0.10648E+07,
     + 0.11033E+07, 0.11429E+07, 0.11837E+07, 0.12256E+07, 0.12687E+07,
     + 0.13131E+07, 0.13586E+07, 0.14055E+07, 0.14536E+07, 0.15031E+07,
     + 0.15539E+07),
// c...        --       727
      stdarrayify( 0.20518E+04, 0.29051E+04, 0.37601E+04,
     + 0.46209E+04, 0.54961E+04, 0.63969E+04, 0.73353E+04, 0.83227E+04,
     + 0.93698E+04, 0.10486E+05, 0.11681E+05, 0.12962E+05, 0.14337E+05,
     + 0.15815E+05, 0.17403E+05, 0.19110E+05, 0.20942E+05, 0.22909E+05,
     + 0.25018E+05, 0.27278E+05, 0.29699E+05, 0.32290E+05, 0.35060E+05,
     + 0.38019E+05, 0.41177E+05, 0.44545E+05, 0.48135E+05, 0.51957E+05,
     + 0.56023E+05, 0.60346E+05, 0.64938E+05, 0.69812E+05, 0.74981E+05,
     + 0.80461E+05, 0.86264E+05, 0.92406E+05, 0.98902E+05, 0.10577E+06,
     + 0.11302E+06, 0.12067E+06, 0.12875E+06, 0.13726E+06, 0.14622E+06,
     + 0.15566E+06, 0.16559E+06, 0.17604E+06, 0.18702E+06, 0.19855E+06,
     + 0.21066E+06, 0.22336E+06, 0.23669E+06, 0.25065E+06, 0.26528E+06,
     + 0.28061E+06, 0.29664E+06, 0.31342E+06, 0.33096E+06, 0.34930E+06,
     + 0.36845E+06, 0.38845E+06, 0.40933E+06, 0.43111E+06, 0.45383E+06,
     + 0.47751E+06, 0.50219E+06, 0.52790E+06, 0.55466E+06, 0.58252E+06,
     + 0.61151E+06, 0.64166E+06, 0.67300E+06, 0.70558E+06, 0.73943E+06,
     + 0.77458E+06, 0.81108E+06, 0.84896E+06, 0.88827E+06, 0.92904E+06,
     + 0.97131E+06, 0.10151E+07, 0.10605E+07, 0.11076E+07, 0.11563E+07,
     + 0.12068E+07, 0.12590E+07, 0.13130E+07, 0.13689E+07, 0.14267E+07,
     + 0.14865E+07, 0.15483E+07, 0.16121E+07, 0.16781E+07, 0.17462E+07,
     + 0.18165E+07, 0.18892E+07, 0.19641E+07, 0.20415E+07, 0.21213E+07,
     + 0.22036E+07, 0.22884E+07, 0.23759E+07, 0.24661E+07, 0.25590E+07,
     + 0.26547E+07, 0.27533E+07, 0.28549E+07, 0.29594E+07, 0.30670E+07,
     + 0.31778E+07, 0.32918E+07, 0.34090E+07, 0.35296E+07, 0.36536E+07,
     + 0.37812E+07, 0.39123E+07, 0.40470E+07, 0.41855E+07, 0.43278E+07,
     + 0.44739E+07),
// c...        --       838
      stdarrayify( 0.12066E+03, 0.17085E+03, 0.22116E+03,
     + 0.27190E+03, 0.32364E+03, 0.37711E+03, 0.43305E+03, 0.49219E+03,
     + 0.55516E+03, 0.62256E+03, 0.69492E+03, 0.77276E+03, 0.85657E+03,
     + 0.94685E+03, 0.10441E+04, 0.11488E+04, 0.12614E+04, 0.13826E+04,
     + 0.15127E+04, 0.16525E+04, 0.18024E+04, 0.19630E+04, 0.21351E+04,
     + 0.23191E+04, 0.25158E+04, 0.27260E+04, 0.29502E+04, 0.31892E+04,
     + 0.34438E+04, 0.37148E+04, 0.40031E+04, 0.43094E+04, 0.46346E+04,
     + 0.49797E+04, 0.53455E+04, 0.57331E+04, 0.61434E+04, 0.65775E+04,
     + 0.70364E+04, 0.75212E+04, 0.80330E+04, 0.85730E+04, 0.91424E+04,
     + 0.97423E+04, 0.10374E+05, 0.11039E+05, 0.11738E+05, 0.12474E+05,
     + 0.13246E+05, 0.14057E+05, 0.14908E+05, 0.15801E+05, 0.16737E+05,
     + 0.17717E+05, 0.18744E+05, 0.19819E+05, 0.20944E+05, 0.22120E+05,
     + 0.23349E+05, 0.24634E+05, 0.25975E+05, 0.27376E+05, 0.28837E+05,
     + 0.30361E+05, 0.31950E+05, 0.33605E+05, 0.35330E+05, 0.37126E+05,
     + 0.38996E+05, 0.40942E+05, 0.42965E+05, 0.45069E+05, 0.47256E+05,
     + 0.49528E+05, 0.51888E+05, 0.54338E+05, 0.56882E+05, 0.59521E+05,
     + 0.62259E+05, 0.65097E+05, 0.68040E+05, 0.71090E+05, 0.74249E+05,
     + 0.77522E+05, 0.80910E+05, 0.84417E+05, 0.88046E+05, 0.91801E+05,
     + 0.95684E+05, 0.99699E+05, 0.10385E+06, 0.10814E+06, 0.11257E+06,
     + 0.11715E+06, 0.12187E+06, 0.12675E+06, 0.13179E+06, 0.13699E+06,
     + 0.14235E+06, 0.14788E+06, 0.15358E+06, 0.15946E+06, 0.16552E+06,
     + 0.17176E+06, 0.17819E+06, 0.18482E+06, 0.19164E+06, 0.19867E+06,
     + 0.20590E+06, 0.21335E+06, 0.22101E+06, 0.22889E+06, 0.23699E+06,
     + 0.24533E+06, 0.25390E+06, 0.26271E+06, 0.27177E+06, 0.28108E+06,
     + 0.29064E+06),
// c...        --       837
      stdarrayify( 0.14071E+04, 0.19923E+04, 0.25789E+04,
     + 0.31704E+04, 0.37733E+04, 0.43962E+04, 0.50477E+04, 0.57360E+04,
     + 0.64687E+04, 0.72525E+04, 0.80938E+04, 0.89984E+04, 0.99723E+04,
     + 0.11021E+05, 0.12150E+05, 0.13366E+05, 0.14673E+05, 0.16079E+05,
     + 0.17589E+05, 0.19211E+05, 0.20949E+05, 0.22812E+05, 0.24807E+05,
     + 0.26940E+05, 0.29221E+05, 0.31656E+05, 0.34254E+05, 0.37023E+05,
     + 0.39972E+05, 0.43111E+05, 0.46449E+05, 0.49996E+05, 0.53762E+05,
     + 0.57756E+05, 0.61991E+05, 0.66477E+05, 0.71226E+05, 0.76249E+05,
     + 0.81558E+05, 0.87167E+05, 0.93088E+05, 0.99334E+05, 0.10592E+06,
     + 0.11286E+06, 0.12016E+06, 0.12785E+06, 0.13594E+06, 0.14444E+06,
     + 0.15337E+06, 0.16274E+06, 0.17258E+06, 0.18290E+06, 0.19371E+06,
     + 0.20504E+06, 0.21691E+06, 0.22933E+06, 0.24233E+06, 0.25592E+06,
     + 0.27012E+06, 0.28496E+06, 0.30046E+06, 0.31663E+06, 0.33351E+06,
     + 0.35111E+06, 0.36946E+06, 0.38858E+06, 0.40850E+06, 0.42924E+06,
     + 0.45083E+06, 0.47329E+06, 0.49666E+06, 0.52095E+06, 0.54620E+06,
     + 0.57243E+06, 0.59967E+06, 0.62796E+06, 0.65732E+06, 0.68778E+06,
     + 0.71938E+06, 0.75214E+06, 0.78611E+06, 0.82131E+06, 0.85777E+06,
     + 0.89553E+06, 0.93463E+06, 0.97511E+06, 0.10170E+07, 0.10603E+07,
     + 0.11051E+07, 0.11514E+07, 0.11993E+07, 0.12488E+07, 0.12999E+07,
     + 0.13527E+07, 0.14073E+07, 0.14636E+07, 0.15217E+07, 0.15816E+07,
     + 0.16435E+07, 0.17072E+07, 0.17730E+07, 0.18408E+07, 0.19107E+07,
     + 0.19827E+07, 0.20569E+07, 0.21334E+07, 0.22121E+07, 0.22931E+07,
     + 0.23765E+07, 0.24624E+07, 0.25507E+07, 0.26416E+07, 0.27351E+07,
     + 0.28312E+07, 0.29301E+07, 0.30317E+07, 0.31361E+07, 0.32434E+07,
     + 0.33537E+07));
  
  gsi = xgj[iso-1];
  const auto& q = qoft[iso-1];
  
  if (t < 70 or t > 3000)
    qt = -1.0;
  else
    qt = atob(t, tdat, q);
}

void calcw(CommonBlock& cmn,
           const Index& n,
           const Index& iband,
           const Numeric& temp)
{
  for (Index i=0; i<n; i++) {
    cmn.YLT.YT[i] = 0;
    for (Index j=0; j<n; j++) {
      cmn.Wmatrix.W(i, j) = 0;
    }
  }
  
  if (cmn.Bands.li[iband] > 8 or std::abs(cmn.Bands.li[iband] - cmn.Bands.lf[iband]) > 1) {
    for (Index i=0; i<n; i++) {
      cmn.Wmatrix.W(i, i) = cmn.GamT.HWT[i];
    }
    return;
  }
  
  Vector s(n);
  for (Index i=0; i<n; i++) {
    s[i] = cmn.LineSg.Sig(i, iband) * cmn.PopuT.PopuT[i] * Constant::pow2(cmn.DipoTcm.DipoT(i, iband));
  }
  
  for (Index i=0; i<n; i++) {
    for (Index j=i+1; j<n; j++) {
      if (s[j] > s[i]) {
        // Switch switches 2 elements in a vector :: std::swap(A(i), A(j));
        // SwitchTwo switches two elements in a Matrix :: std::swap(A(i, k), A(j, k));
        // SwitchInt is as SwitchTwo but for Integers :: std::swap(A(i, k), A(j, k));
        std::swap(cmn.DipoRigid.Dipo0(i, iband), cmn.DipoRigid.Dipo0(j, iband));
        std::swap(cmn.DipoTcm.DipoT(i, iband), cmn.DipoTcm.DipoT(j, iband));
        std::swap(cmn.LineSg.Sig(i, iband), cmn.LineSg.Sig(j, iband));
        std::swap(cmn.Jiln.Ji(i, iband), cmn.Jiln.Ji(j, iband));
        std::swap(cmn.Jfln.Jf(i, iband), cmn.Jiln.Ji(j, iband));
        std::swap(cmn.GamT.HWSDV2T[i], cmn.GamT.HWSDV2T[j]);
        std::swap(cmn.PopuT.PopuT[i], cmn.PopuT.PopuT[j]);
        std::swap(cmn.SHIFT.shft[i], cmn.SHIFT.shft[j]);
        std::swap(cmn.GamT.HWT[i], cmn.GamT.HWT[j]);
        std::swap(s[i], s[j]);
      }
    }
  }
  
  const Numeric dlgt0t = std::log(parameters::T0 / temp);
  const Index lli = std::min(cmn.Bands.li[iband], cmn.Bands.lf[iband]);
  const Index llf = std::max(cmn.Bands.li[iband], cmn.Bands.lf[iband]);
  
  // Set off-diagonal elements
  for (Index i=0; i<n; i++) {
    Index jji, jjf;
    if (cmn.Bands.li[iband] <= cmn.Bands.lf[iband]) {
      jji=cmn.Jiln.Ji(i, iband);
      jjf=cmn.Jfln.Jf(i, iband);
    } else {
      jji=cmn.Jfln.Jf(i, iband);
      jjf=cmn.Jiln.Ji(i, iband);
    }
    
    for (Index j=0; j<n; j++) {
      Index jjip, jjfp;
      if (cmn.Bands.li[iband] <= cmn.Bands.lf[iband]) {
        jjip=cmn.Jiln.Ji(j, iband);
        jjfp=cmn.Jfln.Jf(j, iband);
      } else {
        jjip=cmn.Jfln.Jf(j, iband);
        jjfp=cmn.Jiln.Ji(j, iband);
      }
      
      if (jjip > jji) continue;
      
      if (cmn.Bands.Isot[iband] > 2 and cmn.Bands.Isot[iband] not_eq 7 and cmn.Bands.Isot[iband] not_eq 10 and (std::abs(cmn.Jiln.Ji(i, iband)-cmn.Jiln.Ji(j, iband)) % 2) not_eq 0) continue;
      
      Numeric w0=0, b0=0;
      if (jji > jjf and jjip > jjfp) {
        w0 = cmn.Wfittedp.W0pp(lli, llf, jji, jjip);
        b0 = cmn.Bfittedp.B0pp(lli, llf, jji, jjip);
      } else if (jji > jjf and jjip == jjfp) {
        w0 = cmn.Wfittedp.W0pq(lli, llf, jji, jjip);
        b0 = cmn.Bfittedp.B0pq(lli, llf, jji, jjip);
      } else if (jji > jjf and jjip < jjfp) {
        w0 = cmn.Wfittedp.W0pr(lli, llf, jji, jjip);
        b0 = cmn.Bfittedp.B0pr(lli, llf, jji, jjip);
      } else if (jji < jjf and jjip > jjfp) {
        w0 = cmn.Wfittedr.W0rp(lli, llf, jji, jjip);
        b0 = cmn.Bfittedr.B0rp(lli, llf, jji, jjip);
      } else if (jji < jjf and jjip == jjfp) {
        w0 = cmn.Wfittedr.W0rq(lli, llf, jji, jjip);
        b0 = cmn.Bfittedr.B0rq(lli, llf, jji, jjip);
      } else if (jji < jjf and jjip < jjfp) {
        w0 = cmn.Wfittedr.W0rr(lli, llf, jji, jjip);
        b0 = cmn.Bfittedr.B0rr(lli, llf, jji, jjip);
      } else if (jji == jjf and jjip > jjfp) {
        w0 = cmn.Wfittedq.W0qp(lli, llf, jji, jjip);
        b0 = cmn.Bfittedq.B0qp(lli, llf, jji, jjip);
      } else if (jji == jjf and jjip == jjfp) {
        w0 = cmn.Wfittedq.W0qq(lli, llf, jji, jjip);
        b0 = cmn.Bfittedq.B0qq(lli, llf, jji, jjip);
      } else if (jji == jjf and jjip < jjfp) {
        w0 = cmn.Wfittedq.W0qr(lli, llf, jji, jjip);
        b0 = cmn.Bfittedq.B0qr(lli, llf, jji, jjip);
      }
      
      const Numeric ycal = std::exp(w0 - b0*dlgt0t);
      cmn.Wmatrix.W(j, i) = ycal;
      cmn.Wmatrix.W(i, j) = ycal * cmn.PopuT.PopuT[i] / cmn.PopuT.PopuT[j];
    }
  }
  
  // Weird undocumented minus sign
  for (Index i=0; i<n; i++) {
    for (Index j=0; j<n; j++) {
      if (j not_eq i) {
        cmn.Wmatrix.W(i, j) = - std::abs(cmn.Wmatrix.W(i, j));
      }
    }
  }
  
  // Set diagonal to measured broadening
  for (Index i=0; i<n; i++) {
    cmn.Wmatrix.W(i, i) = cmn.GamT.HWT[i];
  }
  
  // Sum rule correction
  for (Index i=0; i<n; i++) {
    Numeric sumlw = 0.0;
    Numeric sumup = 0.0;
    
    for (Index j=0; j<n; j++) {
      if (cmn.Bands.Isot[iband] > 2 and cmn.Bands.Isot[iband] not_eq 7 and cmn.Bands.Isot[iband] not_eq 10 and (std::abs(cmn.Jiln.Ji(i, iband)-cmn.Jiln.Ji(j, iband)) % 2) not_eq 0) continue;
      
      if (j > i) {
        sumlw += std::abs(cmn.DipoRigid.Dipo0(j, iband)) * cmn.Wmatrix.W(j, i);
      } else {
        sumup += std::abs(cmn.DipoRigid.Dipo0(j, iband)) * cmn.Wmatrix.W(j, i);
      }
    }
    
    for (Index j=i+1; j<n; j++) {
      if (sumlw == 0) {
        cmn.Wmatrix.W(j, i) = 0.0;
        cmn.Wmatrix.W(i, j) = 0.0;
      } else {
        cmn.Wmatrix.W(j, i) = cmn.Wmatrix.W(j, i) * (- sumup / sumlw);
        cmn.Wmatrix.W(i, j) = cmn.Wmatrix.W(j, i) * cmn.PopuT.PopuT[i] / cmn.PopuT.PopuT[j];
      }
    }
  }
  
  // Rosenkranz coefficients
  for (Index i=0; i<n; i++) {
    Numeric sum0 = 0;
    for (Index j=0; j<n; j++) {
      if (i == j) continue;
      
      if (cmn.Bands.Isot[iband] > 2 and cmn.Bands.Isot[iband] not_eq 7 and cmn.Bands.Isot[iband] not_eq 10 and (std::abs(cmn.Jiln.Ji(i, iband)-cmn.Jiln.Ji(j, iband)) % 2) not_eq 0) continue;
      
      Numeric deltasig = cmn.LineSg.Sig(i, iband) - cmn.LineSg.Sig(j, iband);
      if (std::abs(deltasig) < 1e-4 /*cm-1*/ ) deltasig = 1e-4 /*cm-1*/;
      
      sum0 += 2 * std::abs(cmn.DipoTcm.DipoT(j, iband)) / std::abs(std::abs(cmn.DipoTcm.DipoT(i, iband))) * cmn.Wmatrix.W(j, i) / deltasig;
    }
    
    cmn.YLT.YT[i] = sum0;
  }
}

template <class T>
void printMat(T& mat)
{
  for (Index i=0; i<mat.rows(); i++) {
    for (Index j=0; j<mat.cols(); j++) {
      std::cout<<mat(i, j).real()<<'+'<<mat(i, j).imag()<<'j'<<' ';
    }
    std::cout<<'\n';
  }
  std::exit(0);
}

void eqvlines(CommonBlock& cmn,
              const Index& iband,
              const Index& n,
              const Numeric& sigmoy)
{
  Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> zop(n, n);
  
  for (Index i=0; i<n; i++) {
    for (Index j=0; j<n; j++) {
      zop(i, j) = Complex(cmn.DiagnR.OpR(i, j), cmn.DiagnI.OpI(i, j));
    }
  }
  
  // FIXME: transpose zop because of how Fortran works???
  const Eigen::ComplexEigenSolver<decltype(zop)> diagoanlized_zop(zop, true);
  
  // Extract real and imaginary eigenvalues
  Eigen::RowVectorXd eigvlr(n);
  Eigen::RowVectorXd eigvli(n);
  const Eigen::VectorXcd zval = diagoanlized_zop.eigenvalues();
  for (Index i=0; i<n; i++) {
    eigvlr[i] = zval[i].real();
    eigvli[i] = zval[i].imag();
  }
  
  Eigen::VectorXcd zsum = Eigen::VectorXcd::Zero(n);
  const decltype(zop) zvec = diagoanlized_zop.eigenvectors();
  for (Index i=0; i<n; i++) {
    for (Index j=0; j<n; j++) {
      zsum[i] += cmn.DipoTcm.DipoT(j, iband) * zvec(j, i);
    }
  }
  
  // FIXME: Again, is the order of the matrix correct???
  const decltype(zop) inv_zvec = zvec.inverse();
  
  
  if (iband == 0)
    printMat(inv_zvec);
  for (Index i=0; i<n; i++) {
    Complex z(0, 0);
    for (Index j=0; j<n; j++) {
      z += cmn.PopuT.PopuT[j] * cmn.DipoTcm.DipoT(j, iband) * inv_zvec(i, j);
    }
    zsum[i] *= z;
  }
  
  for (Index i=0; i<n; i++) {
    cmn.Zss.ZS[i] = zsum[i];
    cmn.Zaa.ZA[i] = Complex(eigvlr[i] + sigmoy, eigvli[i]);
  }
  
  for (Index i=0; i<n; i++) {
    cmn.FicLPI.AlphI[i] = zsum[i].real();
    cmn.FicLPR.AlphR[i] = zsum[i].imag();
    cmn.FicLSI.SSI[i] = eigvlr[i];
    cmn.FicLSR.SSR[i] = eigvli[i];
  }
}
           

void convtp(CommonBlock& cmn,
            const Index& iband,
            const Index& isotc,
            const Index& nlinec,
            const Numeric& xh2o,
            const Numeric& xco2,
            const Numeric& temp,
            const Numeric& ptot,
            Numeric& sigmoy,
            Numeric& gamd_gam0_mx,
            const bool mixfull,
            const bool mixsdv)
{
  const Numeric ratiot = parameters::T0 / temp;
  Numeric qt0_co2, qtt_co2, gsi0, gsit;
  qt_co2(parameters::T0, isotc, gsi0, qt0_co2);
  qt_co2(temp, isotc, gsit, qtt_co2);
  const Numeric ratiopart = qt0_co2 / qtt_co2;
  
  const Numeric sqrtm = std::sqrt(temp / parameters::aMass[isotc-1]);
  
  Numeric sumwgt=0;
  sigmoy=0;
  for (Index iline=0; iline<nlinec; iline++) {
    cmn.PopuT.PopuT[iline]=cmn.PopTrf.PopuT0(iline, iband)*ratiopart*std::exp(-parameters::Ct*cmn.Energy.E(iline, iband)*(1/temp-1/parameters::T0));
    if (mixsdv) {
      cmn.GamT.HWT[iline] = ((1-xh2o-xco2) * (cmn.GamSDVT0AIR.HWSDVT0AIR(iline, iband) * std::pow(ratiot, cmn.DTGAMAIR.BHWAIR(iline, iband))))
      + (xh2o * (cmn.GamSDVT0H2O.HWSDVT0H2O(iline, iband) * std::pow(ratiot, cmn.DTGAMH2O.BHWH2O(iline, iband))))
      + (xco2 * (cmn.GamSDVT0CO2.HWSDVT0SELF(iline, iband) * std::pow(ratiot, cmn.DTGAMCO2.BHWSELF(iline, iband))));
      
      cmn.GamT.HWSDV2T[iline] = ((1-xh2o-xco2) * (cmn.GamSDVT0AIR.HWSDVT0AIR(iline, iband) * cmn.GamSDVT0AIR.rHWT0AIR(iline, iband) * std::pow(ratiot, cmn.DTGAMAIR.BHWAIR(iline, iband))))
      + (xh2o * (cmn.GamSDVT0H2O.HWSDVT0H2O(iline, iband)*cmn.GamSDVT0H2O.rHWT0H2O(iline, iband) * std::pow(ratiot, cmn.DTGAMH2O.BHWH2O(iline, iband))))
      + (xco2 * (cmn.GamSDVT0CO2.HWSDVT0SELF(iline, iband) * cmn.GamSDVT0CO2.rHWT0SELF(iline, iband) * std::pow(ratiot, cmn.DTGAMCO2.BHWSELF(iline, iband))));
    } else {
      cmn.GamT.HWT[iline] = ((1-xh2o-xco2) * (cmn.GamVT0AIR.HWVT0AIR(iline,iband) * std::pow(ratiot, cmn.DTGAMAIR.BHWAIR(iline, iband))))
      + (xh2o * (cmn.GamVT0H2O.HWVT0H2O(iline, iband) * std::pow(ratiot, cmn.DTGAMH2O.BHWH2O(iline, iband))))
      + (xco2 * (cmn.GamVT0CO2.HWVT0SELF(iline, iband) * std::pow(ratiot, cmn.DTGAMCO2.BHWSELF(iline, iband))));
    }
    cmn.SHIFT.shft[iline]=cmn.SHIFT0.shft0(iline, iband);
    const Numeric wgt = cmn.PopuT.PopuT[iline] * Constant::pow2(cmn.DipoTcm.DipoT(iline, iband));
    sumwgt += wgt;
    sigmoy += cmn.LineSg.Sig(iline,iband) * wgt;
  }
  sigmoy /= sumwgt;
  
  calcw(cmn, nlinec, iband, temp);
  
  // Adjust for pressure
  for (Index iline=0; iline<nlinec; iline++) {
    cmn.GamT.HWT[iline] *= ptot;
    cmn.GamT.HWSDV2T[iline] *= ptot;
    cmn.YLT.YT[iline] *= ptot;
    const Numeric gamd = parameters::CtGamD * cmn.LineSg.Sig(iline, iband) * sqrtm;
    gamd_gam0_mx = std::max(gamd_gam0_mx, gamd/cmn.GamT.HWT[iline]);
  }
  
  if (mixfull) {
    for (Index iline=0; iline<nlinec; iline++) {
      for (Index ilinep=0; ilinep<nlinec; ilinep++) {
        cmn.DiagnI.OpI(iline, ilinep) = ptot * cmn.Wmatrix.W(iline, ilinep);
        if (iline == ilinep) {
          cmn.DiagnR.OpR(iline, iline) = cmn.LineSg.Sig(iline, iband) - sigmoy + cmn.SHIFT.shft[iline] * ptot;
        } else {
          cmn.DiagnR.OpR(iline, ilinep) = 0;
        }
      }
    }
    
    eqvlines(cmn, iband, nlinec, sigmoy);
  }
}

void qsdv(const Numeric& sg0,
          const Numeric& gamd,
          const Numeric& gam0,
          const Numeric& gam2,
          const Numeric& shift0,
          const Numeric& shift2,
          const Numeric& sg,
          Numeric& ls_qsdv_r,
          Numeric& ls_qsdv_i)
{
  using Constant::pow2;
  Complex x , y, csqrty, z1, z2, w1, w2, aterm;
  Numeric xz1, yz1, xz2, yz2;
  
  const Numeric cte = Constant::sqrt_ln_2 / gamd;
  const Numeric pi = Constant::pi;
  const Numeric rpi = Constant::sqrt_pi;
  const Complex iz = Complex(0, 1);
  
  const Complex c0 = Complex(gam0, shift0);
  const Complex c2 = Complex(gam2, shift2);
  const Complex c0t = (c0 - 1.5 * c2);
  const Complex c2t = c2;
  
  if (std::abs(c2t) == 0) goto label110;
  x = (iz * (sg0 - sg) + c0t) / c2t;
  y = 1 / pow2(2 * cte * c2t);
  csqrty = (gam2 - iz * shift2) / (2 * cte * (pow2(gam2) + pow2(shift2)));
  if (std::abs(x) <= 3.e-8*std::abs(y)) goto label120;  // IN cgs units
  if (std::abs(y) <= 1.e-15*std::abs(x)) goto label140;  // IN cgs units
  z1 = std::sqrt(x + y) - csqrty;
  z2 = z1 + 2 * csqrty;
  
  xz1 = - z1.imag();
  yz1 = z1.real();
  xz2 = - z2.imag();
  yz2 = z2.real();
  
  w1 = Faddeeva::w(Complex(xz1 ,yz1));
  w2 = Faddeeva::w(Complex(xz2 ,yz2));
  aterm = rpi * cte * (w1 - w2);
  goto label10;
  
  label110:
  z1 = (iz*(sg0-sg)+c0t)*cte;
  xz1 = - z1.imag();
  yz1 = z1.real();
  
  w1 = Faddeeva::w(Complex(xz1 ,yz1));
  aterm = rpi*cte* w1;
  goto label10;
  
  label120:
  
  z1 = (iz*(sg0-sg)+c0t)*cte;
  z2 = std::sqrt(x+y)+csqrty;
  xz1 = - z1.imag();
  yz1 = z1.real();
  xz2 = - z2.imag();
  yz2 = z2.real();
  
  w1 = Faddeeva::w(Complex(xz1 ,yz1));
  w2 = Faddeeva::w(Complex(xz2 ,yz2));
  aterm = rpi * cte * (w1-w2);
  goto label10;
  
  label140:
  
  if (std::abs(std::sqrt(x)) <= 4.e3) {  // IN cgs
    const Numeric xxb = - std::sqrt(x).imag();
    const Numeric yxb = - std::sqrt(x).imag();
    const Complex wb = Faddeeva::w(Complex(xxb, yxb));
    aterm = (2 * rpi / c2t) * (1 / rpi - std::sqrt(x) * wb);
  } else {
      aterm = (1 / c2t) * (1 / x - 1.5 / pow2(x));
  }
  
  label10:
  
  const Complex ls_qsdv = (1 / pi) * aterm;
  
  ls_qsdv_r = ls_qsdv.real();
  ls_qsdv_i = ls_qsdv.imag();
}

Index compabs(
  CommonBlock& cmn,
  const Numeric& temp,
  const Numeric& ptot,
  const Numeric& xco2,
  const Numeric& xh2o,
  const Numeric& sigmin,
  const Numeric& sigmax,
  const Numeric& dsig,
  const bool mixsdv,
  const bool mixfull,
  VectorView absv,
  VectorView absy,
  VectorView absw)
{
  using Constant::pow2;
  using Constant::pow3;
  
  constexpr Numeric rdmult = 30;
  
  // Number of points to compute
  const Index nsig = Index(((sigmax - sigmin) / dsig) + 0.5) + 1;
  if (nsig > parameters::nSigmx) 
    throw std::runtime_error("Too many sigmas to compute");
  
  // Set to zero
  absv = 0;
  absy = 0;
  absw = 0;
  
  constexpr Numeric sq_ln2 = Constant::sqrt_ln_2;
  constexpr Numeric sq_ln2pi = sq_ln2 / Constant::sqrt_pi;
  const Numeric dens = xco2 * ptot * parameters::aMolAtm / temp;
  constexpr Numeric u_pi = Constant::inv_pi;
  constexpr Numeric u_sqln2pi = 1 / sq_ln2pi;
  
  readlines(cmn, mixsdv);
  
  for (Index iband=0; iband<cmn.Bands.nBand; iband++) {
    Numeric sigmoy=0;
    Numeric gamdmx=0;
    convtp(cmn, iband, cmn.Bands.Isot[iband], cmn.Bands.nLines[iband],
           xh2o, xco2, temp, ptot, sigmoy, gamdmx, mixfull, mixsdv);
    const Numeric sqrtm = std::sqrt(temp / parameters::aMass[cmn.Bands.Isot[iband]-1]);
    
    const Numeric ds0=(70.67e0+104.1e0*0.21e0*std::pow(gamdmx, 6.4))/(1.0+0.21*std::pow(gamdmx, 5.4));
    const Numeric ds2=(34.97e0+105.e0*9.e0*std::pow(gamdmx, 3.1))/(1.0+9.0*std::pow(gamdmx, 2.1));
    Numeric sigc = sigmin-dsig;
    
    for (Index isig=0; isig<nsig; isig++) {
      sigc += dsig;
      
      for (Index iline=0; iline<cmn.Bands.nLines[iband]; iline++) {
        const Numeric gamd=parameters::CtGamD*cmn.LineSg.Sig(iline,iband)*sqrtm;
        const Numeric cte = sq_ln2 / gamd;
        const Numeric popudipo = cmn.PopuT.PopuT[iline] * pow2(cmn.DipoTcm.DipoT(iline, iband));
        const Numeric dsigc = sigc - cmn.LineSg.Sig(iline, iband) - cmn.SHIFT.shft[iline] * ptot;
        
        if (mixsdv) {
          if (std::abs(dsigc) / cmn.GamT.HWT[iline] > ds0) {
            absv[isig] += popudipo * u_sqln2pi * u_pi * cmn.GamT.HWT[iline] / (pow2(cmn.GamT.HWT[iline]) + pow2(dsigc));
            absy[isig] += popudipo * u_sqln2pi * u_pi * (cmn.GamT.HWT[iline] + cmn.YLT.YT[iline]*dsigc) / (pow2(cmn.GamT.HWT[iline]) + pow2(dsigc));
          } else if (std::abs(dsigc) / cmn.GamT.HWT[iline] > ds2 and 
                     std::abs(dsigc) / cmn.GamT.HWT[iline] < ds0) {
            const Numeric order2r = cmn.GamT.HWT[iline] / (pow2(cmn.GamT.HWT[iline]) + pow2(dsigc))
            + 3 * pow2(cmn.GamT.HWSDV2T[iline]) * (pow3(cmn.GamT.HWT[iline]) - 3 * cmn.GamT.HWT[iline]*pow2(dsigc)) / 
            (2*parameters::Pi*pow3(pow2(cmn.GamT.HWT[iline]) + pow2(dsigc)));
            
            const Numeric order2i = dsigc / (pow2(cmn.GamT.HWT[iline]) + pow2(dsigc)) +
            3 * pow2(cmn.GamT.HWSDV2T[iline]) *(3*pow2(cmn.GamT.HWT[iline])*dsigc-pow3(dsigc)) / 
            (2*parameters::Pi*pow3(pow2(cmn.GamT.HWT[iline]) + pow2(dsigc)));
            
            absv[isig] += popudipo * u_sqln2pi * u_pi * order2r;
            
            absy[isig] += popudipo * u_sqln2pi * u_pi * (order2r + cmn.YLT.YT[iline]*order2i);
          } else {
            const Numeric shft0 = cmn.SHIFT.shft[iline] * ptot;
            
            Numeric wr=0, wi=0;
            qsdv(cmn.LineSg.Sig(iline, iband), gamd, cmn.GamT.HWT[iline], cmn.GamT.HWSDV2T[iline], shft0, 0, sigc, wr, wi);
            
            absv[isig] += popudipo * wr * u_sqln2pi;
            
            absy[isig] += popudipo * u_sqln2pi * (wr + cmn.YLT.YT[iline] * wi);
          }
        } else {
          if (std::abs(cmn.LineSg.Sig(iline, iband)-sigc) > (rdmult*gamd)) {  // NOTE: Removed in updated version of the code...
            absv[isig] += popudipo * u_sqln2pi * u_pi * cmn.GamT.HWT[iline] / (pow2(cmn.GamT.HWT[iline]) + pow2(dsigc));
            
            absy[isig] += popudipo * u_sqln2pi * u_pi * (cmn.GamT.HWT[iline] + cmn.YLT.YT[iline] * dsigc) / (pow2(cmn.GamT.HWT[iline]) + pow2(dsigc));
          } else {
            const Numeric yy = cmn.GamT.HWT[iline] * cte;
            const Numeric xx= (cmn.LineSg.Sig(iline, iband)+cmn.SHIFT.shft[iline] * ptot - sigc) * cte;
            const Complex w = Faddeeva::w(Complex(xx, yy));
            
            absv[isig] += popudipo * w.real() / gamd;
            
            absy[isig] += popudipo * (w.real()+cmn.YLT.YT[iline] * w.imag()) / gamd;
          }
        }
        
        if (mixfull) {
          if (std::abs(cmn.FicLPI.AlphI[iline] + sigmoy - sigc) <= (rdmult*gamd)) {
            const Numeric xx = (cmn.FicLPR.AlphR[iline] + sigmoy - sigc) * cte;
            const Numeric yy = cmn.FicLPI.AlphI[iline] * cte;
            const Complex w = Faddeeva::w(Complex(xx, yy));
            
            absw[isig] += (cmn.FicLSR.SSR[iline] * w.real() - cmn.FicLSI.SSI[iline] * w.imag()) / gamd;
          } else {
            absw[isig] += u_sqln2pi * u_pi * (cmn.Zss.ZS[iline] / (sigc - cmn.Zaa.ZA[iline])).imag();
          }
        }
      }
    }
  }
  
  Numeric sigc = sigmin - dsig;
  for (Index isig=0; isig<nsig; isig++) {
    sigc = sigc + dsig;
    const Numeric fact = sigc * (1-std::exp(-parameters::Ct * sigc / temp));
    absv[isig] *= fact * dens * sq_ln2pi;
    absy[isig] *= fact * dens * sq_ln2pi;
    absw[isig] *= fact * dens * sq_ln2pi;
  }
  
  return nsig;
}

template<typename T> String toString(const T& val) {return String(val);}
template<typename T> Index toIndex(const T& val) {return std::stol(toString(val));}
template<typename T> Index toNumeric(const T& val)
{
  String s = toString(val);
  std::replace(s.begin(), s.end(), 'D', 'E');
  std::replace(s.begin(), s.end(), 'd', 'e');
  return std::stod(val);
}

void detband(CommonBlock& cmn,
             const Numeric& sgminr,
             const Numeric& sgmaxr,
             const Numeric& stotmax)
{
  cmn.Bands.nBand=0;
  std::ifstream fortranfile;
  fortranfile.open("data_new/BandInfo.dat");
  
  String line;
  getline(fortranfile, line);
  while (fortranfile.good()) {
    Numeric stot, sgmin, sgmax;
    Index isotr, lfr, lir, jmxp, jmxq, jmxr;
    char c11, c12, c21, c22, c31, c32, c41, c42, c51, c52, x;
    sscanf(line.c_str(), 
           "%1ld"
           "%c%c" "%1ld" "%c%c"
           "%c%c" "%1ld" "%c%c"
           "%c%c"
           "%12lf"
           "%c" "%12lf"
           "%c" "%12lf"
           "%c%c%c%c%c%c%c%c"
           "%4ld" "%4ld" "%4ld",
           &isotr,
           &c11, &c12, &lfr, &c21, &c22,
           &c31, &c32, &lir, &c41, &c42,
           &c51, &c52,
           &stot,
           &x, &sgmin,
           &x, &sgmax,
           &x, &x, &x, &x, &x, &x, &x, &x,
           &jmxp, &jmxq, &jmxr);
    
    getline(fortranfile, line);
    if (stot < stotmax)
      continue;
    
    if ((sgminr < sgmax) and (sgmaxr > sgmin)) {
      cmn.Bands.Isot[cmn.Bands.nBand] = isotr;
      if (isotr == 0)
        cmn.Bands.Isot[cmn.Bands.nBand] = 10;
      
      cmn.Bands.li[cmn.Bands.nBand] = lir;
      cmn.Bands.lf[cmn.Bands.nBand] = lfr;
      
      if (jmxp < 40 or jmxr < 40) cmn.Bands.li[cmn.Bands.nBand] = 55;
      if (jmxq >= 0 and jmxq < 40) cmn.Bands.li[cmn.Bands.nBand] = 55;
      char name[15];
      sprintf(name, "S%ld%c%c%ld%c%c%c%c%ld%c%c%c%c", isotr, c11, c12, lfr, c21, c22, c31, c32, lir, c41, c42, c51, c52);
      cmn.Bands.BandFile[cmn.Bands.nBand] = name;
      cmn.Bands.nBand++;
      if (cmn.Bands.nBand > parameters::nBmx) {
        throw std::runtime_error("Too many bands");
      }
    }
  }
  fortranfile.close();
}

void readw(CommonBlock& cmn)
{
  for (Index l=0; l<=8; l++) {
    for (Index ideltal=0; ideltal<=1; ideltal++) {
      const Index lli = l;
      const Index llf = l + ideltal;
      
      const String cr = std::to_string(lli) + std::to_string(llf);
      
      std::ifstream fortranfile;
      const String fname = String("data_new/WTfit") + cr + String(".dat");
      fortranfile.open(fname.c_str());
      
      String line;
      getline(fortranfile, line);
      while (fortranfile.good()) {
        char sw0r[21], sb0r[21];
        Numeric dmaxdt, wtmax;
        Index jic, jfc, jipc, jfpc;
        sscanf(line.c_str(), 
               "%20s" "%20s"
               "%14lf" "%14lf"
               "%4ld" "%4ld" "%4ld" "%4ld",
               sw0r, sb0r, &dmaxdt, &wtmax, &jic, &jfc, &jipc, &jfpc);
        String ssw0r = sw0r;
        std::replace(ssw0r.begin(), ssw0r.end(), 'D', 'E');
        const Numeric w0r = std::stod(ssw0r);
        String ssb0r = sb0r;
        std::replace(ssb0r.begin(), ssb0r.end(), 'D', 'E');
        const Numeric b0r = std::stod(ssb0r);
        
        getline(fortranfile, line);
        
        if (lli > cmn.Wfittedp.W0pp.nbooks() or
            llf > cmn.Wfittedp.W0pp.npages() or
            jic > cmn.Wfittedp.W0pp.nrows() or
            jipc > cmn.Wfittedp.W0pp.ncols())
          throw std::runtime_error("Out of bounds in reading...");
          
        if (jic > jfc and jipc > jfpc) {
          cmn.Wfittedp.W0pp(lli,llf,jic,jipc) = w0r;
          cmn.Bfittedp.B0pp(lli,llf,jic,jipc) = b0r;
        } else if(jic > jfc and jipc == jfpc) {
          cmn.Wfittedp.W0pq(lli,llf,jic,jipc) = w0r;
          cmn.Bfittedp.B0pq(lli,llf,jic,jipc) = b0r;
        } else if(jic > jfc and jipc < jfpc) {
          cmn.Wfittedp.W0pr(lli,llf,jic,jipc) = w0r;
          cmn.Bfittedp.B0pr(lli,llf,jic,jipc) = b0r;
        } else if(jic < jfc and jipc > jfpc) {
          cmn.Wfittedr.W0rp(lli,llf,jic,jipc) = w0r;
          cmn.Bfittedr.B0rp(lli,llf,jic,jipc) = b0r;
        } else if(jic < jfc and jipc == jfpc) {
          cmn.Wfittedr.W0rq(lli,llf,jic,jipc) = w0r;
          cmn.Bfittedr.B0rq(lli,llf,jic,jipc) = b0r;
        } else if(jic < jfc and jipc < jfpc) {
          cmn.Wfittedr.W0rr(lli,llf,jic,jipc) = w0r;
          cmn.Bfittedr.B0rr(lli,llf,jic,jipc) = b0r;
        } else if(jic == jfc and jipc > jfpc) {
          cmn.Wfittedq.W0qp(lli,llf,jic,jipc) = w0r;
          cmn.Bfittedq.B0qp(lli,llf,jic,jipc) = b0r;
        } else if(jic == jfc and jipc == jfpc) {
          cmn.Wfittedq.W0qq(lli,llf,jic,jipc) = w0r;
          cmn.Bfittedq.B0qq(lli,llf,jic,jipc) = b0r;
        } else if(jic == jfc and jipc < jfpc) {
          cmn.Wfittedq.W0qr(lli,llf,jic,jipc) = w0r;
          cmn.Bfittedq.B0qr(lli,llf,jic,jipc) = b0r;
        }
      }
    }
  }
}


Vector compute(const Numeric p, const Numeric t, const Numeric xco2, const Numeric xh2o, const Numeric sigmin, const Numeric sigmax, const Numeric stotmax, const Numeric dsig, const calctype type)
{
  CommonBlock cmn;
  detband(cmn, sigmin, sigmax, stotmax);
  readw(cmn);
  
  Vector absv(parameters::nSigmx);
  Vector absy(parameters::nSigmx);
  Vector absw(parameters::nSigmx);
  Index nf;
  if (type == calctype::FullVP or type == calctype::FullRosenkranz or type == calctype::FullW)
    nf = compabs(cmn, t,p, xco2, xh2o, sigmin, sigmax, dsig, false, true, absv, absy, absw);
  else if (type == calctype::SDVP or type == calctype::SDRosenkranz or type == calctype::SDW)
    nf = compabs(cmn, t,p, xco2, xh2o, sigmin, sigmax, dsig, true, false, absv, absy, absw);
  else if (type == calctype::NoneVP or type == calctype::NoneRosenkranz or type == calctype::NoneW)
    nf = compabs(cmn, t,p, xco2, xh2o, sigmin, sigmax, dsig, false, false, absv, absy, absw);
  else
    nf = 0;
  
//   for (Index i=0; i<nf; i++)
//     std::cout<<absv[i]<<' '<<absy[i]<<' '<<absw[i]<<'\n';
  
  Vector absorption(nf);
  switch(type) {
    case calctype::FullVP:
    case calctype::SDVP:
    case calctype::NoneVP:
      for (Index i=0; i<nf; i++)
        absorption[i] = absv[i];
      break;
    case calctype::FullRosenkranz:
    case calctype::SDRosenkranz:
    case calctype::NoneRosenkranz:
      for (Index i=0; i<nf; i++)
        absorption[i] = absy[i];
      break;
    case calctype::FullW:
    case calctype::SDW:
    case calctype::NoneW:
      for (Index i=0; i<nf; i++)
        absorption[i] = absw[i];
      break;
  }
  
  return absorption;
}
};
