//-----------------------------------------------------------------------------
//----- ADSKMyDictionary.cpp : Implementation of ADSKMyDictionary
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ADSKMyDictionary.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 ADSKMyDictionary::kCurrentVersionNumber = 1;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS(
	ADSKMyDictionary, AcDbObject,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
	AcDbProxyEntity::kNoOperation, ADSKMYDICTIONARY,
	ARXTRAININGAPP
	| Product Desc : A description for your object
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

//-----------------------------------------------------------------------------
ADSKMyDictionary::ADSKMyDictionary() : m_Color(0) {
}

ADSKMyDictionary::~ADSKMyDictionary() = default;

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus ADSKMyDictionary::dwgOutFields(AcDbDwgFiler * pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	auto stat = AcDbObject::dwgOutFields(pFiler);
	if (stat != eOk)
		return stat;
	//----- Object version number needs to be saved first
	if ((stat = pFiler->writeUInt32(kCurrentVersionNumber)) != eOk)
		return stat;
	//----- Output params
	//.....
	pFiler->writeItem(m_Color);
	pFiler->writeItem(m_ReferObjId);

	return pFiler->filerStatus();
}

Acad::ErrorStatus ADSKMyDictionary::dwgInFields(AcDbDwgFiler * pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	auto stat = AcDbObject::dwgInFields(pFiler);
	if (stat != eOk)
		return stat;
	//----- Object version number needs to be read first
	Adesk::UInt32 version = 0;
	if ((stat = pFiler->readUInt32(&version)) != eOk)
		return stat;
	if (version > kCurrentVersionNumber)
		return Acad::eMakeMeProxy;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < ADSKMyDictionary::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	pFiler->readItem(&m_Color);
	pFiler->readItem(&m_ReferObjId);

	return pFiler->filerStatus();
}

//- Dxf Filing protocol
Acad::ErrorStatus ADSKMyDictionary::dxfOutFields(AcDbDxfFiler * pFiler) const {
	assertReadEnabled();
	//----- Save parent class information first.
	auto stat = AcDbObject::dxfOutFields(pFiler);
	if (stat != eOk)
		return stat;
	stat = pFiler->writeItem(AcDb::kDxfSubclass, _RXST("ADSKMyDictionary"));
	if (stat != eOk)
		return stat;
	//----- Object version number needs to be saved first
	if ((stat = pFiler->writeUInt32(kDxfInt32, kCurrentVersionNumber)) != eOk)
		return stat;
	//----- Output params
	//.....
	pFiler->writeItem(170, m_Color);
	pFiler->writeItem(330, m_ReferObjId);

	return pFiler->filerStatus();
}

Acad::ErrorStatus ADSKMyDictionary::dxfInFields(AcDbDxfFiler * pFiler) {
	assertWriteEnabled();
	//----- Read parent class information first.
	auto stat = AcDbObject::dxfInFields(pFiler);
	if (stat != eOk || !pFiler->atSubclassData(_RXST("ADSKMyDictionary")))
		return pFiler->filerStatus();
	//----- Object version number needs to be read first
	struct resbuf rb;
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
	//if ( version < ADSKMyDictionary::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params in non order dependant manner
	while (stat == eOk && (stat = pFiler->readResBuf(&rb)) == eOk) {
		switch (rb.restype) {
			//----- Read params by looking at their DXF code (example below)
			//case AcDb::kDxfXCoord:
			//	if ( version == 1 )
			//		cen3d =asPnt3d (rb.resval.rpoint) ;
			//	else 
			//		cen2d =asPnt2d (rb.resval.rpoint) ;
			//	break ;
			//.....
		case 170:
			m_Color = rb.resval.rint;
			break;

		case 330:
#ifdef _WIN64
			m_ReferObjId.setFromOldId(static_cast<Adesk::IntDbId>(rb.resval.rlong));
#else
			m_ReferObjId.setFromOldId(rb.resval.rlong);
#endif          
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

Acad::ErrorStatus ADSKMyDictionary::color(Adesk::UInt16 & Color)
{
	assertReadEnabled();
	Color = m_Color;
	return eOk;
}

Acad::ErrorStatus ADSKMyDictionary::setColor(const Adesk::UInt16 Color)
{
	assertWriteEnabled();
	m_Color = Color;
	return eOk;
}

Acad::ErrorStatus ADSKMyDictionary::referObjId(AcDbSoftPointerId & ReferObjId)
{
	assertReadEnabled();
	ReferObjId = m_ReferObjId;
	return eOk;
}

Acad::ErrorStatus ADSKMyDictionary::setReferObjId(const AcDbSoftPointerId ReferObjId)
{
	assertWriteEnabled();
	m_ReferObjId = ReferObjId;
	return eOk;
}

// -----------------------------------------------------------------------------
void ADSKMyDictionary::modified(const AcDbObject * dbObj)
{
	AcDbObject::modified(dbObj);
	AcDbEntity* pEnt;
	acdbOpenAcDbEntity(pEnt, m_ReferObjId, AcDb::kForWrite);
	pEnt->setColorIndex(AcDbEntity::cast(dbObj)->colorIndex());
	m_Color = AcDbEntity::cast(dbObj)->colorIndex();
	pEnt->close();
}
