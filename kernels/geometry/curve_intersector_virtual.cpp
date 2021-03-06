// ======================================================================== //
// Copyright 2009-2018 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //
 
#include "curve_intersector_virtual.h"
#include "intersector_epilog.h"

#include "../subdiv/bezier_curve.h"
#include "../subdiv/bspline_curve.h"

#include "curveNi_intersector.h"
#include "curveNv_intersector.h"
#include "curveNi_mb_intersector.h"

#include "curve_intersector_distance.h"
#include "curve_intersector_ribbon.h"
#include "curve_intersector_oriented.h"
#include "curve_intersector_sweep.h"

namespace embree
{
  namespace isa
  {
    template<typename Curve3fa, int N>
    static VirtualCurveIntersector::Intersectors RibbonNiIntersectors()
    {
      VirtualCurveIntersector::Intersectors intersectors;
      intersectors.intersect1 = (VirtualCurveIntersector::Intersect1Ty)&CurveNiIntersector1<N>::template intersect_t<RibbonCurve1Intersector1<Curve3fa>, Intersect1EpilogMU<VSIZEX,true> >;
      intersectors.occluded1  = (VirtualCurveIntersector::Occluded1Ty) &CurveNiIntersector1<N>::template occluded_t <RibbonCurve1Intersector1<Curve3fa>, Occluded1EpilogMU<VSIZEX,true> >;
      intersectors.intersect4 = (VirtualCurveIntersector::Intersect4Ty)&CurveNiIntersectorK<N,4>::template intersect_t<RibbonCurve1IntersectorK<Curve3fa,4>, Intersect1KEpilogMU<VSIZEX,4,true> >;
      intersectors.occluded4  = (VirtualCurveIntersector::Occluded4Ty) &CurveNiIntersectorK<N,4>::template occluded_t <RibbonCurve1IntersectorK<Curve3fa,4>, Occluded1KEpilogMU<VSIZEX,4,true> >;
#if defined(__AVX__)
      intersectors.intersect8 = (VirtualCurveIntersector::Intersect8Ty)&CurveNiIntersectorK<N,8>::template intersect_t<RibbonCurve1IntersectorK<Curve3fa,8>, Intersect1KEpilogMU<VSIZEX,8,true> >;
      intersectors.occluded8  = (VirtualCurveIntersector::Occluded8Ty) &CurveNiIntersectorK<N,8>::template occluded_t <RibbonCurve1IntersectorK<Curve3fa,8>, Occluded1KEpilogMU<VSIZEX,8,true> >;
#endif
#if defined(__AVX512F__)
      intersectors.intersect16 = (VirtualCurveIntersector::Intersect16Ty)&CurveNiIntersectorK<N,16>::template intersect_t<RibbonCurve1IntersectorK<Curve3fa,16>, Intersect1KEpilogMU<VSIZEX,16,true> >;
      intersectors.occluded16  = (VirtualCurveIntersector::Occluded16Ty) &CurveNiIntersectorK<N,16>::template occluded_t <RibbonCurve1IntersectorK<Curve3fa,16>, Occluded1KEpilogMU<VSIZEX,16,true> >;
#endif
      return intersectors;
    }

