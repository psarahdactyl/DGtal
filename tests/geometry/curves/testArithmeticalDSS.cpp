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

/**
 * @file testArithmeticalDSS.cpp
 * @ingroup Tests
 * @author Tristan Roussillon (\c tristan.roussillon@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2013/06/28
 *
 * Functions for testing class ArithmeticalDSS.
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/kernel/CPointPredicate.h"
#include "DGtal/base/CConstBidirectionalRange.h"
#include "DGtal/geometry/curves/ArithmeticalDSS.h"
#include "DGtal/geometry/curves/ArithmeticalDSSFactory.h"
#include "DGtal/geometry/curves/StabbingLineComputer.h"
///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

///////////////////////////////////////////////////////////////////////////////
// Functions for testing class ArithmeticalDSS.
///////////////////////////////////////////////////////////////////////////////
/**
 * Test of the main operators
 * @tparam DSS a model of arithmetical DSS,
 * either naive or standard
 */
template <typename DSS>
bool mainTest()
{
  BOOST_CONCEPT_ASSERT(( concepts::CPointPredicate<DSS> ));
  BOOST_CONCEPT_ASSERT(( CConstBidirectionalRange<DSS> ));

  typedef typename DSS::Point Point;

  unsigned int nbok = 0;
  unsigned int nb = 0;

  trace.beginBlock ( "Main operators..." );

  //operateur constructeur, copie, affectation
  trace.info() << "constructor, copy, assignement, equality" << std::endl;

  DSS dss(0, 1,
	  Point(0,0), Point(1,0),
	  Point(0,0), Point(1,0),
	  Point(0,0), Point(1,0) );
  DSS dss2 = dss;
  DSS dss3(Point(0,0), Point(1,1), true);
  DSS dss4 = dss3;
  dss3 = dss2 = dss;

  //egalite, difference
  DSS dss5(0, -1,
	   Point(1,0), Point(0,0),
	   Point(1,0), Point(0,0),
	   Point(1,0), Point(0,0) );

  if ( (dss == dss2)
       &&(dss == dss3)
       &&(dss != dss4)
       &&(dss == dss5) )
    nbok++;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;

  //validite
  trace.info() << "valid dss" << std::endl;
  if ( dss.isValid() && dss3.isValid() && dss5.isValid() )
    nbok++;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;

  DSS dss6(0, 1,
	   Point(1,0), Point(0,0),
	   Point(1,0), Point(0,0),
	   Point(1,0), Point(0,0) );

  trace.info() << "not valid dss" << std::endl;
  if (!dss6.isValid())
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;

  //accessors
  trace.info() << "a,b,mu,omega accessors" << std::endl;

  if ( (dss.a() == 0)&&(dss.b() == 1)&&(dss.mu() == 0)&&(dss.omega() == 1) )
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;

  trace.info() << "points accessors" << std::endl;
  if ( (dss.front() == Point(1,0))&&(dss.back() == Point(0,0)) )
    nbok++;
  nb++;
  if ( (dss.Ul() == Point(1,0))&&(dss.Uf() == Point(0,0)) )
    nbok++;
  nb++;
  if ( (dss.Ll() == Point(1,0))&&(dss.Lf() == Point(0,0)) )
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
  	       << std::endl;

  DSS dss7(Point(0,0), Point(8,5), true);

  trace.info() << "remainder, position, tests" << std::endl;
  trace.info() << dss7 << std::endl;

  if ( (dss7.isValid())
       && (dss7.remainder( Point(8,5) ) == 0)
       &&(dss7.remainder( Point(16,10) ) == 0)
       &&(dss7.remainder( Point(3,2) ) == -1)
       &&(dss7.remainder( Point(5,3) ) == 1) )
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
  	       << std::endl;

  if ( (dss7.orthogonalPosition( Point(0,0) ) == 0)
       &&(dss7.orthogonalPosition( Point(8,5) ) == 89)
       &&(dss7.orthogonalPosition( Point(1,0) ) == 8)
       &&(dss7.orthogonalPosition( Point(-1,0) ) == -8) )
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
  	       << std::endl;

  if ( (dss7.isInDSL( Point(0,0) ))
       &&(dss7.isInDSL( Point(16,10) ))
       &&(dss7.isInDSL( Point(5,3) ))
       &&(!dss7.isInDSL( Point(3,2) )) )
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
  	       << std::endl;

  if ( (dss7( Point(0,0) ))
       &&(!dss7( Point(16,10) ))
       &&(dss7( Point(5,3) ))
       &&(!dss7( Point(3,2) ))
       &&(!dss7( Point(-1,0) )) )
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
  	       << std::endl;

  trace.info() << "shift" << std::endl;
  if (dss.remainder(dss.shift()) == dss.omega())
    nbok++;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") "
  	       << std::endl;

  trace.endBlock();

  return nbok == nb;
}


///////////////////////////////////////////////////////////////////////////////
/**
 * Test of the range services
 * @param dss an instance of DSS
 * @tparam DSS a model of arithmetical DSS,
 * either naive or standard
 */
template <typename DSS>
bool rangeTest(const DSS& dss)
{
  unsigned int nbok = 0;
  unsigned int nb = 0;

  trace.beginBlock ( "Range/Iterator services..." );
  trace.info() << dss << std::endl;

  if (dss.isValid())
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
  	       << std::endl;

  {//forward pass
    typedef typename DSS::ConstIterator I;
    BOOST_CONCEPT_ASSERT(( boost_concepts::ReadableIteratorConcept<I> ));
    BOOST_CONCEPT_ASSERT(( boost_concepts::BidirectionalTraversalConcept<I> ));
    bool res = true;
    int c = 0;
    for (I it = dss.begin(), itEnd = dss.end();
	 ( (it != itEnd)&&(res)&&(c<100) );
	 ++it, ++c)
      {
	trace.info() << *it << " ";
	if ( !dss(*it) )
	  res = false;
      }
    trace.info() << " : " << c << " points " << std::endl;
    trace.info() << std::endl;

    if ( (res)&&(c == (dss.omega()+1))
	 &&(*dss.begin() == dss.back())
	 &&(*--dss.end() == dss.front()) )
      nbok++;
    nb++;

    trace.info() << "(" << nbok << "/" << nb << ") "
		 << std::endl;
  }

  {//backward pass
    typedef typename DSS::ConstReverseIterator I;
    bool res = true;
    int c = 0;
    for (I it = dss.rbegin(), itEnd = dss.rend();
	 ( (it != itEnd)&&(res)&&(c<100) );
	 ++it, ++c)
      {
	trace.info() << *it << " ";
	if ( !dss(*it) )
	  res = false;
      }
    trace.info() << " : " << c << " points " << std::endl;
    trace.info() << std::endl;

    if ( (res)&&(c == (dss.omega()+1))
	 &&(*dss.rbegin() == dss.front())
	 &&(*--dss.rend() == dss.back()) )
      nbok++;
    nb++;

    trace.info() << "(" << nbok << "/" << nb << ") "
		 << std::endl;
  }

  trace.endBlock();

  return nbok == nb;
}

