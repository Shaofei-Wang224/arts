/* Copyright (C) 2012 
Richard Larsson <ric.larsson@gmail.com>

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
USA. */

/** Contains some additional functionality of the rational class
   \file   linemixing.cc
   
   \author Richard Larsson
   \date   2012-10-31
**/

#include "linemixingdata.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Line mixing interactions to get cross section goes below here
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Note that first order is used by LBLRTM on the data we have.
void LineMixingData::GetLBLRTM(Numeric& Y, Numeric& G, const Numeric& Temperature, const Index& order=1)
    {
      if( mtype != LM_LBLRTM )
        throw std::runtime_error("Trying to access LineMixingData of another type than defined by the LineRecord.\n");
      
      if(mdata.nelem() != 3)
        throw std::runtime_error("Trying to access LineMixingData of LBLRTM type but the LineMixingData does not match this format.\n");
      
      if(mdata[0].nelem() != 4 && mdata[1].nelem() != 4 && mdata[2].nelem() != 4)
        throw std::runtime_error("Trying to access LineMixingData of LBLRTM type but the LineMixingData does not match this format.\n");
      
      // Helper to understand the following interpolation
      const Vector& t = mdata[0];
      const Vector& y = mdata[1];
      const Vector& g = mdata[2];
      
      // Interpolation variables
      GridPosPoly gp;
      Vector itw;
      
      // Interpolation variale determination
      gridpos_poly(gp, t, Temperature, order);
      interpweights(itw, gp);
      
      // Interpolated values
      Y = interp(itw, y, gp);
      G = interp(itw, g, gp);
}