    template<typename Curve3fa, int N>
    static VirtualCurveIntersector::Intersectors RibbonNvIntersectors()
    {
      VirtualCurveIntersector::Intersectors intersectors;
      intersectors.intersect1 = (VirtualCurveIntersector::Intersect1Ty)&CurveNvIntersector1<N>::template intersect_t<RibbonCurve1Intersector1<Curve3fa>, Intersect1EpilogMU<VSIZEX,true> >;
      intersectors.occluded1  = (VirtualCurveIntersector::Occluded1Ty) &CurveNvIntersector1<N>::template occluded_t <RibbonCurve1Intersector1<Curve3fa>, Occluded1EpilogMU<VSIZEX,true> >;
      intersectors.intersect4 = (VirtualCurveIntersector::Intersect4Ty)&CurveNvIntersectorK<N,4>::template intersect_t<RibbonCurve1IntersectorK<Curve3fa,4>, Intersect1KEpilogMU<VSIZEX,4,true> >;
      intersectors.occluded4  = (VirtualCurveIntersector::Occluded4Ty) &CurveNvIntersectorK<N,4>::template occluded_t <RibbonCurve1IntersectorK<Curve3fa,4>, Occluded1KEpilogMU<VSIZEX,4,true> >;
#if defined(__AVX__)
      intersectors.intersect8 = (VirtualCurveIntersector::Intersect8Ty)&CurveNvIntersectorK<N,8>::template intersect_t<RibbonCurve1IntersectorK<Curve3fa,8>, Intersect1KEpilogMU<VSIZEX,8,true> >;
      intersectors.occluded8  = (VirtualCurveIntersector::Occluded8Ty) &CurveNvIntersectorK<N,8>::template occluded_t <RibbonCurve1IntersectorK<Curve3fa,8>, Occluded1KEpilogMU<VSIZEX,8,true> >;
#endif
#if defined(__AVX512F__)
      intersectors.intersect16 = (VirtualCurveIntersector::Intersect16Ty)&CurveNvIntersectorK<N,16>::template intersect_t<RibbonCurve1IntersectorK<Curve3fa,16>, Intersect1KEpilogMU<VSIZEX,16,true> >;
      intersectors.occluded16  = (VirtualCurveIntersector::Occluded16Ty) &CurveNvIntersectorK<N,16>::template occluded_t <RibbonCurve1IntersectorK<Curve3fa,16>, Occluded1KEpilogMU<VSIZEX,16,true> >;
#endif
      return intersectors;
    }

    template<typename Curve3fa, int N>
    static VirtualCurveIntersector::Intersectors RibbonNiMBIntersectors()
    {
      VirtualCurveIntersector::Intersectors intersectors;
      intersectors.intersect1 = (VirtualCurveIntersector::Intersect1Ty)&CurveNiMBIntersector1<N>::template intersect_t<RibbonCurve1Intersector1<Curve3fa>, Intersect1EpilogMU<VSIZEX,true> >;
      intersectors.occluded1  = (VirtualCurveIntersector::Occluded1Ty) &CurveNiMBIntersector1<N>::template occluded_t <RibbonCurve1Intersector1<Curve3fa>, Occluded1EpilogMU<VSIZEX,true> >;
      intersectors.intersect4 = (VirtualCurveIntersector::Intersect4Ty)&CurveNiMBIntersectorK<N,4>::template intersect_t<RibbonCurve1IntersectorK<Curve3fa,4>, Intersect1KEpilogMU<VSIZEX,4,true> >;
      intersectors.occluded4  = (VirtualCurveIntersector::Occluded4Ty) &CurveNiMBIntersectorK<N,4>::template occluded_t <RibbonCurve1IntersectorK<Curve3fa,4>, Occluded1KEpilogMU<VSIZEX,4,true> >;
#if defined(__AVX__)
      intersectors.intersect8 = (VirtualCurveIntersector::Intersect8Ty)&CurveNiMBIntersectorK<N,8>::template intersect_t<RibbonCurve1IntersectorK<Curve3fa,8>, Intersect1KEpilogMU<VSIZEX,8,true> >;
      intersectors.occluded8  = (VirtualCurveIntersector::Occluded8Ty) &CurveNiMBIntersectorK<N,8>::template occluded_t <RibbonCurve1IntersectorK<Curve3fa,8>, Occluded1KEpilogMU<VSIZEX,8,true> >;
#endif
#if defined(__AVX512F__)
      intersectors.intersect16 = (VirtualCurveIntersector::Intersect16Ty)&CurveNiMBIntersectorK<N,16>::template intersect_t<RibbonCurve1IntersectorK<Curve3fa,16>, Intersect1KEpilogMU<VSIZEX,16,true> >;
      intersectors.occluded16  = (VirtualCurveIntersector::Occluded16Ty) &CurveNiMBIntersectorK<N,16>::template occluded_t <RibbonCurve1IntersectorK<Curve3fa,16>, Occluded1KEpilogMU<VSIZEX,16,true> >;
#endif
      return intersectors;
    }

