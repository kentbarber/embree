// ======================================================================== //
// Copyright 2009-2015 Intel Corporation                                    //
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

#pragma once

#include "../../common/ray.h"
#include "filter.h"

namespace embree
{
  namespace isa
  {
    template<int M>
      struct LineIntersectorHitM
      {
        __forceinline LineIntersectorHitM(const vfloat<M>& u, const vfloat<M>& v, const vfloat<M>& t, const Vec3<vfloat<M>>& Ng)
          : vu(u), vv(v), vt(t), vNg(Ng) {}
        
        __forceinline void finalize() {}
        
        __forceinline Vec2f uv (const size_t i) const { return Vec2f(vu[i],vv[i]); }
        __forceinline float t  (const size_t i) const { return vt[i]; }
        __forceinline Vec3fa Ng(const size_t i) const { return Vec3fa(vNg.x[i],vNg.y[i],vNg.z[i]); }
        
      public:
        vfloat<M> vu;
        vfloat<M> vv;
        vfloat<M> vt;
        Vec3<vfloat<M>> vNg;
      };
    
    template<int M>
      struct LineIntersector1
      {
        typedef Vec3<vfloat<M>> Vec3vfM;
        typedef Vec4<vfloat<M>> Vec4vfM;
        
        struct Precalculations
        {
          __forceinline Precalculations (const Ray& ray, const void* ptr)
          {
            const float s = rsqrt(dot(ray.dir,ray.dir));
            depth_scale = s;
            ray_space = frame(s*ray.dir).transposed();
          }
          
          vfloat<M> depth_scale;
          LinearSpace3<Vec3vfM> ray_space;
        };
        
        template<typename Epilog>
        static __forceinline bool intersect(Ray& ray, const Precalculations& pre,
                                            const vbool<M>& valid_i, const Vec4vfM& v0, const Vec4vfM& v1,
                                            const Epilog& epilog)
        {
#if 0    
          /* transform end points into ray space */
          Vec4vfM p0(xfmVector(pre.ray_space,v0.xyz()-Vec3vfM(ray.org)), v0.w);
          Vec4vfM p1(xfmVector(pre.ray_space,v1.xyz()-Vec3vfM(ray.org)), v1.w);

          /* approximative intersection with cone */
          const Vec4vfM v = p1-p0;
          const Vec4vfM w = -p0;
          const vfloat<M> d0 = w.x*v.x + w.y*v.y;
          const vfloat<M> d1 = v.x*v.x + v.y*v.y;
          const vfloat<M> u = clamp(d0*rcp(d1),vfloat<M>(zero),vfloat<M>(one));
          const Vec4vfM p = p0 + u*v;
          const vfloat<M> t = p.z*pre.depth_scale;
          const vfloat<M> d2 = p.x*p.x + p.y*p.y;
          const vfloat<M> r = p.w;
          const vfloat<M> r2 = r*r;
          vbool<M> valid = valid_i & d2 <= r2 & vfloat<M>(ray.tnear) < t & t < vfloat<M>(ray.tfar);
          if (unlikely(none(valid))) return false;
          
          /* ignore denormalized segments */
          const Vec3vfM T = v1.xyz()-v0.xyz();
          valid &= T.x != vfloat<M>(zero) | T.y != vfloat<M>(zero) | T.z != vfloat<M>(zero);
          if (unlikely(none(valid))) return false;
          
          /* update hit information */
          LineIntersectorHitM<M> hit(u,zero,t,T);
          return epilog(valid,hit);
#else

          Vec4<vfloat<M>> a = v0;
          Vec4<vfloat<M>> b = v1;
          Vec3<vfloat<M>> a3(v0.x,v0.y,v0.z);
          Vec3<vfloat<M>> b3(v1.x,v1.y,v1.z);

          const vfloat<M> rl = rcp_length(b3-a3);
          const Vec3<vfloat<M>> P0 = a3, dP = (b3-a3)*rl;
          const vfloat<M> r0 = a.w, dr = (b.w-a.w)*rl;
          const Vec3<vfloat<M>> org = ray.org, dO = ray.dir;
          
          const Vec3<vfloat<M>> O = org-P0;
          const vfloat<M> OO = dot(O,O);
          const vfloat<M> dOdO = dot(dO,dO);
          const vfloat<M> dOz = dot(dP,dO);
          const vfloat<M> Oz = dot(dP,O);
          const vfloat<M> OdO = dot(dO,O);
          const vfloat<M> R = r0 + Oz*dr;
          
          const vfloat<M> A = dOdO - sqr(dOz) * (1.0f+sqr(dr));
          const vfloat<M> B = 2.0f * (OdO - dOz*(Oz + R*dr));
          const vfloat<M> C = OO - (sqr(Oz) + sqr(R));
          
          const vfloat<M> D = B*B - 4.0f*A*C;
          vbool<M> valid = D >= 0.0f;
          if (none(valid)) return false;
          
          const vfloat<M> Q = sqrt(D);
          //const vfloat<M> t0 = (-B-Q)*rcp2A;
          //const vfloat<M> t1 = (-B+Q)*rcp2A;
          const vfloat<M> t0 = (-B-Q)/(2.0f*A);
          const vfloat<M> u0 = Oz+t0*dOz;
          const vfloat<M> t = t0;
          const vfloat<M> u = u0*rl;
          valid &= (ray.tnear < t) & (t < ray.tfar) & (0.0f <= u) & (u <= 1.0f);
          if (unlikely(none(valid))) return false;
          
          /* update hit information */
          const Vec3<vfloat<M>> Pr = Vec3<vfloat<M>>(ray.org) + t*Vec3<vfloat<M>>(ray.dir);
          const Vec3<vfloat<M>> Pl = a3 + u*(b3-a3);
          LineIntersectorHitM<M> hit(u,zero,t,Pr-Pl);
          return epilog(valid,hit);

#endif
        }
      };
    
