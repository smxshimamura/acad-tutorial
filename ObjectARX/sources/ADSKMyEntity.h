#pragma once

#include "dbmain.h"

class ADSKMyEntity : public AcDbEntity {

public:
    ACRX_DECLARE_MEMBERS(ADSKMyEntity);

protected:
    static Adesk::UInt32 kCurrentVersionNumber;

public:
    ADSKMyEntity();
    ~ADSKMyEntity() override;

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

    // -----------------------------------------------------------------------------
    virtual void subList(void) const;
    // -----------------------------------------------------------------------------
    virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& xform);

public:
    //----- AcDbEntity protocols
    // Member variable access
    AcGePoint3d vertex1(void) const;
    AcGePoint3d vertex2(void) const;
    AcGePoint3d vertex3(void) const;
    Acad::ErrorStatus setVertex1(AcGePoint3d newVal);
    Acad::ErrorStatus setVertex2(AcGePoint3d newVal);
    Acad::ErrorStatus setVertex3(AcGePoint3d newVal);
private:
    AcGePoint3d m_Vertex1;
    AcGePoint3d m_Vertex2;
    AcGePoint3d m_Vertex3;
};

ACDB_REGISTER_OBJECT_ENTRY_AUTO(ADSKMyEntity)
