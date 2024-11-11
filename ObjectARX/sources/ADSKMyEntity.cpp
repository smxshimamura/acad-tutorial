#include "StdAfx.h"
#include "ADSKMyEntity.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 ADSKMyEntity::kCurrentVersionNumber = 1;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS(
    ADSKMyEntity, AcDbEntity,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation, ADSKMYENTITY,
    ARXTRAININGAPP
    | Product Desc : A description for your object
    | Company : Your company name
    | WEB Address : Your company WEB site address
)

//-----------------------------------------------------------------------------
ADSKMyEntity::ADSKMyEntity() {
    m_Vertex1.set(0.0, 0.0, 0.0);
    m_Vertex2.set(0.0, 0.0, 0.0);
    m_Vertex3.set(0.0, 0.0, 0.0);
}

ADSKMyEntity::~ADSKMyEntity() {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus ADSKMyEntity::dwgOutFields(AcDbDwgFiler * pFiler) const {
    assertReadEnabled();
    //----- Save parent class information first.
    auto stat = AcDbEntity::dwgOutFields(pFiler);
    if (stat != Acad::eOk)
        return (stat);
    //----- Object version number needs to be saved first
    if ((stat = pFiler->writeUInt32(kCurrentVersionNumber)) != Acad::eOk)
        return (stat);
    //----- Output params
    //.....
    pFiler->writeItem(m_Vertex1);
    pFiler->writeItem(m_Vertex2);
    pFiler->writeItem(m_Vertex3);

    return pFiler->filerStatus();
}

Acad::ErrorStatus ADSKMyEntity::dwgInFields(AcDbDwgFiler * pFiler) {
    assertWriteEnabled();
    //----- Read parent class information first.
    auto stat = AcDbEntity::dwgInFields(pFiler);
    if (stat != Acad::eOk)
        return (stat);
    //----- Object version number needs to be read first
    Adesk::UInt32 version = 0;
    if ((stat = pFiler->readUInt32(&version)) != Acad::eOk)
        return (stat);
    if (version <= kCurrentVersionNumber)
        return (Acad::eMakeMeProxy);

    if (version >= 1) pFiler->readItem(&m_Vertex1);
    if (version >= 1) pFiler->readItem(&m_Vertex2);
    if (version >= 1) pFiler->readItem(&m_Vertex3);

    return pFiler->filerStatus();
}

//- Dxf Filing protocol
Acad::ErrorStatus ADSKMyEntity::dxfOutFields(AcDbDxfFiler * pFiler) const {
    assertReadEnabled();
    //----- Save parent class information first.
    auto stat = AcDbEntity::dxfOutFields(pFiler);
    if (stat != Acad::eOk)
        return (stat);
    stat = pFiler->writeItem(AcDb::kDxfSubclass, _RXST("ADSKMyEntity"));
    if (stat != Acad::eOk)
        return (stat);
    //----- Object version number needs to be saved first
    if ((stat = pFiler->writeUInt32(kDxfInt32, ADSKMyEntity::kCurrentVersionNumber)) != Acad::eOk)
        return (stat);
    //----- Output params
    pFiler->writeItem(10, m_Vertex1);
    pFiler->writeItem(11, m_Vertex2);
    pFiler->writeItem(12, m_Vertex3);

    return (pFiler->filerStatus());
}

Acad::ErrorStatus ADSKMyEntity::dxfInFields(AcDbDxfFiler * pFiler) {
    assertWriteEnabled();
    //----- Read parent class information first.
    auto stat = AcDbEntity::dxfInFields(pFiler);
    if (stat != Acad::eOk || !pFiler->atSubclassData(_RXST("ADSKMyEntity")))
        return (pFiler->filerStatus());
    //----- Object version number needs to be read first
    struct resbuf rb;
    pFiler->readItem(&rb);
    if (rb.restype != AcDb::kDxfInt32) {
        pFiler->pushBackItem();
        pFiler->setError(Acad::eInvalidDxfCode, _RXST("\nError: expected group code %d (version #)"), AcDb::kDxfInt32);
        return (pFiler->filerStatus());
    }
    const Adesk::UInt32 version = static_cast<Adesk::UInt32>(rb.resval.rlong);
    if (version > kCurrentVersionNumber)
        return (Acad::eMakeMeProxy);
    //----- Read params in non order dependant manner
    while (stat == Acad::eOk && (stat = pFiler->readResBuf(&rb)) == Acad::eOk) {
        switch (rb.restype) {
        case 10:
            m_Vertex1 = asPnt3d(rb.resval.rpoint);
            break;

        case 11:
            m_Vertex2 = asPnt3d(rb.resval.rpoint);
            break;

        case 12:
            m_Vertex3 = asPnt3d(rb.resval.rpoint);
            break;

        default:
            //----- An unrecognized group. Push it back so that the subclass can read it again.
            pFiler->pushBackItem();
            stat = Acad::eEndOfFile;
            break;
        }
    }
    //----- At this point the es variable must contain eEndOfFile
    //----- - either from readResBuf() or from pushback. If not,
    //----- it indicates that an error happened and we should
    //----- return immediately.
    if (stat != Acad::eEndOfFile)
        return Acad::eInvalidResBuf;

    return pFiler->filerStatus();
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean ADSKMyEntity::subWorldDraw(AcGiWorldDraw * mode) {
    assertReadEnabled();

    AcGePoint3d pt[4];
    pt[0] = m_Vertex1;
    pt[1] = m_Vertex2;
    pt[2] = m_Vertex3;
    pt[3] = m_Vertex1;

    return mode->geometry().polyline(4, pt);
}


Adesk::UInt32 ADSKMyEntity::subSetAttributes(AcGiDrawableTraits * traits) {
    assertReadEnabled();
    return AcDbEntity::subSetAttributes(traits);
}

// -----------------------------------------------------------------------------
void ADSKMyEntity::subList(void) const
{
    AcDbEntity::subList();

    acutPrintf(_T("\nVertex1 = ( %lf , %lf , %lf )"),
        m_Vertex1[X], m_Vertex1[Y], m_Vertex1[Z]);
    acutPrintf(_T("\nVertex2 = ( %lf , %lf , %lf )"),
        m_Vertex2[X], m_Vertex2[Y], m_Vertex2[Z]);
    acutPrintf(_T("\nVertex3 = ( %lf , %lf , %lf )"),
        m_Vertex3[X], m_Vertex3[Y], m_Vertex3[Z]);
}

// -----------------------------------------------------------------------------
Acad::ErrorStatus ADSKMyEntity::subTransformBy(const AcGeMatrix3d& xform)
{
    AcDbEntity::subTransformBy(xform);
    setVertex1(m_Vertex1.transformBy(xform));
    setVertex2(m_Vertex2.transformBy(xform));
    setVertex3(m_Vertex3.transformBy(xform));

    return Acad::eOk;
}

// -------------------------------------------------------------------
AcGePoint3d ADSKMyEntity::vertex1(void) const
{
    assertReadEnabled();
    return m_Vertex1;
}

AcGePoint3d ADSKMyEntity::vertex2(void) const
{
    assertReadEnabled();
    return m_Vertex2;
}

AcGePoint3d ADSKMyEntity::vertex3(void) const
{
    assertReadEnabled();
    return m_Vertex3;
}

Acad::ErrorStatus ADSKMyEntity::setVertex1(AcGePoint3d newVal)
{
    assertWriteEnabled();
    m_Vertex1 = newVal;
    return Acad::eOk;
}

Acad::ErrorStatus ADSKMyEntity::setVertex2(AcGePoint3d newVal)
{
    assertWriteEnabled();
    m_Vertex2 = newVal;
    return Acad::eOk;
}

Acad::ErrorStatus ADSKMyEntity::setVertex3(AcGePoint3d newVal)
{
    assertWriteEnabled();
    m_Vertex3 = newVal;
    return Acad::eOk;
}