    template<typename Curve3fa, int N>
    static VirtualCurveIntersector::Intersectors CurveNiIntersectors()
    {
      VirtualCurveIntersector::Intersectors intersectors;
      intersectors.intersect1 = (VirtualCurveIntersector::Intersect1Ty)&CurveNiIntersector1<N>::template intersect_t<SweepCurve1Intersector1<Curve3fa>, Intersect1Epilog1<true> >;
      intersectors.occluded1  = (VirtualCurveIntersector::Occluded1Ty) &CurveNiIntersector1<N>::template occluded_t <SweepCurve1Intersector1<Curve3fa>, Occluded1Epilog1<true> >;
      intersectors.intersect4 = (VirtualCurveIntersector::Intersect4Ty)&CurveNiIntersectorK<N,4>::template intersect_t<SweepCurve1IntersectorK<Curve3fa,4>, Intersect1KEpilog1<4,true> >;
      intersectors.occluded4  = (VirtualCurveIntersector::Occluded4Ty) &CurveNiIntersectorK<N,4>::template occluded_t <SweepCurve1IntersectorK<Curve3fa,4>, Occluded1KEpilog1<4,true> >;
#if defined(__AVX__)
      intersectors.intersect8 = (VirtualCurveIntersector::Intersect8Ty)&CurveNiIntersectorK<N,8>::template intersect_t<SweepCurve1IntersectorK<Curve3fa,8>, Intersect1KEpilog1<8,true> >;
      intersectors.occluded8  = (VirtualCurveIntersector::Occluded8Ty) &CurveNiIntersectorK<N,8>::template occluded_t <SweepCurve1IntersectorK<Curve3fa,8>, Occluded1KEpilog1<8,true> >;
#endif
#if defined(__AVX512F__)
      intersectors.intersect16 = (VirtualCurveIntersector::Intersect16Ty)&CurveNiIntersectorK<N,16>::template intersect_t<SweepCurve1IntersectorK<Curve3fa,16>, Intersect1KEpilog1<16,true> >;
      intersectors.occluded16  = (VirtualCurveIntersector::Occluded16Ty) &CurveNiIntersectorK<N,16>::template occluded_t <SweepCurve1IntersectorK<Curve3fa,16>, Occluded1KEpilog1<16,true> >;
#endif
      return intersectors;
    }

    template<typename Curve3fa, int N>
    static VirtualCurveIntersector::Intersectors CurveNvIntersectors()
    {
      VirtualCurveIntersector::Intersectors intersectors;
      intersectors.intersect1 = (VirtualCurveIntersector::Intersect1Ty)&CurveNvIntersector1<N>::template intersect_t<SweepCurve1Intersector1<Curve3fa>, Intersect1Epilog1<true> >;
      intersectors.occluded1  = (VirtualCurveIntersector::Occluded1Ty) &CurveNvIntersector1<N>::template occluded_t <SweepCurve1Intersector1<Curve3fa>, Occluded1Epilog1<true> >;
      intersectors.intersect4 = (VirtualCurveIntersector::Intersect4Ty)&CurveNvIntersectorK<N,4>::template intersect_t<SweepCurve1IntersectorK<Curve3fa,4>, Intersect1KEpilog1<4,true> >;
      intersectors.occluded4  = (VirtualCurveIntersector::Occluded4Ty) &CurveNvIntersectorK<N,4>::template occluded_t <SweepCurve1IntersectorK<Curve3fa,4>, Occluded1KEpilog1<4,true> >;
#if defined(__AVX__)
      intersectors.intersect8 = (VirtualCurveIntersector::Intersect8Ty)&CurveNvIntersectorK<N,8>::template intersect_t<SweepCurve1IntersectorK<Curve3fa,8>, Intersect1KEpilog1<8,true> >;
      intersectors.occluded8  = (VirtualCurveIntersector::Occluded8Ty) &CurveNvIntersectorK<N,8>::template occluded_t <SweepCurve1IntersectorK<Curve3fa,8>, Occluded1KEpilog1<8,true> >;
#endif
#if defined(__AVX512F__)
      intersectors.intersect16 = (VirtualCurveIntersector::Intersect16Ty)&CurveNvIntersectorK<N,16>::template intersect_t<SweepCurve1IntersectorK<Curve3fa,16>, Intersect1KEpilog1<16,true> >;
      intersectors.occluded16  = (VirtualCurveIntersector::Occluded16Ty) &CurveNvIntersectorK<N,16>::template occluded_t <SweepCurve1IntersectorK<Curve3fa,16>, Occluded1KEpilog1<16,true> >;
#endif
      return intersectors;
    }

