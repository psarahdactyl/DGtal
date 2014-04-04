/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#pragma once

/**
 * @file VCMDigitalSurfaceEstimator.h
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2014/02/13
 *
 * Header file for module VCMDigitalSurfaceEstimator.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(VCMDigitalSurfaceEstimator_RECURSES)
#error Recursive header files inclusion detected in VCMDigitalSurfaceEstimator.h
#else // defined(VCMDigitalSurfaceEstimator_RECURSES)
/** Prevents recursive inclusion of headers. */
#define VCMDigitalSurfaceEstimator_RECURSES

#if !defined VCMDigitalSurfaceEstimator_h
/** Prevents repeated inclusion of headers. */
#define VCMDigitalSurfaceEstimator_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/geometry/surfaces/estimation/VoronoiCovarianceMeasureOnDigitalSurface.h"
#include "DGtal/geometry/surfaces/estimation/VCMGeometricFunctors.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

  /////////////////////////////////////////////////////////////////////////////
  // template class VCMDigitalSurfaceEstimator
  /**
   * Description of template class 'VCMDigitalSurfaceEstimator' <p>
   * \brief Aim: This class adapts a
   * VoronoiCovarianceMeasureOnDigitalSurface to be a model of
   * CSurfaceLocalGeometricEstimator. It uses the \b Voronoi \b
   * Covariance \b Measure to estimate geometric quantities. The type
   * TVCMGeometricFunctor specifies which is the estimated quantity.
   * For instance, VCMNormalVectorFunctor returns the estimated VCM
   * surface \b outward normal for given surfels.
   *
   * @tparam TDigitalSurfaceContainer the type of digital surface
   * container (model of CDigitalSurfaceContainer).
   *
   * @tparam TSeparableMetric a model of CSeparableMetric used for
   * computing the Voronoi map (e.g. Euclidean metric is
   * DGtal::ExactPredicateLpSeparableMetric<TSpace, 2> )
   *
   * @tparam TKernelFunction the type of the kernel function chi_r used
   * for integrating the VCM, a map: Point -> Scalar.
   *
   * @tparam TGeometricFunctor the type of the functor Surfel ->
   * Quantity which chooses what is the returned estimation. Any
   * VCMGeometricFunctors::VCMNormalVectorFunctor, ... is ok.
   */
  template <typename TDigitalSurfaceContainer, typename TSeparableMetric, 
            typename TKernelFunction, typename TVCMGeometricFunctor>
  class VCMDigitalSurfaceEstimator
  {
    BOOST_CONCEPT_ASSERT(( CDigitalSurfaceContainer< TDigitalSurfaceContainer > ));
    BOOST_CONCEPT_ASSERT(( CSeparableMetric<TSeparableMetric> ));

    // ----------------------- public types ------------------------------
  public:
    typedef VCMDigitalSurfaceEstimator<TDigitalSurfaceContainer, TSeparableMetric, TKernelFunction, TVCMGeometricFunctor> Self; //< my own type
    typedef TDigitalSurfaceContainer DigitalSurfaceContainer; //< the chosen container
    typedef TSeparableMetric                          Metric; //< the chosen metric
    typedef TKernelFunction                   KernelFunction; //< the kernel function
    typedef TVCMGeometricFunctor         VCMGeometricFunctor; //< the geometric functor (normal, principal directions)
    /// the type of computing the Voronoi covariance measure on a digital surface.
    typedef VoronoiCovarianceMeasureOnDigitalSurface<DigitalSurfaceContainer, Metric, KernelFunction>
    VCMOnSurface;
    typedef typename VCMOnSurface::Surface           Surface; //< the digital surface

    // ----------------------- model of CSurfaceLocalGeometricEstimator ----------------
    typedef typename Surface::Surfel                  Surfel; //< the signed surface element
    typedef typename VCMGeometricFunctor::Quantity  Quantity; //< the estimation type is given by the geometric functor
    // -----------------------               other types         -----------------------
    typedef typename Surface::SCell                    SCell; //< the signed cell
    typedef typename Surface::ConstIterator    ConstIterator; //< the iterator on surfels
    typedef typename VCMOnSurface::Scalar             Scalar; //< the "real number" type

    // ----------------------- Standard services ------------------------------
  public:

    /**
     * Destructor.
     */
    ~VCMDigitalSurfaceEstimator();

    /**
     * Default constructor.
     */
    VCMDigitalSurfaceEstimator ();

    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    VCMDigitalSurfaceEstimator( const Self& other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    Self& operator= ( const Self& other );

    /**
     * Constructor. The estimator is invalid and \ref setParams \b must be called.
     *
     * @param surface the digital surface that is aliased in this. The
     * user can \b secure the aliasing by passing a
     * CountedConstPtrOrConstPtr.
     */
    VCMDigitalSurfaceEstimator( ConstAlias< Surface > surface );

    /**
     * Constructor from VoronoiCovarianceMeasureOnDigitalSurface. The
     * estimator is immediately valid.
     *
     * @param aVCMOnSurface any valid Voronoi covariance measure,
     * which is aliased (possibly securely).
     */
    VCMDigitalSurfaceEstimator( ConstAlias< VCMOnSurface > aVCMOnSurface );

    /**
     * Attach a digital surface.
     *
     * @param surface the digital surface that is aliased in this. The
     * user can \b secure the aliasing by passing a
     * CountedConstPtrOrConstPtr.
     */
    void attach( ConstAlias<Surface> surface );

    /**
     * Initialisation of estimator specific parameters.
     *
     * @param[in] surfelEmbedding the chosen embedding for surfels.
     *
     * @param[in] R the offset radius for the set of points. Voronoi cells
     * are intersected with this offset. The unit corresponds to a step in the digital space.
     *
     * @param[in] r (an upper bound of) the radius of the support of the
     * kernel function \a chi_r (note \f$\chi_r\f$ in the VCM
     * paper). The unit corresponds to a step in the digital
     * space. This parameter is used for preparing the data structure
     * that answers to proximity queries.
     *
     * @param[in] chi_r the kernel function whose support has radius less
     * or equal to \a r.
     *
     * @param[in] t the radius for the trivial normal estimator, which is
     * used for finding the correct orientation inside/outside for the
     * VCM.
     *
     * @param[in] aMetric an instance of the metric.
     *
     * @param[in] verbose if 'true' displays information on ongoing computation.
     */
    void setParams( Surfel2PointEmbedding surfelEmbedding,
                    const Scalar R, const Scalar r, KernelFunction chi_r,
                    const Scalar t = 2.5, Metric aMetric = Metric(), bool verbose = true );

    /**
     * Model of CSurfaceLocalGeometricEstimator. Initialisation.
     * Unused. The VCM is necessarily initialized on the whole surface
     * and \a h is not used for normal estimation.
     *
     * @tparam SurfelConstIterator any model of forward readable iterator on Surfel.
     * @param[in] h grid size (must be >0).
     * @param[in] ite iterator on the first surfel of the surface.
     * @param[in] itb iterator after the last surfel of the surface.
     */
    template <typename SurfelConstIterator>
    void init( const Scalar h, 
               SurfelConstIterator itb,
               SurfelConstIterator ite );

    /**
     * @tparam SurfelConstIterator any model of forward readable iterator on Surfel.
     * @param [in] it the surfel iterator at which we evaluate the quantity (within the range given at \ref init).
     * @return the estimated quantity at *it
     */
    template <typename SurfelConstIterator>
    Quantity eval( SurfelConstIterator it ) const;

    /**
     * @return the estimated quantity in the range [itb,ite)
     * @param [in] itb starting surfel iterator (within the range given at \ref init).
     * @param [in] ite end surfel iterator (within the range given at \ref init).
     * @param [in,out] result resulting output iterator
     *
     */
    template <typename SurfelConstIterator,typename OutputIterator>
    OutputIterator eval( SurfelConstIterator itb,
                         SurfelConstIterator ite,
                         OutputIterator result ) const;

    // ----------------------- Interface --------------------------------------
  public:

    /**
     * Writes/Displays the object on an output stream.
     * @param out the output stream where the object is written.
     */
    void selfDisplay ( std::ostream & out ) const;

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool isValid() const;

    // ------------------------- Protected Datas ------------------------------
  protected:
    /// A (possibly) shared object storing the digital surface.
    CountedConstPtrOrConstPtr<Surface> mySurface;
    /// the embedding chosen for the surfels.
    Surfel2PointEmbedding mySurfelEmbedding;
    /// A (possibly) shared object storing the whole Voronoi covariance measure.
    CountedConstPtrOrConstPtr<VCMOnSurface> myVCMOnSurface;
    /// The geometric functor
    VCMGeometricFunctor myGeomFct;

    // ------------------------- Private Datas --------------------------------
  private:

    // ------------------------- Hidden services ------------------------------
  protected:

  private:

    // ------------------------- Internals ------------------------------------
  private:

  }; // end of class VCMDigitalSurfaceEstimator


  /**
   * Overloads 'operator<<' for displaying objects of class 'VCMDigitalSurfaceEstimator'.
   * @param out the output stream where the object is written.
   * @param object the object of class 'VCMDigitalSurfaceEstimator' to write.
   * @return the output stream after the writing.
   */
  template <typename TDigitalSurfaceContainer, typename TSeparableMetric, 
            typename TKernelFunction, typename TVCMGeometricFunctor>
  std::ostream&
  operator<< ( std::ostream & out, 
               const VCMDigitalSurfaceEstimator< TDigitalSurfaceContainer, TSeparableMetric, 
                                                 TKernelFunction, TVCMGeometricFunctor > & object );

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#include "DGtal/geometry/surfaces/estimation/VCMDigitalSurfaceEstimator.ih"

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined VCMDigitalSurfaceEstimator_h

#undef VCMDigitalSurfaceEstimator_RECURSES
#endif // else defined(VCMDigitalSurfaceEstimator_RECURSES)
