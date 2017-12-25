#include "filter_blend.h"
extern "C" {
#include "libavutil/imgutils.h"
#include "libavutil/eval.h"
#include "libavutil/opt.h"
#include "libavutil/pixfmt.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/bufferqueue.h"
#include "libavfilter/formats.h"
#include "libavfilter/internal.h"
#include "libavfilter/blend.h"
#include "libavutil/error.h"
}
#include <QDebug>

#define TOP    0
#define BOTTOM 1

// storm
#if 1
typedef int (*FuncDef)(const uint8_t *top, ptrdiff_t top_linesize,          
        const uint8_t *bottom, ptrdiff_t bottom_linesize,    
        uint8_t *dst, ptrdiff_t dst_linesize,                
        ptrdiff_t width, ptrdiff_t height,              
        FilterParams *param, double *values, int starty);
#endif
#if 0
typedef int (*FuncDef)(const uint8_t *top, ptrdiff_t top_linesize,          
        const uint8_t *bottom, ptrdiff_t bottom_linesize,    
        uint8_t *dst, ptrdiff_t dst_linesize,                
        ptrdiff_t width, ptrdiff_t height,              
        FilterParams *param, double *values);
#endif
typedef struct BlendFuncDef {
    FuncDef func_arg;
    /*int (*func_arg)(const uint8_t *top, ptrdiff_t top_linesize,          
                               const uint8_t *bottom, ptrdiff_t bottom_linesize,    
                               uint8_t *dst, ptrdiff_t dst_linesize,                
                               ptrdiff_t width, ptrdiff_t height,              
                               FilterParams *param, double *values);*/
}BlendFuncDef;
//1. (all 3)
static const char *const var_names[] = {   "X",   "Y",   "W",   "H",   "SW",   "SH",   "T",   "N",   "A",   "B",   "TOP",   "BOTTOM"
    ,   "bowtieh"
    ,   "bowtiev"
    ,   "diagboxout"
    ,   "diagcrossout"
    ,   "diagdownright"
    ,   "filledvdown"
    ,   "filledvleft"
    ,   "filledvright"
    ,   "filledvup"
    ,   "barsh"
    ,   "barsvertical"
    ,   "crossfade"
    ,   "checkerb"
    ,   "circle"
    ,   "circles"
    ,   "diamond"
    ,   "heart"
    ,   "rectangle"
    ,   "wheel"
    ,   "insetbottoml"
    ,   "insetbottomr"
    ,   "insettopleft"
    ,   "insettopr"
    ,   "iris"
    ,   "revealdown"
    ,   "revealright"
    ,   "revealleft"
    ,   "roll"
    ,   "slide"
    ,   "slideupt"
    ,   "slideleftt"
    ,   "slidedownt"
    ,   "splith"
    ,   "splitv"
    ,   "fanin"
    ,   "test"
    , NULL };
//2. (all 3)
enum                                   { VAR_X, VAR_Y, VAR_W, VAR_H, VAR_SW, VAR_SH, VAR_T, VAR_N, VAR_A, VAR_B, VAR_TOP, VAR_BOTTOM
    , VAR_BOWTIEH
    , VAR_BOWTIEV
    , VAR_DIAGBOXOUT
    , VAR_DIAGCROSSOUT
    , VAR_DIAGDOWNRIGHT
    , VAR_FILLEDVDOWN
    , VAR_FILLEDVLEFT
    , VAR_FILLEDVRIGHT
    , VAR_FILLEDVUP
    , VAR_BARSH
    , VAR_BARSVERTICAL
    , VAR_CROSSFADE
    , VAR_CHECKERB
    , VAR_CIRCLE
    , VAR_CIRCLES
    , VAR_DIAMOND
    , VAR_HEART
    , VAR_RECTANGLE
    , VAR_WHEEL
    , VAR_INSETBOTTOML
    , VAR_INSETBOTTOMR
    , VAR_INSETTOPLEFT
    , VAR_INSETTOPR
    , VAR_IRIS
    , VAR_REVEALDOWN
    , VAR_REVEALRIGHT
    , VAR_REVEALLEFT
    , VAR_ROLL
    , VAR_SLIDE
    , VAR_SLIDEUPT
    , VAR_SLIDELEFTT
    , VAR_SLIDEDOWNT
    , VAR_SPLITH
    , VAR_SPLITV
    , VAR_FANIN
    , VAR_TEST
    , VAR_VARS_NB };
#if 0
#define DEFINE_ANIMATION_EXPR(type, name, div, expr_before_xexchange, expr_before_expr, expr)\
static inline int animation_expr_## name(const uint8_t *_top, ptrdiff_t top_linesize,          \
                               const uint8_t *_bottom, ptrdiff_t bottom_linesize,    \
                               uint8_t *_dst, ptrdiff_t dst_linesize,                \
                               ptrdiff_t width, ptrdiff_t height,              \
                               FilterParams *param, double *values, int /*starty*/)\
{\
    const type *top = (type*)_top;                                             \
    const type *bottom = (type*)_bottom;                                       \
    type *dst = (type*)_dst;                                                   \
    AVExpr *e = param->e;                                                      \
    int y, x;                                                                  \
    double W=values[VAR_W]; \
    double T=values[VAR_T]; \
    double N=values[VAR_N]; \
    double H=values[VAR_H]; \
    double f, tant, xpartlen=0, ypartlen=0, change_rate, xchange, ychange, wh, whalf, hhalf; \
    double tmp1=0, tmp2=0, tmp3=0; \
    double t=e->param[1]->value;\
    double at=e->param[2]->value;\
    t = t <= at? t:at;\
    f = 25; if(f){} \
    tant = tan(N*M_PI/(2*f*t));if(tant){} \
    change_rate=N/(t*f); if(change_rate){} \
    if(xchange){} if(ychange){} \
    wh = W/H; \
    whalf = W/2; hhalf = H/2; \
    if(W){} if(T){} if(N){} if(H){} if(xpartlen){} if(ypartlen){} if(wh){} if(whalf){} if(hhalf){} \
    if(tmp1){} if(tmp2){} if(tmp3){} \
\
    expr_before_xexchange; \
    xchange=xpartlen*change_rate; \
    ychange=ypartlen*change_rate; \
    for (y = 0; y < height; y++) {                                             \
        for (x = 0; x < width; x++) {                                          \
            expr_before_expr; \
            dst[x] = expr; \
        }\
        dst    += dst_linesize;                                                \
        top    += top_linesize;                                                \
        bottom += bottom_linesize;                                             \
    }\
    return 0;\
}
#endif
#define DEFINE_ANIMATION_EXPR(type, name, div, expr_before_xexchange, expr_before_expr, expr)\
static inline int animation_expr_## name(const uint8_t *_top, ptrdiff_t top_linesize,          \
                               const uint8_t *_bottom, ptrdiff_t bottom_linesize,    \
                               uint8_t *_dst, ptrdiff_t dst_linesize,                \
                               ptrdiff_t width, ptrdiff_t height,              \
                               FilterParams *param, double *values, int /*starty*/)\
{\
    const type *top = (type*)_top;                                             \
    const type *bottom = (type*)_bottom;                                       \
    type *dst = (type*)_dst;                                                   \
    AVExpr *e = param->e;                                                      \
    int y, x;                                                                  \
    double W=values[VAR_W]; \
    double T=values[VAR_T]; \
    double N=values[VAR_N]; \
    double H=values[VAR_H]; \
    double f, tant, xpartlen=0, ypartlen=0, change_rate, xchange, ychange, wh, whalf, hhalf; \
    double tmp1=0, tmp2=0, tmp3=0; \
    double t=e->param[1]->value;\
    double at=e->param[2]->value;\
    double tn=e->param[1]->value;\
    t = t <= at? t:at;\
    f = 25; if(f){} \
    tant = tan(N*M_PI/(2*tn));if(tant){} \
    change_rate=N/(tn); if(change_rate){} \
    if(xchange){} if(ychange){} \
    wh = W/H; \
    whalf = W/2; hhalf = H/2; \
    if(W){} if(T){} if(N){} if(H){} if(xpartlen){} if(ypartlen){} if(wh){} if(whalf){} if(hhalf){} \
    if(tmp1){} if(tmp2){} if(tmp3){} \
\
    expr_before_xexchange; \
    xchange=xpartlen*change_rate; \
    ychange=ypartlen*change_rate; \
    for (y = 0; y < height; y++) {                                             \
        for (x = 0; x < width; x++) {                                          \
            expr_before_expr; \
            dst[x] = expr; \
        }\
        dst    += dst_linesize;                                                \
        top    += top_linesize;                                                \
        bottom += bottom_linesize;                                             \
    }\
    return 0;\
}
//#define DEFINE_ANIMATION_EXPR(uint8_t, name, 1, expr_before_xexchange, expr_before_expr, expr)
static inline int animation_expr_test1(const uint8_t *_top, ptrdiff_t top_linesize,          
                               const uint8_t *_bottom, ptrdiff_t bottom_linesize,    
                               uint8_t *_dst, ptrdiff_t dst_linesize,                
                               ptrdiff_t width, ptrdiff_t height,              
                               FilterParams *param, double *values)
{
    const uint8_t *top = (uint8_t*)_top;                                             
    const uint8_t *bottom = (uint8_t*)_bottom;                                       
    uint8_t *dst = (uint8_t*)_dst;                                                   
    AVExpr *e = param->e;                                                      
    int y, x;                                                                  
    double W=values[VAR_W]; 
    double T=values[VAR_T]; 
    double N=values[VAR_N]; 
    double H=values[VAR_H]; 
    double f, tant, xpartlen=0, ypartlen=0, change_rate, xchange, ychange, wh, whalf, hhalf; 
    double tmp1=0, tmp2=0; 
    double t=e->param[1]->value;
    double at=e->param[2]->value;
    t = t <= at? t:at;
    f = 25; if(f){} 
    tant = tan(N*M_PI/(2*f*t));if(tant){} 
    change_rate=N/(t*f); if(change_rate){} 
    if(xchange){} if(ychange){} 
    wh = W/H; 
    whalf = W/2; hhalf = H/2; 
    if(W){} if(T){} if(N){} if(H){} if(xpartlen){} if(ypartlen){} if(wh){} if(whalf){} if(hhalf){} 
    if(tmp1){} if(tmp2){} 

    xpartlen=M_PI*90/180,xchange=xpartlen*N/(t*f),tmp1=tan(xchange);
    xchange=xpartlen*change_rate; 
    ychange=ypartlen*change_rate; 
    for (y = 0; y < height; y++) {                                             
        for (x = 0; x < width; x++) {                                          
            if (!(tmp1>=(H-y)/(W-x) || x<=(H-W*tmp1-y)*tmp1)) {
                av_log(NULL, AV_LOG_VERBOSE, "y: %d x: %d\n",(int)(H-(H-y)*cos(xchange)-(W-x)*sin(xchange)), (int)(W-(W-x)*cos(xchange)+(H-y)*sin(xchange)) );} 
            dst[x] =((tmp1>=(H-y)/(W-x) || x<=(H-W*tmp1-y)*tmp1)? bottom[x]:(*(top+322*top_linesize+503))  ) ; 
        }
        dst    += dst_linesize;                                                
        top    += top_linesize;                                                
        bottom += bottom_linesize;                                             
    }
    return 0;
}

