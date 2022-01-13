#ifndef GenericEvent_hh
#define GenericEvent_hh

#include "TClonesArray.h"
#include "TH2F.h"
#include "TObject.h"

#include "Cluster.hh"
#include "DataTypes.hh"

template <size_t NJINF, size_t NTDRS, size_t NCHAVA, size_t NADCS, size_t NVAS> class GenericEvent : public TObject {
  // handy shortcuts for common types
  template <typename T> using JArray = Array1<T, NJINF>;
  template <typename T> using TdrArray = Array2<T, NJINF, NTDRS>;
  template <typename T> using VAArray = Array3<T, NJINF, NTDRS, NVAS>;
  template <typename T> using ChArray = Array3<T, NJINF, NTDRS, NCHAVA * NVAS>;

public:
  friend class DecodeData;
  friend class DecodeDataOCA;
  friend class DecodeDataFOOT;

public:
  //! Default contructor
  GenericEvent();

  //! Default destructor
  ~GenericEvent();

  // TODO: check if can be removed [VF]
  //  // saved on disk, used as array size for the streamers
  //  int _NJINF = NJINF;
  //  int _NTDRS = NTDRS;
  //  int _NCHAVA = NCHAVA;
  //  int _NADCS = NADCS;
  //  int _NVAS = NVAS;
  //  int _NCHA = NCHAVA * NVAS; //is needed for the array size as said before

  static constexpr size_t GetNJINF(){ return NJINF; }
  static constexpr size_t GetNTDRS(){ return NTDRS; }
  static constexpr size_t GetNADCS(){ return NADCS; }
  static constexpr size_t GetNCHAVA(){ return NCHAVA; }
  static constexpr size_t GetNVAS(){ return NVAS; }
  static constexpr size_t GetNCH(){ return NVAS * NCHAVA; }

  //! Clear the event
  void Clear();

  //! Add a Cluster to the array
  Cluster *AddCluster(int Jinfnum, int lad, int side);
  //! Get the Cluster in the position ii of the array
  Cluster *GetCluster(int ii);

  inline int GetNClusTot() { return NClusTot; };

  static double ComputeCN(int size, shortint *RawSignal, float *pede, float *RawSoN, int *status,
                          double threshold = 3.0);

  //  int NGoldenClus(int lad, int side);
  //! Load LadderConf parameter from an ASCII file
  static void ReadLadderConf(TString filename, bool DEBUG = false);
  //! Get Gain Correction "component" for the vanum VA of the tdrnum TDR for the jinfnum JINF
  static LadderConf *GetLadderConf() { return ladderconf; };
  //! Load Alignment parameter from an ASCII file
  static void ReadAlignment(TString filename, bool DEBUG = false);
  //! Get Alignment parameter "component" for the tdrnum TDR of the jinfnum JINF
  static float GetAlignPar(int jinfnum, int tdrnum, int component);
  static float GetMultiplicityFlip(int jinfnum, int tdrnum);
  //! Load VA Gain Correction from an ASCII file
  static void ReadGainCorrection(TString filename, bool DEBUG = false);
  //! Get Gain Correction "component" for the vanum VA of the tdrnum TDR for the jinfnum JINF
  static float GetGainCorrectionPar(int jinfnum, int tdrnum, int vanum, int component);

  void ExcludeTDRFromTrack(int jinfnum, int tdrnum, int side,
                           bool verbose = true); // to be called just one, before event loop, or, for a "temporary ban"
                                                 // call, afterwards, 'IncludeBack'
  void IncludeBackTDRFromTrack(int jinfnum, int tdrnum, int side,
                               bool verbose = true); // to be called after 'Exclude' for a "temporary ban"
  bool FindTrackAndFit(int nptsS, int nptsK, bool verbose = false);
  bool FindHigherChargeTrackAndFit(int nptsS, double threshS, int nptsK, double threshK, bool verbose = false);
  double RefineTrack(double nsigmaS = 5.0, int nptsS = 3, double nsigmaK = 5.0, int nptsK = 3, bool verbose = false);
  double GetThetaTrack() { return _theta; };
  double GetPhiTrack() { return _phi; };
  double GetS0Track() { return _S0; };
  double GetK0Track() { return _K0; };
  double GetChiTrack() { return _chisq; };
  double GetChiTrackS() { return _chisqS; };
  double GetChiTrackK() { return _chisqK; };
  double ExtrapolateTrack(double z, int component);
  bool IsClusterUsedInTrack(int index_cluster);
  inline unsigned int GetTrackHitPattern(int side, int jinfnum = 0) {
    return _track_cluster_pattern[jinfnum][side];
  }; // is in binary format! Use std::bitset to show!
  bool IsTDRInTrack(int side, int tdrnum, int jinfnum = 0);
  inline std::vector<std::pair<int, std::pair<int, int>>> GetHitVector() { return _v_trackhit; }
  inline unsigned int GetNHitsTrack() { return (unsigned int)(_v_trackhit.size()); }
  inline unsigned int GetNHitsSTrack() { return (unsigned int)(_v_trackS.size()); }
  inline unsigned int GetNHitsKTrack() { return (unsigned int)(_v_trackK.size()); }
  double GetChargeTrack(int side);

  double GetCalPed_PosNum(int tdrposnum, int channel, int Jinfnum = 0);
  double GetCalSigma_PosNum(int tdrposnum, int channel, int Jinfnum = 0);
  double GetRawSignal_PosNum(int tdrposnum, int channel, int Jinfnum = 0);
  double GetCN_PosNum(int tdrposnum, int va, int Jinfnum = 0);
  float GetRawSoN_PosNum(int tdrposnum, int channel, int Jinfnum = 0);
  double GetCalStatus_PosNum(int tdrposnum, int va, int Jinfnum = 0);

  double GetCalPed(RHClass *rh, int tdrnum, int channel, int Jinfnum = 0);
  double GetCalSigma(RHClass *rh, int tdrnum, int channel, int Jinfnum = 0);
  double GetRawSignal(RHClass *rh, int tdrnum, int channel, int Jinfnum = 0);
  double GetCN(RHClass *rh, int tdrnum, int va, int Jinfnum = 0);
  float GetRawSoN(RHClass *rh, int tdrnum, int channel, int Jinfnum = 0);
  double GetCalStatus(RHClass *rh, int tdrposnum, int va, int Jinfnum = 0);
  // CB:
  bool FindTracksAndVertex(bool vertex = false);
  std::pair<double, double> GetVertexK();
  std::pair<double, double> GetVertexS();
  trackColl *GetTracks(int);
  int GetNTracks() { return _NTrks; };
  void RecombineXY(double);
  // TH2F* h1;

private:
  static bool ladderconfnotread;
  static LadderConf *ladderconf;
  static bool alignmentnotread;
  static TdrArray<float[3]> alignpar;
  static TdrArray<bool> multflip;

  static bool gaincorrectionnotread;
  static VAArray<float[2]> gaincorrectionpar;

  double CombinatorialFit(std::vector<std::pair<int, std::pair<double, double>>> **v_cog_laddS,
                          std::vector<std::pair<int, std::pair<double, double>>> **v_cog_laddK, int ijinf, int itdr,
                          std::vector<std::pair<int, std::pair<double, double>>> v_cog_trackS,
                          std::vector<std::pair<int, std::pair<double, double>>> v_cog_trackK, int nptsS, int nptsK,
                          bool verbose = false);
  double SingleFit(std::vector<std::pair<int, std::pair<double, double>>> vS,
                   std::vector<std::pair<int, std::pair<double, double>>> vK, std::vector<double> &v_chilayS,
                   std::vector<double> &v_chilayK, double &theta, double &thetaerr, double &phi, double &phierr,
                   double &iDirS, double &iDirSerr, double &iDirK, double &iDirKerr, double &iDirZ, double &iDirZerr,
                   double &mS, double &mSerr, double &mK, double &mKerr, double &S0, double &S0err, double &K0,
                   double &K0err, double &chisqS, double &chisqK, bool verbose = false);
  double SingleFit(std::vector<std::pair<int, std::pair<double, double>>> vS,
                   std::vector<std::pair<int, std::pair<double, double>>> vK, bool verbose = false);
  void AssignAsBestTrackFit();
  void ClearTrack();
  void ClearTrack_sf();

  void StoreTrackClusterPatterns();
  void FillHitVector();
  //------------CB:metodi privati che vengono usati da FindTracksAndVertex()------------//
  void Track(std::vector<std::pair<int, std::pair<double, double>>> &hits,
             std::vector<std::pair<int, std::pair<double, double>>> &rejects);
  std::pair<double, double> Hough(std::vector<std::pair<int, std::pair<double, double>>> &vec);
  std::vector<Hit> CleanTrack(std::vector<Hit> &hits);

  // for variable length arrays we need to "pass" the size (another data member, that cannot be static, look NJINF vs
  // _NINFS, for example) cfr. https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html#inputoutput
  // 11.3.4 Variable length array

  //! Progressive Event number
  int Evtnum{0};
  //! Jinj Status
  int JINJStatus{0};
  //! Jinf Status
  JArray<int> JINFStatus{0};
  //! Status word for the TDRs (  TDRStatus & 0x1f == TDR ID)
  TdrArray<int> TDRStatus{{0}};
  //! Common Noise from Calibration
  VAArray<double> CNoise{{{0}}};
  //! Cluster number for (side 0(S) 1(K))
  TdrArray<int[2]> NClus{{{0}}};
  //! Total number of clusters
  int NClusTot{0};
  //! 0 if there are hits on all the ladders
  int notgood{0};
  //! (TClones) Array of the recontructed clusters
  TClonesArray *Cls = nullptr;
  //! (TClones) Array of the recontructed hits

  //! pointer to the data (filled just when reading RAW data) and just for the first 8 TRDs (even when there're more
  //! Jinf's)
  // was 8 since more than 8 raw TDRs cannot be read by a single Jinf
  // in the MC, instead, can be long as we want
  // Viviana  changed to [NTDRS] what about CH 1024->4096
  // NCHA=NVAS*NCHAVA
  // TODO: check how much space we waste [MD]
  ChArray<double> CalSigma{{{0}}};
  ChArray<double> CalPed{{{0}}};
  ChArray<short> RawSignal{{{0}}};
  ChArray<float> RawSoN{{{0}}}; //! (do not stream on file! Can be recomputed easily!)
  ChArray<int> CalStatus{{{0}}};
  TdrArray<short> ReadTDR{{0}};

  //------------CB:qui salvo gli output di FindTracksAndVertex()------------//
  int _NTrks;
  trackColl _TrS;
  trackColl _TrK;
  std::pair<double, double> _vertexK;
  std::pair<double, double> _vertexS;

  // track parameters and points
  double _chisq;    //!
  double _chisqS;   //!
  double _chisqK;   //!
  double _mS;       //! angular coefficient in the S-Z view (mX = iDirS/iDirZ)
  double _mK;       //! angular coefficient in the K-Z view (mX = iDirK/iDirZ)
  double _mSerr;    //!
  double _mKerr;    //!
  double _iDirS;    //! cosine director S
  double _iDirK;    //! cosine director K
  double _iDirZ;    //! cosine director Z (DirS*DirS+DirK*DirK+DirZ*DirZ=1)
  double _iDirSerr; //!
  double _iDirKerr; //!
  double _iDirZerr; //!
  double _theta;    //!
  double _phi;      //!
  double _thetaerr; //!
  double _phierr;   //!
  double _S0;       //!
  double _K0;       //!
  double _S0err;    //!
  double _K0err;    //!
  std::vector<std::pair<int, std::pair<double, double>>> _v_trackS; //!
  std::vector<std::pair<int, std::pair<double, double>>> _v_trackK; //!
  std::vector<double> _v_chilayS;                                   //!
  std::vector<double> _v_chilayK;                                   //!
  //! filled by FillHitVector(). Here the int is the ladder number and the second pair is <cluster index S, cluster
  //! index K>
  std::vector<std::pair<int, std::pair<int, int>>> _v_trackhit; //!
  //! filled by StoreTrackClusterPatterns()
  JArray<unsigned int[2]> _track_cluster_pattern{{0}}; //!

  // int is jinfnum*100+tdrnum
  std::vector<int> _v_ladderS_to_ignore; //!
  std::vector<int> _v_ladderK_to_ignore; //!

  ClassDef(GenericEvent, 1)
};

#endif