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
 * @file testShapeFromPoints.cpp
 * @author Tristan Roussillon (\c
 * tristan.roussillon@liris.cnrs.fr ) Laboratoire d'InfoRmatique en
 * Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS,
 * France
 *
 *
 * @date 2011/06/27

 * Functions for testing various shapes from points
 *
 * This file is part of the DGtal library.
 */

///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <boost/program_options.hpp>

#include "DGtal/base/Common.h"
#include "DGtal/kernel/PointVector.h"
#include "DGtal/geometry/2d/CircleFrom3Points.h"
#include "DGtal/geometry/2d/CircleFrom2Points.h"

#include "DGtal/io/boards/Board2D.h"

#include "ConfigTest.h"

///////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace DGtal;

///////////////////////////////////////////////////////////////////////////////
// testing Functions
///////////////////////////////////////////////////////////////////////////////

/**
 * Simple tests
 *
 */
template <typename Coordinate>
bool testCircleFrom3Points()
{

  trace.beginBlock("Simple test for CircleFrom3Points"); 
  
  typedef PointVector<2,Coordinate> Point; 
  CircleFrom3Points<Point> c; 
  Point o(0,0); 
  Board2D board; 
  board << SetMode(o.styleName(), "Grid") << o; 

  {
    c.init( Point(0,1), Point(150,18), Point(250,-48) ); 
    trace.info() << c << endl;
    trace.info() << o << " is at distance " << c.signedDistance(o) << endl;
    if (c.signedDistance(o) != -4026300) return false; 
    board << c; 
    board.saveEPS("circle1.eps");
  }
  {
    c.init( Point(0,1), Point(150,18), Point(100,48) ); 
    trace.info() << c << endl;
    trace.info() << o << " is at distance " << c.signedDistance(o) << endl;
    if (c.signedDistance(o) != 442200) return false; 
    board << c; 
    board.saveEPS("circle2.eps");
  }
  {
    c.init( Point(0,1), Point(150,18), Point(100,48) ); 
    trace.info() << c << endl;
    trace.info() << Point(150,18) << " is at distance " << c.signedDistance(Point(150,18)) << endl;
    if (c.signedDistance(Point(150,18)) != 0) return false; 
  }
  {
    c.init( Point(0,0), Point(150,20), Point(15,2) ); 
    trace.info() << c << endl;
    trace.info() << o << " is at distance " << c.signedDistance(o) << endl;
    if (c.signedDistance(o) != 0) return false; 
    board << c; 
    board.saveEPS("circle4.eps");
  }

  trace.endBlock(); 
  
  return true; 
}

template <typename Coordinate>
bool testCircleFrom2Points()
{

  trace.beginBlock("Simple test for CircleFrom2Points"); 
  
  typedef PointVector<2,Coordinate> Point; 
  Point o(0,0);
  Point pole(0,1); 
  CircleFrom2Points<Point> c( pole ); 
  Board2D board; 
  board << SetMode(o.styleName(), "Grid") << o << pole; 

  {
    c.init( Point(15,2), Point(10,5) ); 
    trace.info() << c << endl;
    trace.info() << o << " is at distance " << c.signedDistance(o) << endl;
    if (c.signedDistance(o) != 470) return false; 
    board << c; 
    board.saveSVG("circle5.svg", Board2D::BoundingBox, 5000 );
    board.saveEPS("circle5.eps", Board2D::BoundingBox, 5000 );
#ifdef WITH_CAIRO
    board.saveCairo("circle5.pdf", Board2D::CairoPDF, Board2D::BoundingBox, 5000);
#endif

  }

  trace.endBlock(); 
  
  return true; 
}


///////////////////////////////////////////////////////////////////////////////
// Standard services - public :

int main( int argc, char** argv )
{
  trace.beginBlock ( "Testing shapes from points" );
  trace.info() << "Args:";
  for ( int i = 0; i < argc; ++i )
    trace.info() << " " << argv[ i ];
  trace.info() << endl;


  bool res = testCircleFrom3Points<int>()
  && testCircleFrom3Points<double>()
  && testCircleFrom3Points<BigInteger>()
  && testCircleFrom2Points<int>()
; 


  trace.emphase() << ( res ? "Passed." : "Error." ) << endl;
  trace.endBlock();
  
  return res ? 0 : 1;
}
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
