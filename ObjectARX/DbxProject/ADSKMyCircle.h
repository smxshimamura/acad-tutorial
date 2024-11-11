#pragma once
#include "StdAfx.h"
#include "dbmain.h"
class AAPI ADSKMyCircle : public AcDbEntity {

public:
    ACRX_DECLARE_MEMBERS(ADSKMyCircle)

protected:
    static Adesk::UInt32 kCurrentVersionNumber;

public:
    ADSKMyCircle();
    virtual ~ADSKMyCircle();

    //----- AcDbObject protocols
    //- Dwg Filing protocol
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
    virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);

    //- Dxf Filing protocol
    virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler* pFiler) const;
    virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler* pFiler);

    //----- AcDbEntity protocols
    //- Graphics protocol
protected:
    virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* mode);
    virtual Adesk::UInt32 subSetAttributes(AcGiDrawableTraits* traits);

    // -------------------------------------------------------------------
    virtual void subList(void) const;

public:
    Acad::ErrorStatus setRadius(const double Radius);
    Acad::ErrorStatus radius(double& Radius);
    Acad::ErrorStatus setCenter(const AcGePoint3d Center);
    Acad::ErrorStatus center(AcGePoint3d& Center);

private:
    AcGePoint3d m_Center;
    double m_Radius;
};

#ifdef DBXPROJECT_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ADSKMyCircle)
#endif
