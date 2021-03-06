/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
     \\/     M anipulation  |
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
    Foam::face

Description
    A face is a list of labels corresponding to mesh vertices.

SeeAlso
    Foam::triFace

SourceFiles
    faceI.H
    face.C
    faceIntersection.C
    faceContactSphere.C
    faceAreaInContact.C
    faceTemplates.C

\*---------------------------------------------------------------------------*/

#ifndef faceData_H
#define faceData_H

#include "pointField.H"
#include "labelList.H"
#include "edgeList.H"
#include "vectorField.H"
#include "faceListFwd.H"
#include "intersection.H"
#include "pointHit.H"
#include "ListListOps.H"
#include "triPointRef.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions and operators

class faceData;
class triFace;
template<class T> class gpuList;
template<class T> class List;

template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
class DynamicList;
__host__ __device__
inline bool operator==(const faceData& a, const faceData& b);
__host__ __device__
inline bool operator!=(const faceData& a, const faceData& b);

Istream& operator>>(Istream&, faceData&);
Ostream& operator<<(Ostream&, const faceData&);

typedef gpuList<faceData> faceDatagpuList;
typedef List<faceData> faceDataList;

/*---------------------------------------------------------------------------*\
                           Class face Declaration
\*---------------------------------------------------------------------------*/

class faceData
{
private:
    label start_;
    label size_;


public:

    // Static data members

        static const char* const typeName;
        static const faceData zero;

    // Constructors

        //- Construct null
        __host__ __device__
        inline faceData();

        //- Construct given size
        __host__ __device__
        explicit inline faceData(label,label);

    // Member Functions

        __host__ __device__
        inline label size() const
        {
             return size_;
        }

        __host__ __device__
        inline label start() const
        {
             return start_;
        }

        //- Centre point of face
        __host__ __device__
        point centre(const label*, const point*) const;

        //- Calculate average value at centroid of face
        template<class Type>
        __host__ __device__
        Type average(const label*,const point*, const Type*,Type) const;

        //- Magnitude of face area
        __host__ __device__
        inline scalar mag(const label*,const point*) const;

        //- Vector normal; magnitude is equal to area of face
        __host__ __device__
        vector normal(const label*,const point*) const;

        //- Navigation through face vertices

            //- Next vertex on face
            __host__ __device__
            inline label nextLabel(const label*,const label i) const;

            //- Previous vertex on face
            __host__ __device__
            inline label prevLabel(const label*,const label i) const;


        //- Return the volume swept out by the face when its points move
        __host__ __device__
        inline scalar sweptVol
        (
            const label* indeces,
            const point* oldPoints,
            const point* newPoints
        ) const;
/*
        //- Return potential intersection with face with a ray starting
        //  at p, direction n (does not need to be normalized)
        //  Does face-centre decomposition and returns triangle intersection
        //  point closest to p. Face-centre is calculated from point average.
        //  For a hit, the distance is signed.  Positive number
        //  represents the point in front of triangle
        //  In case of miss the point is the nearest point on the face
        //  and the distance is the distance between the intersection point
        //  and the original point.
        //  The half-ray or full-ray intersection and the contact
        //  sphere adjustment of the projection vector is set by the
        //  intersection parameters
        __host__ __device__
        pointHit ray
        (
            const scalar* indeces,
            const point& p,
            const vector& n,
            const point*,
            const intersection::algorithm alg = intersection::FULL_RAY,
            const intersection::direction dir = intersection::VECTOR
        ) const;

        //- Fast intersection with a ray.
        //  Does face-centre decomposition and returns triangle intersection
        //  point closest to p. See triangle::intersection for details.
        __host__ __device__
        pointHit intersection
        (
            const label*,
            const point& p,
            const vector& q,
            const point& ctr,
            const point*,
            const intersection::algorithm alg,
            const scalar tol = 0.0
        ) const;

        //- Return nearest point to face
        __host__ __device__
        pointHit nearestPoint
        (
            const label*,
            const point& p,
            const point*,
        ) const;

        //- Return nearest point to face and classify it:
        //  + near point (nearType=POINT, nearLabel=0, 1, 2)
        //  + near edge (nearType=EDGE, nearLabel=0, 1, 2)
        //    Note: edges are counted from starting vertex so
        //    e.g. edge n is from f[n] to f[0], where the face has n + 1
        //    points
        __host__ __device__
        pointHit nearestPointClassify
        (
            const label*,
            const point& p,
            const point*,
            label& nearType,
            label& nearLabel
        ) const;
*/
        //- Return contact sphere diameter
        __host__ __device__
        scalar contactSphereDiameter
        (
            const label*,
            const point& p,
            const vector& n,
            const point*
        ) const;
/*
        //- Return area in contact, given the displacement in vertices
        __host__ __device__
        scalar areaInContact
        (
            const label*,
            const point*,
            const scalar* v
        ) const;
*/
    // Friend Operators

        __host__ __device__
        friend bool operator==(const faceData& a, const faceData& b);
        __host__ __device__
        friend bool operator!=(const faceData& a, const faceData& b);

        friend Istream& operator>>(Istream&, faceData&);
        friend Ostream& operator<<(Ostream&, const faceData&);

};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "faceDataI.H"

#ifdef NoRepository
#   include "faceDataTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
