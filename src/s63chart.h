/******************************************************************************
 * Project:  OpenCPN
 * Purpose:  S63 Plugin Chart
 * Author:   David Register
 *******************************************************************************/


#ifndef _S63CH_H_
#define _S53CH_H_

#include "wx/wxprec.h"
#include <wx/wfstream.h>


#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include <wx/process.h>
#include "wx/socket.h"


class PI_VE_Element;
class PI_VC_Element;

WX_DECLARE_HASH_MAP( int, PI_VE_Element *, wxIntegerHash, wxIntegerEqual, PI_VE_Hash );
WX_DECLARE_HASH_MAP( int, PI_VC_Element *, wxIntegerHash, wxIntegerEqual, PI_VC_Hash );

#ifndef PI
#define PI        3.1415926535897931160E0      /* pi */
#endif


enum
{
    BUILD_SENC_OK,
    BUILD_SENC_NOK_RETRY,
    BUILD_SENC_NOK_PERMANENT
};

typedef struct  {
    double x;
    double y;
} MyPoint;


extern "C" int G_PtInPolygon(MyPoint *, int, float, float) ;

#ifndef BPP
#define BPP 24
#endif

#if 0
//----------------------------------------------------------------------------------
//      SENC Server Process container
//----------------------------------------------------------------------------------

class ServerProcess: public wxProcess
{
public:
    ServerProcess();
    ~ServerProcess();
    
    void OnTerminate(int pid, int status);
    wxString    m_outstring;
    bool        term_happened;
    
};
#endif

#if 0
//----------------------------------------------------------------------------------
//      SENC Client
//----------------------------------------------------------------------------------
#define SOCKET_ID             5001
#define DS_SERVERSOCKET_ID       5002
#define DS_ACTIVESERVERSOCKET_ID 5003

class SENCclient: public wxInputStream
{
public:
    SENCclient(const wxString &senc_file_name);
    ~SENCclient();
    
    int Open(void);
    void Close();
    wxString GetServerOutput();
    
    int reset(void);
    int NetRead( void *destination, size_t length, size_t *read_actual);
    //    int UnRead( char *destination, int length);
    //    int fgets( char *destination, int max_length);
    
    // Over ride methods from the base class
    size_t OnSysRead(void *buffer, size_t size);
    bool Eof() const;
    
    wxString m_senc_file;
    
    wxSocketClient      *m_sock;
    bool m_eof;
    
    ServerProcess       *m_sproc;
    long                m_server_pid;
    bool                m_OK;
    
};

#endif


// ----------------------------------------------------------------------------
// ChartS63 Definition
// ----------------------------------------------------------------------------

class  ChartS63 : public PlugInChartBaseGL
{
      DECLARE_DYNAMIC_CLASS(ChartS63)

    public:
      //    Public methods

      ChartS63();
      virtual ~ChartS63();

//      void ChartBaseBSBCTOR(void);
//      void ChartBaseBSBDTOR(void);
      wxString GetFileSearchMask(void);

      //    Accessors

      virtual wxBitmap *GetThumbnail(int tnx, int tny, int cs);

      double GetNormalScaleMin(double canvas_scale_factor, bool b_allow_overzoom);
      double GetNormalScaleMax(double canvas_scale_factor, int canvas_width);


      virtual int Init( const wxString& name, int init_flags );

      virtual int latlong_to_pix_vp(double lat, double lon, int &pixx, int &pixy, PlugIn_ViewPort& vp);
      virtual int vp_pix_to_latlong(PlugIn_ViewPort& vp, int pixx, int pixy, double *lat, double *lon);
      virtual void latlong_to_chartpix(double lat, double lon, double &pixx, double &pixy);

      wxBitmap &RenderRegionView(const PlugIn_ViewPort& VPoint, const wxRegion &Region);
      bool RenderViewOnDC(wxMemoryDC& dc, const PlugIn_ViewPort& VPoint);
 
      int RenderRegionViewOnGL( const wxGLContext &glc, const PlugIn_ViewPort& VPoint,
                                const wxRegion &Region, bool b_use_stencil );
      

      virtual bool AdjustVP(PlugIn_ViewPort &vp_last, PlugIn_ViewPort &vp_proposed);
      virtual double GetNearestPreferredScalePPM(double target_scale_ppm);

      virtual bool IsRenderDelta(PlugIn_ViewPort &vp_last, PlugIn_ViewPort &vp_proposed);

      void GetValidCanvasRegion(const PlugIn_ViewPort& VPoint, wxRegion  *pValidRegion);

      virtual bool GetChartExtent(ExtentPI *pext);