    template<int M, int K>
      struct LineIntersectorK
      {
        typedef Vec3<vfloat<M>> Vec3vfM;
        typedef Vec4<vfloat<M>> Vec4vfM;
        
        struct Precalculations 
        {
          __forceinline Precalculations (const vbool<K>& valid, const RayK<K>& ray)
          {
            int mask = movemask(valid);
            depth_scale = rsqrt(dot(ray.dir,ray.dir));
            while (mask) {
              size_t k = __bscf(mask);
              ray_space[k] = frame(depth_scale[k]*Vec3fa(ray.dir.x[k],ray.dir.y[k],ray.dir.z[k])).transposed();
            }
          }
          
          vfloat<K> depth_scale;
          LinearSpace3<Vec3vfM> ray_space[K];
        };
        
        template<typename Epilog>
        static __forceinline bool intersect(RayK<K>& ray, size_t k, const Precalculations& pre,
                                            const vbool<M>& valid_i, const Vec4vfM& v0, const Vec4vfM& v1,
                                            const Epilog& epilog)
        {
          /* transform end points into ray space */
          const Vec3vfM ray_org(ray.org.x[k],ray.org.y[k],ray.org.z[k]);
          const Vec3vfM ray_dir(ray.dir.x[k],ray.dir.y[k],ray.dir.z[k]);
          Vec4vfM p0(xfmVector(pre.ray_space[k],v0.xyz()-ray_org), v0.w);
          Vec4vfM p1(xfmVector(pre.ray_space[k],v1.xyz()-ray_org), v1.w);
          
          /* approximative intersection with cone */
          const Vec4vfM v = p1-p0;
          const Vec4vfM w = -p0;
          const vfloat<M> d0 = w.x*v.x + w.y*v.y;
          const vfloat<M> d1 = v.x*v.x + v.y*v.y;
          const vfloat<M> u = clamp(d0*rcp(d1),vfloat<M>(zero),vfloat<M>(one));
          const Vec4vfM p = p0 + u*v;
          const vfloat<M> t = p.z*pre.depth_scale[k];
          const vfloat<M> d2 = p.x*p.x + p.y*p.y;
          const vfloat<M> r = p.w;
          const vfloat<M> r2 = r*r;
          vbool<M> valid = valid_i & d2 <= r2 & vfloat<M>(ray.tnear[k]) < t & t < vfloat<M>(ray.tfar[k]);
          if (unlikely(none(valid))) return false;
          
          /* ignore denormalized segments */
          const Vec3vfM T = v1.xyz()-v0.xyz();
          valid &= T.x != vfloat<M>(zero) | T.y != vfloat<M>(zero) | T.z != vfloat<M>(zero);
          if (unlikely(none(valid))) return false;
          
          /* update hit information */
          LineIntersectorHitM<M> hit(u,zero,t,T);
          return epilog(valid,hit);
        }
      };
  }
}
