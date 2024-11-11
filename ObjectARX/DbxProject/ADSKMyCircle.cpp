//-----------------------------------------------------------------------------
//----- ADSKMyCircle.cpp : Implementation of ADSKMyCircle
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "tchar.h"
#include "ADSKMyCircle.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 ADSKMyCircle::kCurrentVersionNumber = 1;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS(
	ADSKMyCircle, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, ADSKMYCIRCLE,
	ARXTRANINGAPP
	| Product Desc : A description for your object
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

//-----------------------------------------------------------------------------
ADSKMyCircle::ADSKMyCircle() : m_Radius(0) {
}

ADSKMyCircle::~ADSKMyCircle() {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus ADSKMyCircle::dwgOutFields(AcDbDwgFiler * pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	auto stat = AcDbEntity::dwgOutFields(pFiler);
	if (stat != Acad::eOk)
		return stat;
	//----- Object version number needs to be saved first
	if ((stat = pFiler->writeUInt32(ADSKMyCircle::kCurrentVersionNumber)) != Acad::eOk)
		return stat;
	//----- Output params
	//.....
	stat = pFiler->writeItem(m_Center);
	stat = pFiler->writeItem(m_Radius);

	return pFiler->filerStatus();
}

Acad::ErrorStatus ADSKMyCircle::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	auto stat = AcDbEntity::dwgInFields(pFiler);
	if (stat != Acad::eOk)
		return stat;
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0;
	if ((stat = pFiler->readUInt32(&version)) != Acad::eOk)
		return stat;
	if (version > kCurrentVersionNumber)
		return Acad::eMakeMeProxy;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < ADSKMyCircle::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	pFiler->readItem(&m_Center);
	pFiler->readItem(&m_Radius);

	return pFiler->filerStatus();
}

//- Dxf Filing protocol
Acad::ErrorStatus ADSKMyCircle::dxfOutFields(AcDbDxfFiler * pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	auto stat = AcDbEntity::dxfOutFields(pFiler);
	if (stat != Acad::eOk)
		return stat;
	stat = pFiler->writeItem(AcDb::kDxfSubclass, _RXST("ADSKMyCircle"));
	if (stat != Acad::eOk)
		return stat;
	//----- Object version number needs to be saved first
	if ((stat = pFiler->writeUInt32(kDxfInt32, kCurrentVersionNumber)) != Acad::eOk)
		return stat;
	//----- Output params
	//.....
	pFiler->writeItem(10, m_Center);
	pFiler->writeItem(140, m_Radius);

	return pFiler->filerStatus();
}

Acad::ErrorStatus ADSKMyCircle::dxfInFields(AcDbDxfFiler * pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	auto stat = AcDbEntity::dxfInFields(pFiler);
	if (stat != Acad::eOk || !pFiler->atSubclassData(_RXST("ADSKMyCircle")))
		return pFiler->filerStatus();
	//----- Object version number needs to be read first
	resbuf rb;
	pFiler->readItem(&rb);
	if (rb.restype != AcDb::kDxfInt32) {
		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode, _RXST("\nError: expected group code %d (version #)"), AcDb::kDxfInt32);
		return pFiler->filerStatus();
	}
	Adesk::UInt32 version = static_cast<Adesk::UInt32>(rb.resval.rlong);
	if (version > kCurrentVersionNumber)
		return Acad::eMakeMeProxy;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < ADSKMyCircle::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params in non order dependant manner
	while (stat == Acad::eOk && (stat = pFiler->readResBuf(&rb)) == Acad::eOk) {
		switch (rb.restype) {
			//----- Read params by looking at their DXF code (example below)
			//case AcDb::kDxfXCoord:
			//	if ( version == 1 )
			//		cen3d =asPnt3d (rb.resval.rpoint) ;
			//	else 
			//		cen2d =asPnt2d (rb.resval.rpoint) ;
			//	break ;
			//.....
		case 10:
			m_Center = asPnt3d(rb.resval.rpoint);
			break;

		case 140:
			m_Radius = rb.resval.rreal;
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
		return (Acad::eInvalidResBuf);

	return pFiler->filerStatus();
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean ADSKMyCircle::subWorldDraw(AcGiWorldDraw * mode) {
	assertReadEnabled();

	mode->subEntityTraits().setFillType(kAcGiFillAlways);
	mode->geometry().circle(m_Center, m_Radius, AcGeVector3d::kZAxis);

	return Acad::eOk;
	//return (AcDbEntity::subWorldDraw (mode)) ;
}


Adesk::UInt32 ADSKMyCircle::subSetAttributes(AcGiDrawableTraits * traits) {
	assertReadEnabled();
	return AcDbEntity::subSetAttributes(traits);
}


// -----------------------------------------------------------------------------
void ADSKMyCircle::subList(void) const
{
	assertReadEnabled();
	AcDbEntity::subList();
	acutPrintf(_T("\n\t\tCenter = ( %lf , %lf , %lf )"),

		m_Center.x, m_Center.y, m_Center.z);
	acutPrintf(_T("\n\t\tRadius = %lf"), m_Radius);
}

Acad::ErrorStatus ADSKMyCircle::center(AcGePoint3d & Center)
{
	assertReadEnabled();
	Center = m_Center;
	return Acad::eOk;
}

Acad::ErrorStatus ADSKMyCircle::setCenter(const AcGePoint3d Center)
{
	assertWriteEnabled();
	m_Center = Center;
	return Acad::eOk;
}

Acad::ErrorStatus ADSKMyCircle::radius(double& Radius)
{
	assertReadEnabled();
	Radius = m_Radius;
	return Acad::eOk;
}

Acad::ErrorStatus ADSKMyCircle::setRadius(const double Radius)
{
	assertWriteEnabled();
	m_Radius = Radius;
	return Acad::eOk;
}