      void SetColorScheme(int cs, bool bApplyImmediate);

      void ComputeSourceRectangle(const PlugIn_ViewPort &vp, wxRect *pSourceRect);
      wxRect GetSourceRect();

      wxImage *GetImage();

      double GetRasterScaleFactor();

#if 0
      int GetCOVREntries();
      int GetCOVRTablePoints(int iTable);
      int  GetCOVRTablenPoints(int iTable);
      float *GetCOVRTableHead(int iTable);
#endif

      int GetCOVREntries(){ return  m_nCOVREntries; }
      int GetCOVRTablePoints(int iTable) { return m_pCOVRTablePoints[iTable]; }
      int  GetCOVRTablenPoints(int iTable){ return m_pCOVRTablePoints[iTable]; }
      float *GetCOVRTableHead(int iTable){ return m_pCOVRTable[iTable]; }

      int GetNoCOVREntries(){ return  m_nNoCOVREntries; }
      int GetNoCOVRTablePoints(int iTable) { return m_pNoCOVRTablePoints[iTable]; }
      int  GetNoCOVRTablenPoints(int iTable){ return m_pNoCOVRTablePoints[iTable]; }
      float *GetNoCOVRTableHead(int iTable){ return m_pNoCOVRTable[iTable]; }

      int GetNativeScale(){ return m_Chart_Scale;}
      
      ListOfPI_S57Obj *GetObjRuleListAtLatLon(float lat, float lon, float select_radius,
                                                                 PlugIn_ViewPort *VPoint);
      wxString CreateObjDescriptions( ListOfPI_S57Obj* obj_list );
      wxString GetObjectAttributeValueAsString( PI_S57Obj *obj, int iatt, wxString curAttrName );
      wxString GetAttributeDecode( wxString& att, int ival );
      

      wxString          m_extended_error;
      
      struct _pi_chart_context     *m_this_chart_context;

      
protected:
//    Methods
      bool              InitFrom_ehdr( wxString &efn );
      PI_InitReturn     FindOrCreateSenc( const wxString& name );
      int               BuildSENCFile( const wxString& FullPath_os63, const wxString& SENCFileName );
      int               BuildRAZFromSENCFile( const wxString& FullPath );
      int               _insertRules(PI_S57Obj *obj);
      PI_InitReturn     PostInit( int flags, int cs );
      void              SetVPParms(const PlugIn_ViewPort &vpt);
      void              ResetPointBBoxes(const PlugIn_ViewPort &vp_last, const PlugIn_ViewPort &vp_this);
      void              SetLinePriorities(void);
      
        // Rendering
      bool DoRenderViewOnDC(wxMemoryDC& dc, const PlugIn_ViewPort& VPoint, bool force_new_view);
      bool DoRenderRegionViewOnDC(wxMemoryDC& dc, const PlugIn_ViewPort& VPoint, const wxRegion &Region, bool b_overlay);
      int DCRenderRect(wxMemoryDC& dcinput, const PlugIn_ViewPort& vp, wxRect *rect);
      bool DCRenderLPB(wxMemoryDC& dcinput, const PlugIn_ViewPort& vp, wxRect* rect);
      wxBitmap *GetCloneBitmap();
      bool IsCacheValid(){ return (pDIB != NULL); }
      void InvalidateCache();

      void SetClipRegionGL( const wxGLContext &glc, const PlugIn_ViewPort& VPoint,
                            const wxRegion &Region, bool b_render_nodta, bool b_useStencil );
      void SetClipRegionGL( const wxGLContext &glc, const PlugIn_ViewPort& VPoint, const wxRect &Rect,
                            bool b_render_nodta, bool b_useStencil );
      bool DoRenderRectOnGL( const wxGLContext &glc, const PlugIn_ViewPort& VPoint, wxRect &rect, bool b_useStencil );
      
      void UpdateLUPsOnStateChange( void );
      void ClearRenderedTextCache();
      
      //  Query
      bool DoesLatLonSelectObject( float lat, float lon, float select_radius, PI_S57Obj *obj );
      bool IsPointInObjArea( float lat, float lon, float select_radius, PI_S57Obj *obj );
      
      int               my_fgets( char *buf, int buf_len_max, wxInputStream &ifs );

      int               m_global_color_scheme;
      wxBitmap          *m_pBMPThumb;

      double            m_ppm_avg;

      wxRegion          m_last_Region;
      wxString          m_lastColorScheme;
      wxRect            m_last_vprect;
      
      wxBitmap          *m_pCloneBM;
      wxMask            *m_pMask;
      
      PlugIn_ViewPort    m_last_vp;
//      PixelCache        *pDIB;
      wxBitmap          *pDIB;
      