    template<typename Curve3fa, int N>
    static VirtualCurveIntersector::Intersectors CurveNiMBIntersectors()
    {
      VirtualCurveIntersector::Intersectors intersectors;
      intersectors.intersect1 = (VirtualCurveIntersector::Intersect1Ty)&CurveNiMBIntersector1<N>::template intersect_t<SweepCurve1Intersector1<Curve3fa>, Intersect1Epilog1<true> >;
      intersectors.occluded1  = (VirtualCurveIntersector::Occluded1Ty) &CurveNiMBIntersector1<N>::template occluded_t <SweepCurve1Intersector1<Curve3fa>, Occluded1Epilog1<true> >;
      intersectors.intersect4 = (VirtualCurveIntersector::Intersect4Ty)&CurveNiMBIntersectorK<N,4>::template intersect_t<SweepCurve1IntersectorK<Curve3fa,4>, Intersect1KEpilog1<4,true> >;
      intersectors.occluded4  = (VirtualCurveIntersector::Occluded4Ty) &CurveNiMBIntersectorK<N,4>::template occluded_t <SweepCurve1IntersectorK<Curve3fa,4>, Occluded1KEpilog1<4,true> >;
#if defined(__AVX__)
      intersectors.intersect8 = (VirtualCurveIntersector::Intersect8Ty)&CurveNiMBIntersectorK<N,8>::template intersect_t<SweepCurve1IntersectorK<Curve3fa,8>, Intersect1KEpilog1<8,true> >;
      intersectors.occluded8  = (VirtualCurveIntersector::Occluded8Ty) &CurveNiMBIntersectorK<N,8>::template occluded_t <SweepCurve1IntersectorK<Curve3fa,8>, Occluded1KEpilog1<8,true> >;
#endif
#if defined(__AVX512F__)
      intersectors.intersect16 = (VirtualCurveIntersector::Intersect16Ty)&CurveNiMBIntersectorK<N,16>::template intersect_t<SweepCurve1IntersectorK<Curve3fa,16>, Intersect1KEpilog1<16,true> >;
      intersectors.occluded16  = (VirtualCurveIntersector::Occluded16Ty) &CurveNiMBIntersectorK<N,16>::template occluded_t <SweepCurve1IntersectorK<Curve3fa,16>, Occluded1KEpilog1<16,true> >;
#endif
      return intersectors;
    }

    template<typename Curve3fa, int N>
    static VirtualCurveIntersector::Intersectors OrientedCurveNiIntersectors()
    {
      VirtualCurveIntersector::Intersectors intersectors;
      intersectors.intersect1 = (VirtualCurveIntersector::Intersect1Ty)&CurveNiIntersector1<N>::template intersect_n<OrientedCurve1Intersector1<Curve3fa>, Intersect1Epilog1<true> >;
      intersectors.occluded1  = (VirtualCurveIntersector::Occluded1Ty) &CurveNiIntersector1<N>::template occluded_n <OrientedCurve1Intersector1<Curve3fa>, Occluded1Epilog1<true> >;
      intersectors.intersect4 = (VirtualCurveIntersector::Intersect4Ty)&CurveNiIntersectorK<N,4>::template intersect_n<OrientedCurve1IntersectorK<Curve3fa,4>, Intersect1KEpilog1<4,true> >;
      intersectors.occluded4  = (VirtualCurveIntersector::Occluded4Ty) &CurveNiIntersectorK<N,4>::template occluded_n <OrientedCurve1IntersectorK<Curve3fa,4>, Occluded1KEpilog1<4,true> >;
#if defined(__AVX__)
      intersectors.intersect8 = (VirtualCurveIntersector::Intersect8Ty)&CurveNiIntersectorK<N,8>::template intersect_n<OrientedCurve1IntersectorK<Curve3fa,8>, Intersect1KEpilog1<8,true> >;
      intersectors.occluded8  = (VirtualCurveIntersector::Occluded8Ty) &CurveNiIntersectorK<N,8>::template occluded_n <OrientedCurve1IntersectorK<Curve3fa,8>, Occluded1KEpilog1<8,true> >;
#endif
#if defined(__AVX512F__)
      intersectors.intersect16 = (VirtualCurveIntersector::Intersect16Ty)&CurveNiIntersectorK<N,16>::template intersect_n<OrientedCurve1IntersectorK<Curve3fa,16>, Intersect1KEpilog1<16,true> >;
      intersectors.occluded16  = (VirtualCurveIntersector::Occluded16Ty) &CurveNiIntersectorK<N,16>::template occluded_n <OrientedCurve1IntersectorK<Curve3fa,16>, Occluded1KEpilog1<16,true> >;
#endif
      return intersectors;
    }