// diagonals
DEFINE_ANIMATION_EXPR(uint8_t, bowtieh, 1, , , ( ((x<=tant*y && x<=tant*(H-y))||(x>=(W-y*tant) && x>=(W-H*tant+y*tant)) || tant<0)?bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, bowtiev, 1, , , ( ( (y<=x*tant && y<=(W-x)*tant) || (y>=H-x*tant && y>=(H-W*tant+x*tant) ) || tant<0 )?bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, diagboxout, 1, xpartlen=W/2, , ( ( (x<=whalf && ( x<=(whalf-xchange-y*wh) || (y<=hhalf && x>=(xchange+whalf-wh*y)) || x<=(y*wh-xchange-whalf) || (y>=hhalf && x>=(xchange+wh*y-whalf)) )) || (x>=whalf && (x>=(xchange+whalf+y*wh) || (y<=hhalf && x<=(whalf-xchange+y*wh) ) || x>=(3*whalf+xchange-wh*y) || (y>=hhalf && x<=(3*whalf-xchange-wh*y)) ) ) )? top[x]:bottom[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, diagcrossout, 1, xpartlen=W/2, ,( (  (x<=whalf && (x>=wh*y+xchange || x>=(W-wh*y+xchange) || (y<=hhalf && x<=(y*wh-xchange)) || (y>=hhalf && x<=(W-y*wh-xchange)) )) || (x>=whalf && (x<=(W-xchange-y*wh) || x<=W*y/H-xchange || (y<=hhalf && x>=(W-y*wh+xchange)) || (y>=hhalf && x>=(y*wh+xchange)) ) ) )?top[x]:bottom[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, diagdownright, 1, xpartlen=W+H, ,( (x<=xchange-y)? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, filledvdown, 1, xpartlen=(W+H)/2 , , ( ((x>=whalf && x<=(whalf+xchange-y*wh/2)) || (x<=whalf && x>=(wh*y/2-xchange+whalf)) )? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, filledvleft, 1, xpartlen=W/2, , ( (( y<=hhalf && x>=(3*whalf-wh*y-3*xchange))|| (y>=hhalf && x>=(whalf+wh*y-3*xchange) ) )? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, filledvright, 1, xpartlen=W*3/2, , ( ( abs(hhalf-y)<=H*(xchange-x)/W ) ? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, filledvup, 1, xpartlen=W*3/2, , ( ( abs(whalf-x)<=xchange-W+y*W/H) ? bottom[x]:top[x] ) )
// dissolves
DEFINE_ANIMATION_EXPR(uint8_t, barsh, 1, ypartlen=H/54 , ,((  y%((int)(ypartlen))>=0 && y%((int)(ypartlen))<=ychange)? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, barsvertical, 1, xpartlen=W/72, ,((  x%((int)(xpartlen))>=0 && x%((int)(xpartlen))<=xchange)? bottom[x]:top[x] ) )
//DEFINE_ANIMATION_EXPR(uint8_t, crossfade, 1, , , (bottom[x]*(T>=t?1:(T/t))+top[x]*(1-(T>=t?1:(T/t)))) )
DEFINE_ANIMATION_EXPR(uint8_t, crossfade, 1, , , (bottom[x]*(N>=tn?1:(N/tn))+top[x]*(1-(N>=tn?1:(N/tn)))) )
// patterns and shapes
DEFINE_ANIMATION_EXPR(uint8_t, checkerb, 1, (xpartlen=W/12,ypartlen=H/18) , ,(( (((int)floor(y/ypartlen))%2==0 && x%((int)xpartlen)>=0 && x%((int)xpartlen)<=xchange) ||(((int)floor(y/ypartlen))%2==1 &&  ((((int)x)%((int)xpartlen)>=xpartlen/2 && ((int)x)%((int)xpartlen)<=xchange+xpartlen/2) || ( xchange>=xpartlen/2 && ((int)x)%((int)xpartlen)<=xchange-xpartlen/2) ) ) )? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, circle, 1, xpartlen=W/2, ,((( (x-whalf)*(x-whalf)+(y-hhalf)*(y-hhalf)<=xchange*xchange) )? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, circles, 1, (xpartlen=(W/5)/2, ypartlen=xpartlen*H/W), (tmp1=x-((int)(x/(2*xpartlen)))*2*xpartlen-xpartlen,tmp2=y-((int)(y/(2*ypartlen)))*2*ypartlen-ypartlen ),((tmp1*tmp1+tmp2*tmp2<=xchange*xchange )? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, diamond, 1, (xpartlen=W), ,(( (x<=whalf && ( (y<=hhalf && x>=W-y*W/H-xchange) ||(y>=hhalf && x>=y*W/H-xchange)) ) || (x>=whalf && (( y<=hhalf && x<=xchange+W*y/H) ||(y>=hhalf && x<=xchange-W*y/H+W)))  )? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, heart, 1, xpartlen=(W+H)/2, , (( (x<=whalf && ((y>=hhalf && x>=y-hhalf+whalf-xchange) || (y<=hhalf && x>=hhalf-y+whalf-xchange) || (y<=hhalf && x>=whalf-xchange/2 && x<=sqrt(xchange*xchange/4-y*y+y*H-y*xchange-hhalf*hhalf+hhalf*xchange)+whalf-xchange/2 ) || (y<=hhalf && x<=whalf-xchange/2 && x>=-sqrt(xchange*xchange/4-y*y+y*H-y*xchange-hhalf*hhalf+hhalf*xchange)+whalf-xchange/2)) ) || (x>=whalf && ((y>=hhalf && x<=whalf+xchange+hhalf-y) || (y<=hhalf && x<=whalf+xchange+y-hhalf) || (x>=whalf+xchange/2 && x<=sqrt(xchange*xchange/4-y*y+y*H-y*xchange-hhalf*hhalf+hhalf*xchange)+whalf+xchange/2 ) || (x<=whalf+xchange/2 && x>=-sqrt(xchange*xchange/4-y*y+y*H-y*xchange-hhalf*hhalf+hhalf*xchange)+whalf+xchange/2)  ) ) )? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, rectangle, 1, (xpartlen=W/2, ypartlen=H/2), ,(( abs(W/2-x)<=xchange && abs(H/2-y)<=ychange)? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, wheel, 1, (xpartlen=M_PI*90/180, xchange=xpartlen*N/(t*f),tmp1=tan(xchange)), ,( (T>=t || ( (((x>=whalf && y<hhalf) ||(x<=whalf && y>=hhalf)) && tmp1>=(whalf-x)/(y-hhalf))|| (((x<=whalf && y<hhalf) || (x>=whalf && y>hhalf)) && tmp1>=(y-hhalf)/(x-whalf)) || tmp1<0))? bottom[x]:top[x] ) )
// reveals
//insetbottomleft
DEFINE_ANIMATION_EXPR(uint8_t, insetbottoml, 1, (xpartlen=W, ypartlen=H), ,((W-xchange<=x && ychange>=y)? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, insetbottomr, 1, (xpartlen=W, ypartlen=H), ,((xchange>=x && ychange>=y)? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, insettopleft, 1, (xpartlen=W, ypartlen=H), ,((W-xchange<=x && H-ychange<=y)? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, insettopr, 1, (xpartlen=W, ypartlen=H), ,((xchange>=x && H-ychange<=y)? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, iris, 1, (xpartlen=whalf, ypartlen=hhalf), ,( (abs(y-hhalf)<=ychange || abs(x-whalf)<=xchange)? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, revealdown, 1, (ypartlen=H), ,( (y<=ychange)? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, revealright, 1, (xpartlen=W), ,( (x<=xchange)? bottom[x]:top[x] ) )
DEFINE_ANIMATION_EXPR(uint8_t, revealleft, 1,(xpartlen=W) , , ((x>=W-xchange)?bottom[x]:top[x]) )
//roll uncomplete
DEFINE_ANIMATION_EXPR(uint8_t, roll, 1, (xpartlen=M_PI*90/180,xchange=xpartlen*N/(tn),tmp1=tan(xchange),tmp2=sin(xchange), tmp3=cos(xchange)), ,((tmp1>=(H-y)/(W-x) || x<=(H-W*tmp1-y)*tmp1|| tmp1<=0)? bottom[x]:*(_top+(int)((H-y)*tmp3-(W-x)*tmp2)*top_linesize+(int)((W-x)*tmp3+(H-y)*tmp2))  ))
DEFINE_ANIMATION_EXPR(uint8_t, slide, 1, (ypartlen=H), ,((y>=H-ychange)? bottom[x]:(*(top+((int)ychange)*top_linesize+x) ) ))
DEFINE_ANIMATION_EXPR(uint8_t, slideupt, 1, (ypartlen=H), ,((y>=H-ychange)? (H>=ychange?(*(_bottom+((int)(y-(H-ychange)))*bottom_linesize+x)):bottom[x] ):(*(top+((int)ychange)*top_linesize+x) ) ))
DEFINE_ANIMATION_EXPR(uint8_t, slideleftt, 1, (xpartlen=W), ,((x>=W-xchange)? (W>=xchange?(*(_bottom+((int)y)*bottom_linesize+x-(int)(W-xchange))):bottom[x] ):(*(_top+((int)y)*top_linesize+x+(int)xchange) ) ))
//DEFINE_ANIMATION_EXPR(uint8_t, sliderightt, 1, (xpartlen=W), ,((x<xchange)? (W>=xchange?(*(_bottom+((int)y)*bottom_linesize+x-(int)(W-xchange))):bottom[x] ):(*(_top+((int)y)*top_linesize+x+(int)xchange) ) ))
DEFINE_ANIMATION_EXPR(uint8_t, slidedownt, 1, (ypartlen=H), ,((y<ychange)? (H>=ychange?(*(_bottom+((int)(H-ychange+y))*bottom_linesize+x)):bottom[x] ):(*(_top+((int)(y-ychange))*top_linesize+x) ) ))
DEFINE_ANIMATION_EXPR(uint8_t, splith, 1,(ypartlen=H/2) , , ((abs(y-hhalf)<=ychange)?bottom[x]:top[x]) )
DEFINE_ANIMATION_EXPR(uint8_t, splitv, 1,(xpartlen=W/2) , , ((abs(x-whalf)<=xchange)?bottom[x]:top[x]) )
// shatters
// sweeps and curls
DEFINE_ANIMATION_EXPR(uint8_t, fanin, 1, (xpartlen=M_PI*180/180,xchange=xpartlen*N/tn,tmp1=tan(xchange)), ,(( (x<=W/5 && (xchange>=xpartlen/2 || tmp1*abs(W/5-x) >=abs(hhalf-y))) || (x>=W*4/5 && (xchange>=xpartlen/2 || tmp1*(x-4*W/5)>=abs(hhalf-y))) || (x>=W/5 && x<=W*4/5 && xchange>=xpartlen/2 && (-tmp1*(x-W/5)<=abs(hhalf-y) || -tmp1*(4*W/5-x)<=abs(hhalf-y)) )  ) ? bottom[x]:top[x]  ))
// Wipes
// Cinematic
// Contemporary
DEFINE_ANIMATION_EXPR(uint8_t, test, 1, (xpartlen=M_PI*360/180,xchange=xpartlen*N/(t*f),tmp1=tan(xchange), tmp2=tan(xpartlen-xchange)), ,(( ( y<=H/2 && ((x>=W/5 && x<=W/2 && (xchange>=xpartlen/4 || tmp1>=(hhalf-y)/(x-W/5) )) || (x>=W/2 && x<=4*W/5 && ( xchange>=xpartlen/4 || tmp1>=(hhalf-y)/(4*W/5-x)) )|| (x<=W/5 && (xchange>=xpartlen/2 ||  (xchange>=xpartlen/4 && -tmp1<=(hhalf-y)/(W/5-x)) ) ) || (x>=4*W/5 && (xchange>=xpartlen/2 ||  (xchange>=xpartlen/4 && -tmp1<=(hhalf-y)/(x-4*W/5))) ) ) ) || (y>=hhalf && ( (x<=W/5 && (xchange>=3*xpartlen/4 || (xchange>=xpartlen/2 && tmp1>=(y-hhalf)/(W/5-x)) )) || (x>=4*W/5 && (xchange>=3*xpartlen/4 || (xchange>=xpartlen/2 && tmp1>=(y-hhalf)/(x-4*W/5))) ) || (x>=W/2 && x<=4*W/5 && (xchange>=xpartlen || (xchange>=3*xpartlen/4 && tmp2>=(y-hhalf)/(x-W/5) )) )  ) ) ) ? bottom[x]:top[x]  ))
//3. (all 3)
const BlendFuncDef blendfuncs[] = {
                                        {NULL}, {NULL},{NULL},{NULL},{NULL},{NULL},{NULL},{NULL},{NULL},{NULL},{NULL},  {NULL},
    {.func_arg = animation_expr_bowtieh},
    {.func_arg = animation_expr_bowtiev},
    {.func_arg = animation_expr_diagboxout},
    {.func_arg = animation_expr_diagcrossout},
    {.func_arg = animation_expr_diagdownright},
    {.func_arg = animation_expr_filledvdown},
    {.func_arg = animation_expr_filledvleft},
    {.func_arg = animation_expr_filledvright},
    {.func_arg = animation_expr_filledvup},
    {.func_arg = animation_expr_barsh},
    {.func_arg = animation_expr_barsvertical},
    {.func_arg = animation_expr_crossfade},
    {.func_arg = animation_expr_checkerb},
    {.func_arg = animation_expr_circle},
    {.func_arg = animation_expr_circles},
    {.func_arg = animation_expr_diamond},
    {.func_arg = animation_expr_heart},
    {.func_arg = animation_expr_rectangle},
    {.func_arg = animation_expr_wheel},
    {.func_arg = animation_expr_insetbottoml},
    {.func_arg = animation_expr_insetbottomr},
    {.func_arg = animation_expr_insettopleft},
    {.func_arg = animation_expr_insettopr},
    {.func_arg = animation_expr_iris},
    {.func_arg = animation_expr_revealdown},
    {.func_arg = animation_expr_revealright},
    {.func_arg = animation_expr_revealleft},
    {.func_arg = animation_expr_roll},
    {.func_arg = animation_expr_slide},
    {.func_arg = animation_expr_slideupt},
    {.func_arg = animation_expr_slideleftt},
    {.func_arg = animation_expr_slidedownt},
    {.func_arg = animation_expr_splith},
    {.func_arg = animation_expr_splitv},
    {.func_arg = animation_expr_fanin},
    {.func_arg = animation_expr_test},
    {NULL}
};
typedef struct ThreadData {
    const AVFrame *top, *bottom;
    AVFrame *dst;
    ////AVFilterLink *inlink;
    int plane;
    int w, h;
    FilterParams *param;
} ThreadData;

#define COMMON_OPTIONS \
    { "c0_mode", "set component #0 blend mode", OFFSET(params[0].mode), AV_OPT_TYPE_INT, {.i64=0}, 0, BLEND_NB-1, FLAGS, "mode"},\
    { "c1_mode", "set component #1 blend mode", OFFSET(params[1].mode), AV_OPT_TYPE_INT, {.i64=0}, 0, BLEND_NB-1, FLAGS, "mode"},\
    { "c2_mode", "set component #2 blend mode", OFFSET(params[2].mode), AV_OPT_TYPE_INT, {.i64=0}, 0, BLEND_NB-1, FLAGS, "mode"},\
    { "c3_mode", "set component #3 blend mode", OFFSET(params[3].mode), AV_OPT_TYPE_INT, {.i64=0}, 0, BLEND_NB-1, FLAGS, "mode"},\
    { "all_mode", "set blend mode for all components", OFFSET(all_mode), AV_OPT_TYPE_INT, {.i64=-1},-1, BLEND_NB-1, FLAGS, "mode"},\
    { "addition",   "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_ADDITION},   0, 0, FLAGS, "mode" },\
    { "addition128", "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_ADDITION128}, 0, 0, FLAGS, "mode" },\
    { "and",        "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_AND},        0, 0, FLAGS, "mode" },\
    { "average",    "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_AVERAGE},    0, 0, FLAGS, "mode" },\
    { "burn",       "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_BURN},       0, 0, FLAGS, "mode" },\
    { "darken",     "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_DARKEN},     0, 0, FLAGS, "mode" },\
    { "difference", "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_DIFFERENCE}, 0, 0, FLAGS, "mode" },\
    { "difference128", "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_DIFFERENCE128}, 0, 0, FLAGS, "mode" },\
    { "divide",     "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_DIVIDE},     0, 0, FLAGS, "mode" },\
    { "dodge",      "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_DODGE},      0, 0, FLAGS, "mode" },\
    { "exclusion",  "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_EXCLUSION},  0, 0, FLAGS, "mode" },\
    { "freeze",     "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_FREEZE},     0, 0, FLAGS, "mode" },\
    { "glow",       "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_GLOW},       0, 0, FLAGS, "mode" },\
    { "hardlight",  "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_HARDLIGHT},  0, 0, FLAGS, "mode" },\
    { "hardmix",    "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_HARDMIX},    0, 0, FLAGS, "mode" },\
    { "heat",       "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_HEAT},       0, 0, FLAGS, "mode" },\
    { "lighten",    "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_LIGHTEN},    0, 0, FLAGS, "mode" },\
    { "linearlight","", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_LINEARLIGHT},0, 0, FLAGS, "mode" },\
    { "multiply",   "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_MULTIPLY},   0, 0, FLAGS, "mode" },\
    { "multiply128","", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_MULTIPLY128},0, 0, FLAGS, "mode" },\
    { "negation",   "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_NEGATION},   0, 0, FLAGS, "mode" },\
    { "normal",     "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_NORMAL},     0, 0, FLAGS, "mode" },\
    { "or",         "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_OR},         0, 0, FLAGS, "mode" },\
    { "overlay",    "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_OVERLAY},    0, 0, FLAGS, "mode" },\
    { "phoenix",    "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_PHOENIX},    0, 0, FLAGS, "mode" },\
    { "pinlight",   "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_PINLIGHT},   0, 0, FLAGS, "mode" },\
    { "reflect",    "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_REFLECT},    0, 0, FLAGS, "mode" },\
    { "screen",     "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_SCREEN},     0, 0, FLAGS, "mode" },\
    { "softlight",  "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_SOFTLIGHT},  0, 0, FLAGS, "mode" },\
    { "subtract",   "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_SUBTRACT},   0, 0, FLAGS, "mode" },\
    { "vividlight", "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_VIVIDLIGHT}, 0, 0, FLAGS, "mode" },\
    { "xor",        "", 0, AV_OPT_TYPE_CONST, {.i64=BLEND_XOR},        0, 0, FLAGS, "mode" },\
    { "c0_expr",  "set color component #0 expression", OFFSET(params[0].expr_str), AV_OPT_TYPE_STRING, {.str=NULL}, CHAR_MIN, CHAR_MAX, FLAGS },\
    { "c1_expr",  "set color component #1 expression", OFFSET(params[1].expr_str), AV_OPT_TYPE_STRING, {.str=NULL}, CHAR_MIN, CHAR_MAX, FLAGS },\
    { "c2_expr",  "set color component #2 expression", OFFSET(params[2].expr_str), AV_OPT_TYPE_STRING, {.str=NULL}, CHAR_MIN, CHAR_MAX, FLAGS },\
    { "c3_expr",  "set color component #3 expression", OFFSET(params[3].expr_str), AV_OPT_TYPE_STRING, {.str=NULL}, CHAR_MIN, CHAR_MAX, FLAGS },\
    { "all_expr", "set expression for all color components", OFFSET(all_expr), AV_OPT_TYPE_STRING, {.str=NULL}, CHAR_MIN, CHAR_MAX, FLAGS },\
    { "c0_opacity",  "set color component #0 opacity", OFFSET(params[0].opacity), AV_OPT_TYPE_DOUBLE, {.dbl=1}, 0, 1, FLAGS },\
    { "c1_opacity",  "set color component #1 opacity", OFFSET(params[1].opacity), AV_OPT_TYPE_DOUBLE, {.dbl=1}, 0, 1, FLAGS },\
    { "c2_opacity",  "set color component #2 opacity", OFFSET(params[2].opacity), AV_OPT_TYPE_DOUBLE, {.dbl=1}, 0, 1, FLAGS },\
    { "c3_opacity",  "set color component #3 opacity", OFFSET(params[3].opacity), AV_OPT_TYPE_DOUBLE, {.dbl=1}, 0, 1, FLAGS },\
    { "all_opacity", "set opacity for all color components", OFFSET(all_opacity), AV_OPT_TYPE_DOUBLE, {.dbl=1}, 0, 1, FLAGS}

#define OFFSET(x) offsetof(BlendContext, x)
#define FLAGS AV_OPT_FLAG_FILTERING_PARAM|AV_OPT_FLAG_VIDEO_PARAM


////AVFILTER_DEFINE_CLASS(blend);

#define COPY(src)                                                            \
static void blend_copy ## src(const uint8_t *top, ptrdiff_t top_linesize,    \
                            const uint8_t *bottom, ptrdiff_t bottom_linesize,\
                            uint8_t *dst, ptrdiff_t dst_linesize,            \
                            ptrdiff_t width, ptrdiff_t height,               \
                            FilterParams *param, double *values, int starty) \
{                                                                            \
    av_image_copy_plane(dst, dst_linesize, src, src ## _linesize,            \
                        width, height);                                 \
}

COPY(top)
COPY(bottom)

#undef COPY

static void blend_normal_8bit(const uint8_t *top, ptrdiff_t top_linesize,
                              const uint8_t *bottom, ptrdiff_t bottom_linesize,
                              uint8_t *dst, ptrdiff_t dst_linesize,
                              ptrdiff_t width, ptrdiff_t height,
                              FilterParams *param, double *values, int starty)
{
    const double opacity = param->opacity;
    int i, j;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            dst[j] = top[j] * opacity + bottom[j] * (1. - opacity);
        }
        dst    += dst_linesize;
        top    += top_linesize;
        bottom += bottom_linesize;
    }
}

static void blend_normal_16bit(const uint8_t *_top, ptrdiff_t top_linesize,
                                  const uint8_t *_bottom, ptrdiff_t bottom_linesize,
                                  uint8_t *_dst, ptrdiff_t dst_linesize,
                                  ptrdiff_t width, ptrdiff_t height,
                                  FilterParams *param, double *values, int starty)
{
    const uint16_t *top = (uint16_t*)_top;
    const uint16_t *bottom = (uint16_t*)_bottom;
    uint16_t *dst = (uint16_t*)_dst;
    const double opacity = param->opacity;
    int i, j;
    dst_linesize /= 2;
    top_linesize /= 2;
    bottom_linesize /= 2;

    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            dst[j] = top[j] * opacity + bottom[j] * (1. - opacity);
        }
        dst    += dst_linesize;
        top    += top_linesize;
        bottom += bottom_linesize;
    }
}

#define DEFINE_BLEND8(name, expr)                                              \
static void blend_## name##_8bit(const uint8_t *top, ptrdiff_t top_linesize,         \
                                 const uint8_t *bottom, ptrdiff_t bottom_linesize,   \
                                 uint8_t *dst, ptrdiff_t dst_linesize,               \
                                 ptrdiff_t width, ptrdiff_t height,                \
                                 FilterParams *param, double * /*values*/, int /*starty*/) \
{                                                                              \
    double opacity = param->opacity;                                           \
    int i, j;                                                                  \
                                                                               \
    for (i = 0; i < height; i++) {                                             \
        for (j = 0; j < width; j++) {                                          \
            dst[j] = top[j] + ((expr) - top[j]) * opacity;                     \
        }                                                                      \
        dst    += dst_linesize;                                                \
        top    += top_linesize;                                                \
        bottom += bottom_linesize;                                             \
    }                                                                          \
}

#define DEFINE_BLEND16(name, expr)                                             \
static void blend_## name##_16bit(const uint8_t *_top, ptrdiff_t top_linesize,       \
                                  const uint8_t *_bottom, ptrdiff_t bottom_linesize, \
                                  uint8_t *_dst, ptrdiff_t dst_linesize,             \
                                  ptrdiff_t width, ptrdiff_t height,           \
                                  FilterParams *param, double *values, int starty)         \
{                                                                              \
    const uint16_t *top = (uint16_t*)_top;                                     \
    const uint16_t *bottom = (uint16_t*)_bottom;                               \
    uint16_t *dst = (uint16_t*)_dst;                                           \
    double opacity = param->opacity;                                           \
    int i, j;                                                                  \
    dst_linesize /= 2;                                                         \
    top_linesize /= 2;                                                         \
    bottom_linesize /= 2;                                                      \
                                                                               \
    for (i = 0; i < height; i++) {                                             \
        for (j = 0; j < width; j++) {                                          \
            dst[j] = top[j] + ((expr) - top[j]) * opacity;                     \
        }                                                                      \
        dst    += dst_linesize;                                                \
        top    += top_linesize;                                                \
        bottom += bottom_linesize;                                             \
    }                                                                          \
}

#define A top[j]
#define B bottom[j]

#define MULTIPLY(x, a, b) ((x) * (((a) * (b)) / 255))
#define SCREEN(x, a, b)   (255 - (x) * ((255 - (a)) * (255 - (b)) / 255))
#define BURN(a, b)        (((a) == 0) ? (a) : FFMAX(0, 255 - ((255 - (b)) << 8) / (a)))
#define DODGE(a, b)       (((a) == 255) ? (a) : FFMIN(255, (((b) << 8) / (255 - (a)))))

DEFINE_BLEND8(addition,   FFMIN(255, A + B))
DEFINE_BLEND8(addition128, av_clip_uint8(A + B - 128))
DEFINE_BLEND8(average,    (A + B) / 2)
DEFINE_BLEND8(subtract,   FFMAX(0, A - B))
DEFINE_BLEND8(multiply,   MULTIPLY(1, A, B))
DEFINE_BLEND8(multiply128,av_clip_uint8((A - 128) * B / 32. + 128))
DEFINE_BLEND8(negation,   255 - FFABS(255 - A - B))
DEFINE_BLEND8(difference, FFABS(A - B))
DEFINE_BLEND8(difference128, av_clip_uint8(128 + A - B))
DEFINE_BLEND8(screen,     SCREEN(1, A, B))
DEFINE_BLEND8(overlay,    (A < 128) ? MULTIPLY(2, A, B) : SCREEN(2, A, B))
DEFINE_BLEND8(hardlight,  (B < 128) ? MULTIPLY(2, B, A) : SCREEN(2, B, A))
DEFINE_BLEND8(hardmix,    (A < (255 - B)) ? 0: 255)
DEFINE_BLEND8(heat,       (A == 0) ? 0 : 255 - FFMIN(((255 - B) * (255 - B)) / A, 255))
DEFINE_BLEND8(freeze,     (B == 0) ? 0 : 255 - FFMIN(((255 - A) * (255 - A)) / B, 255))
DEFINE_BLEND8(darken,     FFMIN(A, B))
DEFINE_BLEND8(lighten,    FFMAX(A, B))
DEFINE_BLEND8(divide,     av_clip_uint8(B == 0 ? 255 : 255 * A / B))
DEFINE_BLEND8(dodge,      DODGE(A, B))
DEFINE_BLEND8(burn,       BURN(A, B))
DEFINE_BLEND8(softlight,  (A > 127) ? B + (255 - B) * (A - 127.5) / 127.5 * (0.5 - fabs(B - 127.5) / 255): B - B * ((127.5 - A) / 127.5) * (0.5 - fabs(B - 127.5)/255))
DEFINE_BLEND8(exclusion,  A + B - 2 * A * B / 255)
DEFINE_BLEND8(pinlight,   (B < 128) ? FFMIN(A, 2 * B) : FFMAX(A, 2 * (B - 128)))
DEFINE_BLEND8(phoenix,    FFMIN(A, B) - FFMAX(A, B) + 255)
DEFINE_BLEND8(reflect,    (B == 255) ? B : FFMIN(255, (A * A / (255 - B))))
DEFINE_BLEND8(glow,       (A == 255) ? A : FFMIN(255, (B * B / (255 - A))))
DEFINE_BLEND8(and,        A & B)
DEFINE_BLEND8(or,         A | B)
DEFINE_BLEND8(xor,        A ^ B)
DEFINE_BLEND8(vividlight, (A < 128) ? BURN(2 * A, B) : DODGE(2 * (A - 128), B))
DEFINE_BLEND8(linearlight,av_clip_uint8((B < 128) ? B + 2 * A - 255 : B + 2 * (A - 128)))

#undef MULTIPLY
#undef SCREEN
#undef BURN
#undef DODGE

#define MULTIPLY(x, a, b) ((x) * (((a) * (b)) / 65535))
#define SCREEN(x, a, b)   (65535 - (x) * ((65535 - (a)) * (65535 - (b)) / 65535))
#define BURN(a, b)        (((a) == 0) ? (a) : FFMAX(0, 65535 - ((65535 - (b)) << 16) / (a)))
#define DODGE(a, b)       (((a) == 65535) ? (a) : FFMIN(65535, (((b) << 16) / (65535 - (a)))))

DEFINE_BLEND16(addition,   FFMIN(65535, A + B))
DEFINE_BLEND16(addition128, av_clip_uint16(A + B - 32768))
DEFINE_BLEND16(average,    (A + B) / 2)
DEFINE_BLEND16(subtract,   FFMAX(0, A - B))
DEFINE_BLEND16(multiply,   MULTIPLY(1, A, B))
DEFINE_BLEND16(multiply128, av_clip_uint16((A - 32768) * B / 8192. + 32768))
DEFINE_BLEND16(negation,   65535 - FFABS(65535 - A - B))
DEFINE_BLEND16(difference, FFABS(A - B))
DEFINE_BLEND16(difference128, av_clip_uint16(32768 + A - B))
DEFINE_BLEND16(screen,     SCREEN(1, A, B))
DEFINE_BLEND16(overlay,    (A < 32768) ? MULTIPLY(2, A, B) : SCREEN(2, A, B))
DEFINE_BLEND16(hardlight,  (B < 32768) ? MULTIPLY(2, B, A) : SCREEN(2, B, A))
DEFINE_BLEND16(hardmix,    (A < (65535 - B)) ? 0: 65535)
DEFINE_BLEND16(heat,       (A == 0) ? 0 : 65535 - FFMIN(((65535 - B) * (65535 - B)) / A, 65535))
DEFINE_BLEND16(freeze,     (B == 0) ? 0 : 65535 - FFMIN(((65535 - A) * (65535 - A)) / B, 65535))
DEFINE_BLEND16(darken,     FFMIN(A, B))
DEFINE_BLEND16(lighten,    FFMAX(A, B))
DEFINE_BLEND16(divide,     av_clip_uint16(B == 0 ? 65535 : 65535 * A / B))
DEFINE_BLEND16(dodge,      DODGE(A, B))
DEFINE_BLEND16(burn,       BURN(A, B))
DEFINE_BLEND16(softlight,  (A > 32767) ? B + (65535 - B) * (A - 32767.5) / 32767.5 * (0.5 - fabs(B - 32767.5) / 65535): B - B * ((32767.5 - A) / 32767.5) * (0.5 - fabs(B - 32767.5)/65535))
DEFINE_BLEND16(exclusion,  A + B - 2 * A * B / 65535)
DEFINE_BLEND16(pinlight,   (B < 32768) ? FFMIN(A, 2 * B) : FFMAX(A, 2 * (B - 32768)))
DEFINE_BLEND16(phoenix,    FFMIN(A, B) - FFMAX(A, B) + 65535)
DEFINE_BLEND16(reflect,    (B == 65535) ? B : FFMIN(65535, (A * A / (65535 - B))))
DEFINE_BLEND16(glow,       (A == 65535) ? A : FFMIN(65535, (B * B / (65535 - A))))
DEFINE_BLEND16(and,        A & B)
DEFINE_BLEND16(or,         A | B)
DEFINE_BLEND16(xor,        A ^ B)
DEFINE_BLEND16(vividlight, (A < 32768) ? BURN(2 * A, B) : DODGE(2 * (A - 32768), B))
DEFINE_BLEND16(linearlight,av_clip_uint16((B < 32768) ? B + 2 * A - 65535 : B + 2 * (A - 32768)))

#define DEFINE_BLEND_EXPR(type, name, div)                                     \
static void blend_expr_## name(const uint8_t *_top, ptrdiff_t top_linesize,          \
                               const uint8_t *_bottom, ptrdiff_t bottom_linesize,    \
                               uint8_t *_dst, ptrdiff_t dst_linesize,                \
                               ptrdiff_t width, ptrdiff_t height,              \
                               FilterParams *param, double *values, int starty) \
{                                                                              \
    const type *top = (type*)_top;                                             \
    const type *bottom = (type*)_bottom;                                       \
    type *dst = (type*)_dst;                                                   \
    AVExpr *e = param->e;                                                      \
    int y, x;                                                                  \
    dst_linesize /= div;                                                       \
    top_linesize /= div;                                                       \
    bottom_linesize /= div;                                                    \
                                                                               \
    for (y = 0; y < height; y++) {                                             \
        values[VAR_Y] = y + starty;                                            \
        for (x = 0; x < width; x++) {                                          \
            values[VAR_X]      = x;                                            \
            values[VAR_TOP]    = values[VAR_A] = top[x];                       \
            values[VAR_BOTTOM] = values[VAR_B] = bottom[x];                    \
            dst[x] = av_expr_eval(e, values, NULL);                            \
        }                                                                      \
        dst    += dst_linesize;                                                \
        top    += top_linesize;                                                \
        bottom += bottom_linesize;                                             \
    }                                                                          \
}
//DEFINE_BLEND_EXPR(uint8_t, 8bit, 1)
//DEFINE_BLEND_EXPR(uint16_t, 16bit, 2)
static void blend_expr_8bit(const uint8_t *_top, ptrdiff_t top_linesize,          const uint8_t *_bottom, ptrdiff_t bottom_linesize,    uint8_t *_dst, ptrdiff_t dst_linesize,                ptrdiff_t width, ptrdiff_t height,              FilterParams *param, double *values, int starty)           ; 
static void blend_expr_16bit(const uint8_t *_top, ptrdiff_t top_linesize,          const uint8_t *_bottom, ptrdiff_t bottom_linesize,    uint8_t *_dst, ptrdiff_t dst_linesize,                ptrdiff_t width, ptrdiff_t height,              FilterParams *param, double *values, int starty)            ;
//按线程id来划分任务，用于多线程处理
//frame_count_out 从1开始
//pts秒
static int filter_slice(ThreadData *td, int jobnr, int nb_jobs
        , int64_t frame_count_out , double pts)
{
    int slice_start = (td->h *  jobnr   ) / nb_jobs;
    int slice_end   = (td->h * (jobnr+1)) / nb_jobs;
    int height      = slice_end - slice_start;
    const uint8_t *top    = td->top->data[td->plane];
    const uint8_t *bottom = td->bottom->data[td->plane];
    uint8_t *dst    = td->dst->data[td->plane];
    double values[VAR_VARS_NB];

    values[VAR_N]  = frame_count_out;
    values[VAR_T]  = pts;//td->dst->pts == AV_NOPTS_VALUE ? NAN : td->dst->pts * av_q2d(td->inlink->time_base);
    values[VAR_W]  = td->w;
    values[VAR_H]  = td->h;
    values[VAR_SW] = td->w / (double)td->dst->width;
    values[VAR_SH] = td->h / (double)td->dst->height;

    td->param->blend(top + slice_start * td->top->linesize[td->plane],
                     td->top->linesize[td->plane],
                     bottom + slice_start * td->bottom->linesize[td->plane],
                     td->bottom->linesize[td->plane],
                     dst + slice_start * td->dst->linesize[td->plane],
                     td->dst->linesize[td->plane],
                     td->w, height, td->param, &values[0], slice_start);
    return 0;
}

//loop 划分线程任务
AVFrame *blend_frame(BlendContext *s, AVFrame *top_buf,
                            const AVFrame *bottom_buf, AVFrame *&dst_buf, 
                            int64_t frame_count_out , double pts)
{
    int plane;

    //av_log(NULL, AV_LOG_INFO, "blend_frame pts: %f frame_count_out: %ld\n", pts, frame_count_out);
    qDebug()<<"blend_frame blend_frame pts: "<<pts<<" frame_count_out: "<<frame_count_out
        <<" nb_planes: "<<s->nb_planes;
    for (plane = 0; plane < s->nb_planes; plane++) {
        int hsub = plane == 1 || plane == 2 ? s->hsub : 0;
        int vsub = plane == 1 || plane == 2 ? s->vsub : 0;
        int outw = AV_CEIL_RSHIFT(dst_buf->width,  hsub);
        int outh = AV_CEIL_RSHIFT(dst_buf->height, vsub);
        FilterParams *param = &s->params[plane];
#if 0
        ThreadData td = { .top = top_buf, .bottom = bottom_buf, .dst = dst_buf,
                          .w = outw, .h = outh, .param = param, .plane = plane
                          };
#endif
        ThreadData td;
        td.top = top_buf;
        td.bottom = bottom_buf;
        td.dst = dst_buf;
        td.w = outw;
        td.h = outh;
        td.param = param;
        td.plane = plane;

        //ctx->internal->execute(ctx, filter_slice, &td, NULL, FFMIN(outh, ff_filter_get_nb_threads(ctx)));
        filter_slice(&td, 0, 1, frame_count_out, pts);
    }

    if (!s->tblend)
        av_frame_free(&top_buf);

    return dst_buf;
}
#if 0
static av_cold int init(AVFilterContext *ctx)
{
    BlendContext *s = ctx->priv;

    s->tblend = !strcmp(ctx->filter->name, "tblend");

    s->dinput.process = blend_frame;
    return 0;
}
#endif

av_cold void blend_uninit(BlendContext *s)
{
    int i;

    av_frame_free(&s->prev_frame);

    for (i = 0; i < FF_ARRAY_ELEMS(s->params); i++)
        av_expr_free(s->params[i].e);
}

void ff_blend_init(FilterParams *param, int is_16bit)
{
    switch (param->mode) {
    case BLEND_ADDITION:   param->blend = is_16bit ? blend_addition_16bit   : blend_addition_8bit;   break;
    case BLEND_ADDITION128: param->blend = is_16bit ? blend_addition128_16bit : blend_addition128_8bit; break;
    case BLEND_AND:        param->blend = is_16bit ? blend_and_16bit        : blend_and_8bit;        break;
    case BLEND_AVERAGE:    param->blend = is_16bit ? blend_average_16bit    : blend_average_8bit;    break;
    case BLEND_BURN:       param->blend = is_16bit ? blend_burn_16bit       : blend_burn_8bit;       break;
    case BLEND_DARKEN:     param->blend = is_16bit ? blend_darken_16bit     : blend_darken_8bit;     break;
    case BLEND_DIFFERENCE: param->blend = is_16bit ? blend_difference_16bit : blend_difference_8bit; break;
    case BLEND_DIFFERENCE128: param->blend = is_16bit ? blend_difference128_16bit: blend_difference128_8bit; break;
    case BLEND_DIVIDE:     param->blend = is_16bit ? blend_divide_16bit     : blend_divide_8bit;     break;
    case BLEND_DODGE:      param->blend = is_16bit ? blend_dodge_16bit      : blend_dodge_8bit;      break;
    case BLEND_EXCLUSION:  param->blend = is_16bit ? blend_exclusion_16bit  : blend_exclusion_8bit;  break;
    case BLEND_FREEZE:     param->blend = is_16bit ? blend_freeze_16bit     : blend_freeze_8bit;     break;
    case BLEND_GLOW:       param->blend = is_16bit ? blend_glow_16bit       : blend_glow_8bit;       break;
    case BLEND_HARDLIGHT:  param->blend = is_16bit ? blend_hardlight_16bit  : blend_hardlight_8bit;  break;
    case BLEND_HARDMIX:    param->blend = is_16bit ? blend_hardmix_16bit    : blend_hardmix_8bit;    break;
    case BLEND_HEAT:       param->blend = is_16bit ? blend_heat_16bit       : blend_heat_8bit;       break;
    case BLEND_LIGHTEN:    param->blend = is_16bit ? blend_lighten_16bit    : blend_lighten_8bit;    break;
    case BLEND_LINEARLIGHT:param->blend = is_16bit ? blend_linearlight_16bit: blend_linearlight_8bit;break;
    case BLEND_MULTIPLY:   param->blend = is_16bit ? blend_multiply_16bit   : blend_multiply_8bit;   break;
    case BLEND_MULTIPLY128:param->blend = is_16bit ? blend_multiply128_16bit: blend_multiply128_8bit;break;
    case BLEND_NEGATION:   param->blend = is_16bit ? blend_negation_16bit   : blend_negation_8bit;   break;
    case BLEND_NORMAL:     param->blend = param->opacity == 1 ? blend_copytop :
                                          param->opacity == 0 ? blend_copybottom :
                                          is_16bit ? blend_normal_16bit     : blend_normal_8bit;     break;
    case BLEND_OR:         param->blend = is_16bit ? blend_or_16bit         : blend_or_8bit;         break;
    case BLEND_OVERLAY:    param->blend = is_16bit ? blend_overlay_16bit    : blend_overlay_8bit;    break;
    case BLEND_PHOENIX:    param->blend = is_16bit ? blend_phoenix_16bit    : blend_phoenix_8bit;    break;
    case BLEND_PINLIGHT:   param->blend = is_16bit ? blend_pinlight_16bit   : blend_pinlight_8bit;   break;
    case BLEND_REFLECT:    param->blend = is_16bit ? blend_reflect_16bit    : blend_reflect_8bit;    break;
    case BLEND_SCREEN:     param->blend = is_16bit ? blend_screen_16bit     : blend_screen_8bit;     break;
    case BLEND_SOFTLIGHT:  param->blend = is_16bit ? blend_softlight_16bit  : blend_softlight_8bit;  break;
    case BLEND_SUBTRACT:   param->blend = is_16bit ? blend_subtract_16bit   : blend_subtract_8bit;   break;
    case BLEND_VIVIDLIGHT: param->blend = is_16bit ? blend_vividlight_16bit : blend_vividlight_8bit; break;
    case BLEND_XOR:        param->blend = is_16bit ? blend_xor_16bit        : blend_xor_8bit;        break;
    }

    if (param->opacity == 0 && param->mode != BLEND_NORMAL) {
        param->blend = blend_copytop;
    }

    if (ARCH_X86)
        ff_blend_init_x86(param, is_16bit);
}

//static int config_output(AVFilterLink *outlink)
av_cold int blend_init(BlendContext *s, AVFrame* top, AVFrame* bottom, const char* all_expr)
{
    memset(s, 0, sizeof(BlendContext));
    s->all_mode=BLEND_UNSET;
    s->params[0].opacity=1;
    s->params[1].opacity=1;
    s->params[2].opacity=1;
    s->params[3].opacity=1;
    s->all_opacity=1;
    s->all_expr=all_expr;

    const AVPixFmtDescriptor *pix_desc = av_pix_fmt_desc_get((AVPixelFormat)top->format);
    int ret, plane, is_16bit;

    if (!s->tblend) {
        if (top->format != bottom->format) {
            //av_log(NULL, AV_LOG_ERROR, "inputs must be of same pixel format top->format: %d bottom->format: %d\n", top->format, bottom->format);
            qInfo()<<"blend_init error inputs must be of same pixel format top->format: "<<top->format
                <<" bottom->format: "<<bottom->format;
            return AVERROR(EINVAL);
        }
        if (top->width                       != bottom->width ||
            top->height                       != bottom->height /*||
            toplink->sample_aspect_ratio.num != bottomlink->sample_aspect_ratio.num ||
            toplink->sample_aspect_ratio.den != bottomlink->sample_aspect_ratio.den*/
            ) {
#if 0
            av_log(NULL, AV_LOG_ERROR, "First input link top parameters "
                   "(size %dx%d) do not match the corresponding "
                   "second input link bottom parameters (%dx%d)\n",
                   top->width, top->height,
                   bottom->width, bottom->height);
#endif
            qInfo()<<"blend_init error First input link top parameters (size "<<top->width<<"x"
                <<top->height<<" do not match the corresponding second input link bottom parameters "
                <<bottom->width<<"x"<<bottom->height;
            return AVERROR(EINVAL);
        }
    }
#if 0
    outlink->w = top->width;
    outlink->h = top->height;
    outlink->time_base = toplink->time_base;
    outlink->sample_aspect_ratio = toplink->sample_aspect_ratio;
    outlink->frame_rate = toplink->frame_rate;
#endif
    s->hsub = pix_desc->log2_chroma_w;
    s->vsub = pix_desc->log2_chroma_h;

    is_16bit = pix_desc->comp[0].depth == 16;
    s->nb_planes = av_pix_fmt_count_planes((AVPixelFormat)top->format);
    qDebug()<<"blend_init top->format: "<<top->format <<" nb_planes: "<<s->nb_planes;

#if 0
    if (!s->tblend)
    {
        if ((ret = ff_dualinput_init(ctx, &s->dinput)) < 0)
            return ret;
    }
#endif

    for (plane = 0; plane < FF_ARRAY_ELEMS(s->params); plane++) {
        FilterParams *param = &s->params[plane];

        if (s->all_mode >= 0)
            param->mode = s->all_mode;
        if (s->all_opacity < 1)
            param->opacity = s->all_opacity;

        ff_blend_init(param, is_16bit);

        if (s->all_expr && !param->expr_str) {
            param->expr_str = av_strdup(s->all_expr);
            if (!param->expr_str)
                return AVERROR(ENOMEM);
        }
        if (param->expr_str) {
            ret = av_expr_parse(&param->e, param->expr_str, var_names,
                                NULL, NULL, NULL, NULL, 0, NULL);
            if (ret < 0)
                return ret;
            param->blend = is_16bit? blend_expr_16bit : blend_expr_8bit;
        }
    }

    return 0;
}

//storm
static void blend_expr_16bit(const uint8_t *_top, ptrdiff_t top_linesize,          
                               const uint8_t *_bottom, ptrdiff_t bottom_linesize,    
                               uint8_t *_dst, ptrdiff_t dst_linesize,                
                               ptrdiff_t width, ptrdiff_t height,              
                               FilterParams *param, double *values, int starty)            
{                                                                              
    const uint16_t *top = (uint16_t*)_top;                                             
    const uint16_t *bottom = (uint16_t*)_bottom;                                       
    uint16_t *dst = (uint16_t*)_dst;                                                   
    AVExpr *e = param->e;                                                      
    int y, x;                                                                  
    dst_linesize /= 2;                                                       
    top_linesize /= 2;                                                       
    bottom_linesize /= 2;                                                    
                                                                               
    for (y = 0; y < height; y++) {                                             
        values[VAR_Y] = y + starty;                                                     
        for (x = 0; x < width; x++) {                                          
            values[VAR_X]      = x;                                            
            values[VAR_TOP]    = values[VAR_A] = top[x];                       
            values[VAR_BOTTOM] = values[VAR_B] = bottom[x];                    
            dst[x] = av_expr_eval(e, values, NULL);                            
        }                                                                      
        dst    += dst_linesize;                                                
        top    += top_linesize;                                                
        bottom += bottom_linesize;                                             
    }                                                                          
}
//static int cntExpr=0; // storm
static void blend_expr_normal_8bit(const uint8_t *_top, ptrdiff_t top_linesize,          
                               const uint8_t *_bottom, ptrdiff_t bottom_linesize,    
                               uint8_t *_dst, ptrdiff_t dst_linesize,                
                               ptrdiff_t width, ptrdiff_t height,              
                               FilterParams *param, double *values, int starty)            
{                                                                              
    const uint8_t *top = (uint8_t*)_top;                                             
    const uint8_t *bottom = (uint8_t*)_bottom;                                       
    uint8_t *dst = (uint8_t*)_dst;                                                   
    AVExpr *e = param->e;                                                      
    int y, x;                                                                  
    //double W = values[VAR_W];
    //double H = values[VAR_H];
    //double N = values[VAR_N];
    
    dst_linesize /= 1;                                                       
    top_linesize /= 1;                                                       
    bottom_linesize /= 1;                                                    
    for (y = 0; y < height; y++) {                                             
        values[VAR_Y] = y + starty;                                                     
        for (x = 0; x < width; x++) {                                          
            values[VAR_X]      = x;                                            
            values[VAR_TOP]    = values[VAR_A] = top[x];                       
            values[VAR_BOTTOM] = values[VAR_B] = bottom[x];                    
            dst[x] = av_expr_eval(e, values, NULL);                            
        }                                                                      
        dst    += dst_linesize;                                                
        top    += top_linesize;                                                
        bottom += bottom_linesize;                                             
    }
}
static void blend_expr_8bit(const uint8_t *_top, ptrdiff_t top_linesize,          
                               const uint8_t *_bottom, ptrdiff_t bottom_linesize,    
                               uint8_t *_dst, ptrdiff_t dst_linesize,                
                               ptrdiff_t width, ptrdiff_t height,              
                               FilterParams *param, double *values, int starty)            
{                                                                              
    const uint8_t *top = (uint8_t*)_top;                                             
    const uint8_t *bottom = (uint8_t*)_bottom;                                       
    uint8_t *dst = (uint8_t*)_dst;                                                   
    AVExpr *e = param->e;                                                      
#if 0
    int y, x;                                                                  
    double W = values[VAR_W];
    double H = values[VAR_H];
    double N = values[VAR_N];
#endif 
    dst_linesize /= 1;                                                       
    top_linesize /= 1;                                                       
    bottom_linesize /= 1;                                                    
    //printf ("cntExpr: %d\n", ++cntExpr) ; //why 75(50 frame) time? 2 frame == 3 
    //av_log(NULL, AV_LOG_VERBOSE, "cntExpr:  %d\n", ++cntExpr) ;
#if 0
    av_log(NULL, AV_LOG_INFO, "values[VAR_N]: %f values[VAR_T]: %f values[VAR_W]: %f values[VAR_H]: %f values[VAR_SW]: %f values[VAR_SH]: %f e->type: %d\n"
            , values[VAR_N], values[VAR_T], values[VAR_W], values[VAR_H], values[VAR_SW], values[VAR_SH], e->type) ;
#endif
    qDebug()<<"blend_expr_8bit values[VAR_N]: "<<values[VAR_N]<<" values[VAR_T]:"<< values[VAR_T]
        <<" values[VAR_W]: "<<values[VAR_W]<<" values[VAR_H]: "<<values[VAR_H]<<" values[VAR_SW]: "
        <<values[VAR_SW]<<" values[VAR_SH]: "<<values[VAR_SH]<<" e->type: "<<e->type;
    if(e->type == AVExpr::e_if)
    {
        FuncDef blend_fun = blendfuncs[e->param[0]->a.const_index].func_arg;
        if(blend_fun)
        {
            blend_fun(top, top_linesize, bottom, bottom_linesize, dst,dst_linesize, width, height, param, values, starty);
        }
        else
        {
            blend_expr_normal_8bit(top, top_linesize, bottom, bottom_linesize, dst,dst_linesize, width, height, param, values, starty);
        }
#if 0
        if(e->param[0]->a.const_index == VAR_REVEALLEFT)
        {
            FuncDef abc = blendfuncs[VAR_REVEALLEFT].func_arg;
            abc(top, top_linesize, bottom, bottom_linesize, dst,dst_linesize, width, height, param, values);
        }
        if(e->param[0]->a.const_index == VAR_CROSSFADE)
        {
            FuncDef abc = blendfuncs[VAR_CROSSFADE].func_arg;
            abc(top, top_linesize, bottom, bottom_linesize, dst,dst_linesize, width, height, param, values);
        }
#endif
    }
    else
    {
        blend_expr_normal_8bit(top, top_linesize, bottom, bottom_linesize, dst,dst_linesize, width, height, param, values, starty);
    }
#if 0
    for (y = 0; y < height; y++) {                                             
        values[VAR_Y] = y;                                                     
        for (x = 0; x < width; x++) {                                          
            values[VAR_X]      = x;                                            
            values[VAR_TOP]    = values[VAR_A] = top[x];                       
            values[VAR_BOTTOM] = values[VAR_B] = bottom[x];                    
            // if(gte(N*SW*50+X,W),B,A)
            //
            if(e->a.const_index == VAR_AB)
            {
                //if((values[VAR_N]*values[VAR_SW]*50+values[VAR_X]) >= values[VAR_W])
                //if(values[VAR_X] >= values[VAR_W]*(25-values[VAR_N]+1)/25)
                if(values[VAR_X] <= (values[VAR_W]/2) *(1-values[VAR_Y]/(values[VAR_H]/2)))
                {
                    dst[x] = values[VAR_B];//av_expr_eval(e, values, NULL);                            
                }
                else
                {
                    dst[x] = values[VAR_A];//av_expr_eval(e, values, NULL);                            
                }
            }
            // 'if(ab,1.5,2)'
            else if(e->type == e_if)
            {
                if(e->param[0]->a.const_index == VAR_AB)
                {
                    double t=e->param[1]->value;
                    double at=e->param[2]->value;
                    double f = 25;
                    t = t <= at? t:at;
                    if ((x<=((H*W-2*W*y)/(2*H)-W*N/(2*f)) )
                            || (x<=(2*y*W-H*W)/(2*H)-W*N/(2*f))
                            || (x>=(W*H+2*y*W)/(2*H)+W*N/(2*f))
                            || (x>= (3*W*H-2*W*y)/(2*H)+W*N/(2*f))
                            || ((x>=(W*H+2*W*y)/(W*H+8*y)+W*N/(2*f)) && x<=W/2 && y <= H/2 )
                            )
                    {
                        dst[x] = values[VAR_A];//av_expr_eval(e, values, NULL);                            
                    }
                    else
                    {
                        dst[x] = values[VAR_B];//av_expr_eval(e, values, NULL);                            
                    }
                }
            }
            else
            {
                dst[x] = av_expr_eval(e, values, NULL);                            
            }
            /*
            if(x<=(cntExpr*1024/75))
                dst[x] = values[VAR_BOTTOM];//av_expr_eval(e, values, NULL);                            
            else
                dst[x] = values[VAR_TOP];//av_expr_eval(e, values, NULL);                            
            */
        }                                                                      
        dst    += dst_linesize;                                                
        top    += top_linesize;                                                
        bottom += bottom_linesize;                                             
    }
#endif
#if 0
    top = (uint8_t*)_top;                                             
    bottom = (uint8_t*)_bottom;                                       
    dst = (uint8_t*)_dst;                                                   
    for (y = 0; y < 1; y++) {                                             
        values[VAR_Y] = y;                                                     
        for (x = 0; x < width; x++) {                                          
            values[VAR_X]      = x;                                            
            values[VAR_TOP]    = values[VAR_A] = top[x];                       
            values[VAR_BOTTOM] = values[VAR_B] = bottom[x];                    
            dst[x] = av_expr_eval(e, values, NULL);                            
        }                                                                      
        dst    += dst_linesize;                                                
        top    += top_linesize;                                                
        bottom += bottom_linesize;                                             
    }
#endif
}