      //  SM Projection parms, stored as convenience to expedite pixel conversions
      double    m_easting_vp_center, m_northing_vp_center;
      double    m_pixx_vp_center, m_pixy_vp_center;
      double    m_view_scale_ppm;

      int               m_plib_state_hash;
      bool              m_bLinePrioritySet;
      
      long              m_sync_cmd_pid;
      ExtentPI          m_FullExtent;
      bool              m_bExtentSet;

      wxFileName        m_SENCFileName;
      wxString          m_senc_dir;
      
      wxString          m_full_base_path;               // From metadata (os63) file
      wxString          m_cell_permit;

      //    Chart region coverage information
      //    Charts may have multiple valid regions within the lat/lon box described by the chart extent
      //    The following table structure contains this embedded information

      //    ENC charts often contain multiple entries

      int         m_nCOVREntries;                       // number of coverage table entries
      int         *m_pCOVRTablePoints;                  // int table of number of points in each coverage table entry
      float       **m_pCOVRTable;                       // table of pointers to list of floats describing valid COVR

      int         m_nNoCOVREntries;                       // number of NoCoverage table entries
      int         *m_pNoCOVRTablePoints;                  // int table of number of points in each NoCoverage table entry
      float       **m_pNoCOVRTable;                       // table of pointers to list of floats describing valid NOCOVR

      double      m_ref_lat;
      double      m_ref_lon;
      
      PI_VE_Hash     m_ve_hash;
      PI_VC_Hash     m_vc_hash;
      
      PI_S57Obj *razRules[PI_PRIO_NUM][PI_LUPNAME_NUM];
      
      //  Object arrays used by S52PLIB TOPMAR rendering logic
      wxArrayPtrVoid *pFloatingATONArray;
      wxArrayPtrVoid *pRigidATONArray;
      
      wxDateTime  m_date000;                    // extracted from DSID:ISDT
      wxString    m_edtn000;                    // extracted from DSID:EDTN
      
      
      

};

class PI_S57ObjX : public PI_S57Obj
{
public:
    
    //  Public Methods
    PI_S57ObjX();
    ~PI_S57ObjX();
    PI_S57ObjX(char *first_line, wxInputStream *scl);
    
    //      wxString GetAttrValueAsString ( char *attr );
    //      int GetAttributeIndex( const char *AttrSeek );
    wxString GetAttrValueAsString( const char *AttrName );
    
    // Private Methods
private:
    bool IsUsefulAttribute(char *buf);
    //      int my_fgets( char *buf, int buf_len_max, wxInputStream& ifs );
    int my_bufgetlx( char *ib_read, char *ib_end, char *buf, int buf_len_max );
    
    int GetAttributeIndex( const char *AttrSeek );
    
};






//----------------------------------------------------------------------------------
//          Classes used to create arrays of geometry elements
//----------------------------------------------------------------------------------

class PI_VE_Element
{
public:
    int         index;
    int         nCount;
    double      *pPoints;
    int         max_priority;
};

class PI_VC_Element
{
public:
    int         index;
    double      *pPoint;
};


WX_DECLARE_OBJARRAY(PI_VE_Element, PI_ArrayOfVE_Elements);
WX_DECLARE_OBJARRAY(PI_VC_Element, PI_ArrayOfVC_Elements);

//----------------------------------------------------------------------------------
//      SENC Server Process container
//----------------------------------------------------------------------------------

class ServerProcess: public wxProcess
{
public:
    ServerProcess();
    ~ServerProcess();
    
    void OnTerminate(int pid, int status);
    wxString    m_outstring;
    bool        term_happened;
    
};


//----------------------------------------------------------------------------------
//      SENC Client
//----------------------------------------------------------------------------------
#define SOCKET_ID             5001
#define DS_SERVERSOCKET_ID       5002
#define DS_ACTIVESERVERSOCKET_ID 5003

class SENCclient: public wxInputStream
{
public:
    SENCclient();
    ~SENCclient();
    
    void Attach( const wxString& FullPath );
    int Open(void);
    void Close();
    wxString GetServerOutput();
    
    int reset(void);
    int NetRead( void *destination, size_t length, size_t *read_actual);
    //    int UnRead( char *destination, int length);
    //    int fgets( char *destination, int max_length);
    
    // Over ride methods from the base class
    size_t OnSysRead(void *buffer, size_t size);
    bool Eof() const;
    
    wxString m_senc_file;
    
    wxSocketClient      *m_sock;
    bool                m_private_eof;
    
    ServerProcess       *m_sproc;
    long                m_server_pid;
    bool                m_OK;
    
};

#endif