///////////////////////////////////////////////////////////////////////////////
/**
 * Unit test of the extension service
 * @param dss an instance of DSS
 * @tparam DSS a model of arithmetical DSS,
 * either naive or standard
 * @param newPointToFront point to add to the dss front
 * @param newPointToBack point to add to the dss back
 * @param nbok (returned) number of passed tests
 * @param nb (returned) number of tests
 * @param code index of the tested configuration
 */
template <typename DSS>
void extensionTest(const DSS& dss,
		   typename DSS::Point newPointToFront,
		   typename DSS::Point newPointToBack,
		   unsigned int& nbok, unsigned int& nb,
		   const unsigned short int& code = 0)
{
  trace.info() << dss << std::endl;
  if (dss.isValid())
    nbok++;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") " << std::endl;

  trace.info() << "to front " << newPointToFront << std::endl;
  if (dss.isExtendableFront( newPointToFront ) == code)
    nbok++;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") " << std::endl;

  DSS mdss = dss; //local and modifiable copy
  if (code == 0)
    {
      if ( (!mdss.extendFront(newPointToFront)) )
	nbok++;
      nb++;
    }
  else
    {
      if ( (mdss.extendFront(newPointToFront))&&(mdss.isValid()) )
	nbok++;
      nb++;
      std::cerr << mdss.isValid() << std::endl;
    }
  trace.info() << mdss << std::endl;
  trace.info() << "(" << nbok << "/" << nb << ") " << std::endl;

  trace.info() << "to back " << newPointToBack << std::endl;
  if (dss.isExtendableBack( newPointToBack ) == code)
    nbok++;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") " << std::endl;

  mdss = dss; //local and modifiable copy
  if (code == 0)
    {
      if ( (!mdss.extendBack(newPointToBack)) )
	nbok++;
      nb++;
    }
  else
    {
      if ( (mdss.extendBack(newPointToBack))&&(mdss.isValid()) )
	nbok++;
      nb++;
      std::cerr << mdss.isValid() << std::endl;
    }
  trace.info() << mdss << std::endl;
  trace.info() << "(" << nbok << "/" << nb << ") " << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
/**
 * Unit test of the retraction service
 * @param dss an instance of DSS
 * @tparam DSS a model of arithmetical DSS,
 * either naive or standard
 * @param nbok (returned) number of passed tests
 * @param nb (returned) number of tests
 * @param res result of the retraction:
 * 'true' if done, 'false' otherwise
 */
template <typename DSS>
void retractionTest(const DSS& dss,
		    unsigned int& nbok,
		    unsigned int& nb,
		    bool res = true)
{
  typedef typename DSS::Point Point;

  trace.info() << dss << std::endl;
  if (dss.isValid())
    nbok++;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") " << std::endl;

  //local and modifiable copy
  DSS mdss = dss;

  //forward test
  Point first = mdss.back();
  trace.info() << "remove " << first << std::endl;
  if ( ( (mdss.retractBack())
	 && (mdss.isValid())
	 && (mdss(first) == false) ) == res )
    nbok++;
  nb++;
  trace.info() << "(" << nbok << "/" << nb << ") " << std::endl;

  if (res)
    {
      if ( (mdss.extendBack(first))
	   && (mdss.isValid()) && (mdss == dss) )
	nbok++;
      nb++;
      trace.info() << "(" << nbok << "/" << nb << ") " << std::endl;
    }

  //backward test
  Point last = mdss.front();
  trace.info() << "remove " << last << std::endl;
  if ( ( (mdss.retractFront())
	 && (mdss.isValid())
	 && (mdss(last) == false) ) == res )
    nbok++;
  nb++;
  trace.info() << mdss << std::endl;
  trace.info() << "(" << nbok << "/" << nb << ") " << std::endl;

  if (res)
    {
      if ( (mdss.extendFront(last))
	   && (mdss.isValid()) && (mdss == dss) )
	nbok++;
      nb++;
      trace.info() << "(" << nbok << "/" << nb << ") " << std::endl;
    }

}

/**
 * Test of the update services
 * @tparam DSS a model of arithmetical DSS,
 * either naive or standard
 */
template <typename DSS>
bool updateTest()
{
  typedef typename DSS::Point Point;
  typedef typename DSS::Vector Vector;

  unsigned int nbok = 0;
  unsigned int nb = 0;

  trace.beginBlock ( "Extension services..." );

  if (nbok == nb)
    {
      trace.info() << "not connected point" << std::endl;
      DSS dss(Point(0,0), Point(8,5), true);
      extensionTest( dss, Point(9,7), Point(-2,1), nbok, nb );
    }

  if (nbok == nb)
    {
      trace.info() << "not compatible second step" << std::endl;
      DSS dss(Point(0,0), Point(1,1), true);
      extensionTest( dss, Point(0,2), Point(-1,1), nbok, nb );
    }

  if (nbok == nb)
    {
      trace.info() << "a third step" << std::endl;
      DSS dss(Point(0,0), Point(2,1), true);
      extensionTest( dss, Point(2,2), Point(0,1), nbok, nb );
    }

  if (nbok == nb)
    {
      trace.info() << "strongly exterior" << std::endl;
      DSS dss(Point(0,0), Point(8,5), true);
      extensionTest( dss, Point(9,6), Point(-1,0), nbok, nb );
    }

  if (nbok == nb)
    {
      trace.info() << "confounded points" << std::endl;
      DSS dss(Point(0,0), Point(8,5), true);
      extensionTest( dss, Point(8,5), Point(0,0), nbok, nb, 9 );
    }

  if (nbok == nb)
    {
      trace.info() << "strongly interior points" << std::endl;
      DSS dss0(Point(0,0), Point(8,5), true);
      DSS dss(5, 8, Point(-2,-2), Point(8,5),
	      dss0.Uf(), dss0.Ul(),
	      dss0.Lf(), dss0.Ll() );
      extensionTest( dss, Point(9,5), Point(-3,-2), nbok, nb, 9 );
    }

  if (nbok == nb)
    {
      trace.info() << "weakly interior points on the left" << std::endl;
      DSS dss0(Point(0,0), Point(8,5), true);
      Point newPointToBack = dss0.Lf()-Vector(8,5);
      Point newPointToFront = dss0.Ul()+Vector(8,5);
      DSS dss(5, 8,
	      newPointToBack+dss0.steps().second,
	      newPointToFront-dss0.steps().second,
	      dss0.Uf(), dss0.Ul(),
	      dss0.Lf(), dss0.Ll()+Vector(8,5) );
      extensionTest( dss, newPointToFront, newPointToBack, nbok, nb, 5 );
    }

  if (nbok == nb)
    {
      trace.info() << "weakly exterior points on the left" << std::endl;
      DSS dss0(Point(0,0), Point(8,5), true);
      Point newPointToBack = dss0.Uf()+dss0.shift()-Vector(8,5);
      Point newPointToFront = dss0.Ll()-dss0.shift()+Vector(8,5);
      DSS dss(5, 8,
	      newPointToBack+dss0.steps().second,
	      newPointToFront-dss0.steps().second,
	      dss0.Uf(), dss0.Ul(),
	      dss0.Lf()-Vector(8,5), dss0.Ll() );
      extensionTest( dss, newPointToFront, newPointToBack, nbok, nb, 7 );
    }

  if (nbok == nb)
    {
      trace.info() << "weakly interior points on the right" << std::endl;
      DSS dss0(Point(0,0), Point(8,5), true);
      Point newPointToBack = dss0.Uf()-Vector(8,5);
      Point newPointToFront = dss0.Ll()+Vector(8,5);
      DSS dss(5, 8,
	      newPointToBack+dss0.steps().first,
	      newPointToFront-dss0.steps().first,
	      dss0.Uf(), dss0.Ul(),
	      dss0.Lf()-Vector(8,5), dss0.Ll() );
      extensionTest( dss, newPointToFront, newPointToBack, nbok, nb, 6 );
    }

  if (nbok == nb)
    {
      trace.info() << "weakly exterior points on the right" << std::endl;
      DSS dss0(Point(0,0), Point(8,5), true);
      Point newPointToBack = dss0.Lf()-Vector(8,5)-dss0.shift();
      Point newPointToFront = dss0.Ul()+Vector(8,5)+dss0.shift();
      DSS dss(5, 8,
	      newPointToBack+dss0.steps().first,
	      newPointToFront-dss0.steps().first,
	      dss0.Uf(), dss0.Ul(),
	      dss0.Lf(), dss0.Ll()+Vector(8,5) );
      extensionTest( dss, newPointToFront, newPointToBack, nbok, nb, 8 );
    }

  if (nbok == nb)
    {
      trace.info() << "first step" << std::endl;
      DSS dss( Point(0,0), Point(0,0) );
      extensionTest( dss, Point(1,0), Point(-1,0), nbok, nb, 1 );
    }

  if (nbok == nb)
    {
      trace.info() << "first step repetition" << std::endl;
      DSS dss(Point(0,0), Point(1,0), true);
      extensionTest( dss, Point(2,0), Point(-1,0), nbok, nb, 2 );
    }

  if (nbok == nb)
    {
      trace.info() << "second step (above)" << std::endl;
      DSS dss0(Point(0,0), Point(2,1), true);
      DSS dss(Point(0,0), Point(2,1) - dss0.steps().second);
      Point newPointToBack = Point(0,0) - dss0.steps().second;
      extensionTest( dss, Point(2,1), newPointToBack, nbok, nb, 3 );
    }

  if (nbok == nb)
    {
      trace.info() << "second step (below)" << std::endl;
      DSS dss0a(Point(0,0), Point(2,-1), true);
      DSS dss0b(Point(0,0), Point(2,1), true);
      DSS dss(Point(0,0), Point(2,-1) - dss0a.steps().first);
      Point newPointToBack = Point(0,0) - dss0a.steps().first;
      extensionTest( dss, Point(2,-1), newPointToBack, nbok, nb, 4 );
    }

  trace.endBlock();

  if (nbok == nb)
    {
      trace.beginBlock ( "Retraction services..." );

      {
	trace.info() << "upper leaning points" << std::endl;
	DSS dss(Point(0,0), Point(8,5), true);
	retractionTest( dss, nbok, nb );
      }

      if (nbok == nb)
	{
	  trace.info() << "lower leaning points" << std::endl;
	  DSS dss0(Point(0,0), Point(8,5), true);
	  Point first = dss0.Lf();
	  Point last = dss0.Lf() + Vector(8,5);
	  DSS dss(5, 8, first, last,
		  Point(8,5), Point(8,5),
		  first, last );
	  retractionTest( dss, nbok, nb );
	}

      if (nbok == nb)
	{
	  trace.info() << "upper leaning points (repetitions)" << std::endl;
	  DSS dss(Point(0,0), Point(16,10), true);
	  retractionTest( dss, nbok, nb );
	}

      if (nbok == nb)
	{
	  trace.info() << "lower leaning points (repetitions)" << std::endl;
	  DSS dss0(Point(0,0), Point(16,10), true);
	  Point first = dss0.Lf();
	  Point last = dss0.Lf() + Vector(16,10);
	  DSS dss(5, 8, first, last,
		  Point(8,5), Point(16,10),
		  first, last );
	  retractionTest( dss, nbok, nb );
	}

      if (nbok == nb)
	{
	  trace.info() << "no change" << std::endl;
	  DSS dss0(Point(0,0), Point(21,13), true);
	  typename DSS::ConstIterator itb = dss0.begin();
	  --itb; --itb; --itb;
	  typename DSS::ConstIterator ite = dss0.end();
	  ++ite; ++ite; ++ite;
	  DSS dss(dss0.a(), dss0.b(), *itb, *ite,
		  dss0.Uf(), dss0.Ul(), dss0.Lf(), dss0.Ll() );
	  retractionTest( dss, nbok, nb );
	}

      if (nbok == nb)
	{
	  trace.info() << "one point" << std::endl;
	  DSS dss(Point(0,0), Point(0,0), true);
	  retractionTest( dss, nbok, nb, false );
	}

      if (nbok == nb)
	{
	  trace.info() << "two points" << std::endl;
	  DSS dss(Point(0,0), Point(1,0), true);
	  retractionTest( dss, nbok, nb );
	}

      if (nbok == nb)
	{
	  trace.info() << "from two steps to one step" << std::endl;
	  DSS dss(Point(0,0), Point(1,1), true);
	  retractionTest( dss, nbok, nb );
	}


      trace.endBlock();
    }

  return nbok == nb;
}

/**
 * Test of the directional position
 * and the checks of the steps
 * @param dss an instance of DSS
 * @tparam DSS a model of arithmetical DSS,
 * either naive or standard
 */
template <typename DSS>
bool compatibleStepsTest(const DSS& dss)
{
  unsigned int nbok = 0;
  unsigned int nb = 0;

  trace.beginBlock ( "directional Position..." );

  trace.info() << "shift: " << dss.shift()
	       << ", front pos: " << dss.position( dss.front() )
	       << ", back pos:  " << dss.position( dss.back() ) << std::endl;
  if ( dss.position( dss.front() )
       > dss.position( dss.back() ) )
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;

  trace.endBlock();

  trace.beginBlock ( "Compatible steps..." );

  ////////////////// forward extension
  DSS mdss = dss;
  if ( mdss.extendFront(mdss.front()-dss.shift()+dss.steps().first) )
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;
  mdss = dss;
  if ( !mdss.extendFront(mdss.front()-dss.shift()) )
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;

  mdss = dss;
  if ( !mdss.extendFront(mdss.front()-dss.shift()-dss.steps().first) )
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;

  ////////////////// backward extension
  mdss = dss;
  if ( mdss.extendBack(mdss.back()+dss.shift()-dss.steps().first) )
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;
  mdss = dss;
  if ( !mdss.extendBack(mdss.back()+dss.shift()) )
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;

  mdss = dss;
  if ( !mdss.extendBack(mdss.back()+dss.shift()+dss.steps().first) )
    nbok++;
  nb++;

  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;

  trace.endBlock();

  return nbok == nb;
}

/**
 * Test of the main constructors
 * @tparam DSS a model of arithmetical DSS,
 * either naive or standard
 */
template <typename DSS>
bool constructorsTest()
{
  BOOST_CONCEPT_ASSERT(( concepts::CPointPredicate<DSS> ));
  BOOST_CONCEPT_ASSERT(( CConstBidirectionalRange<DSS> ));

  typedef typename DSS::Point Point;

  unsigned int nbok = 0;
  unsigned int nb = 0;

  trace.beginBlock ( "constructors..." );

  {
    //pattern
    DSS dss0( Point(0,0), Point(8,5) );
    trace.info() << dss0 << std::endl;

    //construction by points range
    DSS dss( dss0.begin(), dss0.end() );
    trace.info() << dss << std::endl;

    if ( (dss0.isValid())
	 &&(dss.isValid())
	 && (dss0 == dss)
	 && (dss.Lf() == dss.Ll())
	 && (dss.Uf() == dss.back())
	 && (dss.Ul() == dss.front())
	 && (dss.back() != dss.front()) )
      nbok++;
    nb++;
    trace.info() << "(" << nbok << "/" << nb << ") "
		 << std::endl;

    //reversed pattern
    DSS rdss0( Point(0,0), Point(8,5), false );
    trace.info() << rdss0 << std::endl;

    //construction by points range
    DSS rdss( rdss0.begin(), rdss0.end() );
    trace.info() << rdss << std::endl;

    if ( (rdss0.isValid())
	 &&(rdss.isValid())
	 && (rdss0 == rdss)
	 && (rdss.Uf() == rdss.Ul())
	 && (rdss.Lf() == rdss.back())
	 && (rdss.Ll() == rdss.front())
	 && (rdss.back() != rdss.front())
	 && (rdss != dss) )
      nbok++;
    nb++;
    trace.info() << "(" << nbok << "/" << nb << ") "
		 << std::endl;

  }

  trace.endBlock();

  return nbok == nb;
}

/**
 * Compares smartCH and reversedSmartCH to 
 * the classical incremental recognition algorithm 
 * for one subgement of a greater DSS
 * @param aDSS DSS containing the subsegment
 * @param x minimal position
 * @param y maximal position
 * @return 'true' if results match, 'false' otherwise
 * @tparam DSS an arithmetical DSS (either naive or standard)
 */
template <typename DSS>
bool comparisonSubsegment(const DSS& aDSS, 
			  typename DSS::Position x, typename DSS::Position y)
{
  typename DSS::DSL dsl = aDSS.dsl(); 
  DSS dss0( dsl.begin(dsl.getPoint(x)), dsl.end(dsl.getPoint(y)) ); //classical (linear-time)
  DSS dss1( dsl, dsl.getPoint(x), dsl.getPoint(y) ); //smartCH (log)
  DSS dss2( aDSS, dsl.getPoint(x), dsl.getPoint(y) ); //reversedSmartCH (log)
  return ( (dss0 == dss1)&&(dss0 == dss2) ); 
}

/**
 * Compares smartCH and reversedSmartCH to 
 * the classical incremental recognition algorithm 
 * for various intercepts and lengths
 * @param a numerator of the slope
 * @param b denominator of the slope
 * @return 'true' if the two algorithms return the same results, 
 * 'false' otherwise
 * @tparam DSL an arithmetical DSL (either naive or standard)
 */
template <typename DSL>
bool comparisonSubsegment(typename DSL::Coordinate a, typename DSL::Coordinate b)
{
  unsigned int nbok = 0;
  unsigned int nb = 0;

  trace.beginBlock ( "Subsegment comparison ..." );

  DSL aDSL(a, b, 0); 
  for (typename DSL::Integer mu = 0; ( (mu-1 >= -aDSL.omega())&&(nbok == nb) ); --mu)
    {
      trace.info() << "mu=" << mu << std::endl; 

      typedef typename DSL::Point Point; 
      typedef typename DSL::Coordinate Coordinate; 
      typedef typename DSL::Integer Integer; 
      typedef ArithmeticalDSS<Coordinate,Integer,DSL::foregroundAdjacency> DSS; 

      Point startingPoint = aDSL.getPoint(0); 
      ASSERT( aDSL(startingPoint) ); 
      Point endingPoint = aDSL.getPoint(2*aDSL.patternLength()+1); 
      ASSERT( aDSL(endingPoint) ); 

      DSS dss = DSS(aDSL.begin(startingPoint), aDSL.end(endingPoint)); 

      for (typename DSL::Position l = 0; ( (l <= 2*aDSL.patternLength())&&(nbok == nb) ); ++l)
	{
	  trace.info() << "l=" << l << std::endl; 

	  for (typename DSL::Position k = 0; ( (k <= l)&&(nbok == nb) ); ++k)
	    {
	      trace.info() << "k=" << k << std::endl; 

	      if (comparisonSubsegment(dss, k, l))
		nbok++;
	      nb++; 

	    }
	}

    }

  trace.endBlock(); 

  return (nb == nbok); 
}

//---------------------------------------------------------------------------
bool unionTest()
{
  unsigned int nb = 0;
  unsigned int nbok = 0;
  
  typedef DGtal::ArithmeticalDSS<int32_t,int32_t,8> DSS;
  typedef typename DSS::Point Point;

  trace.beginBlock("Testing union of two DSSs");

  // Different tests to cover all possible configurations
  // to be tested for different octants
  
  //-------------------------------------------------
  //---------- Union is part of a DSL----------------

  // DSS1 included in DSS2
  
  trace.beginBlock("Simplest inclusion: DSS1 in DSS2");
  // octant 0
  trace.info() << "octant 0\n";
  DSS DSS1(1,2,Point(2,2),Point(6,4),Point(2,2),Point(6,4),Point(3,2),Point(5,3));
  DSS DSS2(3,5,Point(-2,-1),Point(9,6),Point(2,2),Point(7,5),Point(0,0),Point(5,3));
  DSS res=DSS1.Union(DSS2);
  nb++;
  nbok +=(res==DSS2)?1:0;
  
  // octant 1
  trace.info() << "octant 1\n";
  DSS1 = DSS(2,1,Point(2,2),Point(4,6),Point(2,3),Point(3,5),Point(2,2),Point(4,6));
  assert(DSS1.isValid());
  DSS2 = DSS(5,3,Point(-1,-2),Point(6,9),Point(0,0),Point(3,5),Point(2,2),Point(5,7));
  assert(DSS2.isValid());
  res = DSS1.Union(DSS2);
  nb++;
  nbok +=(res==DSS2)?1:0;

  // octant 2
  trace.info() << "octant 2\n";
  DSS1 = DSS(2,-1,Point(-2,2),Point(-4,6),Point(-2,2),Point(-4,6),Point(-2,3),Point(-3,5));
  assert(DSS1.isValid());
  DSS2 = DSS(5,-3,Point(1,-2),Point(-6,9),Point(-2,2),Point(-5,7),Point(0,0),Point(-3,5));
  assert(DSS2.isValid());
  res = DSS1.Union(DSS2);
  nb++;
  nbok +=(res==DSS2)?1:0;

  // octant 3
  trace.info() << "octant 3\n";
  DSS1 = DSS(1,-2,Point(-2,2),Point(-6,4),Point(-3,2),Point(-5,3),Point(-2,2),Point(-6,4));
  assert(DSS1.isValid());
  DSS2 = DSS(3,-5,Point(2,-1),Point(-9,6),Point(0,0),Point(-5,3),Point(-2,2),Point(-7,5));
  assert(DSS2.isValid());
  res = DSS1.Union(DSS2);
  nb++;
  nbok +=(res==DSS2)?1:0;

  // octant 4
  trace.info() << "octant 4\n";
  DSS1 = DSS(-1,-2,Point(-2,-2),Point(-6,-4),Point(-2,-2),Point(-6,-4),Point(-3,-2),Point(-5,-3));
  assert(DSS1.isValid());
  DSS2 = DSS(-3,-5,Point(2,1),Point(-9,-6),Point(-2,-2),Point(-7,-5),Point(0,0),Point(-5,-3));
  assert(DSS2.isValid());
  res = DSS1.Union(DSS2);
  nb++;
  nbok +=(res==DSS2)?1:0;

  // octant 5 - take octant3 - DSS(a,b...) -> DSS(-a,b...) + Point(x,y) -> Point(x,-y) + inverse lower and upper leaning points

  trace.endBlock();
  
  // DSS2 included in DSS1 + see unionComparisonTest below
  trace.beginBlock("Simplest inclusion: DSS2 in DSS1");
  // octant 0
  trace.info() << "octant 0\n";
  DSS1 = DSS(3,5,Point(-2,-1),Point(9,6),Point(2,2),Point(7,5),Point(0,0),Point(5,3));
  DSS2 = DSS(1,2,Point(2,2),Point(6,4),Point(2,2),Point(6,4),Point(3,2),Point(5,3));
  res = DSS1.Union(DSS2);
  nb++;
  nbok +=(res==DSS1)?1:0;
  
  trace.endBlock();
  
  // DSS2 belongs to DSS1's supporting DSL

  trace.beginBlock("DSS2 belongs to DSS1's supporting DSL");
  // octant 0
  trace.info() << "octant 0 - no new leaning points\n";
  DSS1 = DSS(3,7,Point(1,3),Point(12,7),Point(3,4),Point(10,7),Point(5,4),Point(12,7));
  DSS2 = DSS(1,2,Point(14,8),Point(16,9),Point(15,9),Point(15,9),Point(14,8),Point(16,9));
  res = DSS1.Union(DSS2);
  nb++;
  nbok +=(res==DSS(3,7,Point(1,3),Point(16,9),Point(3,4),Point(10,7),Point(5,4),Point(12,7)))?1:0;
  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;

  trace.info() << "octant 0 - new leaning points in DSS2\n";
  DSS1 = DSS(3,7,Point(1,3),Point(10,7),Point(3,4),Point(10,7),Point(5,4),Point(5,4));
  DSS2 = DSS(1,2,Point(12,7),Point(17,10),Point(13,8),Point(17,10),Point(12,7),Point(16,9));
  res = DSS1.Union(DSS2);
  nb++;
  nbok +=(res==DSS(3,7,Point(1,3),Point(17,10),Point(3,4),Point(17,10),Point(5,4),Point(12,7)))?1:0;
  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;
  
  trace.info() << "octant 0 - new leaning points between DSS1 and DSS2\n";
  DSS1 = DSS(3,7,Point(1,3),Point(10,7),Point(3,4),Point(10,7),Point(5,4),Point(5,4));
  DSS2 = DSS(1,2,Point(13,8),Point(15,9),Point(13,8),Point(15,9),Point(14,8),Point(14,8));
  res = DSS1.Union(DSS2);
  nb++;
  nbok +=(res==DSS(3,7,Point(1,3),Point(15,9),Point(3,4),Point(10,7),Point(5,4),Point(12,7)))?1:0;
  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;
  
  trace.endBlock();
  
  // DSS1 belongs to DSS2 supporting DSL

  // DSS1 and DSS2 connected and union is part of a DSL -> see
  // unionComparisonTest below
  
  // DSS1 and DSS2 not connected but easy case and union is part of a
  // DSL -> see unionComparisonTest below
  
  // DSS1 and DSS2 not connected and union is part of a DSL
  

  //-------------------------------------------------
  //---------- Union is not part of a DSL -----------
  
  // DSS1 and DSS2 not in the same octant
  
  // DSS1 and DSS2 connected and union is not part of a DSL
  
  // DSS1 and DSS2 not connected but easy case and union is part of a DSL
  
  // DSS1 and DSS2 not connected and union is not part of a DSL
  
  trace.endBlock();
  
  return (nb==nbok);
  
}




template <typename ConstPairIterator, typename DSS>
DSS computeMinDSSFromPreimage(typename StabbingLineComputer<ConstPairIterator>::PreimagePtr P, typename DSS::Point first, typename DSS::Point last)
{
  typedef typename StabbingLineComputer<ConstPairIterator>::Preimage::Container Hull; 
  Hull pH, qH;
  
  pH = P->pHull();
  qH = P->qHull();
  
  typedef typename DSS::Point Point;
  typedef typename DSS::Integer Integer;
  typedef typename DSS::Vector Vector;
  
  
  Point Uf = P->Uf(); // = pH.rbegin()
  Point Ul = P->Ul(); // = pH.begin()
  Point Lf = P->Lf(); // = qH.rbegin()
  Point Ll = P->Ll(); // = qH.begin()
  
  Point rUf,rUl,rLf,rLl;
  
  // std::cout << Uf << " " << Ul << " " << Lf <<  " " << Ll << std::endl;
  
  Vector Vlow = Lf - Ul;
  Vector Vup = Uf - Ll;
  
  typename Hull::iterator it;
  typename Hull::reverse_iterator rit;
  
  /***********************/
  /// Ul
  
  Point cur, next;
  bool ok = true;
  it = pH.begin();
  cur = *it;
  //std::cout << "first = " << cur;
  while(it != pH.end() && ok)
    {
      if(++it !=pH.end())
	{
	  next = *(it);
	  //std::cout << "next = " << next;
	  if(determinant(next-cur,Vlow)!=0)
	    ok = false;
	  else
	    cur = next;
	}
      else
	ok = false;
    }
  rUl = cur;
  //std::cout << rUl;
  /*************************/
  /// Uf

  ok = true;
  rit = pH.rbegin();
  //  std::cout << "first = " << *rit;
  cur = *rit;
  while(rit != pH.rend() && ok)
    {
      if(++rit !=pH.rend())
	{
	  next = *(rit);
	  //std::cout << "next =" << next;
	  if(determinant(cur-next,Vup)!=0)
	    ok = false;
	  else
	    cur = next;
	}
      else
	ok = false;
    }
  rUf = cur;
  //std::cout << rUf;
  /***********************/
  /// Ll
  
  it = qH.begin();
  cur = *it;
  ok = true;
  while(it != qH.end() && ok)
    {
      if(++it != qH.end())
	{
	  next = *(it);
	  if(determinant(next-cur,Vup)!=0)
	    ok = false;
	  else
	    cur = next;
	}
      else
	ok = false;
    }
  rLl = cur;

  /*************************/
  /// Lf

  ok = true;
  rit = qH.rbegin();
  cur = *rit;
  while(rit != pH.rend() && ok)
    {
      if(++rit != pH.rend())
	{
	  next = *rit;
	  if(determinant(cur-next,Vlow)!=0)
	    ok = false;
	  else
	    cur = next;
	}
      else
	ok = false;
    }
  rLf = cur;

  /****************************/
  
  Integer a, b;
  if(rUl != rUf)
    {
      a = rUf[1]-rUl[1];
      b = rUf[0]-rUl[0];
    }
  else
    {
      a = rLf[1]-rLl[1];
      b = rLf[0]-rLl[0];
    }
  
  IntegerComputer<Integer> ic;
  Integer g = ic.gcd(a,b);
  a = a/g;
  b = b/g;
		 
  DSS resDSS(a,b,first, last, rLl, rLf, rUl+Point(0,-1), rUf+Point(0,-1));
  return resDSS;

}




// Test of the union of DSSs in the easy (connected or first point of
// DSS2 and last point of DSS1 have the same ordinate) and inclusion cases
// - compare the result with ArithmeticalDSS recognition algorithm
bool unionComparisonTest(int modb, int modx, unsigned int nbtries)
{
  unsigned int nb = 0;
  unsigned int nbok = 0;
  
  typedef DGtal::ArithmeticalDSS<int32_t,int32_t,8> DSS;
  typedef typename DSS::DSL DSL;
  typedef typename DSS::Point Point;
  typedef typename DSS::Integer Integer;
  
  DGtal::IntegerComputer<Integer> ic;

  trace.beginBlock("Testing results wrt Arithmetical DSS recognition algorithm");

  for ( unsigned int i = 0; i < nbtries; ++i )
    {
      // Pick up a random DSL slope
      Integer b( random() % modb + 1 );
      Integer a( random() % b +1);
      while(ic.gcd(a,b) !=1)
	a =random()%b +1; // |a| < |b|
      
      // Pick-up random signs for a and b
      // a = a*((random()%2==0)?1:-1);
      // b = b*((random()%2==0)?1:-1);

      if ( ic.gcd( a, b ) == 1 )
        {
	  
          for ( unsigned int j = 0; j < 5; ++j )
            {
	      // Pick up the DSL intercept
              Integer mu = random() % (2*modb);
	      DSL baseDSL(a,b,-mu);
	      
	      for (Integer x = 0; x < 10; ++x )
                {
		  // elemMove equals 1 or -1
		  Integer elemMove = ((baseDSL.steps()).first)[0]; 
		  // modx modulates the length of the subsegments
		  
		  // Pick up the beginning of the first subsegment
		  Integer x1 = random() % modx;
		  // Pick up the end of the first subsegment
		  Integer x2 = x1 + (modx + (random() % modx))*elemMove;
		  
		  /************************************************/
		  // Connected DSSs: The beginning of the second
		  //subsegment is randomly set between x1 and x2 or just
		  //after x2. 
		  //Integer x3 = x2+1*elemMove;
		  Integer x3 = x1 + (random() % (x2-x1+b))*elemMove;
		  
		  // The length of the second segment is set to modx
		  Integer x4 = x3 + modx*elemMove;
		  
		  Integer y1,y2,y3,y4;
		  if(baseDSL.shift()[1] < 0)
		    {
		       y1 = ic.floorDiv(a*x1+mu,b);  y2 = ic.floorDiv(a*x2+mu,b);
		       y3 = ic.floorDiv(a*x3+mu,b);  y4 = ic.floorDiv(a*x4+mu,b);
		    }
		  else
		    {
		       y1 = ic.ceilDiv(a*x1+mu,b);  y2 = ic.ceilDiv(a*x2+mu,b);
		       y3 = ic.ceilDiv(a*x3+mu,b);  y4 = ic.ceilDiv(a*x4+mu,b);
		    }
		  
		  Point A,B,C,D;
		  DSL aDSL(baseDSL);
		  //Randomly switch a and b to cover cases where |a| > |b|
		  if(random()%2)
		    {
		      aDSL = DSL(b,-a,-mu);
		      A = Point(-y1,x1); B = Point(-y2,x2);
		      C = Point(-y3,x3); D = Point(-y4,x4);
		    }
		  else
		    {
		      A = Point(x1,y1); B = Point(x2,y2);
		      C = Point(x3,y3); D = Point(x4,y4);
		    }
		  
		  
		  // Computation of the parameters of the two segments
		  // using the subsegment algorithm of [Roussillon,
		  // 2014] 
		  
		  DSS DSS1(aDSL,A,B);
		  DSS DSS2(aDSL,C,D);

		  //std::cout << DSS1 << "\n" << DSS2 << std::endl << "--------" << std::endl;
		  
		  // Treat easy cases only for comparisons with
		  // Arithmetical DSS recognition algorithm
		  if(aDSL.beforeOrEqual(C,B) || ic.dotProduct(C-B,aDSL.shift())==0)
		    {
		      nb++;
		      // Computation of DSS1 \cup DSS2 using the union algorithm [Sivignon, 2014]
		      DSS DSSres = DSS1.Union(DSS2);
		      //std::cout << "------------------\n";
		      
		      // Computation of DSS1 \cup DSS2 using the
		      // Arithmetical DSS algorithm: add points from B++
		      // until D
		      DSS DSSGroundTruth(DSS1);
		      if(aDSL.before(B,D)) // otherwise [CD] is included
					   // in [AB]
			{
			  typename DSS::ConstIterator itbegin = aDSL.begin(B);
			  typename DSS::ConstIterator itend = aDSL.end(D);
			  typename DSS::ConstIterator it = itbegin++;
			  while(it != itend)
			    {
			      DSSGroundTruth.extendFront(*it);
			      it++;
			    }
			}
		      
		      if(DSSres != DSSGroundTruth)
			{
			
			  std::cout << "DSS1 " << DSS1 << "\n" << "DSS2 " << DSS2 << std::endl; 
			  std::cout << DSSres << std::endl;
			  std::cout << DSSGroundTruth << std::endl;
			  std::cout << "------------------\n";
			}
		      nbok+=(DSSres == DSSGroundTruth)?1:0;
		    }
		  else
		    // TODO: comparison with stabbing line computer
		    {
		      DSS DSSres = DSS1.Union(DSS2);
		      
		      typedef pair<Point,Point> Pair;
		      typedef std::vector< Pair > PairContainer;
		      // Iterator on the container
		      typedef PairContainer::const_iterator ConstPairIterator;
		      
		      // Build the "contour" as the concatenation of DSS1 and
		      // the leaning points of DSS2
		      PairContainer contour;
		      DSS::ConstIterator itbegin = aDSL.begin(A);
		      DSS::ConstIterator itend = aDSL.end(B);
		      DSS::ConstIterator it;
		      it = itbegin;
		      while(it!=itend)
			{
			  Pair p = make_pair(*it-aDSL.shift(),*it);
			  contour.push_back(p);
			  ++it;
			}
		      
		      itbegin = aDSL.begin(C);
		      itend = aDSL.end(D);
		      it = itbegin;
		      while(it!= itend)
			{
			  if(*it == DSS2.Uf() || *it == DSS2.Ul() || *it == DSS2.Lf() || *it == DSS2.Ll())
			    {
			      Pair p = make_pair(*it-aDSL.shift(),*it);
			      contour.push_back(p);
			    }
			  ++it;
			}
		      
		      StabbingLineComputer<ConstPairIterator> s; 
		      
		      // //extension
		      s.init( contour.begin() );
		      bool ok=true; bool Clock =false;
		      while ( s.end() != contour.end() && s.extendFront())
		      	{}
		      
		      DSS DSSStab(DSS1);
		      
		      DSSStab = computeMinDSSFromPreimage<ConstPairIterator,DSS>(s.getPreimage(),DSS1.back(),DSS2.front());
		      
		      

		    }
		  //  std::cout << "---------------------------" << std::endl;
		}
	      
	    }
	}
    }
 
  trace.info() << "(" << nbok << "/" << nb << ") "
	       << std::endl;
  trace.endBlock();
  return (nb==nbok);
}



//---------------------------------------------------------------------------
bool createDSSTest()
{
  unsigned int nb = 0;
  unsigned int nbok = 0;
  
  trace.beginBlock("Testing creation of a DSS from direction vector and one upper leaning point");
  
  typedef DGtal::ArithmeticalDSS<DGtal::int32_t> DSS8;
  typedef DSS8::Point Point;
  typedef DGtal::ArithmeticalDSSFactory<DGtal::int32_t> Factory;
  nb++;
  nbok += (Factory::createDSS(3,5,Point(-6,-4),Point(14,8),Point(5,3)) == DSS8(3,5,Point(-6,-4),Point(14,8),Point(-5,-3),Point(10,6),Point(-2,-2),Point(13,7)))?1:0;

  nb++;
  nbok += (Factory::createDSS(3,5,Point(0,0),Point(14,8),Point(3,2)) == DSS8(3,5,Point(0,0),Point(14,8),Point(3,2),Point(13,8),Point(1,0),Point(11,6)))?1:0;

  nb++;
  nbok += (Factory::createDSS(3,5,Point(-3,-2),Point(14,8),Point(3,2)) == DSS8(3,5,Point(-3,-2),Point(14,8),Point(-2,-1),Point(13,8),Point(1,0),Point(11,6)))?1:0;

  nb++;
  nbok += (Factory::createDSS(3,5,Point(0,0),Point(14,8),Point(3,2)) == DSS8(3,5,Point(0,0),Point(14,8),Point(3,2),Point(13,8),Point(1,0),Point(11,6)))?1:0;

  nb++;
  nbok += (Factory::createDSS(3,5,Point(0,0),Point(14,8),Point(3,2)) == DSS8(3,5,Point(0,0),Point(14,8),Point(3,2),Point(13,8),Point(1,0),Point(11,6)))?1:0;
								   
  trace.endBlock();
 
  return (nb==nbok);
}



///////////////////////////////////////////////////////////////////////////////
int main( int argc, char** argv )
{
  trace.beginBlock ( "Testing class ArithmeticalDSS" );
  trace.info() << "Args:";
  for ( int i = 0; i < argc; ++i )
    trace.info() << " " << argv[ i ];
  trace.info() << endl;

  //main operators
  bool res = mainTest<DGtal::ArithmeticalDSS<DGtal::int32_t> >()
#ifdef WITH_BIGINTEGER
    && mainTest<DGtal::ArithmeticalDSS<DGtal::int32_t, DGtal::BigInteger, 4> >()
#endif
    && mainTest<DGtal::NaiveDSS8<DGtal::int32_t> >()
    && mainTest<DGtal::StandardDSS4<DGtal::int32_t> >()
    ;

  {   //range services for 8 adjacency
    typedef DGtal::ArithmeticalDSS<DGtal::int32_t> DSS8;
    typedef DGtal::ArithmeticalDSSFactory<DGtal::int32_t> Factory;
    typedef DSS8::Point Point;

    res = res
      && rangeTest( Factory::createPattern(Point(0,0), Point(8,5)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(5,8)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(-5,8)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(-8,5)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(-8,-5)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(-5,-8)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(5,-8)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(8,-5)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(1,0)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(-1,0)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(0,1)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(0,-1)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(1,1)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(-1,1)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(1,-1)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(-1,-1)) )
      && rangeTest( Factory::createReversedPattern(Point(0,0), Point(8,5)) )
      && rangeTest( Factory::createReversedPattern(Point(0,0), Point(5,8)) )
      ;
  }


  {  //range services for 4 adjacency
    typedef DGtal::ArithmeticalDSS<DGtal::int32_t, DGtal::int32_t, 4> DSS4;
    typedef DGtal::ArithmeticalDSSFactory<DGtal::int32_t, DGtal::int32_t, 4> Factory;
    typedef DSS4::Point Point;

    res = res
      && rangeTest( Factory::createPattern(Point(0,0), Point(8,5)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(5,8)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(-8,-5)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(-5,-8)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(5,-8)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(8,-5)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(1,0)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(-1,0)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(0,1)) )
      && rangeTest( Factory::createPattern(Point(0,0), Point(0,-1)) )
      && rangeTest( Factory::createReversedPattern(Point(0,0), Point(8,5)) )
      ;
  }

  {
    typedef DGtal::ArithmeticalDSS<DGtal::int32_t> DSS8;
    typedef DGtal::ArithmeticalDSSFactory<DGtal::int32_t> Factory;
    typedef DSS8::Point Point;
    res = res
      && compatibleStepsTest( Factory::createPattern(Point(0,0), Point(5,0)) )
      && compatibleStepsTest( Factory::createPattern(Point(0,0), Point(-5,0)) )
      && compatibleStepsTest( Factory::createPattern(Point(0,0), Point(0,5)) )
      && compatibleStepsTest( Factory::createPattern(Point(0,0), Point(0,-5)) )
      && compatibleStepsTest( Factory::createPattern(Point(0,0), Point(5,5)) )
      && compatibleStepsTest( Factory::createPattern(Point(0,0), Point(5,-5)) )
      && compatibleStepsTest( Factory::createPattern(Point(0,0), Point(-5,5)) )
      && compatibleStepsTest( Factory::createPattern(Point(0,0), Point(-5,-5)) )
      ;
  }

  {
    typedef DGtal::ArithmeticalDSS<DGtal::int32_t, DGtal::int32_t, 4> DSS4;
    typedef DGtal::ArithmeticalDSSFactory<DGtal::int32_t, DGtal::int32_t, 4> Factory;
    typedef DSS4::Point Point;
    res = res
      && compatibleStepsTest( Factory::createPattern(Point(0,0), Point(5,0)) )
      && compatibleStepsTest( Factory::createPattern(Point(0,0), Point(-5,0)) )
      && compatibleStepsTest( Factory::createPattern(Point(0,0), Point(0,5)) )
      && compatibleStepsTest( Factory::createPattern(Point(0,0), Point(0,-5)) )
      ;
  }

  res = res
    && updateTest<DGtal::ArithmeticalDSS<DGtal::int32_t> >()
#ifdef WITH_BIGINTEGER
    && updateTest<DGtal::ArithmeticalDSS<DGtal::int32_t, DGtal::BigInteger, 4> >()
#endif
    ;

  res = res
    && constructorsTest<DGtal::ArithmeticalDSS<DGtal::int32_t> >()
#ifdef WITH_BIGINTEGER
    && constructorsTest<DGtal::ArithmeticalDSS<DGtal::int32_t, DGtal::BigInteger, 4> >()
#endif
    && constructorsTest<DGtal::NaiveDSS8<DGtal::int32_t> >()
    && constructorsTest<DGtal::StandardDSS4<DGtal::int32_t> >()
    ;

//   {   //subsegment 
//   res = res 
//     && comparisonSubsegment<NaiveDSL<DGtal::int32_t> >(5,8)
//     && comparisonSubsegment<NaiveDSL<DGtal::int32_t> >(8,13)
//     && comparisonSubsegment<NaiveDSL<DGtal::int32_t> >(12,29)
//     && comparisonSubsegment<NaiveDSL<DGtal::int32_t> >(8,5)
//     && comparisonSubsegment<NaiveDSL<DGtal::int32_t> >(-5,8)
//     && comparisonSubsegment<NaiveDSL<DGtal::int32_t> >(-8,5)
//     && comparisonSubsegment<NaiveDSL<DGtal::int32_t> >(5,-8)
//     && comparisonSubsegment<NaiveDSL<DGtal::int32_t> >(8,-5)
//     && comparisonSubsegment<NaiveDSL<DGtal::int32_t> >(-5,-8)
//     && comparisonSubsegment<NaiveDSL<DGtal::int32_t> >(-8,-5)

//     && comparisonSubsegment<StandardDSL<DGtal::int32_t> >(5,8)
//     && comparisonSubsegment<StandardDSL<DGtal::int32_t> >(8,5)
//     && comparisonSubsegment<StandardDSL<DGtal::int32_t> >(-5,8)
//     && comparisonSubsegment<StandardDSL<DGtal::int32_t> >(-8,5)
//     && comparisonSubsegment<StandardDSL<DGtal::int32_t> >(5,-8)
//     && comparisonSubsegment<StandardDSL<DGtal::int32_t> >(8,-5)
//     && comparisonSubsegment<StandardDSL<DGtal::int32_t> >(-5,-8)
//     && comparisonSubsegment<StandardDSL<DGtal::int32_t> >(-8,-5)
// #ifdef WITH_BIGINTEGER
//     && comparisonSubsegment<StandardDSL<DGtal::int32_t, DGtal::BigInteger> >(5,8)
// #endif
//       ;
//   }
  
  { // createDSS
    res = res && createDSSTest();
  }

  { // union of two DSSs
    res = res && unionTest();
    res = res && unionComparisonTest(1000,50,1000);
  }
  
  

  trace.emphase() << ( res ? "Passed." : "Error." ) << endl;
  trace.endBlock();
  return res ? 0 : 1;
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