    template<typename Curve3fa, int N>
    static VirtualCurveIntersector::Intersectors OrientedCurveNiMBIntersectors()
    {
      VirtualCurveIntersector::Intersectors intersectors;
      intersectors.intersect1 = (VirtualCurveIntersector::Intersect1Ty)&CurveNiMBIntersector1<N>::template intersect_n<OrientedCurve1Intersector1<Curve3fa>, Intersect1Epilog1<true> >;
      intersectors.occluded1  = (VirtualCurveIntersector::Occluded1Ty) &CurveNiMBIntersector1<N>::template occluded_n <OrientedCurve1Intersector1<Curve3fa>, Occluded1Epilog1<true> >;
      intersectors.intersect4 = (VirtualCurveIntersector::Intersect4Ty)&CurveNiMBIntersectorK<N,4>::template intersect_n<OrientedCurve1IntersectorK<Curve3fa,4>, Intersect1KEpilog1<4,true> >;
      intersectors.occluded4  = (VirtualCurveIntersector::Occluded4Ty) &CurveNiMBIntersectorK<N,4>::template occluded_n <OrientedCurve1IntersectorK<Curve3fa,4>, Occluded1KEpilog1<4,true> >;
#if defined(__AVX__)
      intersectors.intersect8 = (VirtualCurveIntersector::Intersect8Ty)&CurveNiMBIntersectorK<N,8>::template intersect_n<OrientedCurve1IntersectorK<Curve3fa,8>, Intersect1KEpilog1<8,true> >;
      intersectors.occluded8  = (VirtualCurveIntersector::Occluded8Ty) &CurveNiMBIntersectorK<N,8>::template occluded_n <OrientedCurve1IntersectorK<Curve3fa,8>, Occluded1KEpilog1<8,true> >;
#endif
#if defined(__AVX512F__)
      intersectors.intersect16 = (VirtualCurveIntersector::Intersect16Ty)&CurveNiMBIntersectorK<N,16>::template intersect_n<OrientedCurve1IntersectorK<Curve3fa,16>, Intersect1KEpilog1<16,true> >;
      intersectors.occluded16  = (VirtualCurveIntersector::Occluded16Ty) &CurveNiMBIntersectorK<N,16>::template occluded_n <OrientedCurve1IntersectorK<Curve3fa,16>, Occluded1KEpilog1<16,true> >;
#endif
      return intersectors;
    }

