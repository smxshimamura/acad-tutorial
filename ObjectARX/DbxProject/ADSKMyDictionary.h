#pragma once
#include "StdAfx.h"
#include "dbmain.h"

class AAPI ADSKMyDictionary : public AcDbObject {

public:
	ACRX_DECLARE_MEMBERS(ADSKMyDictionary);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:
	ADSKMyDictionary();
	~ADSKMyDictionary() override;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const override;
	Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler) override;

	//- Dxf Filing protocol
	Acad::ErrorStatus dxfOutFields(AcDbDxfFiler* pFiler) const override;
	Acad::ErrorStatus dxfInFields(AcDbDxfFiler* pFiler) override;

	Acad::ErrorStatus setColor(const Adesk::UInt16 Color);
	Acad::ErrorStatus color(Adesk::UInt16& Color);
	Acad::ErrorStatus setReferObjId(const AcDbSoftPointerId ReferObjId);
	Acad::ErrorStatus referObjId(AcDbSoftPointerId& ReferObjId);

private:
	Adesk::UInt16 m_Color;
	AcDbSoftPointerId m_ReferObjId;

public:
	void modified(const AcDbObject* dbObj) override;

};

#ifdef DBXPROJECT_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(ADSKMyDictionary)
#endif