void LineMixingData::Get2ndOrder(Numeric& Y, Numeric& G, Numeric& DV, const Numeric& Temperature)
{
      if( mtype != LM_2NDORDER )
        throw std::runtime_error("Trying to access LineMixingData of another type than defined by the LineRecord.\n");
      
      if(mdata.nelem() != 4)
        throw std::runtime_error("Trying to access LineMixingData of 2ndOrder type but the LineMixingData does not match this format.\n");
      
      if(mdata[0].nelem() != 1 && mdata[1].nelem() != 3 && mdata[2].nelem() != 3 && mdata[3].nelem() != 3)
        throw std::runtime_error("Trying to access LineMixingData of 2ndOrder type but the LineMixingData does not match this format.\n");
      
      // Helper to understand the following interpolation
      const Numeric& T0  = mdata[0][0];
      const Vector& y  = mdata[1];
      const Vector& g  = mdata[2];
      const Vector& dv = mdata[3];
      
      Y  =  ( ( y[0] + y[1] * ( T0/Temperature-1. ) ) * pow( T0/Temperature, y[2] ) );
      G  =  ( ( g[0] + g[1] * ( T0/Temperature-1. ) ) * pow( T0/Temperature, g[2] ) );
      DV  = ( ( dv[0] + dv[1] * ( T0/Temperature-1. ) ) * pow( T0/Temperature, dv[2] ) );
      
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Line mixing interactions to get cross section goes above here
// Below is line mixing storage functions.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// This will parse any Vector by the own mtype to the right settings for mdata
void LineMixingData::SetDataFromVectorWithKnownType(const Vector& input)
{
  if(mtype == LM_NONE) // The standard case
    Vector2NoneData(input);
  else if(mtype == LM_LBLRTM) // The LBLRTM case
    Vector2LBLRTMData(input);
  else if(mtype == LM_2NDORDER) // The 2nd order case
    Vector2SecondOrderData(input);
  else
    throw std::runtime_error("You are trying to store a line mixing type that is unknown to ARTS.\n");
}

// This will convert the read vector to the LBLRTM data format
void LineMixingData::Vector2LBLRTMData(const Vector& input)
{
      if(mtype != LineMixingData::LM_LBLRTM)
        throw std::runtime_error("Trying to set LBLRTM data from vector for wrong type.\n");
      
      if(input.nelem() != 12)
        throw std::runtime_error("The line mixing data vector is not of the right length for LBLRTM.\n");
      
      // Then this is a three-long ArrayOfVector
      mdata.resize(3);
      
      // This is supposed to be the temperature vector
      mdata[0].resize(4);
      mdata[0][0] = input[0];
      mdata[0][1] = input[1];
      mdata[0][2] = input[2];
      mdata[0][3] = input[3];
      
      // This is supposed to be the Y-vector
      mdata[1].resize(4);
      mdata[1][0] = input[4];
      mdata[1][1] = input[5];
      mdata[1][2] = input[6];
      mdata[1][3] = input[7];
      
      // This is supposed to be the G vector
      mdata[2].resize(4);
      mdata[2][0] = input[8];
      mdata[2][1] = input[9];
      mdata[2][2] = input[10];
      mdata[2][3] = input[11];
}


// This will convert the read vector to the LBLRTM data format
void LineMixingData::Vector2NoneData(const Vector& input)
{
  if(mtype != LineMixingData::LM_LBLRTM)
        throw std::runtime_error("Trying to set no data from vector for wrong type.\n");
  
  if( input.nelem() != 0 )
    throw std::runtime_error("You are trying to set line mixing data to a none line mixed line.\n");
}


// This will convert the read vector to the LBLRTM data format
void LineMixingData::Vector2SecondOrderData(const Vector& input)
{
      if(mtype != LineMixingData::LM_2NDORDER)
        throw std::runtime_error("Trying to set 2ndOrder data from vector for wrong type.\n");
      
      if(input.nelem() != 10)
        throw std::runtime_error("The line mixing data vector is not of the right length for 2ndOrder.\n");
      
      // Then this is a three-long ArrayOfVector
      mdata.resize(4);
      
      // This is supposed to be the temperature vector
      mdata[0].resize(1);
      mdata[0][0] = input[6];
      
      // This is supposed to be the Y components
      mdata[1].resize(3);
      mdata[1][0] = input[0];
      mdata[1][1] = input[1];
      mdata[1][2] = input[7];
      
      // This is supposed to be the G components
      mdata[2].resize(3);
      mdata[2][0] = input[2];
      mdata[2][1] = input[3];
      mdata[2][2] = input[8];
      
      // This is supposed to be the DV components
      mdata[3].resize(3);
      mdata[3][0] = input[4];
      mdata[3][1] = input[5];
      mdata[3][2] = input[9];
}


// This will convert the stored two char string to LM_Type
void LineMixingData::StorageTag2SetType(const String& input)
 {
  if(input == "NA") // The standard case
    mtype = LM_NONE;
  else if(input == "LL") // The LBLRTM case
    mtype = LM_LBLRTM;
  else if(input == "L2") // The 2nd order case
    mtype = LM_2NDORDER;
  else
    throw std::runtime_error("You are trying to read a line mixing type that is unknown to ARTS.\n");
}
 
 
// This will convert the LBLRTM data format to a vector for storage
void LineMixingData::LBLRTMData2Vector(Vector& output)
{
      output.resize(12);
      
      // This is the T-vector
      output[0]  = mdata[0][0];
      output[1]  = mdata[0][1];
      output[2]  = mdata[0][2];
      output[3]  = mdata[0][3];
      
      // This is the Y-vector
      output[4]  = mdata[1][0];
      output[5]  = mdata[1][1];
      output[6]  = mdata[1][2];
      output[7]  = mdata[1][3];
      
      // This is the G-vector
      output[8]  = mdata[2][0];
      output[9]  = mdata[2][1];
      output[10] = mdata[2][2];
      output[11] = mdata[2][3];
}


// This will convert the read vector to the LBLRTM data format
void LineMixingData::SecondOrderData2Vector(Vector& output)
{
      output.resize(10);
      
      // This is the temperature vector
      output[6] = mdata[0][0];
      
      // This is the Y components
      output[0] = mdata[1][0];
      output[1] = mdata[1][1];
      output[7] = mdata[1][2];
      
      // This is the G components
      output[2] = mdata[2][0];
      output[3] = mdata[2][1];
      output[8] = mdata[2][2];
      
      // This is the DV components
      output[4] = mdata[3][0];
      output[5] = mdata[3][1];
      output[9] = mdata[3][2];
}


// This will convert LM_Type to a two char string for storage
void LineMixingData::Type2StorageTag(String& output)
 {
  output.resize(2); // ARTS format specify that this is the size of the tag
  
  if(mtype == LM_NONE) // The standard case
    output = "NA";
  else if(mtype == LM_LBLRTM) // The LBLRTM case
    output = "LL";
  else if(mtype == LM_2NDORDER) // The 2nd order case
    output = "L2"; 
  else
    throw std::runtime_error("You are trying to store a line mixing type that is unknown to ARTS.\n");
}