    VirtualCurveIntersector* VirtualCurveIntersector4i()
    {
      static VirtualCurveIntersector function_local_static_prim;
      function_local_static_prim.vtbl[Geometry::GTY_ROUND_BEZIER_CURVE] = CurveNiIntersectors <BezierCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_FLAT_BEZIER_CURVE ] = RibbonNiIntersectors<BezierCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_ORIENTED_BEZIER_CURVE] = OrientedCurveNiIntersectors<BezierCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_ROUND_BSPLINE_CURVE] = CurveNiIntersectors <BSplineCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_FLAT_BSPLINE_CURVE ] = RibbonNiIntersectors<BSplineCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_ORIENTED_BSPLINE_CURVE] = OrientedCurveNiIntersectors<BSplineCurve3fa,4>();
      return &function_local_static_prim;
    }

    VirtualCurveIntersector* VirtualCurveIntersector4v()
    {
      static VirtualCurveIntersector function_local_static_prim;
      function_local_static_prim.vtbl[Geometry::GTY_ROUND_BEZIER_CURVE] = CurveNvIntersectors <BezierCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_FLAT_BEZIER_CURVE ] = RibbonNvIntersectors<BezierCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_ORIENTED_BEZIER_CURVE] = OrientedCurveNiIntersectors<BezierCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_ROUND_BSPLINE_CURVE] = CurveNvIntersectors <BSplineCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_FLAT_BSPLINE_CURVE ] = RibbonNvIntersectors<BSplineCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_ORIENTED_BSPLINE_CURVE] = OrientedCurveNiIntersectors<BSplineCurve3fa,4>();
      return &function_local_static_prim;
    }

    VirtualCurveIntersector* VirtualCurveIntersector4iMB()
    {
      static VirtualCurveIntersector function_local_static_prim;
      function_local_static_prim.vtbl[Geometry::GTY_ROUND_BEZIER_CURVE] = CurveNiMBIntersectors <BezierCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_FLAT_BEZIER_CURVE ] = RibbonNiMBIntersectors<BezierCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_ORIENTED_BEZIER_CURVE] = OrientedCurveNiMBIntersectors<BezierCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_ROUND_BSPLINE_CURVE] = CurveNiMBIntersectors <BSplineCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_FLAT_BSPLINE_CURVE ] = RibbonNiMBIntersectors<BSplineCurve3fa,4>();
      function_local_static_prim.vtbl[Geometry::GTY_ORIENTED_BSPLINE_CURVE] = OrientedCurveNiMBIntersectors<BSplineCurve3fa,4>();
      return &function_local_static_prim;
    }

#if defined (__AVX__)
    
    VirtualCurveIntersector* VirtualCurveIntersector8i()
    {
      static VirtualCurveIntersector function_local_static_prim;
      function_local_static_prim.vtbl[Geometry::GTY_ROUND_BEZIER_CURVE] = CurveNiIntersectors <BezierCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_FLAT_BEZIER_CURVE ] = RibbonNiIntersectors<BezierCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_ORIENTED_BEZIER_CURVE] = OrientedCurveNiIntersectors<BezierCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_ROUND_BSPLINE_CURVE] = CurveNiIntersectors <BSplineCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_FLAT_BSPLINE_CURVE ] = RibbonNiIntersectors<BSplineCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_ORIENTED_BSPLINE_CURVE] = OrientedCurveNiIntersectors<BSplineCurve3fa,8>();
      return &function_local_static_prim;
    }

    VirtualCurveIntersector* VirtualCurveIntersector8v()
    {
      static VirtualCurveIntersector function_local_static_prim;
      function_local_static_prim.vtbl[Geometry::GTY_ROUND_BEZIER_CURVE] = CurveNvIntersectors <BezierCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_FLAT_BEZIER_CURVE ] = RibbonNvIntersectors<BezierCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_ORIENTED_BEZIER_CURVE] = OrientedCurveNiIntersectors<BezierCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_ROUND_BSPLINE_CURVE] = CurveNvIntersectors <BSplineCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_FLAT_BSPLINE_CURVE ] = RibbonNvIntersectors<BSplineCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_ORIENTED_BSPLINE_CURVE] = OrientedCurveNiIntersectors<BSplineCurve3fa,8>();
      return &function_local_static_prim;
    }
    
    VirtualCurveIntersector* VirtualCurveIntersector8iMB()
    {
      static VirtualCurveIntersector function_local_static_prim;
      function_local_static_prim.vtbl[Geometry::GTY_ROUND_BEZIER_CURVE] = CurveNiMBIntersectors <BezierCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_FLAT_BEZIER_CURVE ] = RibbonNiMBIntersectors<BezierCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_ORIENTED_BEZIER_CURVE] = OrientedCurveNiMBIntersectors<BezierCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_ROUND_BSPLINE_CURVE] = CurveNiMBIntersectors <BSplineCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_FLAT_BSPLINE_CURVE ] = RibbonNiMBIntersectors<BSplineCurve3fa,8>();
      function_local_static_prim.vtbl[Geometry::GTY_ORIENTED_BSPLINE_CURVE] = OrientedCurveNiMBIntersectors<BSplineCurve3fa,8>();
      return &function_local_static_prim;
    }
  
#endif
  }
}
