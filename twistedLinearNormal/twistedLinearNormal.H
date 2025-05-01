/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
    Copyright (C) 2019 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::extrudeModels::twistedLinearNormal

Description
    Extrudes by transforming points normal to the surface by a given distance 
    and twist it by total angle around selected reference point and normal axis.

    The twistedLinearNormalCoeffs dictionary entries
    \table
        Property  | Description                             | Required | Default
        thickness | Extrusion thickness                     | yes |
        point | Point of rotation | yes |
        totalTwist | Total twist between start and end surfaces | yes
    \endtable

\*---------------------------------------------------------------------------*/

#ifndef twistedLinearNormal_H
#define twistedLinearNormal_H

#include "point.H"
#include "extrudeModel.H"
#include "scalarList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace extrudeModels
{

/*---------------------------------------------------------------------------*\
                 Class extrudeModels::twistedLinearNormal Declaration
\*---------------------------------------------------------------------------*/

class twistedLinearNormal
:
    public extrudeModel
{
    // Private data

        //- Layer thickness
        scalar thickness_;

        // Total twist of mesh 
        scalar totalTwist_;

        // Reference point of rotation around surface normal
        point refPoint_;

        // Twist cell distribution
        scalarList twistByLayer_;

        //- Layer cell distribution
        scalarList layerPoints_;


public:

    //- Runtime type information
    TypeName("twistedLinearNormal");

    // Constructors

        //- Construct from dictionary
        explicit twistedLinearNormal(const dictionary& dict);


    //- Destructor
    virtual ~twistedLinearNormal() = default;


    // Member Operators

        point operator()
        (
            const point& surfacePoint,
            const vector& surfaceNormal,
            const label layer
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace extrudeModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
