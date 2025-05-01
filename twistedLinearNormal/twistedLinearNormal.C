/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2015 OpenFOAM Foundation
    Copyright (C) 2019-2020 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/

#include "twistedLinearNormal.H"
#include "addToRunTimeSelectionTable.H"
#include "unitConversion.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace extrudeModels
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(twistedLinearNormal, 0);

addToRunTimeSelectionTable(extrudeModel, twistedLinearNormal, dictionary);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

twistedLinearNormal::twistedLinearNormal(const dictionary& dict)
:
    extrudeModel(typeName, dict),
    thickness_(coeffDict_.get<scalar>("thickness")),
    totalTwist_(degToRad(coeffDict_.get<scalar>("totalTwist"))),
    refPoint_(coeffDict_.getCompat<point>("point", {{"axisPt", -1812}})),
    twistByLayer_(nLayers_),
    layerPoints_(nLayers_)
    {
        if (thickness_ <= 0)
        {
            FatalErrorInFunction
                << "thickness should be positive : " << thickness_
                << exit(FatalError);
        }

        for (label layer = 0; layer < nLayers_; ++layer)
        {
            layerPoints_[layer] = thickness_*sumThickness(layer + 1);
            twistByLayer_[layer] = totalTwist_ * (layer+1) / nLayers_;
        }
    }


// * * * * * * * * * * * * * * * * Operators * * * * * * * * * * * * * * * * //

point twistedLinearNormal::operator()
(
    const point& surfacePoint,
    const vector& surfaceNormal,
    const label layer
) const
{
    if (layer <= 0)
    {
        return surfacePoint;
    }

    scalar currentLayer = layer - 1;

    // Normal Displacement 
    point normalDisplacement = surfacePoint + layerPoints_[currentLayer] * surfaceNormal;
    if (totalTwist_ == 0)
    {
        return normalDisplacement;
    }
    //---

    // Radial Displacement
    point initialPosition = surfacePoint - refPoint_;
    point A = initialPosition; 

    A -= (surfaceNormal & A) * surfaceNormal;
    point rA = A / mag(A);
    point tA = rA ^ surfaceNormal;

    point radialDisplacement = 
        A * cos(twistByLayer_[currentLayer])
        + tA * mag(A) * sin(twistByLayer_[currentLayer]);
    radialDisplacement -= A;
    //---

    point resultPoint = normalDisplacement + radialDisplacement;
    return resultPoint;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace extrudeModels
} // End namespace Foam

// ************************************************************************* //
